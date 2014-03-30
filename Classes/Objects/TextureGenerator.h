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
	Description	:	Texture Generator for terrain Texture
	History		:	2014, Initial implementation.
*/
#ifndef __KOGO_TextureGenerator_H__
#define __KOGO_TextureGenerator_H__

#include "cocos2d.h"

/// <description>
/// terrain texture generator 
/// </description>
class TextureGenerator
{
public:
    /// <description>
    /// texture effects, use the enum class for various of effects 
    /// </description>
    enum Effect 
    {
        /// generate stripes 
        kEffectStripes  = 0x0001, 
        /// rendering noises
        kEffectNoise    = 0x0002,
        /// rendering gradient 
        kEffectGradient = 0x0004,
        /// rendering highlight
        kEffectHighlight= 0x0008,
        /// all effect 
        kAllEffects = kEffectStripes | kEffectNoise | kEffectGradient | kEffectHighlight
    };

    /// <description>
    /// generate texture with specified effects 
    /// </description>
    static cocos2d::Texture2D* Generate(int texSize, const Color4F& theme = Color4F(1.0f,0.8431f,0.f,1.0f), int effect = Effect::kAllEffects);
};

#endif // __KOGO_TextureGenerator_H__
