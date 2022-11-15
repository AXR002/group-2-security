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
#include <time.h>
#include <string>

bool activeMinigame = false;

Vector3 currentDestination;

class Locations
{
  //Class holding the positions of different locations in game	
  public:
  	Vector3 *pwnIsland = new Vector3(-42038.9,-36328,1200);

    Vector3 *goldFarm = new Vector3(20559,41057.3,2200);

    Vector3 *pirateBay = new Vector3(40655,58162.7,200);

    Vector3 *tailMountains = new Vector3(37193.4,-10582.7,2000);

    Vector3 *moltenCave = new Vector3(47549.4,2688.59,380);

    Vector3 locationArray[5] = {*pwnIsland,*goldFarm,*pirateBay,*tailMountains,*moltenCave};
    std::string locationNames[5] =  {"Pwn Island","Gold Farm","Pirate Bay","Tail Mountains","Molten Cave"};

};

//Original chat function
void (*gameChat)(const char *) = (void (*)(const char *))dlsym(RTLD_NEXT, "_ZN6Player4ChatEPKc");

void messagePlayer(std::string s){
	printf(s.c_str(), gameChat);
    gameChat("");
    printf("\n");
}

void Player::Chat(const char *msg)
{
    Vector3 position = this->GetPosition();
    Locations locations;

    if (strncmp("tp ", msg, 3) == 0)
    {
    	//Allows you to teleport to different locations in game
    	// e.g tp Pwn Island 
        if (strncmp("Pwn Island", msg + 3, 10) == 0){
        	messagePlayer("Teleported to Pwn Island");
            this->SetPosition(*locations.pwnIsland);
        }
        else if (strncmp("Gold Farm", msg + 3, 9) == 0){
        	messagePlayer("Teleported to Gold Farm");	
            this->SetPosition(*locations.goldFarm);
        }
        else if (strncmp("Pirate Bay", msg + 3, 14) == 0){
        	messagePlayer("Teleported to Pirate Bay");
            this->SetPosition(*locations.pirateBay);
        }
        else if (strncmp("Tail Mountains", msg + 3, 14) == 0){
        	messagePlayer("Teleported to Tail Mountains");
            this->SetPosition(*locations.tailMountains);
        }
        else if (strncmp("Molten Cave", msg + 3, 10) == 0){
        	messagePlayer("Teleported to Molten Cave");
            this->SetPosition(*locations.moltenCave);
        }
        else{
            Vector3 *new_pos = new Vector3();
            sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z));
        	messagePlayer("Teleported to new location");
            this->SetPosition(*new_pos);
        }
    }

    if (strncmp("set ", msg, 4) == 0)
    {
    	//Allows you to change the value of the player's
    	//speed, health and mana
        if (strncmp("speed ", msg + 4, 6) == 0)
        {
            float newSpeed;
            sscanf(msg + 10, "%f", &(newSpeed));
            this->m_walkingSpeed = newSpeed;
            std::string m = "Speed set to " + std::to_string(newSpeed);
        	messagePlayer(m);

        }
        else if (strncmp("health ", msg + 4, 7) == 0)
        {
            int newHealth;
            sscanf(msg + 11, "%d", &(newHealth));
            this->m_health = newHealth;
            std::string m = "Health set to " + std::to_string(newHealth);
        	messagePlayer(m);
        }
        else if (strncmp("mana ", msg + 4, 5) == 0)
        {
            int newMana;
            sscanf(msg + 9, "%d", &(newMana));
            this->m_mana = newMana;
            std::string m = "Mana set to " + std::to_string(newMana);
        	messagePlayer(m);
        }
    }

    if (strncmp("save ", msg, 5) == 0)
    {
        if (strncmp("loc", msg+5, 4) == 0)
        {
        	//Allows you to save the players location to a text file
        	messagePlayer("Saving location to text file");
            std::fstream outfile;
		    outfile.open("coords.txt", std::ios::app);
		    outfile << "x: " << position.x << ", y: " << position.y << " z: " << position.z << std::endl;
		    outfile.close();
        }
    }

    if (strncmp("pos",msg,3) == 0)
    {
    	//Prints current position to the console
    	messagePlayer("Printed current position to console");
    	std::cout << "x: " << position.x << "\ny: " << position.y << "\nz: " << position.z <<  "\n" << std::flush;
    }

    if (strncmp("start",msg,5) == 0)
    {
    	//This command starts the minigame
    	if (activeMinigame)
    	{
    		messagePlayer("A game is already active!");
    	}
    	else
    	{
    	    messagePlayer("Game has started!");

            //Choose a random location as the destination
            //from the previous location class
    	    srand(time(NULL));
    	    int randNum = rand() % 5;
    	    std::cout << randNum << std::flush;
    	    currentDestination = locations.locationArray[randNum];

    	    //std::cout << "Make your way towards " << locations.locationNames[randNum] << "!\n" << std::flush;
    	    std::string m = "Make your way towards " + locations.locationNames[randNum];
    	    messagePlayer(m);
        
            //Mini game is now active
    	    activeMinigame = true;

    	}
    }
    
}

void World::Tick(float f){
    Player * player = (Player *)(*(ClientWorld *)this).m_activePlayer.m_object;

    Vector3 position = player->GetPosition();
    float x = position.x;
    float y = position.y;
    float z = position.z;

    //std::cout << "x: " << x << "\ny: " << y << "\nz: " << z <<  "\n" std::flush;

    if (activeMinigame){
    	float xVal = currentDestination.x;
    	float yVal = currentDestination.y;
        
        //Each location has a 10000x10000 sized zone
        // If the player reaches that zone, they have reached the destination
    	if( ( (xVal-10000) <= x) && (x <= (xVal+10000) ) && ( (yVal-10000) <= y) && (y <= (yVal+10000) )){
    		messagePlayer("Destination reached!");
    		activeMinigame = false;
    	}
    }

    //player->Chat("printing coordinates");
    //std::string coord_str = "x" + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z) + "\n";

    //printf("x: %f, y: %f, z: %f\n", x, y, z);
    //player->Chat(coord_str.c_str());

    void (*realWorldTick)(float);

    realWorldTick =(void (*)(float))dlsym(RTLD_NEXT,"_ZN5World4TickEf");
    realWorldTick(f); 
}
