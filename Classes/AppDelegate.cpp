#include "AppDelegate.h"
#include "WorldScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1
#define FULL_SCREEN_MODE 0
#if (!FULL_SCREEN_MODE)
    #define FRAME_SIZE_NAME resolutionSizeNo4
#endif
#define RELATIVE_SIZE_MODE 2

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);

#if (!FULL_SCREEN_MODE || !RELATIVE_SIZE_MODE)
static cocos2d::Size resolutionSizeNo1 = cocos2d::Size(640, 360);
static cocos2d::Size resolutionSizeNo2 = cocos2d::Size(960, 540);
static cocos2d::Size resolutionSizeNo3 = cocos2d::Size(1440, 810);
static cocos2d::Size resolutionSizeNo4 = cocos2d::Size(1920, 1080);
static cocos2d::Size resolutionSizeNo5 = cocos2d::Size(2880, 1620);
static cocos2d::Size resolutionSizeNo6 = cocos2d::Size(3840, 2160);
static cocos2d::Size resolutionSizeNo7 = cocos2d::Size(4800, 2700);
static cocos2d::Size resolutionSizeNo8 = cocos2d::Size(5760, 3240);
static cocos2d::Size resolutionSizeNo9 = cocos2d::Size(7680, 4320);
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#if (FULL_SCREEN_MODE)
        glview = GLViewImpl::createWithFullScreen("RTM");
#else
        glview = GLViewImpl::createWithRect("RTM", cocos2d::Rect(0, 0, FRAME_SIZE_NAME.width, FRAME_SIZE_NAME.height));
#endif
#else
        glview = GLViewImpl::create("RTM");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
#if (!RELATIVE_SIZE_MODE)
    // Set correct scale
    auto frameSize = glview->getFrameSize();
    if (frameSize.height > resolutionSizeNo8.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo9.height / designResolutionSize.height, resolutionSizeNo9.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo7.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo8.height / designResolutionSize.height, resolutionSizeNo8.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo6.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo7.height / designResolutionSize.height, resolutionSizeNo7.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo5.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo6.height / designResolutionSize.height, resolutionSizeNo6.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo4.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo5.height / designResolutionSize.height, resolutionSizeNo5.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo3.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo4.height / designResolutionSize.height, resolutionSizeNo4.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo2.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo3.height / designResolutionSize.height, resolutionSizeNo3.width / designResolutionSize.width));
    }
    else if (frameSize.height > resolutionSizeNo1.height) {
        director->setContentScaleFactor(MIN(resolutionSizeNo2.height / designResolutionSize.height, resolutionSizeNo2.width / designResolutionSize.width));
    }
    else {
        director->setContentScaleFactor(MIN(resolutionSizeNo1.height / designResolutionSize.height, resolutionSizeNo1.width / designResolutionSize.width));
    }
#endif

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = rtm::World::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
