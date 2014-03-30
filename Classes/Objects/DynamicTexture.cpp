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
	Description	:	OpenGL ES Framebuffer Object
	History		:	2014, Initial implementation.
*/
#include "Naga/NagaLib.h"
#include "cocos2d.h"
#include "ccUtils.h"
USING_NS_CC;

#include "Framebuffer.h"
#include "DynamicTexture.h"

/// <description>
/// constructor
/// </description>
DynamicTexture::DynamicTexture()
	: mTexture(nullptr), mPixelFormat(Texture2D::PixelFormat::RGBA8888)
    , mClearColor(Color4F::WHITE)
    , mClearDepth(1.0f)
    , mClearStencil(0)
{
}

/// <description>
/// destructor
/// </description>
DynamicTexture::~DynamicTexture()
{
	CC_SAFE_RELEASE(mTexture);
}

/// <description>
/// clear the color/depth-stencil buffer
/// </description>
void DynamicTexture::Clear(const Color4F& clr, float depthValue/*=1*/, 
        int stencilValue/*=0*/,GLbitfield flag /*= GL_COLOR_BUFFER_BIT*/)
{
   mClearColor = clr;
   mClearDepth = depthValue;
   mClearStencil=stencilValue;
   mClearFlag  = flag;
}

void DynamicTexture::OnClear()
{
    // save clear color
    GLfloat oldClearColor[4] = {0.0f};
    GLfloat oldDepthClearValue = 0.0f;
    GLint   oldStencilClearValue = 0;

    // backup and set
    if (mClearFlag & GL_COLOR_BUFFER_BIT)
    {
        glGetFloatv(GL_COLOR_CLEAR_VALUE, oldClearColor);
        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
    }

    if (mClearFlag & GL_DEPTH_BUFFER_BIT)
    {
        glGetFloatv(GL_DEPTH_CLEAR_VALUE, &oldDepthClearValue);
        glClearDepth(mClearDepth);
    }

    if (mClearFlag & GL_STENCIL_BUFFER_BIT)
    {
        glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &oldStencilClearValue);
        glClearStencil(mClearStencil);
    }

    // clear
    glClear(mClearFlag);

    // restore
    if (mClearFlag & GL_COLOR_BUFFER_BIT)
    {
        glClearColor(oldClearColor[0], oldClearColor[1], oldClearColor[2], oldClearColor[3]);
    }

    if (mClearFlag & GL_DEPTH_BUFFER_BIT)
    {
        glClearDepth(oldDepthClearValue);
    }

    if (mClearFlag & GL_STENCIL_BUFFER_BIT)
    {
        glClearStencil(oldStencilClearValue);
    }
}

/// <description>
/// init the framebuffer & other resources for RenderTexture operations
/// </description>
bool DynamicTexture::Generate(std::function<void()> func)
{
	//kmMat4 OldTransMatrix, OldProjMatrix;
	//kmGLGetMatrix(KM_GL_PROJECTION, &OldProjMatrix);      
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLPushMatrix();
    {
        kmGLLoadIdentity();
        kmMat4 orthoMatrix;        
        Size texSize = mTexture->getContentSize();
        kmMat4OrthographicProjection(&orthoMatrix,0,texSize.width,0,texSize.height,-1024, 1024);
        kmGLMultMatrix(&orthoMatrix);
    }
    
    //kmGLGetMatrix(KM_GL_MODELVIEW, &OldTransMatrix);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLPushMatrix();
    kmGLLoadIdentity();

    Size size = mTexture->getContentSize();
	// Calculate the adjustment ratios based on the old and new projections
	// float widthRatio = size.width / texSize.width;
	// float heightRatio = size.height / texSize.height;
	
//     kmMat4OrthographicProjection(&orthoMatrix, (float)-1.0 / widthRatio,  (float)1.0 / widthRatio,
//         (float)-1.0 / heightRatio, (float)1.0 / heightRatio, -1,1 );    
	glViewport(0,0, (GLsizei)size.width, (GLsizei)size.height);

    // Adjust the orthographic projection and viewport
	mFrameBuffer->Activate();
    // clear the framebuffer using preset values
    OnClear();
    {
        func();
    }
    mFrameBuffer->Deactivate();
	// restore viewport
	Director::getInstance()->setViewport();

    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLPopMatrix();

    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLPopMatrix();

	return true;
}

/// <description>
/// get the texture 
/// </description>
cocos2d::Texture2D* DynamicTexture::GetTexture()
{
	return mTexture;
}

/// <description>
/// init the framebuffer with Texture2D for RenderTexture operations
/// </description>
bool DynamicTexture::Init(int w ,int h, Texture2D::PixelFormat format, GLuint depthStencilFormat)
{
	bool ret = false;
    void *data = nullptr;
	
    do 
    {
        Size size = Director::getInstance()->getWinSizeInPixels();
        w = (int)(w * CC_CONTENT_SCALE_FACTOR());
        h = (int)(h * CC_CONTENT_SCALE_FACTOR());
		
		// textures must be power of two squared
        int powW = 0;
        int powH = 0;

        if (Configuration::getInstance()->supportsNPOT())
        {
            powW = w;
            powH = h;
        }
        else
        {
            powW = ccNextPOT(w);
            powH = ccNextPOT(h);
        }

        auto dataLen = powW * powH * 4;
        data = malloc(dataLen);
        CC_BREAK_IF(! data);

        memset(data, 0, dataLen);
        mPixelFormat = format;

		mTexture= new Texture2D();
		CC_BREAK_IF(!mTexture);
		ret = mTexture->initWithData(data, dataLen, (Texture2D::PixelFormat)mPixelFormat, powW, powH, Size((float)w, (float)h));
		CC_BREAK_IF(!ret);

		mFrameBuffer = nullptr;
		mFrameBuffer = new Framebuffer;
		if (!mFrameBuffer->Init(mTexture, depthStencilFormat))
			break;
       
        mTexture->setAliasTexParameters();
		mTexture->retain();
        ret = true;
    } while (0);
    
    CC_SAFE_FREE(data);
    
    return ret;
}
