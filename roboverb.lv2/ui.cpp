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

#include <lvtk/ext/idle.hpp>
#include <lvtk/ext/parent.hpp>
#include <lvtk/ext/resize.hpp>
#include <lvtk/ext/urid.hpp>

#include <lvtk/options.hpp>

#include "ports.hpp"

#define ROBOVERB_UI_URI "https://kushview.net/plugins/roboverb/ui"

using namespace lvtk;

class RoboverbContent : public lvtk::Widget {
public:
    RoboverbContent()  {
        set_opaque (true);
        set_size (640, 360);
    }
    ~RoboverbContent() = default;

protected:
    void paint (Graphics& g) override {
        g.set_color (0x545454ff);
        g.fill_rect (bounds().at (0));
    }
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

    void port_event (uint32_t port, uint32_t size,
                     uint32_t format, const void* buffer) {
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
