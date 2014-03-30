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

#ifndef __KOGO_FrameBuffer_H__
#define __KOGO_FrameBuffer_H__

/// <description>
/// OpenGL ES Framebuffer Object
/// </description>
class Framebuffer : public NAGA Object
{
public:
	/// <description>
	/// take a snapshot of current image
	/// flip set to be true for saving images to files
	/// </description>
	static cocos2d::Image* TakeSnapShot(bool flip=true);

public:
    Framebuffer();
    ~Framebuffer();

public:
    /// <description>
    /// init the framebuffer with Texture2D for RenderTexture operations
    /// </description>
    bool Init(Texture2D* tex, GLenum depthStencilFormat=0);

    void Activate();
    void Deactivate();

private:
    GLuint      mObjectID,mColorBuffer,mDepthStencil;
    GLint       mOldFBO, mOldRBO, mOldDepth;
};

#endif // __KOGO_FrameBuffer_H__
