/*
    This file is part of Roboverb

    Copyright (C) 2015-2025  Kushview, LLC.  All rights reserved.

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
    using Content = roboverb::Content;
    using Ports   = roboverb::Ports;

    RoboverbUI (const UIArgs& args)
        : UI (args) {
        for (const auto& opt : OptionArray (options())) {
            if (opt.key == map_uri (LV2_UI__scaleFactor))
                m_scale_factor = *(float*) opt.value;
        }

        widget();
    }

    void cleanup() {
        _gui.destroy();
        _gui.setControlHandler (nullptr);
    }

    int idle() {
        _gui.idle();
        return 0;
    }

    bool _block_sending { false };

    void send_control (uint32_t port, float value) {
        if (_block_sending)
            return;
        write (port, value);
    }

    void port_event (uint32_t port, uint32_t size, uint32_t format, const void* buffer) {
        auto content = _gui.widget();
        if (content == nullptr || format != 0 || size != sizeof (float))
            return;

        _block_sending = true;

        const float value = *((float*) buffer);
        const bool bvalue = value != 0.f;

        if (port >= Ports::Comb_1 && port <= Ports::AllPass_4) {
            auto index = static_cast<int> (port - Ports::Comb_1);
            content->update_toggle (index, bvalue);
        } else if (port >= Ports::Wet && port <= Ports::Width) {
            auto index = static_cast<int> (port - Ports::Wet);
            content->update_slider (index, value);
        }

        _block_sending = false;
    }

    LV2UI_Widget widget() {
        if (_gui.widget() == nullptr) {
            _gui.create();
            clap_window_t window;
            window.ptr = (void*) parent.get();
            _gui.setParent (&window);
            _gui.show();
            _gui.setControlHandler (std::bind (
                &RoboverbUI::send_control, this, std::placeholders::_1, std::placeholders::_2));
        }

        return (LV2UI_Widget) _gui.nativeHandle();
    }

private:
    float m_scale_factor { 1.f };
    roboverb::GuiMain _gui;
};

static UIDescriptor<RoboverbUI> s_roboverb_ui (
    ROBOVERB_UI_URI, { LV2_UI__parent });
