#pragma once

#include <functional>

#include <clap/clap.h>

#include <lui/button.hpp>
#include <lui/cairo.hpp>
#include <lui/slider.hpp>
#include <lui/widget.hpp>

#include "ports.hpp"
#include "res.hpp"

/** Color table for the comb and all pass toggles.
    ordered by rows and columns going top-down left-to-right.
*/
static const lui::Color _toggle_colors[] = {
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500,
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500,
    0xffffa500,
    0xffefa500,
    0xffdfa500,
    0xffcfa500
};

class RoboverbToggle : public lui::Button {
public:
    RoboverbToggle()  = default;
    ~RoboverbToggle() = default;

    void set_on_color (lui::Color color) {
        _color_on = color;
        repaint();
    }

    void set_text (const std::string& text) {
        _text = text;
        repaint();
    }

protected:
    void paint_button (lui::Graphics& g, bool highlight, bool down) override {
        g.set_color (0xff111111);
        auto b = bounds().at (0);
        g.fill_rect (b);

        g.set_color (toggled() ? _color_on : _color_off.with_alpha (0.8f));
        g.fill_rect (b.smaller (1));

        g.set_color (text_color());
        g.draw_text (_text, bounds().at (0).as<float>(), lui::Justify::CENTERED);
    }

private:
    float _text_alpha { 0.72f };
    lui::Color _color_on { 0xffffa400 },
        _color_off { 0xff363333 },
        _color_text_off { 0xffffffff },
        _color_text_on { 0xff222222 };
    std::string _text;

    lui::Color text_color() const noexcept {
        auto c = toggled() ? _color_text_on : _color_text_off;
        return c.with_alpha (_text_alpha);
    }
};

class ControlLabel : public lui::Widget {
public:
    ControlLabel (const std::string& text) {
        set_name (text);
        _text = name();
    }

    void paint (lui::Graphics& g) override {
        g.set_color (0xffffffff);
        g.set_font (lui::Font (11.f));
        g.draw_text (_text, bounds().at (0).as<float>(), _align);
    }

    void set_text (const std::string& text) {
        _text = text;
        repaint();
    }

private:
    lui::Justify _align { lui::Justify::MID_LEFT };
    std::string _text;
};

class RoboverbContent : public lui::Widget {
public:
    std::function<void (uint32_t, float)> on_control_changed;

    RoboverbContent() {
        set_opaque (true);
        bg_image = lui::Image::load ((uint8_t*) res::roboverb_bg_jpg, res::roboverb_bg_jpgSize);
        for (int i = RoboverbPorts::Wet; i <= RoboverbPorts::Width; ++i) {
            auto s = add (new lui::Slider());
            s->set_range (0.0, 1.0);
            s->set_type (lui::Slider::HORIZONTAL);

            s->on_value_changed = [&, i, s]() {
                if (on_control_changed) {
                    const auto port  = static_cast<uint32_t> (i);
                    const auto value = static_cast<float> (s->value());
                    on_control_changed (port, value);
                }
            };

            sliders.push_back (s);

            std::string text = "";
            switch (i) {
                case RoboverbPorts::Wet:
                    text = "Wet level";
                    break;
                case RoboverbPorts::Dry:
                    text = "Dry level";
                    break;
                case RoboverbPorts::RoomSize:
                    text = "Room size";
                    break;
                case RoboverbPorts::Damping:
                    text = "Damping";
                    break;
                case RoboverbPorts::Width:
                    text = "Width";
                    break;
            }

            labels.push_back (add (new ControlLabel (text)));
        }

        for (int i = RoboverbPorts::Comb_1; i <= RoboverbPorts::AllPass_4; ++i) {
            auto idx = i - RoboverbPorts::Comb_1;
            auto t   = add (new RoboverbToggle());
            t->toggle (false);
            t->set_on_color (_toggle_colors[idx]);
            std::stringstream text;
            if (i < RoboverbPorts::AllPass_1)
                text << "C" << (idx + 1);
            else
                text << "A" << (i - RoboverbPorts::AllPass_1 + 1);
            t->set_text (text.str());
            t->on_clicked = [&, i, t]() {
                t->toggle (! t->toggled());
                if (on_control_changed) {
                    const auto port = static_cast<uint32_t> (i);
                    on_control_changed (port, t->toggled() ? 1.f : 0.f);
                }
            };

            toggles.push_back (t);
        }

        update_toggles();

        show_all();
        set_size (640 * 0.72, 360 * 0.72);
    }

    ~RoboverbContent() {
        for (auto t : toggles)
            delete t;
        toggles.clear();
        for (auto s : sliders)
            delete s;
        sliders.clear();
    }

