#include "cocos2d.h"
USING_NS_CC;

#define MIN3(x,y,z)  ((y) <= (z) ? \
    ((x) <= (y) ? (x) : (y)) \
    : \
    ((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
    ((x) >= (y) ? (x) : (y)) \
    : \
    ((x) >= (z) ? (x) : (z)))

/// <description>
/// r,g,b values are from 0 to 1
/// h = [0,360], s = [0,1], v = [0,1]
///		if s == 0, then h = -1 (undefined)
/// </description>
Color4F RGBtoHSV( const Color4F& clr)
{    
    float r = clr.r, g = clr.g, b = clr.b,a=clr.a;
    float min = MIN3( r, g, b );
    float max = MAX3( r, g, b );

    float delta = max - min, h = 0.f, s = 0.f, v = max;
    if( max != 0 )
        s = delta / max;	// s
    else {
        // r = g = b = 0		// s = 0, v is undefined
        return Color4F(-1,0,v,a);
    }

    if( r == max )
        h = ( g - b ) / delta;		// between yellow & magenta
    else if( g == max )
        h = 2 + ( b - r ) / delta;	// between cyan & yellow
    else
        h = 4 + ( r - g ) / delta;	// between magenta & cyan
    h *= 60;				// degrees
    if( h < 0 )
        h += 360;

    return Color4F(h,s,v,a);
}

/// <description>
/// convert HSV to RGB
/// </description>
Color4F HSVtoRGB( const Color4F& r )
{
    float h = r.r, s = r.g, v = r.b, a = r.a;
    if( s == 0 ) 
        return Color4F(v,v,v,r.a);

    h /= 60;			        // sector 0 to 5
    int i = floor( h );
    float f = h - i;			// factorial part of h
    float p = v * ( 1 - s );
    float q = v * ( 1 - s * f );
    float t = v * ( 1 - s * ( 1 - f ) );

    switch( i ) 
    {
    case 0:
        return Color4F(v,t,p,a);
    case 1:
        return Color4F(q,v,p,a);
    case 2:
        return Color4F(p,v,t,a);
    case 3:
        return Color4F(p,q,v,a);
    case 4:
        return Color4F(t,p,v,a);
    default:		// case 5:        
        break;
    }

    return Color4F(v,p,q,a);
}
