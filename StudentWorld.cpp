#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}


StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    alienDead = 0;
    alienLeft = 0;
    maxAliens = 0;
    aliens = 0;
}

int StudentWorld::init()
{
    
    nach = new NachenBlaster(this);
    
    for(int i = 0; i < 30; i++)
    {
        act.push_back(new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this));
    }
    alienLeft = 6 + (4 * getLevel());
    maxAliens = 4 + (0.5 * getLevel());
    alienDead = 0;
    aliens = 0;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    nach->doSomething();
    
    for(int i = 0; i < act.size(); i++)
    {
        act[i]->doSomething();
    }
    
    vector<Actors*> :: iterator it;
    it = act.begin();
    
    
    while(it != act.end())
    {
        if(!(*it)->getAlive())
        {
            delete *it;
            it = act.erase(it);
            
        }
        else
        {
            it++;
        }
    }

    
    
    if(!nach->getAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
        
    }
    
    if(alienDead == 6 + (4 * getLevel()))
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
        
    }
    
    
    aliens = 0;
    for(int i = 0; i < act.size(); i++)
    {
        if(act[i]->getIdentitiy() == "SMALLGON" || act[i]->getIdentitiy() == "SMOREGON" || act[i]->getIdentitiy() == "SNAGGLEGON")
        {
            aliens++;
        }
    }
    
    
    
    
    
    alienLeft = 6 + (4 * getLevel()) - alienDead;
    if(aliens < min(maxAliens, alienLeft))
    {
        int S1 = 60;
        int S2 = 20 + getLevel() * 5;
        int S3 = 5 + getLevel() * 10;
        
        int S = randInt(1, S1 + S2 + S3);
        
        if(S <= S1)
        {
            act.push_back(new Smallgon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            
        }
        
        else if(S <= S1 + S2)
        {
            act.push_back(new Smoregon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            
        }
        
        else
        {
            act.push_back(new Snagglegon(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
            
        }
    }
    
    if(randInt(1, 15) == 7)
    {
        act.push_back(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
        
    }
    
    
    nach->setCab(nach->getCab()+1);
    
    ostringstream oss;
    oss << "Lives: " << getLives() << "  Health: "  << setw(3) <<  nach->getHits()*2 << "%  Score: " << getScore() << "  Level: "  << getLevel() << "  Cabbages: " << setw(3) << static_cast<int>((nach->getCab()/30)*100) << "%  Torpedoes: " << nach->getTorpedos();
    string s = oss.str();
    setGameStatText(s);    
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(nach != nullptr)
    {
        delete nach;
        nach = nullptr;
    }
    
    
     vector<Actors*> :: iterator it;
     it = act.begin();
     
     while(it != act.end())
     {
     delete *it;
     it = act.erase(it);
         
     }
}

StudentWorld :: ~StudentWorld()
{
    cleanUp();
    
}

void StudentWorld :: addToVector(Actors* a)
{
    act.push_back(a);
}




bool StudentWorld :: collisionWithAlien(Actors* a)
{
    for(int i = 0; i < act.size(); i++)
    {
        if(act[i]->getIdentitiy() == "SMALLGON" || act[i]->getIdentitiy() == "SMOREGON" || act[i]->getIdentitiy() == "SNAGGLEGON")
        {
            if(sqrt((a->getX() - act[i]->getX()) * (a->getX() - act[i]->getX()) + (a->getY() - act[i]->getY()) * (a->getY() - act[i]->getY())) < (.75 * (a->getRadius() + act[i]->getRadius())))
            {
                if(a->getIdentitiy() == "CABBAGE")
                {
                    act[i]->setHits(act[i]->getHits() - 2);
                }
                else
                {
                    act[i]->setHits(act[i]->getHits() - 8);
                }
                if(!act[i]->getAlive())
                {

                    alienDead++;
                    
                    if(act[i]->getIdentitiy() == "SMALLGON")
                    {
                        increaseScore(250);
                    }
                    if(act[i]->getIdentitiy() == "SMOREGON")
                    {
                        increaseScore(250);
                        // if(randInt(1, 3) == 1)
                        {
                           // if(randInt(1, 2) == 1)
                            {
                                addToVector(new Repair(act[i]->getX(), act[i]->getY(), this));
                                
                            }
                            // else
                            {
                                addToVector(new FlatTorpGoodie(act[i]->getX(), act[i]->getY(), this));
                                
                            }
                        }
                    }
                    if(act[i]->getIdentitiy() == "SNAGGLEGON")
                    {
                        increaseScore(1000);
                        // if(randInt(1, 6) == 1)
                        {
                            addToVector(new ExtraLife(act[i]->getX(), act[i]->getY(), this));
                            
                        }
                    }
                    act.push_back(new Explosion(act[i]->getX(), act[i]->getY(), this));
                    
                    playSound(SOUND_DEATH);
                }
                else
                {
                    playSound(SOUND_BLAST);
                }
                return true;
            }
        }
    }
    return false;
}



bool StudentWorld :: collisionWithNach(Actors* a)
{
    if(sqrt((a->getX() - nach->getX()) * (a->getX() - nach->getX()) + (a->getY() - nach->getY()) * (a->getY() - nach->getY())) < (.75 * (a->getRadius() + nach->getRadius())))
    {
        if(a->getIdentitiy() == "TURNIP")
        {
            nach->setHits(nach->getHits()-2);
        }
        if(a->getIdentitiy() == "FLATTORPEDO")
        {
            nach->setHits(nach->getHits()-8);
        }
        if(a->getIdentitiy() == "SMALLGON" || a->getIdentitiy() == "SMOREGON" || a->getIdentitiy() == "SNAGGLEGON")
        {
            alienDead++;
            nach->setHits(nach->getHits()-5);
        }
        if(a->getIdentitiy() == "REPAIR")
        {
            nach->setHits(nach->getHits()+10);
        }
        if(a->getIdentitiy() == "FLATTORPGOODIE")
        {
            nach->setTorpedos(nach->getTorpedos()+5);
        }
        if(a->getIdentitiy() == "EXTRALIFE")
        {
            incLives();
        }
        return true;
    }
    return false;
}



void StudentWorld :: shootAtNach(Aliens* a)
{
    if(nach->getX() < a->getX())
    {
        if(nach->getY() - a->getY() <= 4 && nach->getY() - a->getY() >= -4)
        {
            if(a->getIdentitiy() == "SMALLGON")
            {
                if(randInt(1, (20/getLevel())+5) == 1)
                {
                    act.push_back(new Turnip(a->getX() - 14, a->getY(), this));
                    
                    playSound(SOUND_ALIEN_SHOOT);
                }
            }
            if(a->getIdentitiy() == "SMOREGON")
            {
                if(randInt(1, (20/getLevel())+5) == 1)
                {
                    act.push_back(new Turnip(a->getX() - 14, a->getY(), this));
                    
                    playSound(SOUND_ALIEN_SHOOT);
                }
                if(randInt(1, (20/getLevel())+5) == 1)
                {
                    a->setTravDir(180);
                    a->setFlight(VIEW_WIDTH);
                    a->setSpeed(5.0);
                }
            }
            if(a->getIdentitiy() == "SNAGGLEGON")
            {
                if(randInt(1, (15/getLevel())+10) == 1)
                {
                    act.push_back(new FlatTorpedo(a->getX() - 14, a->getY(), 90, this, "NACHENBLASTER"));
                    
                    playSound(SOUND_TORPEDO);
                }
            }
        }
    }
}
















