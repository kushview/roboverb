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

#include <lvtk/plugin.hpp>

#include "ports.hpp"
#include "roboverb.hpp"

#define ROBOVERB_URI "https://kushview.net/plugins/roboverb"

using roboverb::Ports;

class Module final : public lvtk::Plugin<Module> {
public:
    Module (const lvtk::Args& args)
        : Plugin (args),
          sampleRate (args.sample_rate),
          bundlePath (args.bundle) {}

    ~Module() {}

    void connect_port (uint32_t port, void* data) {
        switch (port) {
            case Ports::AudioIn_1:
                input[0] = (float*) data;
                break;
            case Ports::AudioIn_2:
                input[1] = (float*) data;
                break;
            case Ports::AudioOut_1:
                output[0] = (float*) data;
                break;
            case Ports::AudioOut_2:
                output[1] = (float*) data;
                break;
        }

        // Lilv will connect NULL on instantiate... just return
        if (data == nullptr)
            return;

        switch (port) {
            case Ports::Wet:
                params.wetLevel = *((float*) data);
                break;
            case Ports::Dry:
                params.dryLevel = *((float*) data);
                break;
            case Ports::RoomSize:
                params.roomSize = *((float*) data);
                break;
            case Ports::Width:
                params.width = *((float*) data);
                break;
            case Ports::Damping:
                params.damping = *((float*) data);
                break;

            case Ports::Comb_1:
            case Ports::Comb_2:
            case Ports::Comb_3:
            case Ports::Comb_4:
            case Ports::Comb_5:
            case Ports::Comb_6:
            case Ports::Comb_7:
            case Ports::Comb_8: {
                const auto value = *((float*) data);
                verb.setCombToggle (port - Ports::Comb_1, value > 0.f);
            } break;

            case Ports::AllPass_1:
            case Ports::AllPass_2:
            case Ports::AllPass_3:
            case Ports::AllPass_4: {
                const auto value = *((float*) data);
                verb.setAllPassToggle (port - Ports::AllPass_1, value > 0.f);
            } break;
        }
    }

    void activate() {
        verb.reset();
        verb.setSampleRate (sampleRate);
    }

    void deactivate() {
        // noop
    }

    void run (uint32_t _nframes) {
        const auto nframes = static_cast<int> (_nframes);

        if (params != verb.getParameters())
            verb.setParameters (params);

        verb.processStereo (input[0], input[1], output[0], output[1], nframes);
    }

private:
    Roboverb verb;
    Roboverb::Parameters params;
    double sampleRate;
    std::string bundlePath;
    float* input[2];
    float* output[2];
};

static const lvtk::Descriptor<Module> sDescriptor (ROBOVERB_URI);
