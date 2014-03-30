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
USING_NS_CC;

#include "Framebuffer.h"

/// <description>
/// constructor
/// </description>
Framebuffer::Framebuffer()
    : mObjectID(0), mColorBuffer(0), mDepthStencil(0)
    , mOldFBO(0), mOldRBO(0)
{

}

/// <description>
/// destructor
/// </description>
Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &mObjectID);
    if (mColorBuffer) {
        glDeleteRenderbuffers(1,&mColorBuffer);
    }
    if (mDepthStencil){
        glDeleteRenderbuffers(1, &mDepthStencil);
    }
}

/// <description>
/// init the framebuffer with Texture2D for RenderTexture operations
/// </description>
bool Framebuffer::Init(Texture2D* _texture, GLenum depthStencilFormat)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &mOldRBO);

    // generate FBO
    glGenFramebuffers(1, &mObjectID);
    glBindFramebuffer(GL_FRAMEBUFFER, mObjectID);

    // associate texture with FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->getName(), 0);

    Size texSize = _texture->getContentSize();
    if (depthStencilFormat != 0)
    {
        //create and attach depth buffer
        glGenRenderbuffers(1, &mDepthStencil);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencil);
        glRenderbufferStorage(GL_RENDERBUFFER, depthStencilFormat, (GLsizei)texSize.width, (GLsizei)texSize.height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencil);        
        if (depthStencilFormat == GL_DEPTH24_STENCIL8) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencil);
        }
    }

    bool bValid = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

    glBindRenderbuffer(GL_RENDERBUFFER, mOldRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mOldFBO);

    return bValid;
}

void Framebuffer::Activate()
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &mOldRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, mObjectID);
}

void Framebuffer::Deactivate()
{
    glBindRenderbuffer(GL_RENDERBUFFER, mOldRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mOldFBO);        
}


/// <description>
/// take a snapshot of current image
/// </description>
cocos2d::Image* Framebuffer::TakeSnapShot(bool flip)
{
	int savedBufferWidth =0, savedBufferHeight = 0, type;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type);
	CHECK_GL_ERROR_DEBUG();
	if ( type == GL_RENDERBUFFER)
	{
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &savedBufferWidth);
		CHECK_GL_ERROR_DEBUG();
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,&savedBufferHeight);
		CHECK_GL_ERROR_DEBUG();
	}
	else if (type == GL_TEXTURE)
	{
		int texName = 0, texLevel = 0;
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texName);
		CHECK_GL_ERROR_DEBUG();
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,&texLevel);
		CHECK_GL_ERROR_DEBUG();
		glGetTexLevelParameteriv(GL_TEXTURE_2D, texLevel, GL_TEXTURE_WIDTH, &savedBufferWidth);
		CHECK_GL_ERROR_DEBUG();
		glGetTexLevelParameteriv(GL_TEXTURE_2D, texLevel, GL_TEXTURE_HEIGHT, &savedBufferHeight);
		CHECK_GL_ERROR_DEBUG();
	}
	else {
		CHECK_GL_ERROR_DEBUG();
		CCASSERT(0,"Unknown attachment type!");
		return nullptr;
	}

    GLubyte *buffer = nullptr;
    GLubyte *tempData = nullptr;
    Image *image = new Image();

    do
    {
        CC_BREAK_IF(! (buffer = new GLubyte[savedBufferWidth * savedBufferHeight * 4]));

        if(! (tempData = new GLubyte[savedBufferWidth * savedBufferHeight * 4]))
        {
            delete[] buffer;
            buffer = nullptr;
            break;
        }

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0,0,savedBufferWidth, savedBufferHeight,GL_RGBA,GL_UNSIGNED_BYTE, tempData);

        if ( flip ) // -- flip is only required when saving image to file
        {
            // to get the actual texture data
            // #640 the image read from rendertexture is dirty
            for (int i = 0; i < savedBufferHeight; ++i)
            {
                memcpy(&buffer[i * savedBufferWidth * 4],
                       &tempData[(savedBufferHeight - i - 1) * savedBufferWidth * 4],
                       savedBufferWidth * 4);
            }

            image->initWithRawData(buffer, savedBufferWidth * savedBufferHeight * 4, savedBufferWidth, savedBufferHeight, 8);
        }
        else
        {
            image->initWithRawData(tempData, savedBufferWidth * savedBufferHeight * 4, savedBufferWidth, savedBufferHeight, 8);
        }
        
    } while (0);

    CC_SAFE_DELETE_ARRAY(buffer);
    CC_SAFE_DELETE_ARRAY(tempData);

    return image;
}
