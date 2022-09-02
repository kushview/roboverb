/*
    This file is part of Roboverb

    Copyright (C) 2015-2019  Kushview, LLC.  All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <iostream>
#include <lvtk/ui.hpp>
#include <lvtk/ext/ui/idle.hpp>
#include <lvtk/ext/ui/parent.hpp>
#include <lvtk/ext/ui/resize.hpp>
#include <lvtk/option_array.hpp>
#include <lvtk/ext/urid.hpp>
#include <lvtk/weak_ref.hpp>

#include "Ports.h"
#include <pugl/pugl.h>
#include <pugl/gl.h>

#include "ui/nanovg/nanovg_gl.h"

#define ROBOVERB_NATIVEUI_URI "https://kushview.net/plugins/roboverb/ui"


namespace lvtk {

class Color {
public:
    Color() = default;
    Color (uint32_t value) { pixel.value = value; }
    
    Color (uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        pixel.component.r = r;
        pixel.component.g = g;
        pixel.component.b = b;
        pixel.component.a = a;
    }
    
    Color (uint8_t r, uint8_t g, uint8_t b) {
        pixel.component.r = r;
        pixel.component.g = g;
        pixel.component.b = b;
        pixel.component.a = 0xff;
    }

    Color (float r, float g, float b, float a) {
        pixel.component.r = convert (r);
        pixel.component.g = convert (g);
        pixel.component.b = convert (b);
        pixel.component.a = convert (a);
    }

    uint8_t red()   const noexcept { return pixel.component.r; }
    uint8_t green() const noexcept { return pixel.component.g; }
    uint8_t blue()  const noexcept { return pixel.component.b; }
    uint8_t alpha() const noexcept { return pixel.component.a; }

    float redf()   const noexcept { return (float)pixel.component.r / 255.f; }
    float greenf() const noexcept { return (float)pixel.component.g / 255.f; }
    float bluef()  const noexcept { return (float)pixel.component.b / 255.f; }
    float alphaf() const noexcept { return (float)pixel.component.a / 255.f; }

private:
    union {
        // struct { uint8_t r, g, b, a; } component;
        struct { uint8_t a, b, g, r; } component;
        uint32_t value { 0x000000ff };
    } pixel;

    template<typename FT>
    static inline uint8_t convert (FT v) {
        return static_cast<uint8_t> (
            (FT)255 * std::max ((FT)0, std::min ((FT)1, v)));
    }
};

class Fill {
public:
    Fill() =default;
    void set_color (Color c) {
        m_color = c;
    }
    Color color() const noexcept { return m_color; }
    bool is_color() const noexcept { return true; }

    Fill (const Color& color) {
        set_color (color);
    }

    Fill (const Fill& o) { operator= (o); }
    Fill& operator= (const Fill& o) {
        m_color = o.m_color;
        return *this;
    }
private:
    Color m_color;
};

template<typename T>
struct Point
{
    T x{}, y{};

    template<typename CT>
    Point<CT> as() const noexcept { 
        return {
            static_cast<CT> (x),
            static_cast<CT> (y)
        };
    }
};

template<typename T>
struct Rectangle {
    T x{}, y{}, width{}, height{};
    Point<T> pos() const noexcept { return { x, y }; }
    bool empty() const noexcept { return width <= T() || height <= T(); }
    
    template<typename CT>
    Rectangle<CT> as() const noexcept {
        return {
            static_cast<CT> (x),
            static_cast<CT> (y),
            static_cast<CT> (width),
            static_cast<CT> (height)
        };
    }

    bool operator== (const Rectangle<T>& o) const noexcept {
        return x == o.x && y == o.y && width == o.width && height == o.height;
    }
};

struct PathItem {
    enum Type {
        MOVE,
        LINE,
        QUADRATIC,
        CUBIC,
        CLOSE
    };

    Type type;
    float x1{}, y1{}, 
          x2{}, y2{}, 
          x3{}, y3{};
};

class Path {
public:
    Path() = default;
    ~Path() = default;
    
    void clear() { data.clear(); }

    void move (Point<float> pt) { 
        move (pt.x, pt.y);
    }

    void move (float x, float y) {
        add_op (MoveOp, x, y);
    }

    void line (float x, float y) {
        if (data.empty())
            move (0, 0);
        add_op (LineOp, x, y);
    }

    void quad (float x1, float y1, 
               float x2, float y2)
    {
        if (data.empty())
            move (0, 0);
        add_op (QuadOp, x1, y1, x2, y2);
    }
    
    void cubic (float x1, float y1, 
                float x2, float y2,
                float x3, float y3)
    {
        if (data.empty())
            move (0, 0);
        add_op (CubicOp, x1, y1, x2, y2, x3, y3);
    }

    void close() {
        if (!data.empty() && CloseOp != data.back())
            data.push_back (CloseOp);
    }

    struct const_iterator {
        const_iterator& operator++() {
            float op = *i++;
            if (op == MoveOp) {
                _item.type = PathItem::MOVE;
                _item.x1 = *i++;
                _item.y1 = *i++;
            } else if (op == LineOp) {
                _item.type = PathItem::LINE;
                _item.x1 = *i++;
                _item.y1 = *i++;
            } else if (op == QuadOp) {
                _item.type = PathItem::QUADRATIC;
                _item.x1 = *i++;
                _item.y1 = *i++;
                _item.x2 = *i++;
                _item.y2 = *i++;
            } else if (op == CubicOp) {
                _item.type = PathItem::CUBIC;
                _item.x1 = *i++;
                _item.y1 = *i++;
                _item.x2 = *i++;
                _item.y2 = *i++;
                _item.x3 = *i++;
                _item.y3 = *i++;
            } else if (op == CloseOp) {
                _item.type = PathItem::CLOSE;
                _item.x1 = _item.y1 =
                _item.x2 = _item.y2 =
                _item.x3 = _item.y3 = 0.f;
            }
            return *this;
        }

        const_iterator operator++ (int) {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        const PathItem& operator*() const {
            return _item;
        }

        bool operator== (const const_iterator& o) { return i == o.i; }
        bool operator!= (const const_iterator& o) { return i != o.i; };
    
    private:
        friend class Path;
        explicit const_iterator (std::vector<float>::const_iterator _i)
            : i (_i) {}
        std::vector<float>::const_iterator i;
        PathItem _item;
    };

    const_iterator begin() const noexcept { return const_iterator (data.begin()); }
    const_iterator end()   const noexcept { return const_iterator (data.end()); }

private:   
    static constexpr float MoveOp  = 111111.f;
    static constexpr float LineOp  = 222222.f;
    static constexpr float QuadOp  = 333333.f;
    static constexpr float CubicOp = 444444.f;
    static constexpr float CloseOp = 555555.f;
    
    std::vector<float> data;
    template<typename T> void add_op (T v) { data.push_back ((float) v); }
    template<typename T, typename... Args> 
    void add_op (T first, Args... args) {
        add_op (first);
        add_op (args...);
    }
};

class DrawingContext {
public:
    DrawingContext() = default;
    virtual ~DrawingContext() = default;
    
    virtual void set_origin (const Point<int>& pt) =0;
    virtual void set_clip_bounds (const Rectangle<int>& r) =0;
    virtual Rectangle<int> clip_bounds() const =0;

    virtual void save_state() =0;
    virtual void restore_state() =0;

    virtual void set_fill (const Fill& fill) =0;
    virtual void fill_rect (const Rectangle<float>& r) =0;
};

class NanoVGContext : public DrawingContext {
    static constexpr auto create  = nvgCreateGL3;
    static constexpr auto destroy = nvgDeleteGL3;

public:
    explicit NanoVGContext (int flags = NVG_ANTIALIAS | NVG_STENCIL_STROKES)
        : ctx (create (flags))
    {
        assert (ctx != nullptr);
        set_fill (Color (0.f, 0.f, 0.f, 1.f));
    }
    
    ~NanoVGContext()
    {
        if (ctx)
            destroy (ctx);
    }

    void set_origin (const Point<int>& pt) override {
        state.origin = pt.as<float>();
    }

    void set_clip_bounds (const Rectangle<int>& r) override {
        auto rf = r.as<float>();
        if (rf == state.clip)
            return;
        nvgScissor (ctx, rf.x, rf.y, rf.width, rf.height);
        state.clip = rf;
    }

    Rectangle<int> clip_bounds() const override {
        return state.clip.as<int>();
    }

    void set_fill (const Fill& fill) override {
        auto c = fill.color();
        state.color.r = c.redf();
        state.color.g = c.greenf();
        state.color.b = c.bluef();
        state.color.a = c.alphaf();        
    }

    void save_state() override {
        stack.push_back (state); 
        nvgSave (ctx); 
    }

    void restore_state() override {
        if (stack.empty())
            return;
        state = stack.back();
        stack.pop_back();
        nvgRestore (ctx);
    }

    void begin_frame (int width, int height, float pixel_ratio) {
        nvgBeginFrame (ctx, width, height, pixel_ratio);
    }

    void end_frame() {
        nvgEndFrame (ctx);
    }

    void fill_rect (const Rectangle<float>& r) override {
        nvgBeginPath (ctx);
        
        nvgRect (ctx, 
            r.x + state.origin.x, 
            r.y + state.origin.y, 
            r.width, r.height
        );

        nvgFillColor (ctx, state.color);
        nvgFill (ctx);
    }

    operator NVGcontext*() const { return ctx; }

private:
    NVGcontext* ctx { nullptr };
    struct State {
        NVGcolor color;
        Point<float> origin;
        Rectangle<float> clip;

        State& operator= (const State& o) {
            color = o.color;
            origin = o.origin;
            clip = o.clip;
            return *this;
        }
    };

    State state;
    std::vector<State> stack;
};

class Graphics final {
public:
    Graphics (DrawingContext& d)
        : context (d) {}

    ~Graphics() = default;
    
    void save_state()       { context.save_state(); }
    void restore_state()    { context.restore_state(); }

    void set_color (Color color) {
        context.set_fill (color);
    }

    void fill_rect (const Rectangle<float>& r) {
        context.fill_rect (r);
    }

    void set_origin (const Point<int>& pt) {
        context.set_origin (pt);
    }

    Rectangle<int> clip_bounds() const noexcept {
        return context.clip_bounds();
    }

private:
    DrawingContext& context;
};

class Style;
class Widget;

using NativeHandle = uintptr_t;

class View {
public:
    explicit View (Widget& widget) : m_widget (widget) {}
    virtual ~View() = default;

    Widget& widget() { return m_widget; }
    
    virtual NativeHandle native_handle() const = 0;
    virtual void set_visible (bool visible) = 0;
    virtual void resize (int width, int height) = 0;

private:
    Widget& m_widget;
};

class Widget {
public:
    Widget() {
        _weak_status.reset (this);
    }

    virtual ~Widget() {
        _weak_status.reset (nullptr);
    }

    virtual void resized() {}
    virtual void paint (Graphics&) {}
    
    Rectangle<int> bounds() const noexcept { return _bounds; }
    Rectangle<int> local_bounds() const noexcept {
        auto r = _bounds;
        r.x = r.y = 0;
        return r;
    }

    void set_bounds (Rectangle<int> b) {
        if (_bounds == b)
            return;
        _bounds = b;
    }

    int width()  const noexcept { return _bounds.width; }
    int height() const noexcept { return _bounds.height; }

    void add (Widget& widget) {
        _children.push_back (&widget);
        widget._parent = this;
        resized();
    }

    void remove (Widget* widget) {
        auto it = std::find (_children.begin(), _children.end(), widget);
        if (it != _children.end()) {
            _children.erase (it);
        }
    }

    void remove (Widget& widget) {
        remove (&widget);
    }

    Widget* parent() const noexcept { return _parent; }

    void render (Graphics& g) {
        render_internal (g);
    }

private:
    Widget* _parent = nullptr;
    std::vector<Widget*> _children;
    Rectangle<int> _bounds;
    LVTK_WEAK_REFABLE(Widget, _weak_status)

    void render_internal (Graphics& g) {
        g.save_state();
        paint (g);
        g.restore_state();

        for (auto child : _children) {
            g.save_state();
            g.set_origin (child->bounds().pos());
            child->render (g);
            g.restore_state();
        }
    }
};

class EditorView : public View {
public:
    EditorView (Widget& widget)
        : View (widget),
          world (puglNewWorld (PUGL_MODULE, 0)),
          view (puglNewView (world.get()))
    {
        puglSetClassName (world.get(), "LVTK");
    }
    
    ~EditorView() {
        view.reset();
        world.reset();
    }

    NativeHandle native_handle() const { return puglGetNativeView (view.get()); }
    
    void set_visible (bool visible) override {
        if (visible == puglGetVisible (view.get()))
            return;
        if (visible)
            puglShow (view.get());
        else
            puglHide (view.get());
    }

    void resize (int width, int height) override {
        puglSetSize (view.get(), 
                     static_cast<unsigned int> (width),
                     static_cast<unsigned int> (height));
    }

private:
    struct Deleter {
        void operator()(::PuglView* ptr) { puglFreeView (ptr); }
        void operator()(::PuglWorld* ptr) { puglFreeWorld (ptr); }
    };
    std::unique_ptr<PuglWorld, Deleter> world;
    std::unique_ptr<PuglView,  Deleter> view;
};

}

using namespace lvtk;

class PuglUI final : public UI<PuglUI, Parent, Idle, URID, Options>
{
public:
    PuglUI (const UIArgs& args)
        : UI (args)
    {
        for (const auto& opt : OptionArray (options()))
        {
            if (opt.key == map (LV2_UI__scaleFactor))
                m_scale_factor = *(float*) opt.value;
        }

        world = puglNewWorld (PUGL_MODULE, 0);
        puglSetWorldHandle (world, this);
        puglSetClassName (world, "Roboverb");

        view = puglNewView (world);
        int w = 360; int h = 240;
        puglSetSizeHint (view, PUGL_DEFAULT_SIZE, w, h);
        puglSetSizeHint (view, PUGL_MIN_SIZE, w, h);
        puglSetSizeHint (view, PUGL_MAX_SIZE, w, h);
        if (parent)
            puglSetParentWindow (view, (PuglNativeView)parent.get());
        puglSetBackend (view, puglGlBackend());

        puglSetViewHint (view, PUGL_USE_DEBUG_CONTEXT, PUGL_FALSE);
        puglSetViewHint (view, PUGL_RESIZABLE, PUGL_FALSE);
        // puglSetViewHint(view, PUGL_SAMPLES, opts.samples);
        puglSetViewHint (view, PUGL_DOUBLE_BUFFER, PUGL_TRUE);
        puglSetViewHint (view, PUGL_SWAP_INTERVAL, 0);
        puglSetViewHint (view, PUGL_IGNORE_KEY_REPEAT, PUGL_TRUE);
        puglSetViewHint (view, PUGL_STENCIL_BITS, 8);
        puglSetViewHint (view, PUGL_CONTEXT_VERSION_MAJOR, 3);
        puglSetViewHint (view, PUGL_CONTEXT_VERSION_MINOR, 2);
        puglSetHandle (view, this);
        puglSetEventFunc (view, _pugl_event);
        puglRealize (view);
    }

    void cleanup() {
        if (view)
            puglFreeView (view);
        if (world)
            puglFreeWorld (world);
        view = nullptr;
        world = nullptr;
        ctx.reset();
    }

    int idle() {
        if (world != nullptr)
            return puglUpdate (world, 0.0);
        return 0;
    }

    void port_event (uint32_t port, uint32_t size,
                     uint32_t format, const void*  buffer)
    {
#if 0
        if (format != 0 || size != sizeof (float))
            return;
        
        const float value = *((float*) buffer);
        const bool boolValue = value > 0.f;

        if (port >= RoboverbPorts::Comb_1 && port <= RoboverbPorts::Comb_8)
        {
           
        }
        else if (port >= RoboverbPorts::AllPass_1 && port <= RoboverbPorts::AllPass_4)
        {
           
        }
        else
        {
            switch (port)
            {
                case RoboverbPorts::Wet:
                    break;
                case RoboverbPorts::Dry:
                    break;
                case RoboverbPorts::RoomSize:
                    break;
                case RoboverbPorts::Damping:
                    break;
                case RoboverbPorts::Width:
                    break;
            }
        }
#endif
    }

    LV2UI_Widget get_widget() {
        if (content == nullptr) {
            content = std::make_unique<Content>();

            auto c2 = std::make_unique<Content>();
            WeakRef<Widget> ref = c2.get();
            std::clog << "valid_1=" << (int)ref.valid() << std::endl;
            c2.reset();
            std::clog << "valid_2=" << (int)ref.valid() << std::endl;
            puglShow (view);
        }
        
        auto w =  puglGetNativeView (view);
        return (LV2UI_Widget) w; 
    }

protected:
    PuglStatus configure (const PuglConfigureEvent& ev) {
        // std::clog << "[roboverb] configure: " << ev.x << "x" << ev.y << std::endl;
        return PUGL_SUCCESS;
    }

    PuglStatus expose (const PuglExposeEvent& ev) {
        glClearColor (0.f, 0.f, 0.f, 1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (nullptr == ctx) {
            return PUGL_SUCCESS;
        }
        
        auto r = puglGetFrame (view);
        auto s = static_cast<float> (puglGetScaleFactor (view));
        ctx->begin_frame (r.width, r.height, s);
        Graphics g (*ctx);
        content->resized();
        content->render (g);

        // g.set_color (Color (0x00ff00ff));
        // g.save_state();
        // g.set_color (Color (0x545454ff));
        // g.fill_rect ({ (float)r.x, (float)r.y, 
        //     (float)r.width, (float)r.height });
        
        // g.restore_state();
        // r.x = 10; r.y = 10; r.width -= 20; r.height -= 20;
        // g.fill_rect ({ (float)r.x, (float)r.y, 
        //     (float)r.width, (float)r.height });
       
        ctx->end_frame();
        return PUGL_SUCCESS;
    }

    PuglStatus motion (const PuglMotionEvent& ev) {
        // std::clog << ev.x << "x" << ev.y << std::endl;
        return PUGL_SUCCESS;
    }

private:
    PuglWorld* world = nullptr;
    PuglView* view = nullptr;
    
    std::unique_ptr<NanoVGContext> ctx;

    class Button : public Widget {
    public:
        Button() = default;
        virtual ~Button() = default;

        void paint (Graphics& g) override {
            g.set_color (color);
            g.fill_rect (local_bounds().as<float>());
        }

        Color color { 0xff0000ff };
    };

    class Content : public Widget {
    public:
        Content() {
            add (button1);
            add (button2);
            button2.color = Color (0xff000022);
            set_bounds (Rectangle<int> { 0, 0, 360, 240 });            
        }

        ~Content() {

        }

        void resized() override {
            auto r1 = local_bounds();

            Rectangle<int> r2;
            r2.width = 90;
            r2.height = 30;
            r2.x = (r1.width / 2) - (r2.width / 2);
            r2.y = (r1.height / 2) - (r2.height / 2);
        
            button1.set_bounds (r2);

            r2.y += (r2.height + 2);
            button2.set_bounds (r2);
        }

        void paint (Graphics& g) override {
            g.set_color (Color (0x545454ff));
            g.fill_rect (local_bounds().as<float>());
        }
    private:
        Button button1, button2;
    };
    std::unique_ptr<Content> content;

    float m_scale_factor { 1.f };

    static inline PuglStatus _pugl_event (PuglView* view, const PuglEvent* event) {
        auto self = static_cast<PuglUI*>(puglGetHandle (view));
        PuglStatus status = PUGL_SUCCESS;
        switch (event->type) {
            case PUGL_CONFIGURE:      //< View moved/resized, a #PuglConfigureEvent
                status = self->configure (event->configure);
                break;                

            case PUGL_EXPOSE:         //< View must be drawn, a #PuglExposeEvent
                status = self->expose (event->expose);
                break;

            case PUGL_CREATE:         ///< View created, a #PuglCreateEvent
            {
                if (self->ctx == nullptr)
                    self->ctx = std::make_unique<NanoVGContext>();
                break;
            }
               
            case PUGL_DESTROY:        ///< View destroyed, a #PuglDestroyEvent
                break;            
            case PUGL_MAP:            ///< View made visible, a #PuglMapEvent
                break;
            case PUGL_UNMAP:          ///< View made invisible, a #PuglUnmapEvent
                break;
            case PUGL_UPDATE:         ///< View ready to draw, a #PuglUpdateEvent
                break;
            case PUGL_CLOSE:          ///< View will be closed, a #PuglCloseEvent
                break;
            case PUGL_FOCUS_IN:       ///< Keyboard focus entered view, a #PuglFocusEvent
                break;
            case PUGL_FOCUS_OUT:      ///< Keyboard focus left view, a #PuglFocusEvent
                break;
            case PUGL_KEY_PRESS:      ///< Key pressed, a #PuglKeyEvent
                break;
            case PUGL_KEY_RELEASE:    ///< Key released, a #PuglKeyEvent
                break;
            case PUGL_TEXT:           ///< Character entered, a #PuglTextEvent
                break;
            case PUGL_POINTER_IN:     ///< Pointer entered view, a #PuglCrossingEvent
                break;
            case PUGL_POINTER_OUT:    ///< Pointer left view, a #PuglCrossingEvent
                break;
            case PUGL_BUTTON_PRESS:   ///< Mouse button pressed, a #PuglButtonEvent
                break;
            case PUGL_BUTTON_RELEASE: ///< Mouse button released, a #PuglButtonEvent
                break;
            case PUGL_MOTION:         ///< Pointer moved, a #PuglMotionEvent
                status = self->motion (event->motion);
                break;
            case PUGL_SCROLL:         ///< Scrolled, a #PuglScrollEvent
                break;
            case PUGL_CLIENT:         ///< Custom client message, a #PuglClientEvent
                break;
            case PUGL_TIMER:          ///< Timer triggered, a #PuglTimerEvent
                break;
            case PUGL_LOOP_ENTER:     ///< Recursive loop entered, a #PuglLoopEnterEvent
                break;
            case PUGL_LOOP_LEAVE:     ///< Recursive loop left, a #PuglLoopLeaveEvent
                break;
            case PUGL_DATA_OFFER:     ///< Data offered from clipboard, a #PuglDataOfferEvent
                break;
            case PUGL_DATA:           ///< Data available from clipboard, a #PuglDataEvent
                break;
            case PUGL_NOTHING:        ///< No event
                break;
            default :
                break;
        }

        return status;
    }
};

static UIDescriptor<PuglUI> sNativeRoboverbUI (
    ROBOVERB_NATIVEUI_URI, { 
        LV2_UI__parent 
    }
);
