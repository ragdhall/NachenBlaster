#include "Actor.h"
#include "StudentWorld.h"
#include <math.h>


////////////////////// ACTORS IMPLEMENTATION
Actors :: Actors(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* s)
: GraphObject(imageID, startX, startY, dir, size, depth)
{
    alive = true;
    st = s;
    hits = 0;
}

StudentWorld* Actors :: getWorld()
{
    return st;
}

bool Actors :: getAlive()
{
    return alive;
}

void Actors :: setAlive(bool a)
{
    alive = a;
}

int Actors ::  getHits()
{
    return hits;
}

void Actors :: setHits(int h)
{
    return;
}






//////////////////////// NACHENBLASTER IMPLEMENTATION


NachenBlaster :: NachenBlaster(StudentWorld* s)
: Actors(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, s)
{
    hits = 50;
    cabbages = 30.0;
    torpedos = 0;
}




void NachenBlaster :: setCab(double c)
{
    if(c > 30)
    {
        cabbages = 30;
    }
    else
    {
        cabbages = c;
    }
}

double NachenBlaster :: getCab()
{
    return cabbages;
}

void NachenBlaster :: doSomething()
{
    
    if(!getAlive())
    {
        return;
    }
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_TAB:
                if(torpedos >= 1)
                {
                    torpedos--;
                    getWorld()->addToVector(new FlatTorpedo(getX()+12, getY(), 0, getWorld(), "ALIEN"));
                    
                    getWorld()->playSound(SOUND_TORPEDO);
                }
                break;
            case KEY_PRESS_SPACE:
                if(cabbages >= 5)
                {
                    cabbages = cabbages - 5;
                    getWorld()->addToVector(new Cabbage(getX()+12, getY(), getWorld()));
                    
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
            case KEY_PRESS_LEFT:
                if(getX() >= 6)
                {
                    moveTo(getX()-6, getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                if(getX() < VIEW_WIDTH-6)
                {
                    moveTo(getX()+6, getY());
                }
                break;
            case KEY_PRESS_UP:
                if(getY() < VIEW_HEIGHT-6)
                {
                    moveTo(getX(), getY()+6);
                }
                break;
            case KEY_PRESS_DOWN:
                if(getY() >= 6)
                {
                    moveTo(getX(), getY()-6);
                }
                break;
        }
    }
}



std::string NachenBlaster :: getIdentitiy()
{
    return "NACHENBLASTER";
}


void NachenBlaster :: setHits(int h)
{
    hits = h;
    if(h <= 0)
    {
        h = 0;
        setAlive(false);
    }
    if(h >= 50)
    {
        hits  = 50;
    }
}

int NachenBlaster ::  getHits()
{
    return hits;
}



int NachenBlaster :: getTorpedos()
{
    return torpedos;
}

void NachenBlaster :: setTorpedos(int T)
{
    torpedos = T;
}





Aliens :: Aliens(int ID, double startX, double startY, int h, StudentWorld* s)
: Actors(ID, startX, startY, 0, 1.5, 1, s)
{
    hits = h;
    speed = 2.0;
    trav_dir = 225;
    flight = 0;
}

double Aliens :: getSpeed()
{
    return speed;
}

void Aliens :: setSpeed(double s)
{
    speed = s;
}

int Aliens :: getTravDir()
{
    return trav_dir;
}
void Aliens :: setTravDir(int t)
{
    while (t < 0)
        t += 360;
    
    trav_dir = t % 360;
}

int Aliens ::  getHits()
{
    return hits;
}

void Aliens :: setHits(int h)
{
    hits = h;
    if(h <= 0)
    {
        setAlive(false);
    }
}

void Aliens :: setFlight(int f)
{
    flight = f;
}

int Aliens :: getFlight()
{
    return flight;
}




//////////////////////// SMALLGON IMPLEMENTATION

Smallgon :: Smallgon(double startX, double startY, StudentWorld* s)
: Aliens(IID_SMALLGON, startX, startY, 5 * (1 + (s->getLevel() - 1) * .1), s)
{
}

void Smallgon :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() < 0)
    {
        setAlive(false);
        return;
    }
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(250);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
        
    }
    
    
    if(getFlight() == 0 || getY() >= VIEW_HEIGHT-1 || getY() <= 0)
    {
        if(getY() >= VIEW_HEIGHT-1)
        {
            setTravDir(225);
        }
        if(getY() <= 0)
        {
            setTravDir(135);
        }
        if(getFlight()==0)
        {
            switch(randInt(1, 3))
            {
                case 1:
                    setTravDir(180);
                    break;
                case 2:
                    setTravDir(135);
                    break;
                case 3:
                    setTravDir(225);
                    break;
            }
        }
        setFlight(0);
        int a = randInt(1, 32);
        for(int i = 0; i < a; i++)
        {
            setFlight(getFlight()+1);
        }
        
    }
    getWorld()->shootAtNach(this);
    
    switch(getTravDir())
    {
        case 135:
            moveTo(getX() - getSpeed(), getY() + getSpeed());
            break;
        case 225:
            moveTo(getX() - getSpeed(), getY() - getSpeed());
            break;
        case 180:
            moveTo(getX() - getSpeed(), getY());
            break;
    }
    setFlight(getFlight()-1);
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(250);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
        
    }
}


