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

#include <lvtk/ui.hpp>
#include <lvtk/ext/ui/idle.hpp>
#include <lvtk/ext/ui/parent.hpp>
#include <lvtk/ext/ui/resize.hpp>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

#define NK_PUGL_IMPLEMENTATION
#define NK_PUGL_LOAD_OPENGL_EXTENSIONS
#include "nuklear_pugl.h"

#include "Ports.h"    

#define ROBOVERB_NATIVEUI_URI "https://kushview.net/plugins/roboverb/ui"

using namespace lvtk;

class NativeUI final : public UI<NativeUI, Parent, Idle, Resize>
{
public:
    NativeUI (const UIArgs& args)
        : UI (args)
    {
        nuke->width  = 200;
        nuke->height = 200;
        nuke->handle = this;
        nuke->expose = _expose;
        
        nuke.init();
        nuke.input_begin();
    }

    void cleanup() {
        nuke.input_end();
        nuke.reset();
    }

    int idle() {
        nuke.process_events();
        return 0;
    }

    void port_event (uint32_t port, uint32_t size,
                     uint32_t format, const void*  buffer)
    {
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
    }

    // lvtk
    LV2UI_Widget widget() { return (LV2UI_Widget) nk_pugl_native_window (nuke); }

private:
    nk::pugl nuke;

    void expose() {
        auto* const ctx = &nuke->ctx;
    }

    static void _expose (nk_pugl_handle handle) {
        (static_cast<NativeUI*> (handle))->expose();
    }
};

static UIDescriptor<NativeUI> sNativeRoboverbUI (
    ROBOVERB_NATIVEUI_URI, { 
        LV2_UI__parent 
    }
);
