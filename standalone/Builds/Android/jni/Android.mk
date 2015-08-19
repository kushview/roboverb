# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
endif

LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../../roboverb/Source/AboutBox.cpp\
  ../../../../roboverb/JuceLibraryCode/BinaryData.cpp\
  ../../../../roboverb/Source/CombFilter.cpp\
  ../../../Source/Main.cpp\
  ../../../../roboverb/Source/PluginEditor.cpp\
  ../../../../roboverb/Source/PluginProcessor.cpp\
  ../../../../roboverb/Source/PluginView.cpp\
  ../../../../roboverb/Source/RMS.cpp\
  ../../../../roboverb/Source/Roboverb.cpp\
  ../../../../element/modules/element_base/element_base.cpp\
  ../../../../element/modules/element_gui/element_gui.cpp\
  ../../../../element/modules/element_models/element_models.cpp\
  ../../../../libjuce/src/modules/juce_audio_basics/juce_audio_basics.cpp\
  ../../../../libjuce/src/modules/juce_audio_devices/juce_audio_devices.cpp\
  ../../../../libjuce/src/modules/juce_audio_formats/juce_audio_formats.cpp\
  ../../../../libjuce/src/modules/juce_audio_processors/juce_audio_processors.cpp\
  ../../../../libjuce/src/modules/juce_audio_utils/juce_audio_utils.cpp\
  ../../../../libjuce/src/modules/juce_core/juce_core.cpp\
  ../../../../libjuce/src/modules/juce_cryptography/juce_cryptography.cpp\
  ../../../../libjuce/src/modules/juce_data_structures/juce_data_structures.cpp\
  ../../../../libjuce/src/modules/juce_events/juce_events.cpp\
  ../../../../libjuce/src/modules/juce_graphics/juce_graphics.cpp\
  ../../../../libjuce/src/modules/juce_gui_basics/juce_gui_basics.cpp\
  ../../../../libjuce/src/modules/juce_gui_extra/juce_gui_extra.cpp\
  ../../../../libjuce/src/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp\
  ../../../../libjuce/src/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp\
  ../../../../libjuce/src/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp\

ifeq ($(NDK_DEBUG),1)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../element/modules" -I "../../../libjuce/src/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_standalone_Standalone" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/standalone/Standalone\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.3" -D "JUCE_APP_VERSION_HEX=0x10003"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../element/modules" -I "../../../libjuce/src/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_standalone_Standalone" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/standalone/Standalone\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.3" -D "JUCE_APP_VERSION_HEX=0x10003"
  LOCAL_LDLIBS := -llog -lGLESv2
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../element/modules" -I "../../../libjuce/src/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_standalone_Standalone" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/standalone/Standalone\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.3" -D "JUCE_APP_VERSION_HEX=0x10003"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../element/modules" -I "../../../libjuce/src/modules" -O3 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=10" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_standalone_Standalone" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/standalone/Standalone\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.3" -D "JUCE_APP_VERSION_HEX=0x10003"
  LOCAL_LDLIBS := -llog -lGLESv2
endif

include $(BUILD_SHARED_LIBRARY)
