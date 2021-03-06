#pragma once
#include "components.h"
#include "riggedPoly.h"
#include "projectileManager.h"
#include "assets.h"
#include "gun.h"

#include <complex>
#include <vector>
#include <cmath>

#define _USE_MATH_DEFINES


class Flame{
    private:
        RigidBody* plrRb;
        Skeleton skele;
        //holds reference to projectile manager so it can make sparks
        ProjectileManager* projMan;
    
    public:
        void init(RigidBody* plrRb, Assets* assets,ProjectileManager* projMan);
        void update(Screen* screen, double dt);
        void render(Screen* screen);
};

class Player{
    private: 
        // current screen posisiton relative to player
        complex<double> relScreenPos;
        vector<GunStats> gunBar;

        // how quickly the screen snaps to direction of thrusting 
        double screenVel;
        // how far the screen points in the direction of thrusting
        double screenOffset;

        int contactDamage;
        int startHealth;
    public:
        int health;

        RigidBody rb;
        Polygon poly;
        Gun gun;
        Flame flame;

        double topSpeed;
        double acceleration;
        
        void init(Assets* assets,ProjectileManager* projMan, Stats* stats);
        void spawn(complex<double> pos);
        void update(Screen* screen,double dt);
        void render(Screen* screen);

        //event hanlding
        void events(SDL_Event* event,Screen* screen,double dt);
        //current keys down
        //TODO remove temp input box
        void keys(const Uint8* keys ,Screen* screen,double dt);

        //screen controling
        void setScreenPos(Screen* screen, double dt);


        //callback functions used in collision
        void onCollision(int damage, complex<double> direction);
        int getDamage();
        void die();
};