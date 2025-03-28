# Roboverb CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(Roboverb VERSION 2.0.0)

add_subdirectory(deps/JUCE)
add_subdirectory(deps/clap-juce-extensions EXCLUDE_FROM_ALL)

juce_add_plugin(Roboverb
    COMPANY_NAME Kushview                       # Specify the name of the plugin's author
    IS_SYNTH TRUE                               # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE            # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD FALSE               # Should the plugin be installed to a default location after building?
    LV2URI "https://kushview.net/plugins/roboverb"
    PLUGIN_MANUFACTURER_CODE KshV               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Rbvb                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 LV2 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Roboverb")                        # The name of the final executable, which can differ from the target name
clap_juce_extensions_plugin(TARGET Roboverb
    CLAP_ID "net.kushview.Roboverb"
    CLAP_FEATURES reverb)

target_include_directories(Roboverb PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_sources(Roboverb
    PRIVATE
        src/roboverb.cpp
        src/processor.cpp
        src/res.cpp
        src/ui/aboutbox.cpp
        src/ui/pluginview.cpp)

target_compile_definitions(Roboverb
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Roboverb
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
