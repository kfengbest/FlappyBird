/*
    Copyright 2012 NAGA.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by NAGA or its suppliers or 
	licensors.  Title to the Material remains with NAGA or its suppliers and 
	licensors.  The Material is protected by worldwide copyright laws and 
	treaty provisions.  No part of the Material may be used, copied, reproduced, 
	modified, published, uploaded, posted, transmitted, distributed, or 
	disclosed in any way without NAGA's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by NAGA in
    writing.
*/

/*
	Author		:	Yu Li
	Description	:	Helper utility for Physics Simulation
	History		:	2012, Initial implementation.
*/
#include "Impl.h"

/// <description>
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>
float PhysicsHelper::pointsPerMeter() 
{
    return 32.0f;
}

float PhysicsHelper::metersPerPoint()
{
    return 1.0f / pointsPerMeter();
}

/// <description>
/// unit conversion from Point to Meter 
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
float PhysicsHelper::points2Meter(float p)
{
    return p * metersPerPoint();
}

/// <description>
/// unit conversion from Point to Meter 
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
Point PhysicsHelper::points2Meter(const Point& pt)
{
    return Point(pt.x * metersPerPoint(), pt.y * metersPerPoint());
}

/// <description>
/// unit conversion from Meter to Point 
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
float PhysicsHelper::meter2Point(float m)
{
    return m * pointsPerMeter();
}

/// <description>
/// unit conversion from Meter to Point 
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
Point PhysicsHelper::meter2Point(const Point& pt)
{
    return Point(pt.x * pointsPerMeter(), pt.y * pointsPerMeter());
}

/// <description>
/// consider CC_CONTENT_SCALE_FACTOR
/// </description>
float PhysicsHelper::pixelsPerMeter()
{
    return pointsPerMeter() * CC_CONTENT_SCALE_FACTOR();
}

float PhysicsHelper::metersPerPixel()
{
    return 1.0f / pixelsPerMeter();
}

/// <description>
/// unit conversion from pixel to Meter 
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
Point PhysicsHelper::pixel2Meter(const Point& pt)
{
    return Point(pt.x * metersPerPixel(),  pt.y * metersPerPixel());
}

float PhysicsHelper::pixel2Meter(float p)
{
    return p * metersPerPixel();
}

/// <description>
/// unit conversion from Meter to pixel
/// ignore CC_CONTENT_SCALE_FACTOR
/// </description>    
Point PhysicsHelper::meter2Pixel(const Point& pt)
{
    return Point(pt.x * pixelsPerMeter(), pt.y * pixelsPerMeter());
}

float PhysicsHelper::meter2Pixel(float m)
{
    return m * pixelsPerMeter();
}
