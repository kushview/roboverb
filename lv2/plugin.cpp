
#include <cstdlib>
#include <lv2/core/lv2.h>

#include "../roboverb/Source/PluginProcessor.h"

#define ROBOVERB_URI  "https://kushview.net/products/roboverb"

class Module final
{
public:
	Module (double rate, const char* path)
		: sampleRate (rate),
		  bundlePath (String::fromUTF8 (path))
	{}

	~Module() {}

	void connectPort (uint32 port, void* data) { }
	
	void activate()
	{ 
		processor.prepareToPlay (sampleRate, blockSize);
	}

	void deactivate()
	{ 
		processor.releaseResources();
	}

	void run (uint32 nframes)
	{
		AudioSampleBuffer audio (2, nframes);
		MidiBuffer midi;
		processor.processBlock (audio, midi);
	}

private:
	RoboverbAudioProcessor processor;
	double sampleRate;
	String bundlePath;
	int blockSize = 1024;
};

static LV2_Handle instantiate (const LV2_Descriptor* descriptor,
                               double rate, const char* bundle_path,
                               const LV2_Feature* const* features)
{
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

static const void* extension_data (const char* uri) { return nullptr; }

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
