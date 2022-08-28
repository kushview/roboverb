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

#include <lvtk/plugin.hpp>

#include "Ports.h"
#include "Roboverb.h"

#define ROBOVERB_URI  "https://kushview.net/plugins/roboverb"

class Module final : public lvtk::Plugin<Module>
{
public:
	Module (const lvtk::Args& args)
		: Plugin (args),
		  sampleRate (args.sample_rate), 
		  bundlePath (args.bundle)
	{ }

	~Module() {}

	void connect_port (uint32_t port, void* data)
	{
		switch (port)
		{
			case RoboverbPorts::AudioIn_1:
				input [0] = (float*) data;
				break;
			case RoboverbPorts::AudioIn_2:
				input [1] = (float*) data;
				break;
			case RoboverbPorts::AudioOut_1:
				output [0] = (float*) data;
				break;
			case RoboverbPorts::AudioOut_2:
				output [1] = (float*) data;
				break;
		}

		// Lilv will connect NULL on instantiate... just return
		if (data == nullptr)
			return;

		switch (port)
		{
			case RoboverbPorts::Wet:
				params.wetLevel = *((float*) data);
				break;
			case RoboverbPorts::Dry:
				params.dryLevel = *((float*) data);
				break;
			case RoboverbPorts::RoomSize:
				params.roomSize = *((float*) data);
				break;
			case RoboverbPorts::Width:
				params.width = *((float*) data);
				break;
			case RoboverbPorts::Damping:
				params.damping = *((float*) data);
				break;

			case RoboverbPorts::Comb_1:
			case RoboverbPorts::Comb_2:
			case RoboverbPorts::Comb_3:
			case RoboverbPorts::Comb_4:
			case RoboverbPorts::Comb_5:
			case RoboverbPorts::Comb_6:
			case RoboverbPorts::Comb_7:
			case RoboverbPorts::Comb_8: {
				const auto value = *((float*) data);
				verb.setCombToggle (port - RoboverbPorts::Comb_1, value > 0.f);
			} break;

			case RoboverbPorts::AllPass_1:
			case RoboverbPorts::AllPass_2:
			case RoboverbPorts::AllPass_3:
			case RoboverbPorts::AllPass_4: {
				const auto value = *((float*) data);
				verb.setAllPassToggle (port - RoboverbPorts::AllPass_1, value > 0.f);
			} break;
		}
	}
	
	void activate()
	{
		verb.reset();
		verb.setSampleRate (sampleRate);
	}

	void deactivate()
	{
		// noop
	}

	void run (uint32_t _nframes)
	{
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
	float* input [2];
	float* output [2];
};

static const lvtk::Descriptor<Module> roboverb (ROBOVERB_URI);
