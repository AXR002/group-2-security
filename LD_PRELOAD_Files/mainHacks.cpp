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

std::fstream leaderboard;
std::fstream leaderboard1;
bool activeMinigame = false;
int32_t timerCount = 0;
float second = 0;
float startDistance;
int currentOrigin;
int currentDestination;

class Locations
{
    // Class holding the positions of different locations in game
public:
    Vector3 *pwnIsland = new Vector3(-39730, -17500, 2450);

    Vector3 *goldFarm = new Vector3(20559, 41057.3, 2200);

    Vector3 *pirateBay = new Vector3(40655, 58162.7, 200);

    Vector3 *tailMountains = new Vector3(37190, -10585, 2000);

    Vector3 *moltenCave = new Vector3(47550, 2689, 380);

    Vector3 *ballmerPeak = new Vector3(-8500, -10086, 9500);

    Vector3 *unbearableWoods = new Vector3(-7970, 64020, 2645);

    Vector3 locationArray[7] = {*pwnIsland, *goldFarm, *pirateBay, *tailMountains, *moltenCave, *ballmerPeak, *unbearableWoods};
    std::string locationNames[7] = {"Pwn Island", "Gold Farm", "Pirate Bay", "Tail Mountains", "Molten Cave", "Ballmer Peak", "Unbearable Woods"};
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
        // Allows you to teleport to different locations in game
        //  e.g tp Pwn Island
        if (strncmp("Pwn Island", msg + 3, 10) == 0)
        {
            messagePlayer("LOCATION: Teleported to Pwn Island");
            this->SetPosition(*locations.pwnIsland);
        }
        else if (strncmp("Gold Farm", msg + 3, 9) == 0)
        {
            messagePlayer("LOCATION: Teleported to Gold Farm");
            this->SetPosition(*locations.goldFarm);
        }
        else if (strncmp("Pirate Bay", msg + 3, 14) == 0)
        {
            messagePlayer("LOCATION: Teleported to Pirate Bay");
            this->SetPosition(*locations.pirateBay);
        }
        else if (strncmp("Tail Mountains", msg + 3, 14) == 0)
        {
            messagePlayer("LOCATION: Teleported to Tail Mountains");
            this->SetPosition(*locations.tailMountains);
        }
        else if (strncmp("Molten Cave", msg + 3, 10) == 0)
        {
            messagePlayer("LOCATION: Teleported to Molten Cave");
            this->SetPosition(*locations.moltenCave);
        }
        else if (strncmp("Ballmer Peak", msg + 3, 13) == 0)
        {
            messagePlayer("LOCATION: Teleported to Ballmer Peak");
            this->SetPosition(*locations.ballmerPeak);
        }
        else if (strncmp("Unbearable Woods", msg + 3, 15) == 0)
        {
            messagePlayer("LOCATION: Teleported to Unbearable Woods");
            this->SetPosition(*locations.unbearableWoods);
        }
        else if (sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z)) == 3)
        {
            messagePlayer("LOCATION: Teleported to new location");
            this->SetPosition(*new_pos);
        }
    }

    else if (strncmp("set ", msg, 4) == 0)
    {
        // Allows you to change the value of the player's
        // speed, health and mana
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
        if (strncmp("loc", msg + 5, 4) == 0)
        {
            // Allows you to save the players location to a text file
            messagePlayer("INFO: Saving location to text file");
            std::fstream outfile;
            outfile.open("coords.txt", std::ios::app);
            outfile << "x: " << position.x << ", y: " << position.y << " z: " << position.z << std::endl;
            outfile.close();
        }
    }

    else if (strncmp("pos", msg, 3) == 0)
    {
        // Prints current position to the console
        messagePlayer("Printed current position to console");
        std::cout << "x: " << position.x << "\ny: " << position.y << "\nz: " << position.z << "\n"
                  << std::flush;
    }

    else if (strncmp("start", msg, 5) == 0)
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
            currentOrigin = rand() % 7;
            currentDestination = currentOrigin;

            while (currentDestination == currentOrigin)
            {
                currentDestination = rand() % 7;
            }

    	    Vector3 newLocation = locations.locationArray[currentOrigin];
            this->SetPosition(newLocation);

            std::string m = "LOCATION: You have been teleported to " + locations.locationNames[currentOrigin];
    	    messagePlayer(m);
            m = "MISSION: Make your way to " + locations.locationNames[currentDestination];
            messagePlayer(m);

            m = "INFO: Your time starts now!";
            messagePlayer(m);
    	    
            timerCount = 0;
            startDistance = Vector3::Distance((locations.locationArray[currentDestination]), position);

            //Mini game is now active
    	    activeMinigame = true;
    	}
    }

    else if (strncmp("leaderboard", msg, 9) == 0)
    {
        messagePlayer("----LEADERBOARD----");
        leaderboard.open("leaderboard.txt", std::ios::in);
        if (leaderboard.is_open())
        {

            std::string line;
            while (getline(leaderboard, line))
            {
                messagePlayer(line);
            }

            leaderboard.close();
        }
    }

    else if (strncmp("help", msg, 4) == 0)
    {
        messagePlayer("All in game commands printed to console");
        printf("start - Starts the mini game\n");
        printf("tp [location] - Teleports you to a specific location\n");
        printf("tp x y z - Teleports you to certain coords\n");
        printf("set [health/mana/speed] [value] - Sets specific values to your character\n");
        printf("save - Saves your current location to a file\n");
        printf("pos - Prints your current location to the console\n");
        printf("leaderboard - Shows your race times\n");
    }

    else
    {
        messagePlayer("ERROR: could not parse command");
    }
}

