#include <atomic>
#include <clap/helpers/plugin.hh>
#include <cstring>
#include <iostream>
#include <sstream>

#include "./ports.hpp"
#include "./roboverb.hpp"
#include "./ui.hpp"

using Ports = RoboverbPorts;

#if __APPLE__
#    define ROBOVERB_WINDOW_API CLAP_WINDOW_API_COCOA
#elif defined(__WIN32__)
#    define ROBOVERB_WINDOW_API CLAP_WINDOW_API_WIN32
#else
#    define ROBOVERB_WINDOW_API CLAP_WINDOW_API_X11
#endif

static constexpr const char* ROBOVERB_CLAP_ID = "net.kushview.roboverb";

static const clap_plugin_descriptor_t sDescriptor = {
    .clap_version = CLAP_VERSION,
    .id           = ROBOVERB_CLAP_ID,
    .name         = "Roboverb",
    .vendor       = "Kushview",
    .url          = "https://kushview.net/roboverb",
    .manual_url   = "https://github.com/kushview/roboverb",
    .support_url  = "https://github.com/kushview/roboverb/issues",
    .version      = "2.0.0",
    .description  = "",
    .features     = { nullptr }
};

namespace roboverb {

using BaseType = clap::helpers::Plugin<clap::helpers::MisbehaviourHandler::Terminate,
                                       clap::helpers::CheckingLevel::Maximal>;

class Plugin : public BaseType {
public:
    Plugin (const clap_host* host) : BaseType (&sDescriptor, host) {
        _host = std::make_unique<HostProxy> (host);
    }

protected:
    //-------------//
    // clap_plugin //
    //-------------//
    bool init() noexcept override {
        _host->init();

        const Roboverb::Parameters defaults;
        _verb.setParameters (defaults);
        _uiParams = _rtParams = defaults;
        _verb.reset();

        for (uint32_t id = Ports::Wet; id <= Ports::AllPass_4; ++id) {
            clap_param_info_t param;
            std::strcpy (param.module, "Reverb");
            param.cookie    = nullptr;
            param.flags     = 0;
            param.id        = id;
            param.min_value = 0.0;
            param.max_value = 1.0;

            switch (id) {
                case Ports::Wet:
                    std::strcpy (param.name, "Wet");
                    param.default_value = defaults.wetLevel;
                    break;
                case Ports::Dry:
                    std::strcpy (param.name, "Dry");
                    param.default_value = defaults.dryLevel;
                    break;
                case Ports::RoomSize:
                    std::strcpy (param.name, "Room Size");
                    param.default_value = defaults.roomSize;
                    break;
                case Ports::Damping:
                    std::strcpy (param.name, "Damping");
                    param.default_value = defaults.damping;
                    break;
                case Ports::Width:
                    std::strcpy (param.name, "Width");
                    param.default_value = defaults.width;
                    break;

                case Ports::Comb_1:
                case Ports::Comb_2:
                case Ports::Comb_3:
                case Ports::Comb_4:
                case Ports::Comb_5:
                case Ports::Comb_6:
                case Ports::Comb_7:
                case Ports::Comb_8: {
                    int index           = (id - Ports::Comb_1);
                    param.default_value = _verb.toggledCombFloat (index);
                    break;
                }

                case Ports::AllPass_1:
                case Ports::AllPass_2:
                case Ports::AllPass_3:
                case Ports::AllPass_4: {
                    int index           = (id - Ports::AllPass_1);
                    param.default_value = _verb.toggledAllPassFloat (index);
                    break;
                }
            }

            _paramInfo.push_back (param);
        }
        return true;
    }

    bool activate (double sampleRate, uint32_t minFrameCount, uint32_t maxFrameCount) noexcept override {
        _verb.setSampleRate (sampleRate);
        _doUpdate.store (1);
        return true;
    }

    void deactivate() noexcept override {}
    bool startProcessing() noexcept override {
        return true;
    }
    void stopProcessing() noexcept override {}

    void update (clap_id id, double value) noexcept {
        switch (id) {
            case Ports::Damping:
                _rtParams.damping = static_cast<float> (value);
                break;
            case Ports::Dry:
                _rtParams.dryLevel = static_cast<float> (value);
                break;
            case Ports::RoomSize:
                _rtParams.roomSize = static_cast<float> (value);
                break;
            case Ports::Wet:
                _rtParams.wetLevel = static_cast<float> (value);
                break;
            case Ports::Width:
                _rtParams.width = static_cast<float> (value);
                break;

            case Ports::Comb_1:
            case Ports::Comb_2:
            case Ports::Comb_3:
            case Ports::Comb_4:
            case Ports::Comb_5:
            case Ports::Comb_6:
            case Ports::Comb_7:
            case Ports::Comb_8: {
                auto index = static_cast<int> (id - Ports::Comb_1);
                _verb.setCombToggle (index, value);
                break;
            }

            case Ports::AllPass_1:
            case Ports::AllPass_2:
            case Ports::AllPass_3:
            case Ports::AllPass_4: {
                auto index = static_cast<int> (id - Ports::AllPass_1);
                _verb.setAllPassToggle (index, value);
                break;
            }
        }
    }

