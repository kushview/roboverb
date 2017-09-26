/*
  CombFilter.cpp - This file is part of Roboverb

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

#include "CombFilter.h"

CombFilter::CombFilter (Direction d)
    : bufferIndex(0), bufferSize(0),
      direction(d), last(0.0f),
      inputGain(0)
{ }

CombFilter::~CombFilter() { }

float CombFilter::process (const float input)
{
    jassert(bufferSize > 0);
    if (CombFilter::FeedBack == direction)
    {
        const float output = buffer [bufferIndex];
        last = output; //(output * (1.0f - damp)) + (last * damp);
        JUCE_UNDENORMALISE (last);

        float temp = input + (last * feedback);
        JUCE_UNDENORMALISE (temp);
        buffer[bufferIndex] = temp;

        bufferIndex = (bufferIndex + 1) % bufferSize;
        return output;
    }
    else if (CombFilter::FeedForward == direction)
    {
        const float delayed = last = buffer [bufferIndex];
        JUCE_UNDENORMALISE(last);

        float temp = input;
        JUCE_UNDENORMALISE(temp);
        buffer [bufferIndex] = temp;

        bufferIndex = (bufferIndex + 1) % bufferSize;
        return (delayed * 1.0f) + (input * inputGain);
    }

    return input;
}

void CombFilter::resize (const int newSize)
{
    if (bufferSize != newSize)
    {
        buffer.realloc ((size_t)newSize, sizeof(float));
        buffer.clear (newSize);
        bufferSize = newSize;
        bufferIndex = 0;
    }
}
