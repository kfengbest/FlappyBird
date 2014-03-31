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
	Description	:	Game Layer
	History		:	2014, Initial implementation.
*/
#include "Impl.h"

USING_NS_CC;

/// <description>
/// create the scene needed for the Game Layer
/// </description>
Scene* GameLayer::createScene() 
{
    auto scene = Scene::createWithPhysics();
    auto world = scene->getPhysicsWorld();

#ifdef _DEBUG
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif 

   auto layer = GameLayer::InstancePtr();
   if (layer)
   {       
       scene->addChild(layer,DEPTH_GAME_LAYER);
       return scene;
   }
   
   delete layer;
   layer = nullptr;
   scene->release();
   scene = nullptr;
   return scene;
}

/// <description>
/// layer initialization
/// </description>
bool GameLayer::init() 
{
    if (!Layer::init()) 
        return false;    

    return true;
}

/// <description>
/// destructor
/// </description>
GameLayer::~GameLayer()
{
}

/// <description>
/// get the physics world
/// </description>
PhysicsWorld* GameLayer::getPhysicsWorld()
{
    return this->getScene()->getPhysicsWorld();
}

/// <description>
/// Event callback for pause button is clicked
/// </description>
void GameLayer::menuResetCallback(cocos2d::Object* pSender)
{
    reset();
}

/// <description>
/// Event callback for touch events
/// </description>
void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event*)
{
    if (touches.empty())
        return;

    cocos2d::Touch* touch = touches[0];
    Point location = touch->getLocation();

    Hero::InstancePtr()->setDiving(true); 
}

/// <description>
/// Event callback for touch events
/// </description>
void GameLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event*)
{
    if (touches.empty())
        return;

    cocos2d::Touch* touch = touches[0];
    Point location = touch->getLocation();
    Hero::InstancePtr()->setDiving(false);
}

/// <description>
/// reset the game
/// </description>
void GameLayer::reset()
{
    // temporarily disable the terrain
    ///Terrain::InstancePtr()->reset(); 
    Hero::InstancePtr()->reset();
}

/// <description>
/// Event callback that is invoked every time when Node enters the 'stage'.
/// If the Node enters the 'stage' with a transition, this event is called when the transition starts.
/// During onEnter you can't access a "sister/brother" node.
/// If you override onEnter, you shall call its parent's one, e.g., Node::onEnter().
/// </description>
void GameLayer::onEnter() 
{
    Layer::onEnter();

    Size screenSize = Director::getInstance()->getWinSize();
    float width = screenSize.width, height = screenSize.height;

    //auto sky = Sky::InstancePtr();
    //this->addChild(sky);

    auto terrain = Terrain::create();
    this->addChild(terrain,0,TAG_TERRAIN);

    //auto hero = Hero::InstancePtr();
    //terrain->addChild(hero);

    this->scheduleUpdate();
}

/// <description>
/// get the terrain object
/// </description>
Terrain* GameLayer::getTerrain()
{
    return (Terrain*)this->getChildByTag(TAG_TERRAIN);
}

/// <description>
/// Event callback that is invoked every time the Node leaves the 'stage'.
/// If the Node leaves the 'stage' with a transition, this event is called when the transition finishes.
/// During onExit you can't access a sibling node.
/// If you override onExit, you shall call its parent's one, e.g., Node::onExit().
/// </description>
void GameLayer::onExit()
{
    Layer::onExit();

}

/// <description>
/// Update method will be called automatically every frame if "scheduleUpdate" is called, and the node is "live"
/// </description>
void GameLayer::update(float dt) 
{    
    Hero::InstancePtr()->update(dt);

    Size sz = Director::getInstance()->getWinSize();
    // terrain scale and offset
    Point pos = Hero::InstancePtr()->getPosition();
    float height = pos.y;
    const float minHeight = sz.height*4/5;
    if (height < minHeight) {
        height = minHeight;
    }
    
    static float offset = 0.0f;
    offset += 100.f * dt;
    float scale = minHeight / height;
    getTerrain()->setScale(scale);
    getTerrain()->setOffsetX(pos.x + offset);
}

/// <description>
/// show labels for perfect 
/// </description>
void GameLayer::showPerfectSlide()
{
    showLabel("Perfect!",1.0f,1.2f);
}

/// <description>
/// show different labels 
/// </description>
void GameLayer::showFrenzy()
{    
    showLabel("Frenzy!",2.0f,1.4f);
}

/// <description>
/// show different labels 
/// </description>
void GameLayer::showHit() 
{
    showLabel("hit",1.0f,1.2f);
}

/// <description>
/// show label info 
/// </description>
void GameLayer::showLabel(const char* str, float duration /*= 1.0f*/, float scale /*= 1.2f*/)
{
    Size sz     = Director::getInstance()->getVisibleSize();
    LabelBMFont *label = LabelBMFont::create(str,"good_dog_plain_32.fnt");    
    label->setPosition(sz.width/2, sz.height/16);
    label->runAction(ScaleTo::create(duration,scale));

    auto sequences = Sequence::create(FadeOut::create(duration), 
            CallFuncN::create(CC_CALLBACK_0(Node::removeFromParentAndCleanup, label, true)),
            nullptr);
    label->runAction(sequences);
    this->addChild(label);
}

/// <description>
/// init the UI layouts / need another layer?
/// </description>
bool GameLayer::initUI()
{
    Size sz     = Director::getInstance()->getVisibleSize();
    Point org   = Director::getInstance()->getVisibleOrigin();

    auto resetBtn = MenuItemImage::create(
        "resetButton.png","resetButton.png", CC_CALLBACK_1(GameLayer::menuResetCallback, this));
    Size size = resetBtn->getContentSize();
    float padding = 8.0f;
    resetBtn->setPosition(sz.width-size.width/2-padding, sz.height-size.height/2-padding);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(resetBtn, nullptr);
    menu->setAnchorPoint(Point(1,1));
    menu->setPosition(Point(sz.width, sz.height));
    this->addChild(menu, DEPTH_UI, TAG_MENU);

    return true;
}
