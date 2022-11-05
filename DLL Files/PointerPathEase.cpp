// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>


uintptr_t getPointerPath(int* pointerPath, int length) {
	
	uintptr_t currentOffset = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");

	for (int i = 0; i < length - 1; i++) {
		currentOffset = *(uintptr_t*)(currentOffset + pointerPath[i]);
	}

	uintptr_t desiredVariableAddress = (currentOffset + pointerPath[length - 1]);

	return desiredVariableAddress;
}


DWORD WINAPI MyThread(HMODULE hModule)
{
	AllocConsole();
	FILE* f = new FILE;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Injection worked\n";
	std::cout << "Process ID is: " << GetCurrentProcessId() << std::endl;

	int x_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x4A4, 0x48, 0x4, 0x240, 0xB4, 0x90};
	int y_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x4A4, 0x48, 0x4, 0x240, 0xB4, 0x94 };
	int z_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x244, 0x20C, 0x1C, 0xD4, 0xD4, 0x98 };

	int x_coord_length = sizeof(x_coord_pointer_path) / sizeof(int);
	int y_coord_length = sizeof(y_coord_pointer_path) / sizeof(int);
	int z_coord_length = sizeof(z_coord_pointer_path) / sizeof(int);

	uintptr_t x_coord_address = getPointerPath(x_coord_pointer_path, x_coord_length);
	uintptr_t y_coord_address = getPointerPath(y_coord_pointer_path, y_coord_length);
	uintptr_t z_coord_address = getPointerPath(z_coord_pointer_path, z_coord_length);

	float* x_coord_address_ptr = (float*)x_coord_address;
	float* y_coord_address_ptr = (float*)y_coord_address;
	float* z_coord_address_ptr = (float*)z_coord_address;

	float x_coord = *x_coord_address_ptr;
	float y_coord = *y_coord_address_ptr;
	float z_coord = *z_coord_address_ptr;

	while (true) {
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
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MyThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}



/*
	uintptr_t x1 = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");

	x1 = *(uintptr_t*)(x1 + 0x18FCD60);
	x1 = *(uintptr_t*)(x1 + 0x20);
	x1 = *(uintptr_t*)(x1 + 0x4A4);
	x1 = *(uintptr_t*)(x1 + 0x48);
	x1 = *(uintptr_t*)(x1 + 0x4);
	x1 = *(uintptr_t*)(x1 + 0x240);
	x1 = *(uintptr_t*)(x1 + 0xB4);
	float* x_coord_address = (float*)(x1 + 0x90);

	float x_coord = *x_coord_address;
	*/



/*
	AllocConsole();
	FILE* f = new FILE;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Injection worked\n";
	std::cout << "Process ID is: " << GetCurrentProcessId() << std::endl;

	int x_coord_pointer_path[] = { 0x18FCD60, 0x20, 0x4A4, 0x48, 0x4, 0x240, 0xB4, 0x90};
	int x_coord_length = sizeof(x_coord_pointer_path) / sizeof(int);

	uintptr_t x1 = (uintptr_t)GetModuleHandle(L"PwnAdventure3-Win32-Shipping.exe");

	for (int i=0; i < x_coord_length-1; i++) {
		x1 = *(uintptr_t*)(x1 + x_coord_pointer_path[i]);
	}

	float* x_coord_address = (float*)(x1 + x_coord_pointer_path[x_coord_length - 1]);
	float x_coord = *x_coord_address;

	while (true) {

		if (x_coord != *x_coord_address) {
			x_coord = *x_coord_address;
			std::cout << " X co-ord: " << x_coord << std::endl;
		}

	}

	return 0;

*/