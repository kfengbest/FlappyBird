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
#include "Impl.h"

/// <description>
/// constructor with the texture size
/// </description>
Sky::Sky(int texSize /*= 1024*/)
    : mTextureSize(texSize)
    , mOffsetX(0.f)
    , mScale(1.0f)
{    
}

/// <description>
/// destructions
/// </description>
Sky::~Sky()
{

}

/// <description>
/// actual initialization 
/// </description>
bool Sky::init()
{      
    auto sprite = generateSprite();
    if (!sprite)
        return false;

    this->addChild(sprite, DEPTH_SKY, TAG_SKY);
    return true;
}
		
/// <description>
/// actual initialization 
/// </description>
Sprite* Sky::generateSprite()
{	
    Size sz = Director::getInstance()->getWinSize();
	Texture2D *texture = this->generateTexture();
	
    /// keep the same aspect 
	float w = sz.width/sz.height * mTextureSize;
	float h = mTextureSize;
	Rect rect = Rect(0, 0, w, h);
	
	cocos2d::Sprite *sprite = Sprite::createWithTexture(texture,rect);
	Texture2D::TexParams tp = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
	sprite->getTexture()->setTexParameters(&tp);
	sprite->setAnchorPoint(Point(1.0f/8.0f, 0));
	sprite->setPosition(Point(sz.width/8, 0));
	
	return sprite;
}

/// <description>
/// generate texture
/// </description>
Texture2D* Sky::generateTexture() 
{
	RenderTexture *rt = RenderTexture::create(mTextureSize, mTextureSize);	
	Color3B c(140, 205, 221);
	Color4F cf = ccc4FFromccc3B(c);
	
	rt->beginWithClear(cf.r,cf.g,cf.b,cf.a);
	
	// layer 1: gradient	
	float gradientAlpha = 0.3f;
	
	glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	Vect vertices[4];
	Color4F colors[4];
	int nVertices = 0;
	
	vertices[nVertices] = Vect(0, 0);
	colors[nVertices++] = Color4F(1, 1, 1, 0);
	vertices[nVertices] = Vect(mTextureSize, 0);
	colors[nVertices++] = Color4F(1, 1, 1, 0);
	
	vertices[nVertices] = Vect(0, mTextureSize);
	colors[nVertices++] = Color4F(1, 1, 1, gradientAlpha);
	vertices[nVertices] = Vect(mTextureSize, mTextureSize);
	colors[nVertices++] = Color4F(1, 1, 1, gradientAlpha);

	// adjust vertices for retina
	for (int i=0; i<nVertices; i++) 
    {
		vertices[i].x *= CC_CONTENT_SCALE_FACTOR();
		vertices[i].y *= CC_CONTENT_SCALE_FACTOR();
	}
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
	
	///glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
	
	// layer 2: noise
	
	cocos2d::Sprite *s = cocos2d::Sprite::create("noise.png");
    BlendFunc func;
    func.src = GL_DST_COLOR;
    func.dst = GL_ZERO;
    s->setBlendFunc(func);

	s->setPosition(mTextureSize/2, mTextureSize/2);
	s->setScale((float)mTextureSize/512.0f*CC_CONTENT_SCALE_FACTOR());
	glColor4f(1,1,1,1);
	s->visit();
	
	rt->end();
	
	return rt->getSprite()->getTexture();
}

/// <description>
/// set the x offset of the sky layer
/// </description>
void Sky::setOffsetX(float offsetX)
{
    auto sprite = (Sprite*)getChildByTag(TAG_SKY);
    if (!sprite)
        return;

	if (mOffsetX != offsetX) 
    {        
        mOffsetX = offsetX;
		Size size = sprite->getTextureRect().size;
        sprite->setTextureRect(Rect(mOffsetX, 0, size.width, size.height));
	}
}

/// <description>
/// set the scale of the sky layer
/// </description>
void Sky::setScale(float scale) 
{
    auto sprite = (Sprite*)getChildByTag(TAG_SKY);
    if (!sprite)
        return;

	if (mScale != scale) 
    {      
        Size sz = Director::getInstance()->getWinSize();
		const float minScale = sz.height / (float)mTextureSize;
		if (scale < minScale) {
			mScale = minScale;
		} else {
			mScale = scale;
		}

		sprite->setScale(mScale);
	}
}
