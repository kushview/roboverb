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
#include <lvtk/ui/opengl.hpp>
#include <lvtk/ui/widget.hpp>
#include <lvtk/ui/button.hpp>
#include <lvtk/ui/slider.hpp>

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include <lvtk/options.hpp>

#include "ports.hpp"

#define ROBOVERB_UI_URI "https://kushview.net/plugins/roboverb/ui"

using namespace lvtk;


class RoboverbToggle : public lvtk::Button {
public:
    RoboverbToggle() = default;
    ~RoboverbToggle() = default;

protected:
    void paint_button (Graphics& g, bool highlight, bool down) override {
        g.set_color (0x000000ff);
        auto b = bounds().at(0);
        g.fill_rect (b);

        g.set_color (toggled() ? 0xff000ff : 0x333333ff);
        b = b.smaller (1);
        g.fill_rect (b);
    }
};

class RoboverbContent : public lvtk::Widget {
public:
    RoboverbContent() {
        set_opaque (true);

        for (int i = 0; i < 5; ++i) {
            auto s = add (new lvtk::Slider());
            s->set_range (0.0, 1.0);
            s->set_type (Slider::VERTICAL_BAR);
            sliders.push_back (s);
        }

        for (int i = 0; i < 12; ++i) {
            // auto t = add (new RoboverbToggle());
            // t->toggle (false);
            // toggles.push_back (t);
        }

        show_all();
        set_size (640, 360);
    }

    ~RoboverbContent()  {
        for (auto t : toggles)
            delete t;
        toggles.clear();
        for (auto s : sliders)
            delete s;
        sliders.clear();
    }

    void update_toggle (int index, bool value) {
        
        if (! (index >= 0 && index < (int) toggles.size()))
            return;
        // std::clog << "[roboverb] update_toggle(" << (int)value << ")\n";
        toggles[index]->toggle (value);
    }

    template<typename Ft>
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
        auto btn_size = height() / 3;
        auto btn_hspace = btn_size * 4;
        auto slider_hspace = width() - btn_hspace;

        auto sb = bounds().at(0).slice_left (slider_hspace);
        int w = slider_hspace / 5;
        for (int i = 0; i < 5; ++i) {
            auto sr = sb.slice_left (w);
            sliders[i]->set_bounds (sr.smaller (1, 0));
        }
    }

    void paint (Graphics& g) override {
        g.set_color (0x121212ff);
        g.fill_rect (bounds().at (0));
        g.set_color (0xffffffff);
        g.draw_text ("Roboverb",
                     bounds().at (0).as<float>(),
                     lvtk::Align::CENTERED);
    }

private:
    std::vector<lvtk::Slider*> sliders;
    std::vector<RoboverbToggle*> toggles;
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
    }

    int idle() {
        _main.loop (0);
        return 0;
    }

    void port_event (uint32_t port, uint32_t size, uint32_t format, const void* buffer) {
        if (format != 0 || size != sizeof (float))
            return;

        const float value    = *((float*) buffer);
        const bool bvalue    = value != 0.f;

        if (port >= RoboverbPorts::Comb_1 && port <= RoboverbPorts::AllPass_4) {
            auto index = static_cast<int> (port - RoboverbPorts::Comb_1);
            content->update_toggle (index, bvalue);
        } else if (port >= RoboverbPorts::Wet && port <= RoboverbPorts::Width) {
            auto index = static_cast<int> (port - RoboverbPorts::Wet);
            content->update_slider (index, value);
        }
    }

    LV2UI_Widget widget() {
        if (content == nullptr) {
            content = std::make_unique<Content>();
            _main.elevate (*content, 0, (uintptr_t) parent.get());
            content->set_visible (true);
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
