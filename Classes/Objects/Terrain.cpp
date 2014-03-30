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
	Description	:	Terrain System
	History		:	2014, Initial implementation.
*/
#include "Impl.h"
#include "TextureGenerator.h"

/// <description>
/// constant definition
/// </description>
enum 
{    
    kMaxHillKeyPoints   = 1024,      // 1k
    kMaxHillVertices    = 64 * 1024, // 64k 
    kMaxBorderVertices  = 32 * 1024, // 32k
    kHillSegmentWidth   = 15
};

/// <description>
/// layer initialization
/// </description>
Terrain::Terrain()
    : mStripes(nullptr)
    , mOffsetX(0.0f)
    , mScale(1.0f)
    , mBorderVerticeCount(0)
    , mFromKeyPointI(-1)
    , mToKeyPointI(-1)
{    
    /// generate "theme" color and other colors generated will be close to it 
    mThemeColor = randomColor();
}

/// <description>
/// destructor
/// </description>
Terrain::~Terrain()    
{    
}

/// <description>
/// layer initialization
/// </description>
bool Terrain::init() 
{
    mTextureSize = 1024;      
    this->mStripes = this->generateStripesSprite();
    if (!mStripes)
        return false;

    mStripes->retain();    

    mHillKeyPoints.reserve(kMaxHillKeyPoints);
    if (!mHillVertices.Init(nullptr, kMaxHillVertices, GL_DYNAMIC_DRAW) 
        || !mBorderVertices.Init(nullptr, kMaxBorderVertices, GL_DYNAMIC_DRAW))
        return false;

    this->generateHillKeyPoints();
    //this->generateBorderVertices();
    this->createPhysicsBody();

    /// force to reset the offset
    this->mOffsetX = -1.0f;
    setOffsetX(0.f);
   
    return true;
}

/// <description>
/// return the actual texture size
/// </description>
int Terrain::textureSize()
{
    int texSize = mTextureSize * CC_CONTENT_SCALE_FACTOR();
    if (Configuration::getInstance()->supportsNPOT()) {
        return texSize;
    }

    return ccNextPOT(texSize);
}

/// <description>
/// Event callback that is invoked every time when Node enters the 'stage'.
/// If the Node enters the 'stage' with a transition, this event is called when the transition starts.
/// During onEnter you can't access a "sister/brother" node.
/// If you  onEnter, you shall call its parent's one, e.g., Node::onEnter().
/// </description>
void Terrain::onEnter() 
{
    Node::onEnter();
}

/// <description>
/// Event callback that is invoked every time the Node leaves the 'stage'.
/// If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
/// During onExit you can't access a sibling node.
/// If you  onExit, you shall call its parent's one, e.g., Node::onExit().
/// </description>
void Terrain::onExit() 
{
    Node::onExit();

}

/// <description>
/// Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
/// </description>
void Terrain::update(float delta) 
{

}

/// <description>
/// generate a strip sprite for usage
/// </description>
cocos2d::Sprite* Terrain::generateStripesSprite() 
{	
	auto tex = TextureGenerator::Generate(mTextureSize, mThemeColor, 
        TextureGenerator::kEffectNoise | TextureGenerator::kEffectGradient | TextureGenerator::kEffectHighlight);
	auto sprite = Sprite::createWithTexture(tex);
	Texture2D::TexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
	sprite->getTexture()->setTexParameters(tp);
	
	return sprite;
}

/// <description>
/// render the hill border
/// </description>
void Terrain::renderBorder()
{
	const float borderAlpha = 0.8f;
	const float borderWidth = 1.0f;

	glLineWidth(borderWidth*CC_CONTENT_SCALE_FACTOR());
    auto shader = ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);    
    setShaderProgram(shader);
    CC_NODE_DRAW_SETUP();

    float r = mThemeColor.r, g = mThemeColor.g, b = mThemeColor.b;
    shader->setUniformLocationWith4f(shader->getUniformLocationForName("u_color"),r,g,b,borderAlpha);
    shader->setUniformLocationWith1f(shader->getUniformLocationForName("u_pointSize"),1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mBorderVertices.DrawArrays(GL_LINE_STRIP, 0, (GLsizei)mBorderVerticeCount);
}

