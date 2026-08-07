#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <windows.h>
#include <windowsx.h>

// Window procedure for our main window.
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Dialog procedure for our "about" dialog.
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

void UpdateResult(HWND hWndMain);

#endif
