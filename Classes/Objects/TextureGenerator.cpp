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
#include "Impl.h"
#include "TextureGenerator.h"
#include "Random.h"

enum 
{
    kMinStripes         = 2,
    kMaxStripes         = 10,
};

static Color4F sThemeColor;

/// <description>
/// generate a random color 
/// </description>
Color4F randomColor() 
{
    static randomizer<float> random;
    Color4F r = RGBtoHSV(sThemeColor);
    float h = r.r, s = r.g, v = r.b, a = r.a;
    /// r,g,b values are from 0 to 1
    /// h = [0,360], s = [0,1], v = [0,1]
    ///	    if s == 0, then h = -1 (undefined)
    h += (random() * 2 - 1) * 30.f;
    h = fmod(h,360);

    do {
        s = r.g + (random() * 2 - 1);
    } while (s <= 0 && s >=1);

    do {
        v = r.b + (random() * 2 - 1);
    } while (v <= 0 && v >=1);       

    return HSVtoRGB(Color4F(h,s,v,a));
}

/// <descriptin>
/// render stripes on the texture 
/// </descriptin>
static void renderStripes(int texSize) 
{
	// random even number of stripes
    randomizer<int> random(kMinStripes, kMaxStripes),random2(0,1);
    // use random number to generate diagonal stripes or horizontal stripes
	int nStripes = random(), diagonal=random2();
	if (nStripes%2) {
		nStripes++;
	}

    int nVertices = 0;
    VertexBuffer<V2F_C4F> buffer(nStripes*6);
    buffer.Update(GL_WRITE_ONLY,[&](V2F_C4F* data)
    {
        float x1, x2, y1, y2, dx, dy;
        Color4F c;	

        if (diagonal == 1)
        {
            // diagonal stripes		
            dx = (float)texSize*2 / (float)nStripes;
            dy = 0;

            x1 = -texSize;
            y1 = 0;

            x2 = 0;
            y2 = texSize;

            for (int i=0; i<nStripes/2; i++) 
            {
                c = randomColor();
                for (int j=0; j<2; j++) 
                {
                    for (int k=0; k<6; k++) {
                        data[nVertices+k].colors = c;
                    }
                    data[nVertices++].vertices = Vertex2F(x1+j*texSize, y1);
                    data[nVertices++].vertices = Vertex2F(x1+j*texSize+dx, y1);
                    data[nVertices++].vertices = Vertex2F(x2+j*texSize, y2);
                    data[nVertices++].vertices = Vertex2F(x1+j*texSize+dx, y1);
                    data[nVertices++].vertices = Vertex2F(x2+j*texSize+dx, y2);
                    data[nVertices++].vertices = Vertex2F(x2+j*texSize, y2);
                }
                x1 += dx;
                x2 += dx;
            }	
        } 
        else // horizontal stripes
        {		            
            dx = 0;
            dy = (float)texSize / (float)nStripes;

            x1 = 0;           
            x2 = texSize;

            y1 = 0;
            y2 = y1 + dy;

            for (int i=0; i<nStripes; i++) 
            {
                c = randomColor();
                for (int k=0; k<6; k++) {
                    data[nVertices+k].colors = c;
                }

                data[nVertices++].vertices = Vertex2F(x1, y1);
                data[nVertices++].vertices = Vertex2F(x2, y1);
                data[nVertices++].vertices = Vertex2F(x1, y2);

                data[nVertices++].vertices = Vertex2F(x2, y1);                
                data[nVertices++].vertices = Vertex2F(x2, y2);
                data[nVertices++].vertices = Vertex2F(x1, y2);

                y1 = y2;
                y2 += dy;
            }
        }

        // adjust vertices for retina
        for (int i=0; i<nVertices; i++) {
            data[i].vertices.x *= CC_CONTENT_SCALE_FACTOR();
            data[i].vertices.y *= CC_CONTENT_SCALE_FACTOR();
        }
    });
    
    CC_USE_SHADER(GLProgram::SHADER_NAME_POSITION_COLOR);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    buffer.DrawArrays(GL_TRIANGLES);   
}

/// <description>
/// render the gradient color from top to bottom
/// </description>
static void renderGradient(int texSize)
{
    float gradientAlpha = 0.5f;
    float gradientWidth = texSize;
    Color4F clr = Color4F(0,0,0,0);
	
    VertexBuffer<V2F_C4F> buffer(6);
	int nVertices = 0;
    buffer.Update(GL_WRITE_ONLY, [&](V2F_C4F* data)
    {   
        data[nVertices].vertices = Vertex2F(0, texSize);
        data[nVertices++].colors = clr;        
        data[nVertices].vertices = Vertex2F(0, 0);
        data[nVertices++].colors = Color4F(0, 0, 0, gradientAlpha);
        data[nVertices].vertices = Vertex2F(texSize, texSize);
        data[nVertices++].colors = clr;
        data[nVertices].vertices = Vertex2F(texSize, 0);        
        data[nVertices++].colors = Color4F(0, 0, 0, gradientAlpha);

        /* test triangle
        data[nVertices].vertices = Vertex2F(texSize/2, texSize);
        data[nVertices++].colors = clr;
        data[nVertices].vertices = Vertex2F(0, 0);
        data[nVertices++].colors = clr;
        data[nVertices].vertices = Vertex2F(texSize, 0);
        data[nVertices++].colors = clr;
        */
        
        // adjust vertices for retina
        for (int i=0; i<nVertices; i++) {
            data[i].vertices.x *= CC_CONTENT_SCALE_FACTOR();
            data[i].vertices.y *= CC_CONTENT_SCALE_FACTOR();
        }        
    });

    CC_USE_SHADER(GLProgram::SHADER_NAME_POSITION_COLOR);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA,GL_ZERO, GL_ONE);
    buffer.DrawArrays(GL_TRIANGLE_STRIP,0, nVertices);
}