/// <description>
/// generate the key points for the stage 
/// </description>
void Terrain::generateHillKeyPoints() 
{
    Size size = Director::getInstance()->getWinSize();
    int screenW = size.width;
    int screenH = size.height;

	nHillKeyPoints = 0;
	
	float dx, dy, ny;    
	float x = -(float)screenW/4.0f;
	float y =  (float)screenH*3.0/4.0f;
    mHillKeyPoints.push_back(Vertex2F(x, y));

	// starting point
	x = 0;
	y = screenH/2;
    mHillKeyPoints.push_back(Vertex2F(x, y));
	
	int minDX = 160, rangeDX = 80;
	int minDY = 60,  rangeDY = 60;
	float sign = -1; // +1 - going up, -1 - going  down
	float maxHeight = screenH;
	float minHeight = 20;
    randomizer<unsigned int> rx(0,rangeDX),ry(0,rangeDY);

	for (int i=0; i< kMaxHillKeyPoints-1; ++i) 
    {
		dx = rx() +minDX;
		x += dx;
		dy = ry() +minDY;
		ny = y + dy*sign;
		if(ny > maxHeight) ny = maxHeight;
		if(ny < minHeight) ny = minHeight;
		y = ny;
		sign *= -1;
        mHillKeyPoints.push_back(Vertex2F(x, y));
	}

	// cliff
	x += minDX+rangeDX;
	y = 0;
    mHillKeyPoints.push_back(Vertex2F(x, y));
    nHillKeyPoints = mHillKeyPoints.size();

	// adjust vertices for retina
	for (int i=0; i<nHillKeyPoints; i++) 
    {
		mHillKeyPoints[i].x *= CC_CONTENT_SCALE_FACTOR();
		mHillKeyPoints[i].y *= CC_CONTENT_SCALE_FACTOR();
	}
	
	mFromKeyPointI = 0;
	mToKeyPointI = 0;
}

/// <description>
/// generate the hill border
/// </description>
void Terrain::generateBorderVertices() 
{	
    mBorderVertices.Update(GL_WRITE_ONLY, [&](cocos2d::Vertex2F* borderVertices)
    {
        mBorderVerticeCount = 0;
        Vertex2F p0, p1, pt0, pt1;
        p0 = mHillKeyPoints[0];
        for (int i=1; i<nHillKeyPoints; i++) 
        {
            p1 = mHillKeyPoints[i];

            int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
            float dx = (p1.x - p0.x) / hSegments;
            float da = M_PI / hSegments;
            float ymid = (p0.y + p1.y) / 2;
            float ampl = (p0.y - p1.y) / 2;
            pt0 = p0;
            borderVertices[mBorderVerticeCount++] = pt0;
            for (int j=1; j<hSegments+1; j++) {
                pt1.x = p0.x + j*dx;
                pt1.y = ymid + ampl * cosf(da*j);
                borderVertices[mBorderVerticeCount++] = pt1;
                pt0 = pt1;
            }

            p0 = p1;
        }
    });
}

/// <description>
/// create the physics body for the hill 
/// 
/// </description>
bool Terrain::createPhysicsBody()
{
    /*
    auto _body = PhysicsBody::createEdgePolygon(borderVertices, nBorderVertices);
    if (!_body)
        return false;

    if (this->getPhysicsBody())
    {
        /// remove the previous body from the world
        auto world = this->getPhysicsBody()->getWorld();
        world->removeBody(this->getPhysicsBody());
    }

    this->setPhysicsBody(_body);
    */
    return true;
}

