/*
    This file is part of Roboverb

    Copyright (C) 2015-2023  Kushview, LLC.  All rights reserved.

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
#include <lvtk/ui/button.hpp>
#include <lvtk/ui/opengl.hpp>
#include <lvtk/ui/slider.hpp>
#include <lvtk/ui/widget.hpp>

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include <lvtk/options.hpp>

#include "ports.hpp"

#define ROBOVERB_UI_URI "https://kushview.net/plugins/roboverb/ui"

using namespace lvtk;

/** Color table for the comb and all pass toggles.
    ordered by rows and columns going top-down left-to-right.
*/
static const lvtk::Color _toggle_colors[] = {
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

class RoboverbToggle : public lvtk::Button {
public:
    RoboverbToggle()  = default;
    ~RoboverbToggle() = default;

    void set_on_color (lvtk::Color color) {
        _color_on = color;
        repaint();
    }

    void set_text (const std::string& text) {
        _text = text;
        repaint();
    }

protected:
    void paint_button (Graphics& g, bool highlight, bool down) override {
        g.set_color (0xff111111);
        auto b = bounds().at (0);
        g.fill_rect (b);

        g.set_color (toggled() ? _color_on : _color_off.with_alpha (0.8f));
        g.fill_rect (b.smaller (1));

        g.set_color (text_color());
        g.draw_text (_text, bounds().at (0).as<float>(), lvtk::Align::CENTERED);
    }

private:
    float _text_alpha { 0.72f };
    lvtk::Color _color_on { 0xffffa400 },
        _color_off { 0xff363333 },
        _color_text_off { 0xffffffff },
        _color_text_on { 0xff222222 };
    std::string _text;

    lvtk::Color text_color() const noexcept {
        auto c = toggled() ? _color_text_on : _color_text_off;
        return c.with_alpha (_text_alpha);
    }
};

class ControlLabel : public lvtk::Widget {
public:
    ControlLabel (const std::string& text) {
        set_name (text);
        _text = name();
    }
    
    void paint (lvtk::Graphics& g) override {
        g.set_color (0xffffffff);
        g.set_font (lvtk::Font (11.f));
        g.draw_text (_text, bounds().at(0).as<float>(), _align);
    }

    void set_text (const std::string& text) {
        _text = text;
        repaint();
    }

private:
    lvtk::Align _align { lvtk::Align::LEFT_MIDDLE };
    std::string _text;
};

class RoboverbContent : public lvtk::Widget {
public:
    std::function<void (uint32_t, float)> on_control_changed;

    RoboverbContent() {
        set_opaque (true);

        for (int i = RoboverbPorts::Wet; i <= RoboverbPorts::Width; ++i) {
            auto s = add (new lvtk::Slider());
            s->set_range (0.0, 1.0);
            s->set_type (Slider::HORIZONTAL_BAR);

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
                case RoboverbPorts::Wet: text = "Wet level"; break;
                case RoboverbPorts::Dry: text = "Dry level"; break;
                case RoboverbPorts::RoomSize: text = "Room size"; break;
                case RoboverbPorts::Damping: text = "Damping"; break;
                case RoboverbPorts::Width: text = "Width"; break;
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
        set_size (640 * 0.72, 360 * .72);
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
        sliders[index]->set_value (dvalue, lvtk::Notify::NONE);
        // std::clog << "[roboverb] slider_min ("<< sliders[index]->range().min <<")\n";
        // std::clog << "[roboverb] slider_max ("<< sliders[index]->range().max <<")\n";
        // std::clog << "[roboverb] slider_value ("<< sliders[index]->value() <<")\n";
    }

protected:
    void resized() override {
        const auto btn_size      = height() / 3;
        const auto btn_hspace    = btn_size * 4;
        const auto slider_hspace = width() - btn_hspace;

        auto sb = bounds().at (0).slice_left (slider_hspace);
        sb.slice_top (33);
        int h = sb.height / 5;
        for (int i = 0; i < 5; ++i) {
            auto r = sb.slice_top (h);

            auto sr = r.slice_top (r.height * 0.3333);
            sr.slice_left (6);
            labels[i]->set_bounds (sr);
            r.slice_top (1);
            sliders[i]->set_bounds (r.smaller (3, 2));
        }

        auto tb = bounds().at (0).slice_right (btn_hspace).smaller (2);
        size_t tidx = 0;
        int actual_size = tb.height / 3.f;
        for (int i = 0; i < 3; ++i) {
            auto r = tb.slice_top (actual_size);
            for (int j = 0; j < 4; ++j) {
                auto* t = toggles[tidx++];
                t->set_bounds (r.slice_left (actual_size)
                                   .smaller (1));
            }
        }
    }

    void paint (Graphics& g) override {
        g.set_color (0xff242222);
        g.fill_rect (bounds().at (0));
        g.set_color (0xccffffff);
        g.draw_text ("  ROBOVERB",
                     bounds().at (0).smaller (3, 4).as<float>(),
                     lvtk::Align::TOP_LEFT);
    }

private:
    std::vector<lvtk::Slider*> sliders;
    std::vector<RoboverbToggle*> toggles;
    std::vector<ControlLabel*> labels;
    bool _show_toggle_text { true };
};

class RoboverbUI final : public UI<RoboverbUI, Resize, Parent, Idle, URID, Options> {
public:
    using Content = RoboverbContent;

    RoboverbUI (const UIArgs& args)
        : UI (args),
          _main (lvtk::Mode::MODULE, std::make_unique<lvtk::OpenGL>()) {
        for (const auto& opt : OptionArray (options())) {
            if (opt.key == map_uri (LV2_UI__scaleFactor))
                m_scale_factor = *(float*) opt.value;
        }

        widget();
    }

    void cleanup() {
        content.reset();
    }

    int idle() {
        _main.loop (0);
        return 0;
    }

    bool _block_sending { false };

    void send_control (uint32_t port, float value) {
        if (_block_sending)
            return;
        write (port, value);
    }

    void port_event (uint32_t port, uint32_t size, uint32_t format, const void* buffer) {
        if (format != 0 || size != sizeof (float))
            return;

        _block_sending = true;

        const float value = *((float*) buffer);
        const bool bvalue = value != 0.f;

        if (port >= RoboverbPorts::Comb_1 && port <= RoboverbPorts::AllPass_4) {
            auto index = static_cast<int> (port - RoboverbPorts::Comb_1);
            content->update_toggle (index, bvalue);
        } else if (port >= RoboverbPorts::Wet && port <= RoboverbPorts::Width) {
            auto index = static_cast<int> (port - RoboverbPorts::Wet);
            content->update_slider (index, value);
        }

        _block_sending = false;
    }

    LV2UI_Widget widget() {
        if (content == nullptr) {
            content = std::make_unique<Content>();
            _main.elevate (*content, 0, (uintptr_t) parent.get());
            content->set_visible (true);
            content->on_control_changed = std::bind (
                &RoboverbUI::send_control, this, std::placeholders::_1, std::placeholders::_2);
            if (auto view = content->find_view()) {
                view->set_size (content->width(), content->height());
                notify_size (content->width(), content->height());
            }
        }

        return (LV2UI_Widget) content->find_handle();
    }

private:
    float m_scale_factor { 1.f };
    lvtk::Main _main;
    std::unique_ptr<Content> content;
};

static UIDescriptor<RoboverbUI> s_roboverb_ui (
    ROBOVERB_UI_URI, { LV2_UI__parent });
