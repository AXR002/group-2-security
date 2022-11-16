#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include <cstring>
#include "libGameLogic.h"

void Player::Chat(const char *msg) {

    //Get Pointer to GameAPI
    GameAPI* game = *((GameAPI**)(dlsym(RTLD_NEXT, "Game")));
    printf("GameAPI Pointer: %p\n", game);

    //Get Coin(IItem) Object
    IItem* coin = game->GetItemByName("Coin");
    printf("Coin: %p\n", coin);

    //ItemAndCount Struct
    ItemAndCount myItem;

    if(strncmp("set ", msg, 4)==0) {
        if (strncmp("money ", msg + 4, 6) == 0) {
            int count;
            sscanf(msg + 10, "%d", &(count));
        
            myItem.item = coin;
            myItem.count = count;
            myItem.loadedAmmo = 0;

            //Check if coin key already exists in inventory
            if(this->m_inventory.count(coin) > 0) {
                //Removes coin entry
                this->m_inventory.erase(coin);
            }
            
            this->m_inventory.insert(std::pair<IItem*, ItemAndCount>(coin, myItem));
        }
    }
}
