#ifndef __KOGO_Studio_Impl__
#define __KOGO_Studio_Impl__

#include "cocos2d.h"
USING_NS_CC;

static const char* bird_hero = "bird/bird_hero.png";
static const char* bird_hero2 = "bird/bird_hero2.png";
static const char* bird_hero3 = "bird/bird_hero3.png";
static const char* bird_logo = "bird/bird_logo.png";
static const char* bird_bg  = "bird/bird_bg.png";
static const char* bird_ground = "bird/ground.png";
static const char* bird_obstacle_up = "bird/obstacle_up.png";
static const char* bird_obstacle_down = "bird/obstacle_down.png";
static const char* bird_start_btn = "bird/bird_start_btn.png";
static const char* bird_start_btn_pressed = "bird/brid_start_btn_pressed.png";
static const char* bird_gameover = "bird/bird_gameover.png";
static const char* score_font = "fonts/futura-48.fnt";

enum ObjectTag
{
    TAG_LOGO        = 1000,
    TAG_START_BTN   = 1001,
    TAG_HERO        = 1002,
    TAG_GROUND      = 1003,
    TAG_GAMEOVER    = 1004, 
    TAG_SCORE       = 1005
};

enum GameStatus 
{
    GAME_STATUS_WELCOME    = 0,
    GAME_STATUS_PLAYING    = 10,
    GAME_STATUS_GAME_OVER  = 20,
    GAME_STATUS_RESTART    = 30
};

template <class Layer>
    cocos2d::Scene* createScene() 
    {
        Scene * scene = Scene::create();
        Layer *layer = Layer::create();
        scene->addChild(layer);

        return scene;
    }

#endif // #if!def __KOGO_Studio_Impl__
