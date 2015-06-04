#include "Core.h"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	Core::initialize(hThisInstance, nCmdShow);

	return 0;
}