/// <description>
/// regenerate the hill vertices
/// </description>
void Terrain::resetHillVertices()
{
	static int prevFromKeyPointI = -1;
	static int prevToKeyPointI = -1;
	
	// key points interval for drawing
    Size size = Director::getInstance()->getWinSize();
    int screenW = size.width;
    int screenH = size.height;

	float leftSideX = mOffsetX -screenW/8.0f/this->mScale;
	float rightSideX= mOffsetX+screenW*7.f/8.0f/this->mScale;
	
	// adjust position for retina
	leftSideX  *= CC_CONTENT_SCALE_FACTOR();
	rightSideX *= CC_CONTENT_SCALE_FACTOR();
	
	while (mHillKeyPoints[mFromKeyPointI+1].x < leftSideX) 
    {
		mFromKeyPointI++;
		if (mFromKeyPointI > nHillKeyPoints-1) {
			mFromKeyPointI = nHillKeyPoints-1;
			break;
		}
	}

	while (mHillKeyPoints[mToKeyPointI].x < rightSideX) 
    {
		mToKeyPointI++;
		if (mToKeyPointI > nHillKeyPoints-1) 
        {
			mToKeyPointI = nHillKeyPoints-1;
			break;
		}
	}
	
	if (prevFromKeyPointI != mFromKeyPointI || prevToKeyPointI != mToKeyPointI) 
    {        
        VectList borderVertices;
		// vertices for visible area
        mHillVertices.Update(GL_WRITE_ONLY, [&](V2F_T2F* hillVertices)
        {
            nHillVertices = 0;
            Vertex2F p0, p1, pt0, pt1;
            p0 = mHillKeyPoints[mFromKeyPointI];
            for (int i=mFromKeyPointI+1; i<mToKeyPointI+1; i++) 
            {
                p1 = mHillKeyPoints[i];
                // triangle strip between p0 and p1
                int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
                int vSegments = 1;
                float dx = (p1.x - p0.x) / hSegments;
                float da = M_PI / hSegments;
                float ymid = (p0.y + p1.y) / 2;
                float ampl = (p0.y - p1.y) / 2;                
                pt0 = p0;
                borderVertices.push_back(pt0);
                for (int j=1; j<hSegments+1; j++) 
                {
                    pt1.x = p0.x + j*dx;
                    pt1.y = ymid + ampl * cosf(da*j);
                    for (int k=0; k<vSegments+1; k++) 
                    {
                        hillVertices[nHillVertices].vertices    = Vertex2F(pt0.x, pt0.y-(float)mTextureSize/vSegments*k);
                        hillVertices[nHillVertices++].texCoords = Tex2F(pt0.x/(float)mTextureSize, 1.0f - (float)(k)/vSegments);
                        hillVertices[nHillVertices].vertices    = Vertex2F(pt1.x, pt1.y-(float)mTextureSize/vSegments*k);
                        hillVertices[nHillVertices++].texCoords = Tex2F(pt1.x/(float)mTextureSize, 1.0f - (float)(k)/vSegments);
                    }
                    pt0 = pt1;
                    borderVertices.push_back(pt1);
                }			
                p0 = p1;
            }
        });

        mBorderVerticeCount = 0;
        mBorderVertices.Update(GL_WRITE_ONLY, [&](Vertex2F* vertices)
        {
            std::for_each(borderVertices.begin(), borderVertices.end(), [&](const Vertex2F& v)
                {
                    vertices[mBorderVerticeCount++] = v;
                });            
        });	

		prevFromKeyPointI = mFromKeyPointI;
		prevToKeyPointI = mToKeyPointI;        
	}
}

/// <description>
/// generate a random color 
/// </description>
Color4F Terrain::randomColor() 
{
	const int minSum = 450;
	const int minDelta = 150;
	int r, g, b, min, max;
    randomizer<int> random(0,256);
	while (true) 
    {
		r = random();
		g = random();
		b = random();
		min = MIN(MIN(r, g), b);
		max = MAX(MAX(r, g), b);
		if (max-min < minDelta) 
            continue;
		if (r+g+b < minSum) 
            continue;
		break;
	}
	return ccc4FFromccc3B(ccc3(r, g, b));
}

   /**
     * Override this method to draw your own node.
     * The following GL states will be enabled by default:
     * - `glEnableClientState(GL_VERTEX_ARRAY);`
     * - `glEnableClientState(GL_COLOR_ARRAY);`
     * - `glEnableClientState(GL_TEXTURE_COORD_ARRAY);`
     * - `glEnable(GL_TEXTURE_2D);`
     * AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR NODE
     * But if you enable any other GL state, you should disable it after drawing your node.
     */
void Terrain::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{	
    mRenderCommand.init(_globalZOrder);
    mRenderCommand.func = [&]() 
    {
        /// render the hill 
        {
            setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
            CC_NODE_DRAW_SETUP();
           
            Texture2D* tex = mStripes->getTexture();
            glBindTexture(GL_TEXTURE_2D, tex->getName());	
            mHillVertices.DrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nHillVertices);            

            /// draw the hill border
            renderBorder();

            glBindTexture(GL_TEXTURE_2D, 0);	
        }        
    };

    Director::getInstance()->getRenderer()->addCommand(&mRenderCommand);
}

/// <description>
/// set the terrain offset to 'scroll' the screen
/// </description>
void Terrain::setOffsetX(float offsetX) 
{	
	if (mOffsetX != offsetX ) 
    {
		mOffsetX = offsetX;

        Size size = Director::getInstance()->getWinSize();
        int screenW = size.width;
        int screenH = size.height;

		this->setPosition(screenW/8- mOffsetX * mScale, 0);
		this->resetHillVertices();
	}
}

/// <description>
/// reset the terrain to the initial state
/// </description>
void Terrain::reset() 
{	
    if (mStripes) 
    {
        mStripes->release();
        mStripes = nullptr;
    }

	this->mStripes = this->generateStripesSprite();	
	mFromKeyPointI = 0;
	mToKeyPointI = 0;
}
