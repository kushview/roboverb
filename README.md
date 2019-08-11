#Roboverb
Interstellar Reverb - AU/VST/VST3/LV2

##Building AU/VST/VST3
These formats can be built by normal means with Projucer.  Depending on your system, 
you might have to change the module paths.

##Building LV2
Since the UI is an [lvtk:JUCEUI](https://github.com/lvtk/lvtk/blob/master/lvtk.lv2/manifest.ttl), you will also need to install [libjuce](https://github.com/kushview/libjuce)

```
./waf configure build install
```
