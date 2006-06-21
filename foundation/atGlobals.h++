
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
    AT_R = 2
};

#define AT_PI    (3.14159265358979)

#define AT_DEFAULT_TOLERANCE    (1E-12)

// Various useful macros

#define AT_SQR(x)  ( (x) * (x) )

// Convert from degrees to radians
#define AT_DEG2RAD(x)  ( (x) * AT_PI / 180.0 )

// Convert from radians to degrees
#define AT_RAD2DEG(x)  ( (x) * 180.0 / AT_PI )

// Determine if two floating-point values are close enough to be equal
#define AT_EQUAL(x,y)  ( fabs((x) - (y)) < AT_DEFAULT_TOLERANCE )


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

// Windows-specific DLL linkage directives.  Windows requires explicit export
// and import instructions for its shared libraries (DLL's).  The following
// sections define a token AT_*_DLL for the ATLAS library that specifies
// the linkage.  The token is defined conditionally. If the library itself 
// is being compiled the token expands into an export directive.  If a module 
// that is using the library is being compiled, the token expands into an
// import directive.  On platforms other than Windows, the tokens expand to
// nothing.
//
#ifdef WIN32
    // Atlas library
    #ifdef ATLAS_EXPORTS
        #define ATLAS_DLL  __declspec(dllexport)
    #else
        #define ATLAS_DLL  __declspec(dllimport)
    #endif
#else
    // Define all tokens to be nothing
    #define ATLAS_DLL
#endif

// Under Windows, define the sleep() and usleep() functions as macros of
// the Windows Sleep() function
#ifdef _MSC_VER
    #include <windows.h>
    #include <wingdi.h>

    // Sleep() takes milliseconds so multiply x by 1000 for sleep()
    #define sleep(x)  Sleep((x) * 1000)

    // Sleep() takes milliseconds, so divide x by 1000 for usleep()
    // if the result of (x/1000) is zero, the thread will still sleep
    // (give up the processor).
    #define usleep(x) Sleep((x) / 1000)
#endif


#endif