    void update_toggles() {
        if (toggles.size() < 12)
            return;

        for (int i = RoboverbPorts::Comb_1; i <= RoboverbPorts::AllPass_4; ++i) {
            auto idx = i - RoboverbPorts::Comb_1;
            auto t   = toggles.at (idx);
            t->set_on_color (_toggle_colors[idx]);

            if (_show_toggle_text) {
                std::stringstream text;
                if (i < RoboverbPorts::AllPass_1)
                    text << "C" << (idx + 1);
                else
                    text << "A" << (i - RoboverbPorts::AllPass_1 + 1);
                t->set_text (text.str());
            }
        }
    }

    void update_toggle (int index, bool value) {
        if (! (index >= 0 && index < (int) toggles.size()))
            return;
        toggles[index]->toggle (value);
    }

    template <typename Ft>
    void update_slider (int index, Ft value) {
        if (! (index >= 0 && index < (int) sliders.size()))
            return;
        auto dvalue = static_cast<double> (value);
        sliders[index]->set_value (dvalue, lui::Notify::NONE);
        // std::clog << "[roboverb] slider_min ("<< sliders[index]->range().min <<")\n";
        // std::clog << "[roboverb] slider_max ("<< sliders[index]->range().max <<")\n";
        // std::clog << "[roboverb] slider_value ("<< sliders[index]->value() <<")\n";
    }

protected:
    void resized() override {
        const auto btn_size      = height() / 3 - 10;
        const auto btn_hspace    = btn_size * 4;
        const auto slider_hspace = width() - btn_hspace;

        auto r1 = bounds().at (0);
        auto sb = r1.slice_left (slider_hspace);
        sb.slice_top (40);
        sb.slice_bottom (20);
        int h = sb.height / 5;
        for (int i = 0; i < 5; ++i) {
            auto r = sb.slice_top (h);

            auto sr = r.slice_top (r.height * 0.3333);
            sr.slice_left (6);
            labels[i]->set_bounds (sr);
            r.slice_top (1);
            sliders[i]->set_bounds (r.smaller (3, 2));
        }

        auto tb           = r1.smaller (2); //bounds().at (0).slice_right (btn_hspace).smaller (2);
        size_t tidx       = 0;
        int actual_size   = tb.height / 3.f;
        int actual_h_size = tb.width / 4.f;
        for (int i = 0; i < 3; ++i) {
            auto r = tb.slice_top (actual_size);
            for (int j = 0; j < 4; ++j) {
                auto* t = toggles[tidx++];
                t->set_bounds (r.slice_left (actual_h_size)
                                   .smaller (1));
            }
        }
    }

    void paint (lui::Graphics& g) override {
        if (bg_image) {
            g.draw_image (bg_image, bounds().at (0).as<double>(), lui::Fitment::STRETCH);
        }
        g.set_color (lui::Color (0, 0, 0, 200));
        g.fill_rect (bounds().at (0));
        g.set_color (0xccffffff);
        g.draw_text ("  ROBOVERB",
                     bounds().at (0).smaller (3, 4).as<float>(),
                     lui::Justify::TOP_LEFT);
    }

private:
    std::vector<lui::Slider*> sliders;
    std::vector<RoboverbToggle*> toggles;
    std::vector<ControlLabel*> labels;
    bool _show_toggle_text { true };
    lui::Image bg_image;
};

namespace roboverb {

class GuiMain final {
    std::unique_ptr<lui::Main> gui;
    std::unique_ptr<RoboverbContent> content;
    bool _elevated = false;

public:
    using ControlHandler = std::function<void (uint32_t port, float value)>;
    RoboverbContent* widget() { return content.get(); }

    bool create() {
        if (gui == nullptr)
            gui = std::make_unique<lui::Main> (lui::Mode::MODULE, std::make_unique<lui::Cairo>());
        if (content == nullptr) {
            content = std::make_unique<RoboverbContent>();
        }

        return gui != nullptr && content != nullptr;
    }

    void setControlHandler (ControlHandler handler) {
        if (content != nullptr)
            content->on_control_changed = handler;
    }

    bool destroy() {
        content = nullptr;
        gui     = nullptr;
        return true;
    }

    int width() const { return content->width(); }
    int height() const { return content->height(); }
    void show() { content->set_visible (true); }
    void hide() { content->set_visible (false); }

    bool setParent (const clap_window_t* parent) {
        if (_elevated)
            return true;
        if (content == nullptr || gui == nullptr)
            return false;
        return nullptr != gui->elevate (*content, 0, (uintptr_t) parent->ptr);
    }

    void idle() {
        if (gui)
            gui->loop (0);
    }

    uintptr_t nativeHandle() const noexcept {
        return content != nullptr ? content->find_handle() : 0;
    }
};

} // namespace roboverb