std::string Smallgon :: getIdentitiy()
{
    return "SMALLGON";
}



////////////////////////  SMOREGON IMPLEMENTATION


Smoregon :: Smoregon(double startX, double startY, StudentWorld* s)
: Aliens(IID_SMOREGON, startX, startY, 5 * (1 + (s->getLevel() - 1) * .1), s)
{
}

void Smoregon :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() < 0)
    {
        setAlive(false);
        return;
    }
    
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(250);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
        
        if(randInt(1,3) == 1)
        {
           if(randInt(1, 2)==1)
            {
               getWorld()->addToVector(new Repair(getX(), getY(), getWorld()));
                
            }
            else
            {
                getWorld()->addToVector(new FlatTorpGoodie(getX(), getY(), getWorld()));
                
            }
        }
    }
    
    
    if(getFlight() == 0 || getY() >= VIEW_HEIGHT-1 || getY() <= 0)
    {
        if(getY() >= VIEW_HEIGHT-1)
        {
            setTravDir(225);
        }
        if(getY() <= 0)
        {
            setTravDir(135);
        }
        if(getFlight()==0)
        {
            switch(randInt(1, 3))
            {
                case 1:
                    setTravDir(180);
                    break;
                case 2:
                    setTravDir(135);
                    break;
                case 3:
                    setTravDir(225);
                    break;
            }
        }
        setFlight(0);
        for(int i = 0; i < randInt(1, 32); i++)
        {
            setFlight(getFlight()+1);
        }
    }
    
    getWorld()->shootAtNach(this);
    
    switch(getTravDir())
    {
        case 135:
            moveTo(getX() - getSpeed(), getY() + getSpeed());
            break;
        case 225:
            moveTo(getX() - getSpeed(), getY() - getSpeed());
            break;
        case 180:
            moveTo(getX() - getSpeed(), getY());
            break;
    }
    setFlight(getFlight()-1);
    
    
    // TODO: COLLISION WITH PROJECTILE (DONE IN PROJECTILE IMPLEMENTATION)
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(250);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
        
        if(randInt(1,3) == 1)
        {
            if(randInt(1, 2)==1)
            {
                getWorld()->addToVector(new Repair(getX(), getY(), getWorld()));
                
            }
            else
            {
                getWorld()->addToVector(new FlatTorpGoodie(getX(), getY(), getWorld()));
            }
        }
    }
}



std::string Smoregon :: getIdentitiy()
{
    return "SMOREGON";
}

////////////////////////  SNAGGLEGON IMPLEMENTATION

Snagglegon :: Snagglegon(double startX, double startY, StudentWorld* s)
: Aliens(IID_SNAGGLEGON, startX, startY, 1, s)// 10 * (1 + (s->getLevel() - 1) * .1), s)
{}

void Snagglegon :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() < 0)
    {
        setAlive(false);
        return;
    }
    
    
    
    // TODO: COLLISION WITH PROJECTILE (DONE IN PROJECTILE IMPLEMENTATION)
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
    if(randInt(1, 6) == 1)
        {
            getWorld()->addToVector(new ExtraLife(getX(), getY(), getWorld()));
        }

    }
    
    
    if(getY() >= VIEW_HEIGHT-1 || getY() <= 0)
    {
        if(getY() >= VIEW_HEIGHT-1)
        {
            setTravDir(225);
        }
        else
        {
            setTravDir(135);
        }
    }
    
    getWorld()->shootAtNach(this);
    
    switch(getTravDir())
    {
        case 135:
            moveTo(getX() - getSpeed(), getY() + getSpeed());
            break;
        case 225:
            moveTo(getX() - getSpeed(), getY() - getSpeed());
            break;
    }
    
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addToVector(new Explosion(getX(), getY(), getWorld()));
        if(randInt(1, 6) == 1)
        {
            getWorld()->addToVector(new ExtraLife(getX(), getY(), getWorld()));
        }
        
    }
}



std::string Snagglegon :: getIdentitiy()
{
    return "SNAGGLEGON";
}



//////////////////////// PROJECTILES IMPLEMENTATION

Projectiles :: Projectiles(int ID, double startX, double startY, int dir, StudentWorld* s)
:Actors(ID, startX, startY, dir, 0.5, 1, s)
{}

//////////////////////// CABBAGE IMPLEMENTATION

Cabbage :: Cabbage(double startX, double startY, StudentWorld* s)
:Projectiles(IID_CABBAGE, startX, startY, 0, s)
{}

void Cabbage :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() >= VIEW_WIDTH)
    {
        setAlive(false);
        return;
    }
    
    if(getWorld()->collisionWithAlien(this))
    {
        setAlive(false);
        return;
    }
    
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
    
    if(getWorld()->collisionWithAlien(this))
    {
        setAlive(false);
        return;
    }
}



