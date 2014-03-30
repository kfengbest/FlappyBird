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
	Description	:	Sky simulation 
	History		:	2014, Initial implementation.
*/
#ifndef __KOGO_SKY_H__
#define __KOGO_SKY_H__

/// <description>
/// simulating the Sky system 
/// </description>
class Sky 
    : public cocos2d::Node, public NAGA Singleton<Sky>
{
public:
    CREATE_FUNC(Sky);    
    virtual ~Sky();

public:
    /// <description>
    /// set the offset of the sky layer
    /// </description>
    void setOffsetX(float offsetX);

    /// <description>
    /// set the scale of the sky layer
    /// </description>
    void setScale(float scale);

protected:
    /// <description>
    /// constructor with the texture size
    /// </description>
    Sky(int texSize = 1024);

    /// <description>
    /// layer initialization
    /// </description>
    virtual bool init() override;

    /// <description>
    /// generate sprite with specified texture size
    /// </description>
    cocos2d::Sprite* generateSprite();

    /// <description>
    /// generate texture
    /// </description>
    Texture2D* generateTexture();

private:
    /// <description>
    /// texture size
    /// </description>
    int mTextureSize;
    
    /// <description>
    /// sprite scale/offset
    /// </description>
    float mScale, mOffsetX;
};

#endif // __KOGO_SKY_H__
