#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "cocos2d.h"
#include <vector> // std::vector
#include <memory> // std::unique_ptr

#include <WorldController.h>

namespace rtm {
    
    class World 
        : public cocos2d::Layer
    {
    public:
        static cocos2d::Scene* createScene();
        static World* create();
        ~World() = default;

        virtual bool init() override;
        virtual void update(float time) override;        
        void restart();

        std::unique_ptr<WorldController>& getMap();

    private:
        std::unique_ptr<WorldController> map_;
    };

    void keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
};

#endif // __WORLD_SCENE_INCLUDED__
