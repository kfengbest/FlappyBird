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
	Description	:	GameLayer Scene 
	History		:	2014, Initial implementation.
*/
#ifndef __Game_Scene_H__
#define __Game_Scene_H__
 
class Sky;
class Terrain;
class Hero;

/// <description>
/// Game Layer 
/// </description>
class GameLayer 
    : public cocos2d::Layer , public NAGA Singleton<GameLayer>
{
public:
    /// <description>
    /// create the scene with physics world
    /// </description>    
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameLayer);   
    virtual~GameLayer();

public:
    /// <description>
    /// get the physics world
    /// </description>
    PhysicsWorld* getPhysicsWorld();
    
    /// <description>
    /// show label info 
    /// </description>
    void showLabel(const char* str, float duration = 1.0f, float scale = 1.2f);

    /// <description>
    /// show fixed labels 
    /// </description>
    void showPerfectSlide();
    void showFrenzy();
    void showHit();

protected:
    /// <description>
    /// layer initialization
    /// </description>
    virtual bool init() override;
   
    /// <description>
    /// Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
    /// </description>
    virtual void update(float delta) override;

    /// <description>
    /// Event callback that is invoked every time when Node enters the 'stage'.
    /// If the Node enters the 'stage' with a transition, this event is called when the transition starts.
    /// During onEnter you can't access a "sister/brother" node.
    /// If you override onEnter, you shall call its parent's one, e.g., Node::onEnter().
    /// </description>
    virtual void onEnter() override;

    /// <description>
    /// Event callback that is invoked every time the Node leaves the 'stage'.
    /// If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
    /// During onExit you can't access a sibling node.
    /// If you override onExit, you shall call its parent's one, e.g., Node::onExit().
    /// </description>
    virtual void onExit() override;

    /// <description>
    /// reset the game
    /// </description>
    void reset();

    /// <description>
    /// Event callback for pause button is clicked
    /// </description>
    void menuResetCallback(cocos2d::Object* pSender);

    /// <description>
    /// Event callback for touch events
    /// </description>
    void onTouchesBegan(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>&, cocos2d::Event*);
    
    /// <description>
    /// init the UI layouts / need another layer?
    /// </description>
    bool initUI();    

    /// <description>
    /// get the terrain object
    /// </description>
    Terrain* getTerrain();
};

#endif // __Game_Scene_H__
