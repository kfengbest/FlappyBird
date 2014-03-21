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
 Author         :	Kaven Feng
 Description	:	Welcome Scene
 History		:	2013, Initial implementation.
 */
#include "Impl.h"
#include "WelcomeScene.h"
#include "FlappyBirdLayer.h"

bool WelcomeScene::init()
{    
    auto size = Director::getInstance()->getVisibleSize();

    // game bg
    auto bg = Sprite::create(bird_bg);
    Size cSize = bg->getContentSize();
    bg->setPosition(size.width / 2, size.height / 2);
    //bg->setScale(size.width / bg->getContentSize().width, size.height / bg->getContentSize().height);
    this->addChild(bg);

    // logo
    auto logo = Sprite::create(bird_logo);
    logo->setPosition(Point(size.width / 2, size.height / 2 + logo->getContentSize().height * 2));
    logo->setTag(TAG_LOGO);
    this->addChild(logo, 1);

    // start btn
    auto startBtn = MenuItemImage::create(bird_start_btn, bird_start_btn_pressed, CC_CALLBACK_1(WelcomeScene::start, this));
    auto menu = Menu::create(startBtn, NULL);
    menu->setTag(TAG_START_BTN);
    this->addChild(menu);
        
    auto listener = EventListenerTouchAllAtOnce::create();
    if (listener)
    {
        listener->onTouchesEnded = CC_CALLBACK_2(WelcomeScene::onTouchesEnded, this);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
        return true;
    }

	return false;
}

void WelcomeScene::start(Object* pSender)
{
    Director::getInstance()->replaceScene(FlappyBirdLayer::createScene());
}
