#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <cstring>
#include "libGameLogic.h"


void Player::Chat(const char *msg)
{
    printf("player: %p", this);
 
    if (strncmp("tp ", msg, 3) == 0)
    {
        Vector3 *new_pos = new Vector3();
        sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z));
        this->SetPosition(*new_pos);
    }

    if (strncmp("set ", msg, 4) == 0)
    {
        if (strncmp("speed ", msg + 4, 6) == 0){
            float newSpeed;
            sscanf(msg + 10, "%f", &(newSpeed));
            this->m_walkingSpeed = newSpeed;
        }

        else if (strncmp("health ", msg + 4, 7) == 0){
            int newHealth;
            sscanf(msg + 11, "%d", &(newHealth));
            this->m_health = newHealth;
        }

        else if (strncmp("mana ", msg + 4, 5) == 0){
            int newMana;
            sscanf(msg + 9, "%d", &(newMana));
            this->m_mana = newMana;
        }

    }
}
