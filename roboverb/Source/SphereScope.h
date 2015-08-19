/*
  SphereScope.h - This file is part of Roboverb

  Roboverb: robotic voice and ambience audio effect
  Copyright (C) 2015  Kushview, LLC.  All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SPHERESCOPE_H_INCLUDED
#define SPHERESCOPE_H_INCLUDED

#define SPHERE_DECAY_RATE    (1.0f - 3E-6f)

#include "JuceHeader.h"

class SphereScope : public Component
{
public:
    SphereScope()
    {
        img = ImageCache::getFromMemory (BinaryData::sphere_scope_png,
                                         BinaryData::sphere_scope_pngSize);
        numSteps = img.getHeight() / img.getWidth();
        scale = (float) numSteps - 10.f;
        peakDecay = 1.0;
        peak = 0;
        peakHold = 0;
    }

    ~SphereScope() { }

    inline void setValue (const float val)
    {
        value = 10.f + Decibels::gainToDecibels (val, -70.0f);
        int level = getIECScale (value);
        if (peak < level)
        {
            peak = level;
            peakHold = 0;
            peakDecay = SPHERE_DECAY_RATE;

        }
        else if (++peakHold > 16)
        {
            peak = int (float (peak * peakDecay));
            if (peak < level) {
                peak = level;
            } else {
                peakDecay *= peakDecay;
            }
        }
    }

    inline void paint (Graphics& g) override
    {
        g.drawImage(img, 0, 0, getWidth(), getHeight(),
                         0, img.getWidth() * peak,
                         img.getWidth(), img.getWidth());
        /* g.setColour (Colours::white);
        g.drawFittedText(String(getIECScale(value)), 0, 0, getWidth(), getHeight(),
                          Justification::centred, 1); */
    }

    inline int getIECScale (const float dB) const
    {
        float defaultScale = 1.0;

        if (dB < -70.0)
            defaultScale = 0.0;
        else if (dB < -60.0)
            defaultScale = (dB + 70.0) * 0.0025;
        else if (dB < -50.0)
            defaultScale = (dB + 60.0) * 0.005 + 0.025;
        else if (dB < -40.0)
            defaultScale = (dB + 50.0) * 0.0075 + 0.075;
        else if (dB < -30.0)
            defaultScale = (dB + 40.0) * 0.015 + 0.15;
        else if (dB < -20.0)
            defaultScale = (dB + 30.0) * 0.02 + 0.3;
        else // if (dB < 0.0)
            defaultScale = (dB + 20.0) * 0.025 + 0.5;

        int iecScale = defaultScale * scale;
        if (iecScale >= numSteps)
            iecScale = numSteps - 1;

        return iecScale;
    }

private:
    Image img;
    int numSteps;
    int index;
    float scale, value;
    int peak, peakHold;
    float peakDecay;
};



#endif  // SPHERESCOPE_H_INCLUDED
