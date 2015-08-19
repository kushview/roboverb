/*
  CombFilter.h - This file is part of Roboverb

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

#ifndef KV_COMB_FILTER_H
#define KV_COMB_FILTER_H

#include "JuceHeader.h"


class CombFilter
{
public:
    enum Direction
    {
        FeedBack = 0,
        FeedForward
    };

    explicit CombFilter (Direction d = FeedBack);
    virtual ~CombFilter();

    float process (const float input);
    void resize (const int newSize);
    void setInputGain (const float gain) { inputGain = gain; }
    void setDirection (const Direction d) { direction = d; }
    void setFeedback (const float newFeedback) { feedback = newFeedback; }

protected:
    HeapBlock<float> buffer;
    int bufferIndex, bufferSize;
    Direction direction;
    float last;
    float feedback;
    float inputGain;
};

#endif /* KV_COMB_FILTER_H */
