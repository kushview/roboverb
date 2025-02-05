#include <clap/helpers/plugin.hh>
#include <cstring>

#include "ports.hpp"

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
    Plugin (const clap_host* host) : BaseType (&sDescriptor, host) {}

protected:
    //---------------------------//
    // clap_plugin_timer_support //
    //---------------------------//
    bool implementsTimerSupport() const noexcept override { return true; }
    void onTimer (clap_id timerId) noexcept override {
        if (timerId != 0)
            return;
    }

    //-----------------//
    // clap_plugin_gui //
    //-----------------//
    bool implementsGui() const noexcept override { return false; }
    bool guiIsApiSupported(const char *api, bool isFloating) noexcept override { return false; }
    bool guiGetPreferredApi(const char **api, bool *is_floating) noexcept override {
        return false;
    }
    bool guiCreate(const char *api, bool isFloating) noexcept override { return false; }
    void guiDestroy() noexcept override {}
    bool guiSetScale(double scale) noexcept override { return false; }
    bool guiShow() noexcept override { return false; }
    bool guiHide() noexcept override { return false; }
    bool guiGetSize(uint32_t *width, uint32_t *height) noexcept override { return false; }
    bool guiCanResize() const noexcept override { return false; }
    bool guiGetResizeHints(clap_gui_resize_hints_t *hints) noexcept override { return false; }
    bool guiAdjustSize(uint32_t *width, uint32_t *height) noexcept override {
        return guiGetSize(width, height);
    }
    bool guiSetSize(uint32_t width, uint32_t height) noexcept override { return false; }
    void guiSuggestTitle(const char *title) noexcept override {}
    bool guiSetParent(const clap_window *window) noexcept override { return false; }
    bool guiSetTransient(const clap_window *window) noexcept override { return false; }

private:
};

} // namespace roboverb

static const clap_plugin_factory_t _factory = {
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
        if (factory != &_factory || 0 != std::strcmp (plugin_id, ROBOVERB_CLAP_ID))
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
            return (const void*) &_factory;
        return nullptr;
    }
};

#include <clap/helpers/plugin-proxy.hxx>