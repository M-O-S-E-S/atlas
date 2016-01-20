
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef AT_GLOBALS_HPP
#define AT_GLOBALS_HPP


#include <math.h>


enum
{
    AT_X = 0,
    AT_Y = 1,
    AT_Z = 2,
    AT_W = 3,
    AT_H = 0,
    AT_P = 1,
    AT_R = 2,
    AT_RED = 0,
    AT_GREEN = 1,
    AT_BLUE = 2,
    AT_ALPHA = 3
};

#define AT_PI    (3.14159265358979323846)

#define AT_DEFAULT_TOLERANCE    (1E-12)

// Various useful macros

#define AT_SQR(x)  ( (x) * (x) )

// Convert from degrees to radians
#define AT_DEG2RAD(x)  ( (x) * AT_PI / 180.0 )

// Convert from radians to degrees
#define AT_RAD2DEG(x)  ( (x) * 180.0 / AT_PI )

// Determine if two floating-point values are close enough to be equal
#define AT_EQUAL(x,y)  ( fabs((x) - (y)) < AT_DEFAULT_TOLERANCE )

// Find the maximum (or minimum) of any two values
#define AT_MAX(x,y) ( ((x) > (y)) ? (x) : (y) )
#define AT_MIN(x,y) ( ((x) < (y)) ? (x) : (y) )


// Constants for use in conversion to/from Euler rotations
// The three axes of rotation are specified in left to right order
// i.e. XYZ means rotate around the X-axis, then the Y-axis, finally the Z-axis
// The last letter ('S' or 'R') indicates static or relative rotation axes.
// With static axes, the coordinate axes stay fixed during rotations; each
// rotation around a particular axis rotates points the same way, regardless
// of what other rotations have been done. Relative coordinate axes move with
// each rotation; two X-axis rotations will move in different directions
// if there is an intervening Y or Z-axis rotation. The two types are opposites
// of each other: XYZ static produces the same effect as ZYX relative.
enum atMathEulerAxisOrder
{
    AT_EULER_ANGLES_XYZ_S,
    AT_EULER_ANGLES_XZY_S,
    AT_EULER_ANGLES_YXZ_S,
    AT_EULER_ANGLES_YZX_S,
    AT_EULER_ANGLES_ZXY_S,
    AT_EULER_ANGLES_ZYX_S,
    
    AT_EULER_ANGLES_XYX_S,
    AT_EULER_ANGLES_XZX_S,
    AT_EULER_ANGLES_YXY_S,
    AT_EULER_ANGLES_YZY_S,
    AT_EULER_ANGLES_ZXZ_S,
    AT_EULER_ANGLES_ZYZ_S,

    AT_EULER_ANGLES_XYZ_R,
    AT_EULER_ANGLES_XZY_R,
    AT_EULER_ANGLES_YXZ_R,
    AT_EULER_ANGLES_YZX_R,
    AT_EULER_ANGLES_ZXY_R,
    AT_EULER_ANGLES_ZYX_R,
    
    AT_EULER_ANGLES_XYX_R,
    AT_EULER_ANGLES_XZX_R,
    AT_EULER_ANGLES_YXY_R,
    AT_EULER_ANGLES_YZY_R,
    AT_EULER_ANGLES_ZXZ_R,
    AT_EULER_ANGLES_ZYZ_R
};


#endif

