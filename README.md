# Roboverb
Robotic reverb at your fingertips...

![Screenshot](data/screenshot.png)

#### Status of Formats
- [x] LV2
- [x] CLAP
- [x] AU/VST/VST3
- [ ] AAX

#### Building
Roboverb can be built with CMake.

```bash
git submodule update --init --recursive --depth=1
cmake -Bbuild -GNinja
cd build
ninja -j4
```
