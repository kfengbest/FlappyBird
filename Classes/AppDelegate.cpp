#include "AppDelegate.h"
#include "impl.h"
#include "FlappyBirdLayer.h"
#include "WelcomeScene.h"

USING_NS_CC;
USING_NAGA;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    // initialize director
    auto director = Director::getInstance();
    auto eglview = director->getOpenGLView();
    if(!eglview) {
        eglview = GLView::create("flappy bird");
        eglview->setFrameSize(480, 480*1.775);
        director->setOpenGLView(eglview);
    }
    eglview->setDesignResolutionSize(320.0f, 480.0f, ResolutionPolicy::FIXED_HEIGHT);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object    
    auto scene = CreateScene<WelcomeScene>();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
