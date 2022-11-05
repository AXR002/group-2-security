#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <cstring>
#include <vector>
#include "libGameLogic.h" // <= this is the header for the target program that was reconstructed using gdb

// Compile this code with:
// g++ speedBoost.cpp -shared -fPIC -o speedBoost.so
//
// To run copy the so file to the game dir and run:
// LD_PRELOAD=speedBoost.so ./PwnAdventure3-Linux-Shipping

// This method will be called everytime the user jumps (presses space).
void Player::Chat(const char *msg)
{

    // load a pointer to the GameWorld object:
    printf("[chat] msg= %s\n", msg);
    if (strncmp("tp ", msg, 3) == 0)
    {
        Vector3 *new_pos = new Vector3();
        sscanf(msg + 3, "%f %f %f", &(new_pos->x), &(new_pos->y), &(new_pos->z));
        this->SetPosition(*new_pos);
    }

    // set the player jump to very high values.
    //

    // We now need to call the orginal Set Jump State method, otherwise the server
    // (and other players) will not see us jump. Here is a pointer we will set to
    // that function:
    // void (*realSetJumpState)(bool);

    // To find the address of the real function we need to find its  orginal address
    // For this we need the "mangled" name, which we found open the libGameLogic.so
    // file in IDA looking at the exprots and right clicking and deselecting "demangle"
    // realSetJumpState = (void (*)(bool))dlsym(RTLD_NEXT, "_ZN6Player12SetJumpStateEb");
}