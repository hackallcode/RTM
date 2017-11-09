#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "General.h"

namespace rtm {
    
    class WorldScene 
        : public cocos2d::Layer
    {
    public:
        static cocos2d::Scene* createScene();
        static WorldScene* create();
        ~WorldScene() = default;

        virtual bool init() override;
        virtual void update(float time) override;        
        void restart();

        WorldControllerUnique& getMap();

    private:
        WorldControllerUnique map_;
    };

    void keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
};

#endif // __WORLD_SCENE_INCLUDED__