    void update (const clap_event_param_value_t* ev) noexcept {
        update (ev->param_id, ev->value);
    }

    clap_process_status process (const clap_process* process) noexcept override {
        auto num_in = process->in_events->size (process->in_events);

        bool paramChanged = false;
        for (uint32_t i = 0; i < num_in; ++i) {
            auto ev = process->in_events->get (process->in_events, i);
            switch (ev->type) {
                case CLAP_EVENT_PARAM_VALUE: {
                    update ((const clap_event_param_value_t*) ev);
                    paramChanged = true;
                    break;
                }
            }
        }

        if (paramChanged) {
            _verb.setParameters (_rtParams);
            std::lock_guard<std::mutex> sl (paramMutex);
            _uiParams = _rtParams;
            _doUpdate.store (1);
        }

        _verb.processStereo (process->audio_inputs[0].data32[0],
                             process->audio_inputs[0].data32[1],
                             process->audio_outputs[0].data32[0],
                             process->audio_outputs[0].data32[1],
                             static_cast<int> (process->frames_count));

        return CLAP_PROCESS_CONTINUE;
    }

    void reset() noexcept override {}
    void onMainThread() noexcept override {}
    const void* extension (const char* id) noexcept override {
        (void) id;
        return nullptr;
    }
    bool enableDraftExtensions() const noexcept override { return false; }

    //-------------------------//
    // clap_plugin_audio_ports //
    //-------------------------//
    bool implementsAudioPorts() const noexcept override { return true; }
    uint32_t audioPortsCount (bool isInput) const noexcept override { return 1; }
    bool audioPortsInfo (uint32_t index, bool isInput, clap_audio_port_info* info) const noexcept override {
        std::strcpy (info->name, "Audio");
        info->id            = index;
        info->channel_count = 2;
        info->flags         = 0;
        info->in_place_pair = CLAP_INVALID_ID;
        info->port_type     = CLAP_PORT_STEREO;
        return true;
    }

    //---------------------------//
    // clap_plugin_timer_support //
    //---------------------------//
    bool implementsTimerSupport() const noexcept override { return true; }
    void onTimer (clap_id timerId) noexcept override {
        if (timerId != 0)
            return;

        auto content = _gui.widget();
        if (_doUpdate.load() != 0 && content != nullptr) {
            for (int p = Ports::Wet; p <= Ports::Width; ++p) {
                int index = p - Ports::Wet;
                double pv = 0.0;
                paramsValue ((uint32_t) p, &pv);
                content->update_slider (index, static_cast<float> (pv));
            }

            for (int i = 0; i < 12; ++i) {
                double v = 0.0;
                paramsValue ((clap_id) i + Ports::Comb_1, &v);
                content->update_toggle (i, 0.f != static_cast<float> (v));
            }

            while (_doUpdate.load() != 0)
                _doUpdate.store (0, std::memory_order_acquire);
        }

        _gui.idle();
    }

    //--------------------//
    // clap_plugin_params //
    //--------------------//
    bool implementsParams() const noexcept override { return true; }

    uint32_t paramsCount() const noexcept override { return _paramInfo.size(); }

    bool paramsInfo (uint32_t paramIndex, clap_param_info* info) const noexcept override {
        if (paramIndex < _paramInfo.size()) {
            *info = _paramInfo.at (paramIndex);
            return true;
        }
        return false;
    }

    bool paramsValue (clap_id paramId, double* value) noexcept override {
        bool res = true;
        switch (paramId) {
            case Ports::Wet:
                *value = _uiParams.wetLevel;
                break;
            case Ports::Dry:
                *value = _uiParams.dryLevel;
                break;
            case Ports::RoomSize:
                *value = _uiParams.roomSize;
                break;
            case Ports::Width:
                *value = _uiParams.width;
                break;
            case Ports::Damping:
                *value = _uiParams.damping;
                break;

            case Ports::Comb_1:
            case Ports::Comb_2:
            case Ports::Comb_3:
            case Ports::Comb_4:
            case Ports::Comb_5:
            case Ports::Comb_6:
            case Ports::Comb_7:
            case Ports::Comb_8: {
                auto index = static_cast<int> (paramId - Ports::Comb_1);
                *value     = _verb.toggledCombFloat (index);
                break;
            }

            case Ports::AllPass_1:
            case Ports::AllPass_2:
            case Ports::AllPass_3:
            case Ports::AllPass_4: {
                auto index = static_cast<int> (paramId - Ports::AllPass_1);
                *value     = _verb.toggledAllPassFloat (index);
                break;
            }

            default:
                res = false;
                break;
        }
        return res;
    }

    bool paramsValueToText (clap_id paramId, double value, char* display, uint32_t size) noexcept override {
        std::stringstream strm;
        strm << value;
        auto str = strm.str();
        if (str.size() >= size)
            return false;
        std::strcpy (display, strm.str().c_str());
        return true;
    }

