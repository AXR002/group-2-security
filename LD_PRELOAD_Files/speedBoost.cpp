#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <cstring>
#include "libGameLogic.h"
#include <fstream>
#include <iostream>

void Player::Chat(const char *msg)
{
    Vector3 position = this->GetPosition();

    if (strncmp("tp ", msg, 3) == 0)
    {
        Vector3 *new_pos = new Vector3();
        
        if (strncmp("Pwn Island", msg + 3, 10) == 0){
            new_pos->x = -42038.9;
            new_pos->y = -36328;
            new_pos->z = 1200;

        }
        else if (strncmp("Gold Farm", msg + 3, 9) == 0){
            new_pos->x = 20559;
            new_pos->y = 41057.3;
            new_pos->z = 2200;

        }
        else if (strncmp("The Pirate Bay", msg + 3, 14) == 0){
            new_pos->x = 40655;
            new_pos->y = 58162.7;
            new_pos->z = 200;

        }
        else if (strncmp("Tail Mountains", msg + 3, 14) == 0){
            new_pos->x = 37193.4;
            new_pos->y = -10582.7;
            new_pos->z = 2000;

        }
        else if (strncmp("Molten Cave", msg + 3, 10) == 0){
            new_pos->x = 47549.4;
            new_pos->y = 2688.59;
            new_pos->z = 380;
        }
        else{
            sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z));
        }
        this->SetPosition(*new_pos);
    }

    if (strncmp("set ", msg, 4) == 0)
    {
        if (strncmp("speed ", msg + 4, 6) == 0)
        {
            float newSpeed;
            sscanf(msg + 10, "%f", &(newSpeed));
            this->m_walkingSpeed = newSpeed;
        }

        else if (strncmp("health ", msg + 4, 7) == 0)
        {
            int newHealth;
            sscanf(msg + 11, "%d", &(newHealth));
            this->m_health = newHealth;
        }

        else if (strncmp("mana ", msg + 4, 5) == 0)
        {
            int newMana;
            sscanf(msg + 9, "%d", &(newMana));
            this->m_mana = newMana;
        }
    }

    if (strncmp("save ", msg, 5) == 0)
    {
        if (strncmp("loc", msg+5, 4) == 0)
        {
            std::fstream outfile;
		    outfile.open("coords.txt", std::ios::app);
		    outfile << "x: " << position.x << ", y: " << position.y << " z: " << position.z << std::endl;
		    outfile.close();
        }
    }

    // Real in-game chat where we can input
    //void (*gameChat)(const char *);
    //gameChat = (void (*)(const char *))dlsym(RTLD_NEXT, "_ZN6Player4ChatEPKc");

    //gameChat("");
    //printf("This prints to chat", gameChat);
    
}


void World::Tick(float f){
    Player * player = (Player *)(*(ClientWorld *)this).m_activePlayer.m_object;

    Vector3 position = player->GetPosition();
    float x = position.x;
    float y = position.y;
    float z = position.z;

    //player->Chat("printing coordinates");
    //std::string coord_str = "x" + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + "\n";

    //printf("x: %f, y: %f, z: %f\n", x, y, z);
    //player->Chat(coord_str.c_str());

    void (*realWorldTick)(float);

    realWorldTick =(void (*)(float))dlsym(RTLD_NEXT,"_ZN5World4TickEf");
    realWorldTick(f); 
}
