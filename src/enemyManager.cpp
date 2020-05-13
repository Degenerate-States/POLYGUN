#include "enemyManager.h"

// Swarmer
void Swarmer::init(Assets* assets,RigidBody* plrRB, Stats* stats){
    this->ID = assets->getID();
    this->rb.init(1.,0,0,0.0);

    this->topSpeed = stats->swarmerTopSpeed;
    this->acceleration = stats->swarmerAcceleration;
    this->drag = stats->swarmerDrag;

    this->poly.init(&assets->swarmerAsset,&this->rb,red);
    this->plrRB = plrRB;
    this->rb.active = false;
}

void Swarmer::spawn(complex<double> pos, complex<double> vel){
    this->rb.active = true;
    this->rb.pos = pos;
    this->rb.vel = vel;

}

void Swarmer::die(){
    this->rb.active = false;
}

void Swarmer::update(double dt){
    // accelerates in direction of player
    complex<double> direction = this->plrRB->pos - this->rb.pos;
    direction *= this->acceleration/abs(direction);
    this->rb.applyForce(real(direction),imag(direction),dt);

    //points in direction of motion
    this->rb.setRot(arg(this->rb.vel));

    //clamps velocity
    if (abs(this->rb.vel) > this->topSpeed){
        this->rb.vel *= this->topSpeed/abs(this->rb.vel);
    }
    double speed = abs(this->rb.vel);
    this->rb.vel -= this->drag*dt*this->rb.vel;

    this->rb.update(dt);
    this->poly.update();
}

void Swarmer::render(Screen* screen){
    this->poly.render(screen);
}


//Enemy
void EnemyManager::init(Assets* assets, ProjectileManager* projMan,RigidBody* plrRB, Stats* stats){
    //initalize swarmers
    this->oldestSwarmerIndex = 0;
    for(int i = 0; i < swarmerPoolSize; i++){
        this->swarmers[i] = new Swarmer();
        this->swarmers[i]->init(assets,plrRB,stats);
    }

    //TODO remove tester swarmer spawn below
    this->spawnSwarmer({1,1});
}

void EnemyManager::spawnSwarmer(complex<double> pos, complex<double> vel, double velVarience){
    vel += randComplex(velVarience);

    this->swarmers[this->oldestSwarmerIndex]->spawn(pos, vel);
    // change which bullet is considered the oldest
    this->oldestSwarmerIndex+=1;
    this->oldestSwarmerIndex%=swarmerPoolSize;
}

void EnemyManager::update(Screen* screen, double dt){
    //swarmers
    for(int i = 0; i < swarmerPoolSize; i++){
        if(this->swarmers[i]->rb.active){
            this->swarmers[i]->update(dt);
        }
    }
}

void EnemyManager::checkCollision(ProjectileManager* projMan,double dt){
    //swarmers
    for(int i = 0; i < swarmerPoolSize; i++){
        if(this->swarmers[i]->rb.active){
            projMan->checkCollisionPoly(&this->swarmers[i]->poly,this->swarmers[i]->ID,dt);
        }
    }
}

void EnemyManager::render(Screen* screen){
    for(int i = 0; i < swarmerPoolSize; i++){
        if(this->swarmers[i]->rb.active){
            this->swarmers[i]->render(screen);
        }
    }
}