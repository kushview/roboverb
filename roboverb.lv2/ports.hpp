/*
    This file is part of Roboverb

    Copyright (C) 2015-2023  Kushview, LLC.  All rights reserved.

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

#pragma once

struct RoboverbPorts {
enum Index {
    AudioIn_1       = 0,
    AudioIn_2       = 1,
    AudioOut_1      = 2,
    AudioOut_2      = 3,

    Wet             = 4,
    Dry             = 5,
    RoomSize        = 6,
    Damping         = 7,
    Width           = 8,

    Comb_1          = 9,
    Comb_2          = 10,
    Comb_3          = 11,
    Comb_4          = 12,
    Comb_5          = 13,
    Comb_6          = 14,
    Comb_7          = 15,
    Comb_8          = 16,

    AllPass_1       = 17,
    AllPass_2       = 18,
    AllPass_3       = 19,
    AllPass_4       = 20,
};
};
