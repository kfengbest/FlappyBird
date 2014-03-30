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
#ifndef __KOGO_PhysicsHelper_H__
#define __KOGO_PhysicsHelper_H__

/// <description>
/// PhysicsHelper 
/// </description>
class PhysicsHelper 
{
public:
    /// <description>
    /// ignore CC_CONTENT_SCALE_FACTOR
    /// </description>
    static float pointsPerMeter();
    static float metersPerPoint();
    /// <description>
    /// consider CC_CONTENT_SCALE_FACTOR
    /// </description>
    static float pixelsPerMeter();
    static float metersPerPixel();

    /// <description>
    /// unit conversion from Point to Meter 
    /// ignore CC_CONTENT_SCALE_FACTOR
    /// </description>    
    static float points2Meter(float p);
    static Point points2Meter(const Point& pt);
    /// <description>
    /// unit conversion from Meter to Point 
    /// ignore CC_CONTENT_SCALE_FACTOR
    /// </description>    
    static Point meter2Point(const Point& pt);
    static float meter2Point(float m);       

    /// <description>
    /// unit conversion from pixel to Meter 
    /// ignore CC_CONTENT_SCALE_FACTOR
    /// </description>    
    static float pixel2Meter(float p);
    static Point pixel2Meter(const Point& pt);
    /// <description>
    /// unit conversion from Meter to pixel
    /// ignore CC_CONTENT_SCALE_FACTOR
    /// </description>    
    static Point meter2Pixel(const Point& pt);
    static float meter2Pixel(float m);       
};

#endif // __KOGO_PhysicsHelper_H__
