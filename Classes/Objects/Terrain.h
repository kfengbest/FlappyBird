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
#ifndef __KOGO_Terrain_H__
#define __KOGO_Terrain_H__

/// <description>
/// randomly generated Terrain
/// there may have different layers of terrain for parallax scrolling
/// </description>
class Terrain 
    : public cocos2d::Node
{	
public:
    CREATE_FUNC(Terrain);    
    virtual ~Terrain();

public:
    /// <description>
    /// layer initialization
    /// </description>
    virtual bool init() override;

    /// <description>
    /// Event callback that is invoked every time when Node enters the 'stage'.
    /// If the Node enters the 'stage' with a transition, this event is called when the transition starts.
    /// During onEnter you can't access a "sister/brother" node.
    /// If you override onEnter, you shall call its parent's one, e.g., Node::onEnter().
    /// </description>
    virtual void onEnter() override;

    /// <description>
    /// Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
    /// </description>
    virtual void update(float delta) override;

    /// <description>
    /// Event callback that is invoked every time the Node leaves the 'stage'.
    /// If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
    /// During onExit you can't access a sibling node.
    /// If you override onExit, you shall call its parent's one, e.g., Node::onExit().
    /// </description>
    virtual void onExit() override;

    /// <description>
    /// draw the terrain using own code
    /// </description>
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
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated) override;

    /// <description>
    /// reset the terrain to the initial state
    /// </description>
    void reset();

    /// <description>
    /// set the terrain offset to 'scroll' the screen
    /// </description>
    void setOffsetX(float offsetX);

    /// <description>
    /// return the actual texture size
    /// </description>
    int textureSize();

protected:
    /// <description>
    /// private constructor 
    /// </description>
    Terrain(); 
    
    /// <description>
    /// generate a strip sprite for usage
    /// </description>
    Sprite* generateStripesSprite();

    /// <description>
    /// terrain gemeotry
    /// </description>
    void renderBorder();
    void generateHillKeyPoints();
    void generateBorderVertices();
    bool createPhysicsBody();
    void resetHillVertices();
    Color4F randomColor();

private:    
    typedef std::vector<Vertex2F> VectList;
    VectList mHillKeyPoints;    
    int nHillKeyPoints;
    
    VertexBuffer<V2F_T2F> mHillVertices;    
    int nHillVertices;
    VertexBuffer<cocos2d::Vertex2F> mBorderVertices;
    int mBorderVerticeCount;
    int mFromKeyPointI;
    int mToKeyPointI;

    Color4F mThemeColor;

private:
    Sprite *mStripes;
    float mScale;
    float mOffsetX;
    int mTextureSize;  

    /// custom command object for terrain sprite rendering
    CustomCommand mCustomCommand,mRenderCommand;
};

#endif //__KOGO_Terrain_H__
