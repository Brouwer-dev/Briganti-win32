#ifndef RESOURCE_h
#define RESOURCE_h

#define IDI_APPICON                     101
#define IDR_MAINMENU                    102
#define IDD_ABOUTDIALOG                 103
#define IDR_ACCELERATOR                 104
#define IDD_INTERPRETATIONDIALOG        105
#define IDD_REFERENCEDIALOG             106
#define IDB_CROSS                       107
#define ID_HELP_ABOUT                   40001
#define ID_FILE_EXIT                    40002
#define ID_HELP_INTERPRETATION          40003
#define ID_HELP_REFERENCE               40004

#define IDC_STATIC -1

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#include <windows.h>
#include <stdio.h>
#ifdef WIN32
#define swprintf _snwprintf
#endif


extern HWND hWndResult, hWndLni, hWndComboBGG, hWndComboCS, hWndComboMRI, hWndComboPSA, hWndComboPPC;

#endif  // RESOURCE_h
