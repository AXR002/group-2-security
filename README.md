# group-2-security

## Members: 
- Kevin
- Dilkash 
- Sam 
- Joe 
- Adnan

### Game being hacked: Pwn Adventure 3: Pwnie Island

## How to run our hacks
1. Download the mainHacks.cpp file located [here](LD_PRELOAD_Files/mainHacks.cpp). 

2. Compile the file using the following command:
   - `g++ mainHacks.cpp -shared -fPIC -o mainHacks.so`

3. Place the `.so` file in the same directory as the linux executable for Pwn Adventure 3. 

4. Run the game using the following command:
   - `LD_PRELOAD=mainHacks.so ./PwnAdventure3-Linux-Shipping` 

5. Ensure you play the game on a server in order to utilise the hacks.

6. Type "help" into the in-game chat to see all the different commands our hack allows you to do.

7. Type "start" to start the minigame

8. Enjoy the minigame!
