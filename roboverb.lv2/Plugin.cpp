
#include <cstdlib>
#include <lvtk/plugin.hpp>
#include <juce/audio_basics.h>

#include "Ports.h"
#include "Roboverb.h"

#define ROBOVERB_URI  "https://kushview.net/plugins/roboverb"

class Module final : public lvtk::Instance<Module, lvtk::State>
{
public:
	Module (double rate, const lvtk::String& bundle, const lvtk::FeatureList& f)
		: Instance (rate, bundle, f),
		  stereo (true), 
		  sampleRate (rate), 
		  bundlePath (String::fromUTF8 (bundle.c_str()))
	{
		audio.setSize (2, 2048);
		stateKey 	= map ("https://kushview.net/plugins/roboverb#state");
		atomString 	= map (LV2_ATOM__String);
	}

	~Module() {}

	void connect_port (uint32 port, void* data)
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
				verb.setCombToggle (port - RoboverbPorts::Comb_1, value >= 0.5f);
			} break;

			case RoboverbPorts::AllPass_1:
			case RoboverbPorts::AllPass_2:
			case RoboverbPorts::AllPass_3:
			case RoboverbPorts::AllPass_4: {
				const auto value = *((float*) data);
				verb.setAllPassToggle (port - RoboverbPorts::AllPass_1, value >= 0.5f);
			} break;
		}
	}
	
	void activate()
	{
		audio.clear();
		verb.reset();
		verb.setSampleRate (sampleRate);
	}

	void deactivate()
	{
		// noop
	}

	void run (uint32 _nframes)
	{
		const int nchans = 2;
		const auto nframes = static_cast<int> (_nframes);

		if (params != verb.getParameters())
			verb.setParameters (params);
		
		// verb processes in-place so copy the data.
		audio.setSize (nchans, nframes, false, false, true);

		for (int i = 0; i < nchans; ++i)
			audio.copyFrom (i, 0, input [i], nframes);

		verb.processStereo (audio.getWritePointer (0),
							audio.getWritePointer (1),
							nframes);

		for (int i = 0; i < nchans; ++i)
			memcpy (output [i], audio.getReadPointer (i),
					sizeof (float) * (size_t) nframes);
	}

	lvtk::StateStatus save (lvtk::StateStore &store, uint32_t flags, const lvtk::FeatureList&)
    {
		return lvtk::STATE_SUCCESS;
    }

    lvtk::StateStatus restore (lvtk::StateRetrieve &retrieve, 
							   uint32_t flags, const 
							   lvtk::FeatureList &features) 
    {
        return lvtk::STATE_SUCCESS;
    }

	LV2_URID stateKey = 0;
	LV2_URID atomString = 0;

private:
	Roboverb verb;
	Roboverb::Parameters params;
	bool stereo = true;
	double sampleRate;
	String bundlePath;
	int blockSize = 1024;
	AudioBuffer<float> audio;
	float* input [2];
	float* output [2];
};

static const lvtk::Plugin<Module> roboverb (ROBOVERB_URI);
