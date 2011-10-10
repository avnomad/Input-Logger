#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H
#include <windows.h>

void initializeInput(HWND window);
void processInput(void);
void cleanUpInput();
void moveContextToTop();
void moveContextToBottom();

#endif // PROCESS_INPUT_H