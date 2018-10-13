#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    void addToVector(Actors* a);
    bool collisionWithAlien(Actors* a);
    bool collisionWithNach(Actors* a);
    void shootAtNach(Aliens* a);
private:
    vector<Actors*> act;
    NachenBlaster* nach;
    int alienDead;
    int alienLeft;
    int aliens;
    int maxAliens;
};

#endif // STUDENTWORLD_H_
