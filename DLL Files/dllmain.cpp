// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream> 

int x_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x4A4, 0x48, 0x4, 0x240, 0xB4, 0x90 };
int y_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x4A4, 0x48, 0x4, 0x240, 0xB4, 0x94 };
int z_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x244, 0x20C, 0x1C, 0xD4, 0xD4, 0x98 };
int health_pointer_path[] = { 0x018FED60, 0x4,  0x4BC, 0x238, 0x47C, 0x18, 0x3E0, -0x40 };
int mana_pointer_path[] = { 0x018FED60, 0x4,  0x4BC, 0x238, 0x47C, 0x18, 0x3E0, 0xBC };
int sprint_pointer_path[] = { 0x018FED60, 0x4,  0x4BC, 0x238, 0x47C, 0x18, 0x3E0, 0x120 };

float* x_coord_address_ptr;
float* y_coord_address_ptr;
float* z_coord_address_ptr;
int* health_address_ptr;
int* mana_address_ptr;
float* sprint_address_ptr;

bool flyHack = false;

uintptr_t getPointerPath(int* pointerPath, int length) {

	uintptr_t currentOffset = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");

	for (int i = 0; i < length - 1; i++) {
		currentOffset = *(uintptr_t*)(currentOffset + pointerPath[i]);
	}

	uintptr_t desiredVariableAddress = (currentOffset + pointerPath[length - 1]);

	return desiredVariableAddress;
}

void init() {
	uintptr_t x_coord_address = getPointerPath(x_coord_pointer_path, sizeof(x_coord_pointer_path) / sizeof(int));
	uintptr_t y_coord_address = getPointerPath(y_coord_pointer_path, sizeof(y_coord_pointer_path) / sizeof(int));
	uintptr_t z_coord_address = getPointerPath(z_coord_pointer_path, sizeof(z_coord_pointer_path) / sizeof(int));
	uintptr_t health_address = getPointerPath(health_pointer_path, sizeof(health_pointer_path) / sizeof(int));
	uintptr_t mana_address = getPointerPath(mana_pointer_path, sizeof(mana_pointer_path) / sizeof(int));
	uintptr_t sprint_address = getPointerPath(sprint_pointer_path, sizeof(sprint_pointer_path) / sizeof(int));

	x_coord_address_ptr = (float*)x_coord_address;
	y_coord_address_ptr = (float*)y_coord_address;
	z_coord_address_ptr = (float*)z_coord_address;
	health_address_ptr = (int*)health_address;
	mana_address_ptr = (int*)mana_address;
	sprint_address_ptr = (float*)sprint_address;
}

void setCoords(float x, float y, float z=10000) {
	*x_coord_address_ptr = x;
	*y_coord_address_ptr = y;
	*z_coord_address_ptr = z;
}

void displayCoords() {
	float x_coord = *x_coord_address_ptr;
	float y_coord = *y_coord_address_ptr;
	float z_coord = *z_coord_address_ptr;

	bool x_changed = (x_coord != *(float*)x_coord_address_ptr);
	bool y_changed = (y_coord != *(float*)y_coord_address_ptr);
	bool z_changed = (z_coord != *(float*)z_coord_address_ptr);

	if (x_changed || y_changed || z_changed) {
		x_coord = *(float*)x_coord_address_ptr;
		y_coord = *(float*)y_coord_address_ptr;
		z_coord = *(float*)z_coord_address_ptr;
		std::cout << " X co-ord: " << x_coord << " Y co-ord: " << y_coord << " Z co-ord: " << z_coord << std::endl;
	}
}

void checkForKeyChanges() {
	
	// Teleports the player to the gun shop
	if (GetAsyncKeyState('K') & 1) {
		std::cout << "   K key pressed";
		setCoords(-37883, -19920, 2491);
	}

	// Teleports the player to the Sewers
	if (GetAsyncKeyState('J') & 1) {
		std::cout << "   J key pressed";
		setCoords(-44966, -35418, 670);
	}

	if (GetAsyncKeyState('M') & 1) {
		std::cout << "   M key pressed";
		*mana_address_ptr = *mana_address_ptr + 100;
	}

	//while (heals_remaining!=0) {
	if (GetAsyncKeyState('H') & 1) {
		std::cout << "   H key pressed";
		*health_address_ptr = *health_address_ptr + 100;
		//heals_remaining = heals_remaining - 1;
	//}
	}


	
}

void coordRestraintLogic() {



}



DWORD WINAPI MyThread(HMODULE hModule)
{
	AllocConsole();
	FILE* f = new FILE;
	freopen_s(&f, "CONOUT$", "w", stdout);
	std::cout << "Process ID is: " << GetCurrentProcessId() << std::endl;

	init();

	setCoords(-40982, 9000, 100);

	while (true) {
		//displayCoords();

		checkForKeyChanges();

		//coordRestraintLogic();

	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MyThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}