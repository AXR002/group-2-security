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

bool timerActive = false;
int32_t timerCount = 0;
float second = 0;

Vector3 currentDestination;

class Locations
{
  //Class holding the positions of different locations in game	
  public:
  	Vector3 *pwnIsland = new Vector3(-39730,-17500, 2450);

    Vector3 *goldFarm = new Vector3(20559,41057.3,2200);

    Vector3 *pirateBay = new Vector3(40655,58162.7,200);

    Vector3 *tailMountains = new Vector3(37190,-10585,2000);

    Vector3 *moltenCave = new Vector3(47550,2689,380);

    Vector3 *ballmerPeak = new Vector3(-8500, -10086, 9500);

    Vector3 *unbearableWoods = new Vector3(-7970, 64020, 2645);

    Vector3 locationArray[7] = {*pwnIsland,*goldFarm,*pirateBay,*tailMountains,*moltenCave, *ballmerPeak, *unbearableWoods};
    std::string locationNames[7] =  {"Pwn Island","Gold Farm","Pirate Bay","Tail Mountains","Molten Cave", "Ballmer Peak", "Unbearable Woods"};

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

    Vector3 *new_pos = new Vector3();

    if (strncmp("tp ", msg, 3) == 0)
    {
    	//Allows you to teleport to different locations in game
    	// e.g tp Pwn Island 
        if (strncmp("Pwn Island", msg + 3, 10) == 0){
        	messagePlayer("LOCATION: Teleported to Pwn Island");
            this->SetPosition(*locations.pwnIsland);
        }
        else if (strncmp("Gold Farm", msg + 3, 9) == 0){
        	messagePlayer("LOCATION: Teleported to Gold Farm");	
            this->SetPosition(*locations.goldFarm);
        }
        else if (strncmp("Pirate Bay", msg + 3, 14) == 0){
        	messagePlayer("LOCATION: Teleported to Pirate Bay");
            this->SetPosition(*locations.pirateBay);
        }
        else if (strncmp("Tail Mountains", msg + 3, 14) == 0){
        	messagePlayer("LOCATION: Teleported to Tail Mountains");
            this->SetPosition(*locations.tailMountains);
        }
        else if (strncmp("Molten Cave", msg + 3, 10) == 0){
        	messagePlayer("LOCATION: Teleported to Molten Cave");
            this->SetPosition(*locations.moltenCave);
        }
        else if (strncmp("Ballmer Peak", msg + 3, 11) == 0){
        	messagePlayer("LOCATION: Teleported to Ballmer Peak");
            this->SetPosition(*locations.ballmerPeak);
        }
        else if (strncmp("Unbearable Woods", msg + 3, 15) == 0){
        	messagePlayer("LOCATION: Teleported to Unbearable Woods");
            this->SetPosition(*locations.unbearableWoods);
        }
        else if (sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z)) == 3){
            messagePlayer("LOCATION: Teleported to new location");
            this->SetPosition(*new_pos);
        }       
    }

    else if (strncmp("set ", msg, 4) == 0)
    {
    	//Allows you to change the value of the player's
    	//speed, health and mana
        if (strncmp("speed ", msg + 4, 6) == 0)
        {
            float newSpeed;
            sscanf(msg + 10, "%f", &(newSpeed));
            this->m_walkingSpeed = newSpeed;
            std::string m = "VALUE: Speed set to " + std::to_string(newSpeed);
        	messagePlayer(m);

        }
        else if (strncmp("health ", msg + 4, 7) == 0)
        {
            int newHealth;
            sscanf(msg + 11, "%d", &(newHealth));
            this->m_health = newHealth;
            std::string m = "VALUE: Health set to " + std::to_string(newHealth);
        	messagePlayer(m);
        }
        else if (strncmp("mana ", msg + 4, 5) == 0)
        {
            int newMana;
            sscanf(msg + 9, "%d", &(newMana));
            this->m_mana = newMana;
            std::string m = "VALUE: Mana set to " + std::to_string(newMana);
        	messagePlayer(m);
        }
    }

    else if (strncmp("save ", msg, 5) == 0)
    {
        if (strncmp("loc", msg+5, 4) == 0)
        {
        	//Allows you to save the players location to a text file
        	messagePlayer("INFO: Saving location to text file");
            std::fstream outfile;
		    outfile.open("coords.txt", std::ios::app);
		    outfile << "x: " << position.x << ", y: " << position.y << " z: " << position.z << std::endl;
		    outfile.close();
        }
    }

    else if (strncmp("pos",msg,3) == 0)
    {
    	//Prints current position to the console
    	messagePlayer("Printed current position to console");
    	std::cout << "x: " << position.x << "\ny: " << position.y << "\nz: " << position.z <<  "\n" << std::flush;
    }

    else if (strncmp("start",msg,5) == 0)
    {
    	//This command starts the minigame
    	if (activeMinigame)
    	{
    		messagePlayer("ERROR: A game is already active!");
    	}
    	else
    	{
    	    messagePlayer("INFO: Game has started!");
            //Choose random start and destination location 
            int randFrom = rand() % 7;
            int randTo = randFrom;

            while (randTo == randFrom){
                randTo = rand() % 7;
            }

    	    Vector3 newLocation = locations.locationArray[randFrom];
            this->SetPosition(newLocation);

            currentDestination = locations.locationArray[randTo];

            std::string m = "LOCATION: You have been teleported to " + locations.locationNames[randFrom];
    	    messagePlayer(m);
            m = "MISSION: Make your way to " + locations.locationNames[randTo];
            messagePlayer(m);

            timerActive = true;
            
            m = "INFO: Your time starts now!";
            messagePlayer(m);
    	    
    	    activeMinigame = true;
    	}
    }

    else{
        messagePlayer("ERROR: could not parse command");
    }
}

int32_t Actor::GetHealth() {
    if (timerActive == false) {
	return 100;
    }
    else {
	int32_t timer = (int32_t)timerCount;
	return timer;
    }
}

void World::Tick(float f){
    Player * player = (Player *)(*(ClientWorld *)this).m_activePlayer.m_object;

    Vector3 position = player->GetPosition();
    float x = position.x;
    float y = position.y;
    float z = position.z;

    second += f;
    if (second > 1) {
        if (timerActive == true) {
            timerCount += 1;
            second = 0;
        }
    }

    if (activeMinigame){
    	float xVal = currentDestination.x;
    	float yVal = currentDestination.y;
        
        //Each location has a 5000x5000 sized zone
        // If the player reaches that zone, they have reached the destination
    	if( ( (xVal-5000) <= x) && (x <= (xVal+5000) ) && ( (yVal-5000) <= y) && (y <= (yVal+5000) )){
    		messagePlayer("MISSION: Destination reached!");
            std::string m = "INFO: You took " + std::to_string(timerCount) + " seconds";
            messagePlayer(m);
            timerActive = false;
    		activeMinigame = false;
    	}
    }

    void (*realWorldTick)(float);

    realWorldTick =(void (*)(float))dlsym(RTLD_NEXT,"_ZN5World4TickEf");
    realWorldTick(f); 
}