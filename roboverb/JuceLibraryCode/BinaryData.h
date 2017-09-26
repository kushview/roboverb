/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   roboverb_bg_jpg;
    const int            roboverb_bg_jpgSize = 243888;

    extern const char*   sphere_scope_png;
    const int            sphere_scope_pngSize = 2034111;

    extern const char*   toggle_switch_png;
    const int            toggle_switch_pngSize = 20873;

    extern const char*   red_knob_png;
    const int            red_knob_pngSize = 546683;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 4;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