/// <description>
/// render hight to simulate the sun light 
/// </description>
static void renderHighlight(int texSize) 
{
	float highlightAlpha = 0.5f;
	float highlightWidth = texSize - texSize/4;
	
    VertexBuffer<V2F_C4F> vecBuffer(4);
    int nVertices = 0;
	vecBuffer.Update(GL_WRITE_ONLY, [&](V2F_C4F* highlight)
    {
        highlight[nVertices].vertices = Vertex2F(0, texSize);
        highlight[nVertices++].colors = Color4F(1, 1, 1.f, highlightAlpha); 
        highlight[nVertices].vertices = Vertex2F(0, highlightWidth);
        highlight[nVertices++].colors = Color4F(0, 0, 0, 0);
        highlight[nVertices].vertices = Vertex2F(texSize, texSize);
        highlight[nVertices++].colors = Color4F(1, 1, 1.f, highlightAlpha);        
        highlight[nVertices].vertices = Vertex2F(texSize, highlightWidth);
        highlight[nVertices++].colors = Color4F(0, 0, 0, 0);

        // adjust vertices for retina
        for (int i=0; i<nVertices; i++) 
        {
            highlight[i].vertices.x *= CC_CONTENT_SCALE_FACTOR();
            highlight[i].vertices.y *= CC_CONTENT_SCALE_FACTOR();
        }
    });
    
    CC_USE_SHADER(GLProgram::SHADER_NAME_POSITION_COLOR);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);
	vecBuffer.DrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
}

/// <description>
/// render a noise image onto the current texture
/// </description>
static void renderNoise(int texSize)
{
    auto tex = Director::getInstance()->getTextureCache()->addImage("noise.png");
    VertexBuffer<V2F_T2F> buffer(4);
    buffer.Update(GL_WRITE_ONLY,[&](V2F_T2F* quad)
    {
        quad[0].vertices = Vertex2F(0,0);
        quad[0].texCoords= Tex2F(0.f,0.f);
        quad[1].vertices = Vertex2F(texSize,0);
        quad[1].texCoords= Tex2F(1.0f,0.0f);
        quad[2].vertices = Vertex2F(0,texSize);
        quad[2].texCoords= Tex2F(0,1.0f);
        quad[3].vertices = Vertex2F(texSize,texSize);
        quad[3].texCoords= Tex2F(1.0f,1.0f);

        for (int i=0;i<4;++i)
        {
            quad[i].vertices.x  *= CC_CONTENT_SCALE_FACTOR();
            quad[i].vertices.y  *= CC_CONTENT_SCALE_FACTOR();
        }
    });

    glEnable(GL_TEXTURE_2D);
    glBlendFuncSeparate(GL_DST_COLOR, GL_ZERO, GL_ZERO, GL_ONE );
    CC_USE_SHADER(GLProgram::SHADER_NAME_POSITION_TEXTURE);
    buffer.DrawArrays(GL_TRIANGLE_STRIP);
}

/// <description>
/// generate texture with specified effects 
/// </description>
cocos2d::Texture2D* TextureGenerator::Generate(int texSize, 
    const Color4F& theme/*= Color4F(1.0f,0.8431f,0.f,1.0f)*/, int effect /*= Effect::kAllEffects*/)
{
    // begin render to texture, which binds its own FrameBuffer object and Renderbuffer objects    
    SmartPointer<DynamicTexture> tex = new DynamicTexture;
    tex->Init(texSize, texSize, Texture2D::PixelFormat::RGBA8888, 0);
    sThemeColor = theme;    
    tex->Clear(theme);

    /// generating the texture 
    tex->Generate([&]()
    {
        if (effect & Effect::kEffectStripes){
            renderStripes(texSize);
        }
        if (effect & Effect::kEffectNoise){
            renderNoise(texSize);
        }
        if (effect & Effect::kEffectGradient){
            renderGradient(texSize);
        }
        if (effect & Effect::kEffectHighlight){
            renderHighlight(texSize);
        }

#ifdef _DEBUG    
        auto snap = Framebuffer::TakeSnapShot();
        snap->saveToFile("texture.jpg");
#endif 
    });

    return tex->GetTexture();
}
