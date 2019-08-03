

#include <cstdlib>
#include <cstring>
#include <lv2/core/lv2.h>
#include <lv2/ui/ui.h>
#include <juce/juce.h>

#include "./ports.h"
#include "../roboverb/Source/PluginView.h"

#define ROBOVERB_JUCEUI_URI "https://kushview.net/plugins/roboverb/juceui"

class ModuleUI final {
public:
    ModuleUI (const char* _pluginURI, 
              const char* _bundlePath,
              LV2UI_Write_Function _write,
              LV2UI_Controller _controller)
        : pluginURI (String::fromUTF8 (_pluginURI)),
          bundlePath (String::fromUTF8 (_bundlePath)),
          write (_write),
          controller (_controller)
    { }
    
    void show() {}
    void hide() {}

    void writeToPort (uint32_t port, float value) {
        write (controller, port, sizeof (float), 0, &value);
    }

    void portEvent (uint32_t port, uint32_t size,
                    uint32_t format, const void*  buffer)
    {
        if (format != 0 || size != sizeof (float))
            return;
        
        const float value = *((float*) buffer);
        const bool boolValue = value >= 0.5f;

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

private:
    const String pluginURI;
    const String bundlePath;
    LV2UI_Write_Function write;
    LV2UI_Controller controller;
    PluginView view;
};

static LV2UI_Handle instantiate_component (const struct _LV2UI_Descriptor* descriptor,
                                           const char*                     plugin_uri,
                                           const char*                     bundle_path,
                                           LV2UI_Write_Function            write_function,
                                           LV2UI_Controller                controller,
                                           LV2UI_Widget*                   widget,
                                           const LV2_Feature* const*       features)
{
    auto module = std::unique_ptr<ModuleUI> (new ModuleUI (plugin_uri, bundle_path, write_function, controller));
    *widget = module->getComponentAsWidget();
    
    return static_cast<LV2UI_Handle> (module.release());
}

static void cleanup (LV2UI_Handle ui) {
    delete static_cast <ModuleUI*> (ui);
}

static void port_event (LV2UI_Handle ui, uint32_t port, uint32_t size,
                        uint32_t format, const void*  buffer)
{
    (static_cast<ModuleUI*> (ui))->portEvent (port, size, format, buffer);
}

static int show (LV2UI_Handle ui)
{
    (static_cast<ModuleUI*> (ui))->show();
    return 0;
}

static int hide (LV2UI_Handle ui)
{
    (static_cast<ModuleUI*> (ui))->hide();
    return 0;
}

static const LV2UI_Show_Interface showInterface = {
    show, 
    hide
};

static const void* extension_data (const char* uri) {
    if (strcmp (uri, LV2_UI__showInterface) == 0)
        return &showInterface;
    return nullptr;
}

extern "C" {

static const LV2UI_Descriptor uiDescriptor = {
    ROBOVERB_JUCEUI_URI,
    instantiate_component,
    cleanup,
    port_event,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2UI_Descriptor* lv2ui_descriptor (uint32_t index)
{
    switch (index) {
    case 0:
        return &uiDescriptor;
    default:
        return NULL;
    }
}

}
