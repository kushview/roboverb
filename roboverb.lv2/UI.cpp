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
#include <lvtk/ui/opengl.hpp>
#include <lvtk/ui/widget.hpp>

#include <pugl/gl.h>

#include <lvtk/ext/ui/idle.hpp>
#include <lvtk/ext/ui/parent.hpp>
#include <lvtk/ext/ui/resize.hpp>
#include <lvtk/option_array.hpp>
#include <lvtk/ext/urid.hpp>
#include <lvtk/weak_ref.hpp>

#include "Ports.h"

#define ROBOVERB_NATIVEUI_URI "https://kushview.net/plugins/roboverb/ui"

namespace lvtk {

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


#if 0
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
#endif

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
    
    std::unique_ptr<OpenGLContext> ctx;

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
                    self->ctx = std::make_unique<OpenGLContext>();
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
