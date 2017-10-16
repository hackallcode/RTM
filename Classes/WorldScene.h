#ifndef __WORLD_SCENE_INCLUDED__
#define __WORLD_SCENE_INCLUDED__

#include "cocos2d.h"
#include <vector> // std::vector
#include <memory> // std::unique_ptr

#include "BuildingObject.h"
#include "CarObject.h"

namespace rtm {

    // PARAMETERS
    size_t const CELL_SIZE = 32;

    // Z ORDERS
    int const BACKGROUND_Z_ORDER = 0;
    int const COATING_Z_ORDER = 1;
    int const VEHICLE_Z_ORDER = 2;
    int const BUILDING_Z_ORDER = 3;

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

        void initNewGame();
        std::vector<std::unique_ptr<WorldObject>>& getObjects();
        void spawnCar(CarType type, size_t row = 0, size_t column = 0, float angle = 0.f);
        void spawnBuilding(BuildingType type, size_t row = 0, size_t column = 0, float angle = 0.f);

    private:
        float missedTime_;

        std::vector<std::unique_ptr<WorldObject>> objects_;
        void removeAllObjects_();
        void addBuilding_(BuildingType type, size_t row = 0, size_t column = 0, float angle = 0.f);
        void addCar_(CarType type, size_t row = 0, size_t column = 0, float angle = 0.f);

        void initMap_();
    };

    void keyListener(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event);
};

#endif // __WORLD_SCENE_INCLUDED__
