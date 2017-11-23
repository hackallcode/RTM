#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "General.h"

namespace rtm {
    
    class WorldScene 
        : public cocos2d::Scene
    {
    public:
        static WorldScene* Create();
        ~WorldScene() = default;

        virtual bool init() override;
        virtual void update(float time) override;

        cocos2d::Layer* GetMainLayer() const;

        void SetBackground(std::string const& filename);
        void SetBackground(BackgroundNumber number);

    private:
        static WorldScene* globalScene_;
        cocos2d::Layer* mainLayer_;
        cocos2d::Layer* backgroundLayer_;

        cocos2d::Sprite* background_;
        WorldControllerUnique map_;
        float clickTime_;
        int viewColumn_;
        int viewRow_;
        bool isCtrlPressed_;
        bool isAltPressed_;
        bool isUpArrowPressed_;
        bool isRightArrowPressed_;
        bool isDownArrowPressed_;
        bool isLeftArrowPressed_;

        void OpenMap_();
        void Restart_();
        
        void SetDefaultPosition_();
        void ShiftUp_();
        void ShiftRight_();
        void ShiftDown_();
        void ShiftLeft_();
        void UpdatePosition_();

        void SetDefaultScale_();
        void IncreaseScale_();
        void DecreaseScale_();

        void SetDefaultSpeed_();
        void IncreaseSpeed_();
        void DecreaseSpeed_();

        WorldControllerUnique& GetMap_();

        static std::string GetBackgroundFile_(BackgroundNumber number);
        static void KeyPressed_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
        static void KeyReleased_(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
    };
};

#endif // __WORLD_SCENE_INCLUDED__
