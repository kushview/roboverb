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

#include "Ports.h"
#include "Roboverb.h"
#include "PluginView.h"
#include "../tools/jucer/JuceLibraryCode/BinaryData.cpp"

#define ROBOVERB_JUCEUI_URI "https://kushview.net/plugins/roboverb/juceui"

class ModuleUI final : public lvtk::UIInstance<ModuleUI> {
public:
    ModuleUI (const lvtk::UIArgs& args)
        : UIInstance (args),
          pluginURI (args.plugin),
          bundlePath (args.bundle)
    {
        state = createState();
        view.stabilizeComponents (state);

        bindSlider (*view.wetLevel, RoboverbPorts::Wet);
        bindSlider (*view.dryLevel, RoboverbPorts::Dry);
        bindSlider (*view.roomSize, RoboverbPorts::RoomSize);
        bindSlider (*view.damping,  RoboverbPorts::Damping);
        bindSlider (*view.width,    RoboverbPorts::Width);

        ToggleSwitch* toggles[] = { view.comb1.get(), view.comb2.get(),
                                    view.comb3.get(), view.comb4.get(),
                                    view.comb5.get(), view.comb6.get(),
                                    view.comb7.get(), view.comb8.get(),
                                    view.allpass1.get(), view.allpass2.get(),
                                    view.allpass3.get(), view.allpass4.get() };
        
        for (int i = RoboverbPorts::Comb_1; i <= RoboverbPorts::AllPass_4; ++i)
            bindToggle (*toggles[i - RoboverbPorts::Comb_1], (uint32_t) i);
    }

    ValueTree createState()
    {
        Roboverb verb;
        ValueTree s ("roboverb");
        const auto& params = verb.getParameters();
        BigInteger combs, allpasses;
        combs.setRange (0, 8, false);
        allpasses.setRange(0, 4, false);
        verb.getEnablement (combs, allpasses);

        s.setProperty (Tags::roomSize, params.roomSize, nullptr);
        s.setProperty (Tags::damping, params.damping, nullptr);
        s.setProperty (Tags::wetLevel, params.wetLevel, nullptr);
        s.setProperty (Tags::dryLevel, params.dryLevel, nullptr);
        s.setProperty (Tags::width, params.width, nullptr);
        s.setProperty (Tags::freezeMode, params.freezeMode, nullptr);
        s.setProperty (Tags::enabledAllPasses, allpasses.toString(2), nullptr);
        s.setProperty (Tags::enabledCombs, combs.toString(2), nullptr);

        return s;
    }
    
    void bindSlider (Slider& slider, uint32_t port)
    {
        slider.onValueChange = [this, &slider, port]() {
            write (port, (float) slider.getValue()) ;
        };
    }

    void bindToggle (ToggleSwitch& toggle, uint32_t port)
    {
        toggle.onClick = [this, &toggle, port]() {
            write (port, toggle.getToggleState() ? 1.0 : 0.0);
        };
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
            ToggleSwitch* combs[] = { view.comb1.get(), view.comb2.get(),
                                      view.comb3.get(), view.comb4.get(),
                                      view.comb5.get(), view.comb6.get(),
                                      view.comb7.get(), view.comb8.get() };
            combs[port - RoboverbPorts::Comb_1]->setToggleState (boolValue, dontSendNotification);
        }
        else if (port >= RoboverbPorts::AllPass_1 && port <= RoboverbPorts::AllPass_4)
        {
            ToggleSwitch* allPasses[] = { view.allpass1.get(), view.allpass2.get(),
                                          view.allpass3.get(), view.allpass4.get() };
            allPasses[port - RoboverbPorts::AllPass_1]->setToggleState (boolValue, dontSendNotification);
        }
        else
        {
            switch (port)
            {
                case RoboverbPorts::Wet:
                    view.wetLevel->setValue (value, dontSendNotification);
                    break;
                case RoboverbPorts::Dry:
                    view.dryLevel->setValue (value, dontSendNotification);
                    break;
                case RoboverbPorts::RoomSize:
                    view.roomSize->setValue (value, dontSendNotification);
                    break;
                case RoboverbPorts::Damping:
                    view.damping->setValue (value, dontSendNotification);
                    break;
                case RoboverbPorts::Width:
                    view.width->setValue (value, dontSendNotification);
                    break;
            }
        }
    }

    LV2UI_Widget getComponentAsWidget() { return (LV2UI_Widget) &view; }

    // lvtk
    LV2UI_Widget get_widget() { return getComponentAsWidget(); }
    void idle() {}
    void show() {}
    void hide() {}

private:
    ValueTree state;
    const String pluginURI;
    const String bundlePath;
    PluginView view;
};

lvtk::UI<ModuleUI> sRoboverbUI (ROBOVERB_JUCEUI_URI);
