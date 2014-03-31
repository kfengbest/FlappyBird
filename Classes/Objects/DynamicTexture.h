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
	Description	:	Dynamic Texture Generator
	History		:	2014, Initial implementation.
*/
#ifndef __KOGO_DynamicTexture_H__
#define __KOGO_DynamicTexture_H__

/// <description>
/// Dynamic Texture Generator
/// </description>
class DynamicTexture : public NAGA Object
{
public:
    DynamicTexture();
    ~DynamicTexture();

public:
    /// <description>
    /// init the framebuffer & other resources for RenderTexture operations
    /// </description>
    bool Init(int w ,int h, Texture2D::PixelFormat format, GLuint depthStencilFormat);

    /// <description>
    /// clear the color/depth-stencil buffer
    /// </description>
    void Clear(const Color4F& clr, float depthValue=1.0f, 
            int stencilValue=0, GLbitfield flag = GL_COLOR_BUFFER_BIT);

	/// <description>
    /// generate the texture 
    /// </description>
    bool Generate(std::function<void()> func);

	/// <description>
    /// get the texture 
    /// </description>
	cocos2d::Texture2D* GetTexture();

private:
    void OnClear();

private:
    NAGA SmartPointer<Framebuffer> mFrameBuffer;
	/// generated texture 
	Texture2D *mTexture;
	Texture2D::PixelFormat mPixelFormat;
    Color4F mClearColor;
    float   mClearDepth;
    int     mClearStencil;
    GLbitfield mClearFlag;
	/// projection/world view matrix
    kmMat4 mWorldViewMatrix, mProjectionMatrix;
};

#endif // __KOGO_DynamicTexture_H__