    bool paramsTextToValue (clap_id paramId, const char* display, double* value) noexcept override {
        *value = std::atof (display);
        return true;
    }

    void paramsFlush (const clap_input_events* in,
                      const clap_output_events* out) noexcept override {
        (void) in;
        (void) out;
    }

#if 0
    // This method is meant for implementing contract checking, it isn't part of CLAP.
    // The default implementation will be slow, so consider overriding it with a faster one.
    // Returns -1 if the parameter isn't found.
    virtual int32_t getParamIndexForParamId (clap_id paramId) const noexcept;
    virtual bool isValidParamId (clap_id paramId) const noexcept;
    virtual bool getParamInfoForParamId (clap_id paramId, clap_param_info* info) const noexcept;
#endif

    //-----------------//
    // clap_plugin_gui //
    //-----------------//
    bool implementsGui() const noexcept override { return true; }

    bool guiIsApiSupported (const char* api, bool isFloating) noexcept override {
        return ! isFloating && _host->canUseTimerSupport() && 0 == std::strcmp (api, ROBOVERB_WINDOW_API);
    }

    bool guiGetPreferredApi (const char** api, bool* is_floating) noexcept override {
        *is_floating = false;
        *api         = ROBOVERB_WINDOW_API;
        return true;
    }

    bool guiCreate (const char* api, bool isFloating) noexcept override {
        if (_gui.create()) {
            _gui.setControlHandler ([this] (uint32_t ID, float value) {
                std::lock_guard<std::mutex> sl (paramMutex);
                update (ID, value);
                _verb.setParameters (_rtParams);
                _uiParams = _rtParams;
            });
            return true;
        }
        return false;
    }

    void guiDestroy() noexcept override {
        _gui.setControlHandler (nullptr);
        _gui.destroy();
    }

    bool guiSetScale (double scale) noexcept override { return true; }
    bool guiShow() noexcept override {
        _doUpdate.store (1);
        _gui.show();
        return true;
    }
    bool guiHide() noexcept override {
        _doUpdate.store (1);
        _gui.hide();
        return true;
    }
    bool guiGetSize (uint32_t* width, uint32_t* height) noexcept override {
        *width  = (uint32_t) _gui.width();
        *height = (uint32_t) _gui.height();
        return true;
    }
    bool guiCanResize() const noexcept override { return false; }
    bool guiGetResizeHints (clap_gui_resize_hints_t* hints) noexcept override { return false; }
    bool guiAdjustSize (uint32_t* width, uint32_t* height) noexcept override {
        return guiGetSize (width, height);
    }
    bool guiSetSize (uint32_t width, uint32_t height) noexcept override { return false; }
    void guiSuggestTitle (const char* title) noexcept override {}
    bool guiSetParent (const clap_window* window) noexcept override {
        return _gui.setParent (window);
    }
    bool guiSetTransient (const clap_window* window) noexcept override { return false; }

private:
    using HostProxy = clap::helpers::HostProxy<clap::helpers::MisbehaviourHandler::Terminate,
                                               clap::helpers::CheckingLevel::Maximal>;
    std::unique_ptr<HostProxy> _host;
    Roboverb _verb;
    std::vector<clap_param_info_t> _paramInfo;
    Roboverb::Parameters _uiParams, _rtParams;
    roboverb::GuiMain _gui;

    std::mutex paramMutex;
    std::atomic<int> _doUpdate { 0 };
};

} // namespace roboverb

static const clap_plugin_factory_t sFactory = {
    .get_plugin_count = [] (const clap_plugin_factory_t* factory) -> uint32_t {
        return 1;
    },

    // Retrieves a plugin descriptor by its index.
    // Returns null in case of error.
    // The descriptor must not be freed.
    // [thread-safe]
    .get_plugin_descriptor = [] (const clap_plugin_factory_t* factory, uint32_t index) -> const clap_plugin_descriptor_t* {
        return index == 0 ? &sDescriptor : nullptr;
    },

    // Create a clap_plugin by its plugin_id.
    // The returned pointer must be freed by calling plugin->destroy(plugin);
    // The plugin is not allowed to use the host callbacks in the create method.
    // Returns null in case of error.
    // [thread-safe]
    .create_plugin = [] (const clap_plugin_factory_t* factory, const clap_host_t* host, const char* plugin_id) -> const clap_plugin_t* {
        if (factory != &sFactory || 0 != std::strcmp (plugin_id, ROBOVERB_CLAP_ID))
            return nullptr;

        auto verb   = new roboverb::Plugin (host);
        auto plugin = verb->clapPlugin();

        return plugin;
    }
};

extern "C" CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION,

    .init = [] (const char* plugin_path) -> bool {
        return true;
    },

    .deinit = []() -> void {},

    .get_factory = [] (const char* factory_id) -> const void* {
        if (0 == std::strcmp (factory_id, CLAP_PLUGIN_FACTORY_ID))
            return (const void*) &sFactory;
        return nullptr;
    }
};

#include <clap/helpers/plugin.hxx>
