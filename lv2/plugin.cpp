
#include <cstdlib>
#include <lv2/core/lv2.h>
#include <lv2/ui/ui.h>

#include "./ports.h"
#include "../roboverb/Source/PluginProcessor.h"

#define ROBOVERB_URI  "https://kushview.net/plugins/roboverb"

class Module final
{
public:
	Module (double rate, const char* path, const bool _stereo = true)
		: stereo (_stereo),
		  sampleRate (rate),
		  bundlePath (String::fromUTF8 (path))
	{
		audio.setSize (2, 2048);
	}

	~Module() {}

	void connectPort (uint32 port, void* data)
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
				output[0] = (float*) data;
				break;
			case RoboverbPorts::AudioOut_2:
				output[1] = (float*) data;
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
			audio.copyFrom (0, 0, input [i], nframes);

		verb.processStereo (audio.getWritePointer (0),
							audio.getWritePointer (1),
							nframes);

		for (int i = 0; i < nchans; ++i)
			memcpy (output [i], audio.getReadPointer (i),
					sizeof (float) * (size_t) nframes);
	}

private:
	Roboverb verb;
	Roboverb::Parameters params;
	bool stereo = true;
	double sampleRate;
	String bundlePath;
	int blockSize = 1024;
	AudioSampleBuffer audio;
	float* input [2];
	float* output [2];
};

static LV2_Handle instantiate (const LV2_Descriptor* descriptor,
                               double rate, const char* bundle_path,
                               const LV2_Feature* const* features)
{
	ignoreUnused (features);
	auto module = std::unique_ptr<Module> (new Module (rate, bundle_path));
	return static_cast<LV2_Handle> (module.release());
}

static void connect_port (LV2_Handle instance, uint32_t port, void* data)
{
	(static_cast<Module*>(instance))->connectPort (port, data);
}

static void activate (LV2_Handle instance)
{
	(static_cast<Module*>(instance))->activate();
}

static void run (LV2_Handle instance, uint32_t nframes)
{
	(static_cast<Module*>(instance))->run (nframes);
}

static void deactivate (LV2_Handle instance)
{
	(static_cast<Module*>(instance))->deactivate();
}

static void cleanup (LV2_Handle instance)
{
	delete static_cast<Module*> (instance);
}

static const void* extension_data (const char* uri) {
	return nullptr;
}

extern "C" {

static const LV2_Descriptor descriptor = {
	ROBOVERB_URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    switch (index) {
    case 0:  return &descriptor;
    default: return nullptr;
    }
}

}