int32_t Player::GetMana() {
    Locations locations;
    ClientWorld* world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    IPlayer* iplayer= world->m_activePlayer.m_object;
    Player* player = ((Player*)(iplayer));
    if (activeMinigame == false) {
        return this->m_mana;
    }
    else {
        Vector3 currentPosition = player->GetPosition();
        float distance = Vector3::Distance(currentPosition, locations.locationArray[currentDestination]);
        return (int32_t)distance;
    }
}

int32_t Actor::GetHealth() {
    if (activeMinigame == false) {
	    return this->m_health;
    }
    else {
	    int32_t timer = (int32_t)timerCount;
	    return timer;
    }
}

void writetofile1(std::string m)
{

    leaderboard1.open("leaderboardtmp.txt", std::ios::app);
    if (leaderboard1.is_open())
    {
        leaderboard1 << m;
        leaderboard1.close();
    }
}

void World::Tick(float f)
{
    Player *player = (Player *)(*(ClientWorld *)this).m_activePlayer.m_object;

    Vector3 position = player->GetPosition();
    float x = position.x;
    float y = position.y;
    float z = position.z;

    second += f;
    if (second > 1) {
        if (activeMinigame == true) {
            timerCount += 1;
            second = 0;
        }
    }

    if (activeMinigame)
    {
        Locations locations;
        Vector3 posDest = locations.locationArray[currentDestination];

        float xVal = posDest.x;
        float yVal = posDest.y;

        // Each location has a 1000x1000 sized zone
        //  If the player reaches that zone, they have reached the destination
        if (((xVal - 1000) <= x) && (x <= (xVal + 1000)) && ((yVal - 1000) <= y) && (y <= (yVal + 1000)))
        {
            messagePlayer("MISSION: Destination reached!");
            std::string m = "INFO: You took " + std::to_string(timerCount) + " seconds";
            messagePlayer(m);

            leaderboard.open("leaderboard.txt", std::ios::in);
            leaderboard1.open("leaderboardtmp.txt", std::ios::app);

            std::string j = locations.locationNames[currentOrigin] + "->" + locations.locationNames[currentDestination];
            int p = 0;
            if (leaderboard.is_open())
            {

                std::string line;
                while (getline(leaderboard, line))
                {
                    if (line.find(j) != std::string::npos)
                    {
                        std::cout << "found!" << '\n';
                        p++;
                        std::string k = locations.locationNames[currentOrigin] + "->" + locations.locationNames[currentDestination] + " " + std::to_string(timerCount) + "\n";
                        writetofile1(k);
                        std::cout << "done found!" << '\n';
                    }
                    // continue;
                    leaderboard1 << line + "\n";
                }

                leaderboard.close();
            }
            char filename[] = "leaderboard.txt";

            int result = remove(filename);

            if (p == 0)
            {
                if (leaderboard1.is_open())
                {

                    leaderboard1 << locations.locationNames[currentOrigin] + "->" + locations.locationNames[currentDestination] + " " + std::to_string(timerCount) + "\n";
                    leaderboard1.close();
                }
            }
            rename("leaderboardtmp.txt", "leaderboard.txt");

            activeMinigame = false;
        }
    }

    void (*realWorldTick)(float);

    realWorldTick = (void (*)(float))dlsym(RTLD_NEXT, "_ZN5World4TickEf");
    realWorldTick(f);
}