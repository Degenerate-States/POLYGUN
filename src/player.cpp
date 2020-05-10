#define _USE_MATH_DEFINES
#include "player.h"
#include "assets.h"
#include "bulletManager.h"
#include "components.h"
#include "sdl.h"
#include <complex>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

//theta-phi except wrappes around angle seem between 0 and 2phi, (note also returns sign)
double smallestAngle(double theta, double phi){
    double delta = abs(theta-phi);
    double oppSign = (-1)*(theta-phi) / delta;
    if (2*M_PI -delta > delta){
        return theta-phi;
    }
    else{
        return oppSign*(2*M_PI -delta);
    }
}

void Gun::init(RigidBody* plrRb, Assets* assets,BulletManager* bulletMan, int ID, double bulletVel){
    this->ID = ID;
    this->bulletMan = bulletMan;
    this->bulletVel = bulletVel;
    this->rb.init(1.,0,0,0.0);
    this->plrRb = plrRb;
    this->poly.init(&(assets->gunAsset),&this->rb,white);
}
void Gun::update(Screen* screen,double dt){
    int x,y;
    this->rb.pos = plrRb->pos;
    SDL_GetMouseState(&x, &y);
    this->rb.setRot(arg(screen->pixelScreenToWorld(x,y) - this->rb.pos));
    this->rb.update(dt);
    this->poly.update();
}
void Gun::render(Screen* screen){
    this->poly.render(screen);
}
void Gun::events(SDL_Event* event, Screen* screen, double dt){
    if(event->type == SDL_MOUSEBUTTONDOWN){
        if (event->button.button == SDL_BUTTON_LEFT){
            complex<double> fireDirection = screen->pixelScreenToWorld(event->button.x,event->button.y) - this->rb.pos;
            
            this->bulletMan->fireBullet(white,orange, this->ID, this->rb.pos, fireDirection, this->bulletVel);
        }
    }
}
void Flame::init(RigidBody* plrRb, Assets* assets,BulletManager* bulletMan){
    this->plrRb = plrRb;
    this->rb.init(1.0,0,0,0);
    this->poly.init(&assets->flameAsset,&this->rb,red);
    this->bulletMan = bulletMan;
}

void Flame::update(Screen* screen, double dt){
    this->rb.setRot(this->plrRb->rot);
    this->rb.pos = this->plrRb->pos;

    double flameLen = fmod(-0.001*SDL_GetTicks(),0.12) +0.08;
    this->poly.vertexOffsets[0] = flameLen;


    this->rb.update(dt);
    this->poly.update();
}
void Flame::render(Screen* screen){
    this->poly.render(screen);
}

void Player::init(Assets* assets,BulletManager* bulletMan, Stats* stats){
    this->ID = 1;
    this->rb.init(1.,0,0,0.0);
    this->relScreenPos = 0.0;
    this->screenOffset = stats->screenOffset;
    this->screenVel = stats->screenVel;

    this->topSpeed = stats->plrTopSpeed;
    this->acceleration = stats->plrAcceleration;

    this->poly.init(&assets->plrAsset,&this->rb,white);

    this->gun.init(&this->rb, assets, bulletMan, this->ID, stats->plrBulletVel);
    this->flame.init(&this->rb,assets,bulletMan);
}
void Player::update(Screen* screen,double dt){
    //points in direction of motion
    this->rb.setRot(arg(this->rb.vel));

    //clamps velocity
    if (abs(this->rb.vel) > this->topSpeed){
        this->rb.vel *= this->topSpeed/abs(this->rb.vel);
    }

    this->rb.update(dt);
    this->poly.update();
    this->gun.update(screen, dt);
    this->flame.update(screen,dt);
}
void Player::render(Screen* screen){
    this->poly.render(screen);
    this->gun.render(screen);
    this->flame.render(screen);
}
void Player::events(SDL_Event* event, Screen* screen,double dt){
    this->gun.events(event,screen,dt);
}
void Player::setScreenPos(Screen* screen, double dt){
    // direction of motion is from where screen is to where player is trusting times offset size

    this->relScreenPos = this->screenOffset*this->rb.vel/this->topSpeed;
    // sets screen posisiton to player posistion plus relative posisiton
    screen->rb.pos = this->rb.pos + this->relScreenPos;
}
void Player::keys(const Uint8* keys,Screen* screen,double dt){
    complex<double> direction = 0.0;

    if (keys[SDL_SCANCODE_D]){
        direction += {1,0};
    }
    if (keys[SDL_SCANCODE_A]){
        direction +={-1,0};
    }
    if (keys[SDL_SCANCODE_W]){
        direction +={0,1};
    }
    if (keys[SDL_SCANCODE_S]){
        direction +={0,-1};
    }



    if (abs(direction)!=0.0){
        direction*= this->acceleration/abs(direction);
        this->rb.applyForce(real(direction),imag(direction),dt);
    }

}