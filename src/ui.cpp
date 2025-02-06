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

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include <lvtk/options.hpp>

#include "ports.hpp"
#include "res.hpp"
#include "ui.hpp"

#define ROBOVERB_UI_URI "https://kushview.net/plugins/roboverb/ui"

using namespace lvtk;

class RoboverbUI final : public UI<RoboverbUI, Parent, Idle, URID, Options> {
public:
    using Content = RoboverbContent;

    RoboverbUI (const UIArgs& args)
        : UI (args),
          _main (lui::Mode::MODULE, std::make_unique<lui::Cairo>()) {
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
        }

        return (LV2UI_Widget) content->find_handle();
    }

private:
    float m_scale_factor { 1.f };
    lui::Main _main;
    std::unique_ptr<Content> content;
};

static UIDescriptor<RoboverbUI> s_roboverb_ui (
    ROBOVERB_UI_URI, { LV2_UI__parent });
