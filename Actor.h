#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;

class Actors : public GraphObject
{
public:
    Actors(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    bool getAlive();
    void setAlive(bool a);
    virtual std::string getIdentitiy() = 0;
    virtual int getHits();
    virtual void setHits(int h);
private:
    bool alive;
    StudentWorld* st;
    int hits;
};



class NachenBlaster : public Actors
{
public:
    NachenBlaster(StudentWorld* s);
    void setCab(double c);
    double getCab();
    virtual void doSomething();
    virtual std::string getIdentitiy();
    virtual int getHits();
    virtual void setHits(int h);
    int getTorpedos();
    void setTorpedos(int t);
private:
    int hits;
    double cabbages;
    int torpedos;
};


////////////// ALIENS


class Aliens : public Actors
{
public:
    Aliens(int ID, double startX, double startY, int h, StudentWorld*s);
    virtual void doSomething() = 0;
    virtual std::string getIdentitiy() = 0;
    double getSpeed();
    void setSpeed(double s);
    virtual int getHits();
    virtual void setHits(int h);
    void setFlight(int f);
    int getFlight();
    int getTravDir();
    void setTravDir(int t);
private:
    int hits;
    double speed;
    int trav_dir;
    int flight;
};


class Smallgon : public Aliens
{
public:
    Smallgon(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};


class Smoregon : public Aliens
{
public:
    Smoregon(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};


class Snagglegon : public Aliens
{
public:
    Snagglegon(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};







/////////////////// PROJECTILES


class Projectiles : public Actors
{
public:
    Projectiles(int ID, double startX, double startY, int dir, StudentWorld* s);
    virtual void doSomething() = 0;
    virtual std::string getIdentitiy() = 0;
};

class Cabbage : public Projectiles
{
public:
    Cabbage(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};


class Turnip : public Projectiles
{
public:
    Turnip(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};


class FlatTorpedo : public Projectiles
{
public:
    FlatTorpedo(double startX, double startY, int dir, StudentWorld* s, std::string t);
    virtual void doSomething();
    virtual std::string getIdentitiy();
private:
    std::string target;
};








///////////////// GOODIES


class Goodies : public Actors
{
public:
    Goodies(int ID, double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy() = 0;
};

class ExtraLife : public Goodies
{
public:
    ExtraLife(double startX, double startY, StudentWorld* s);
    virtual std::string getIdentitiy();
};



class Repair : public Goodies
{
public:
    Repair(double startX, double startY, StudentWorld* s);
    virtual std::string getIdentitiy();
};



class FlatTorpGoodie : public Goodies
{
public:
    FlatTorpGoodie(double startX, double startY, StudentWorld* s);
    virtual std::string getIdentitiy();
};






//////////////// EXTRAS

class Star : public Actors
{
public:
    Star(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
};




class Explosion : public Actors
{
public:
    Explosion(double startX, double startY, StudentWorld* s);
    virtual void doSomething();
    virtual std::string getIdentitiy();
private:
    int count;
};







































#endif // ACTOR_H_


