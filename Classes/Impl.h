#ifndef __KOGO_Studio_Impl__
#define __KOGO_Studio_Impl__

#include "cocos2d.h"
#include "ccUtils.h"
#include "NAGA/NagaLib.h"
#include "NagaAdapter.h"

USING_NS_CC;
USING_NAGA;

#include "Util/PhysicsHelper.h"
#include "Objects/Random.h"
#include "Objects/VertexBuffer.h"
#include "Scenes/GameLayer.h"
#include "Objects/Sky.h"
#include "Objects/Terrain.h"
#include "Objects/Framebuffer.h"
#include "Objects/DynamicTexture.h"

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

    /// <description>
    /// object tag 
    /// </description>
    enum ObjectTag
    {
        TAG_LOGO        = 1000,
        TAG_HERO        ,
        TAG_GROUND      ,
        TAG_BACKGROUND  ,
        TAG_OBSTACLE    ,
        TAG_GAMEOVER    , 
        TAG_SCORE       ,
        TAG_SKY         ,
        TAG_TERRAIN     ,
        TAG_SCROLLBAKG  , 
        TAG_MENU        ,
        TAG_START_BTN   ,
        TAG_PAUSE_BTN           
    };

    /// <description>
    /// default depth arrangement
    /// </description>
    enum ObjectDepth 
    {
        /// the sky
        DEPTH_SKY           = 0,
        /// the basic background
        DEPTH_BACKGROUND    = 5,
        /// the parallax background for slow movement 
        DEPTH_PARALLAX_BKG1 = 10, 
        /// the parallax background for slow movement 
        DEPTH_PARALLAX_BKG2 = 15, 
        /// the game layer
        DEPTH_GAME_LAYER    = 20,
        /// the foreground, which includes bubbles, coral reef or anything else  
        DEPTH_FOREGROUND    = 40,
        /// the UI Layer
        DEPTH_UI            = 80,
    };
    
    /// <description>
    /// game status 
    /// </description>
    enum GameStatus 
    {
        GAME_STATUS_WELCOME    = 0,
        GAME_STATUS_PLAYING    = 10,
        GAME_STATUS_GAME_OVER  = 20,
        GAME_STATUS_RESTART    = 30
    };

    extern Color4F RGBtoHSV( const Color4F& );
    extern Color4F HSVtoRGB( const Color4F& );

#define CC_USE_SHADER(shaderName) auto shader = ShaderCache::getInstance()->getProgram(shaderName);\
    shader->use(); \
    shader->setUniformsForBuiltins()


#endif // #if!def __KOGO_Studio_Impl__