std::string Cabbage :: getIdentitiy()
{
    return "CABBAGE";
}


//////////////////////// TURNIP IMPLEMENTATION


Turnip :: Turnip(double startX, double startY, StudentWorld* s)
: Projectiles(IID_TURNIP, startX, startY, 0, s)
{}

void Turnip :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    
    if(getX() < 0)
    {
        setAlive(false);
        return;
    }
    
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        return;
    }
    
    
    moveTo(getX()-6, getY());
    setDirection(getDirection()-20);
    
    if(getWorld()->collisionWithNach(this))
    {
        setAlive(false);
        return;
    }
}


std::string Turnip :: getIdentitiy()
{
    return "TURNIP";
}




//////////////////////// FLATTORPEDO IMPLEMENTATION


FlatTorpedo :: FlatTorpedo(double startX, double startY, int dir, StudentWorld* s, std::string t)
:Projectiles(IID_TORPEDO, startX, startY, dir, s)
{
    target = t;
}

void FlatTorpedo :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() < 0 || getX() >= VIEW_WIDTH)
    {
        setAlive(false);
        return;
    }
    if(target == "NACHENBLASTER")
    {
        
        if(getWorld()->collisionWithNach(this))
        {
            setAlive(false);
            return;
        }
    }
    else
    {
        if(getWorld()->collisionWithAlien(this))
        {
            setAlive(false);
            return;
        }
    }
    
    if(target == "NACHENBLASTER")
    {
        moveTo(getX() - 8, getY());
    }
    else
    {
        moveTo(getX() + 8, getY());
    }
    
    if(target == "NACHENBLASTER")
    {
        
        if(getWorld()->collisionWithNach(this))
        {
            setAlive(false);
            return;
        }
    }
    else
    {
        
        
        if(getWorld()->collisionWithAlien(this))
        {
            setAlive(false);
            return;
        }
    }
}


std::string FlatTorpedo :: getIdentitiy()
{
    return "FLATTORPEDO";
}







///////////////// GOODIES

Goodies :: Goodies(int ID, double startX, double startY, StudentWorld* s)
:Actors(ID, startX, startY, 0, .5, 1, s)
{}

void Goodies :: doSomething()
{
    if(!getAlive())
    {
        return;
    }
    if(getX() < 0 || getY() < 0)
    {
        setAlive(false);
        return;
    }
    
    if(getWorld()->collisionWithNach(this))
    {
        getWorld()->increaseScore(100);
        setAlive(false);
        getWorld()->playSound(SOUND_GOODIE);
        return;
    }
    
    
    moveTo(getX() - 0.75, getY() - 0.75);
    
    if(getWorld()->collisionWithNach(this))
    {
        getWorld()->increaseScore(100);
        setAlive(false);
        getWorld()->playSound(SOUND_GOODIE);
        return;
    }
}

//////////////////////// EXTRALIFE IMPLEMENTATION

ExtraLife :: ExtraLife(double startX, double startY, StudentWorld* s)
:Goodies(IID_LIFE_GOODIE, startX, startY, s)
{}


std::string ExtraLife :: getIdentitiy()
{
    return "EXTRALIFE";
}




////////////////////////  REPAIR IMPLEMENTATION


Repair :: Repair(double startX, double startY, StudentWorld* s)
:Goodies(IID_REPAIR_GOODIE, startX, startY, s)
{}


std::string Repair :: getIdentitiy()
{
    return "REPAIR";
}





////////////////////////  FLATTORPGOODIE IMPLEMENTATION

FlatTorpGoodie :: FlatTorpGoodie(double startX, double startY, StudentWorld* s)
:Goodies(IID_TORPEDO_GOODIE, startX, startY, s)
{}

std::string FlatTorpGoodie :: getIdentitiy()
{
    return "FLATTORPGOODIE";
}






///////////////////////// EXTRAS



//////////////////////// STAR IMPLEMENTATION

Star :: Star(double startX, double startY, StudentWorld* s)
: Actors(IID_STAR, startX, startY, 0, static_cast<double>(randInt(5, 50))/100, 3, s)
{}

void Star :: doSomething()
{
    if(getX()>=0)
    {
        moveTo(getX() - 1, getY());
    }
    else
    {
        setAlive(false);
    }
}

std::string Star :: getIdentitiy()
{
    return "STAR";
}



//////////////////////// EXPLOSION IMPLEMENTATION

Explosion :: Explosion(double startX, double startY, StudentWorld* s)
: Actors(IID_EXPLOSION, startX, startY, 0, 1, 0, s)
{
    count = 0;
}


void Explosion :: doSomething()
{
    if(count == 4)
    {
        setAlive(false);
        return;
    }
    setSize(getSize()*1.5);
    count++;
}


std::string Explosion :: getIdentitiy()
{
    return "EXPLOSION";
}






























