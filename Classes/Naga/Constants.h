#ifndef __Constants_H__
#define __Constants_H__

/// <description>
/// z-depth for each layer
/// </description>
enum LayerDepth 
{
    /// <description>
    /// static background depth
    /// </description>
    kZStaticBKG = 0,
    /// <description>
    /// scroll background depth
    /// </description>
    kZScrollBKG = 5,
    /// <description>
    /// game layer depth 
    /// </description>
    kZGameLayer = 10,
    /// <description>
    /// foreground layer depth
    /// </description>
    kZForeGround = 15
};

/// <description>
/// tag value for each layer
/// </description>
enum ObjectTag 
{
    /// <description>
    /// static background depth
    /// </description>
    kTagStaticBKG = 0,
    /// <description>
    /// scroll background depth
    /// </description>
    kTagScrollBKG = 5,
    /// <description>
    /// game layer depth 
    /// </description>
    kTagGameLayer = 10,
    /// <description>
    /// foreground layer depth
    /// </description>
    kTagForeGround = 15,
    /// <description>
    /// the hero 
    /// </description>
    kTagHero = 1,
};

#endif // __Constants_H__
