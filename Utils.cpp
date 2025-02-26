#include "Utiles.h"

void ShowErrorDialog(const char* message) {
	MessageBoxA(nullptr, message, "Error", MB_OK | MB_ICONERROR);
}