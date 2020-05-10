#ifndef gun_h
#define gun_h
#include "components.h"
#include "projectileManager.h"
#include "assets.h"


class Gun{
    public:
        projectileType type;
        RigidBody rb;
        Polygon poly;
        //holds reference to bullet manager so it can fire
        ProjectileManager* projMan;
        //holds copy of shooters id
        int ID;

        double bulletVel;
        double velVarience;
        //holds reference of shooters rb
        RigidBody* shooterRb;

        void init(RigidBody* plrRb, Assets* assets,ProjectileManager* projMan,projectileType fireType
                    ,int ID, double bulletVel,double shotVarience);
        void update(Screen* screen, double dt);
        void render(Screen* screen);

        //event hanlding
        void events(SDL_Event* event, Screen* screen ,double dt);
        void fire(complex<double> fireDirection);
};

#endif