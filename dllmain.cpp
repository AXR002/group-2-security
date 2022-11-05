// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream> 


// This method defines a threat that will run concurrently with the game
DWORD WINAPI MyThread(HMODULE hModule)
{
	// The following 3 lines enable a writable console
	// We don't actually need a console here, but it is very useful to print debugging information to. 
	AllocConsole();
	FILE* f = new FILE;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Injection worked\n";
	std::cout << "Process ID is: " << GetCurrentProcessId() << std::endl;
	// We can see by looking at the process ID in process explorer that this code is being run by the process it was injected into. 


	// From cheat engine analysis we know that
	// z_coord is at memory address: [[[["PwnAdventure3-Win32-Shipping.exe"+018FCD60] + 20 ] + 238 ] + 280 ] + 98 
	// This code follows that pointer path
	//
	// NB this may lead trying to dereference null pointers or reading memory you don't have access to.
	// In other module you would be expected to catch and handle these possible errors 
	uintptr_t PwnAventAddr = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");
	printf("PwnAventAddr: %p\n", PwnAventAddr);
	uintptr_t firstStep = *(uintptr_t*)(PwnAventAddr + 0x18FCD60);
	printf("PwnAventAddr + 0x18FCD60 = %p has value %p\n", PwnAventAddr + 0x18FCD60, firstStep);
	uintptr_t secondStep = *(uintptr_t*)(firstStep + 0x20);
	printf("firstStep + 0x20 = %p has value %p\n", firstStep + 0x20, secondStep);
	uintptr_t thirdStep = *(uintptr_t*)(secondStep + 0x244);
	printf("secondStep + 0x238 = %p has value %p\n", secondStep + 0x244, thirdStep);
	uintptr_t forthStep = *(uintptr_t*)(thirdStep + 0x20C);
	uintptr_t fStep = *(uintptr_t*)(forthStep + 0x1C);
	uintptr_t sixStep = *(uintptr_t*)(fStep + 0xD4);
	uintptr_t sevStep = *(uintptr_t*)(sixStep + 0xD4);
	
	
	// Pointer to the Health
	uintptr_t se1vStep = *(uintptr_t*)(PwnAventAddr + 0x18FED60);
	uintptr_t se2vStep = *(uintptr_t*)(se1vStep + 0x4);
	uintptr_t se3vStep = *(uintptr_t*)(se2vStep + 0x4BC);
	uintptr_t se4vStep = *(uintptr_t*)(se3vStep + 0x238);
	uintptr_t se5vStep = *(uintptr_t*)(se4vStep + 0x47C);
	uintptr_t se6vStep = *(uintptr_t*)(se5vStep + 0x18);
	uintptr_t se7vStep = *(uintptr_t*)(se6vStep + 0x3E0);
	

	float* z_coord_Address = (float*)(sevStep + 0x98);
	float z_coord = *z_coord_Address;

	int* healthadd = (int*)(se7vStep - 0x40);
	int health = *healthadd;

	// This is the main loop that will run in the background while I play the game
	while (true) {
		// If the player z coordinate (height) changes then print it.
		if (z_coord != *z_coord_Address) {
			z_coord = *z_coord_Address;
			std::cout << " Z co-ord: " << z_coord << std::endl;
		}

		// If the player presses 'F' then add 10000 to the players heigth
		// I.e. this makes 'F' a super jump key that will let me jump through solid objects 
		if (GetAsyncKeyState('F') & 1) {
			std::cout << "   F key pressed";
			*z_coord_Address = *z_coord_Address + 10000;
		}

		if (GetAsyncKeyState('H') & 1) {
			std::cout << "   H key pressed";
			*healthadd = *healthadd + 100;
		}

	}
	return 0;
}


// This is the main method that runs when the DLL is injected.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		// We run the cheat code in a seperate thread to stop it interupting the game execution. 
		// Again we dont catch a possible NULL, if we are going down then we can go down in flames. 
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MyThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}