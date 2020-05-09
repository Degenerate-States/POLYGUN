#include "gameManager.h"
#include "components.h"
#include "config.h"
#include "assets.h"
#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <complex>
#include <tuple>
using namespace std;

//************************//
//**GameManager Methods **//
//************************//
void GameManager::init(Config* cfg,Assets* assets){    
    this->internalInit(cfg);

    // initalize game objects
    this->bulletMan.init(cfg);
    this->plr.init(assets,&this->bulletMan);
    this->box.init(assets);
    

}


void GameManager::events(double dt){
    //Also updates key state array
    while (SDL_PollEvent(&(this->event)))
    {
        this->Check_Quit();

        //game object events
        this->plr.events(&(this->event),&this->screen,dt);
    }

    // keypresses
    this->screen.keys(this->keys,dt);
    this->plr.keys(this->keys,dt);
}

void GameManager::preUpdateInteractions(double dt){
    //this->screen.rb.rot = this->plr.rb.rot;

}
void GameManager::update(double dt){
    this->plr.update(&this->screen,dt);
    this->box.update(&this->screen,dt);
    this->screen.update(dt);
    this->bulletMan.update(dt);
}
void GameManager::postUpdateInteractions(double dt){

    this->bulletMan.checkCollisionPoly(this->box.ID,&this->box.rb,&this->box.poly,dt);
    this->bulletMan.checkCollisionPoly(this->plr.ID,&this->plr.rb, &this->plr.poly,dt);
}
void GameManager::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render everything below
    this->plr.render(&(this->screen));
    this->box.render(&(this->screen));
    this->bulletMan.render(&this->screen,this->spf);

    SDL_GL_SwapWindow(this->screen.window);
}



void GameManager::gameLoop(){
    // Loop condition
    while (this->running) {
        this->events(this->spf);
        this->preUpdateInteractions(this->spf);
        this->update(this->spf);
        this->postUpdateInteractions(this->spf);
        this->render();
        this->fixFramerate();
    }
}


void GameManager::clean(){
    // Destroy everything to not leak memory.
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyRenderer(this->screen.renderer);
    SDL_DestroyWindow(this->screen.window);

    SDL_Quit();
}