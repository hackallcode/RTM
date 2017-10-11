#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "cocos2d.h"
#include <vector> // std::vector
#include <memory> // std::unique_ptr

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

        float getMissedTime() const;

        //std::vector<std::unique_ptr<WorldObject>>& getObjects();
        //void spawnVehicle(VehicleType id, size_t x = 0, size_t y = 0);

        void initNewGame();

    private:
        float missedTime_;

        //std::vector<std::unique_ptr<Objects>> objects_;
        //void addBuilding_(BuildingType id);        
        //void addVehicle_(VehicleType id);
        //void deleteAllVehicles_();

        void initMap_();
    };

    void keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event);
};

#endif // __WORLD_SCENE_INCLUDED__
