#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include "resource.h"
#include "callbacks.h"

HWND hWndResult, hWndLni, hWndComboBGG, hWndComboCS, hWndComboMRI, hWndComboPSA, hWndComboPPC, hWndIcon, hWndButtonReset;

/* Briganti Nomogram 3 - EUROPEAN UROLOGY ONCOLOGY 6 (2023) 543–552
 * BGG = Biopsy grade group 5
 * CS  = Clinical stage at mpMRI
 * MRI = Maximum diameter of the index lesion at mpMRI (mm)
 * PSA = Preoperative PSA (ng/ml)
 * PPC = Percentage of positive cores at systematic biopsy
 */ 

// Our application entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  INITCOMMONCONTROLSEX icc;
  WNDCLASSEX wc;
  LPCTSTR MainWndClass = TEXT("Briganti Nomogram");
  HWND hWnd; 
  HWND hWndLabelResult, hWndLabelLni, hWndLabelBGG, hWndLabelCS, hWndLabelMRI, hWndLabelPSA, hWndLabelPPC;
  HACCEL hAccelerators;
  HMENU hSysMenu;
  MSG msg;
  int k;
  TCHAR A[25];

  // Initialise common controls.
  icc.dwSize = sizeof(icc);
  icc.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&icc);

  // Class for our main window.
  wc.cbSize        = sizeof(wc);
  wc.style         = 0;
  wc.lpfnWndProc   = &MainWndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 0, 0, LR_SHARED);
  wc.hCursor       = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
  wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAINMENU);
  wc.lpszClassName = MainWndClass;
  wc.hIconSm       = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);

  // Register our window classes, or error.
  if (! RegisterClassEx(&wc))
  {
    MessageBox(NULL, TEXT("Error registering window class."), TEXT("Error"), MB_ICONERROR | MB_OK);
    return 0;
  }
  
  // Create instance of main window
  // ================================================================  
  hWnd = CreateWindowEx(0, MainWndClass, MainWndClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                        700, 550, NULL, NULL, hInstance, NULL);

  // Error if window creation failed.
  if (! hWnd)
  {
    MessageBox(NULL, TEXT("Error creating main window."), TEXT("Error"), MB_ICONERROR | MB_OK);
    return 0;
  }

  // BGG
  // ================================================================
  hWndLabelBGG = CreateWindow(TEXT("STATIC"), TEXT("Biopsy grade group 5:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 20, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboBGG = CreateWindow(WC_COMBOBOX, TEXT(""),
         CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 40, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  // Add string to combobox.
  swprintf(A, 3, 25, L"No\0");
  SendMessage(hWndComboBGG,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);
  swprintf(A, 4, 25, L"Yes\0");
  SendMessage(hWndComboBGG,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);
      
  // Send the CB_SETCURSEL message to display an initial item in the selection field  
  SendMessage(hWndComboBGG, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // CS
  // ================================================================
  hWndLabelCS = CreateWindow(TEXT("STATIC"), TEXT("Clinical stage:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 90, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboCS = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 110, 200, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  swprintf(A, 15, 25, L"Organ Confined\0");
  SendMessage(hWndComboCS,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);
  swprintf(A, 24, 25, L"Extracapsular Extension\0");
  SendMessage(hWndComboCS,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);
  swprintf(A, 25, 25, L"Seminal Vesicle Invasion\0");
  SendMessage(hWndComboCS,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);

  SendMessage(hWndComboCS, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // MRI
  // ================================================================
  hWndLabelMRI = CreateWindow(TEXT("STATIC"), TEXT("Maximum diameter of the index lesion at mpMRI (mm):"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 160, 400, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboMRI = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 180, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 0; k <= 23; ++k)
  {
	  swprintf(A, 3, 25, L"%d\0", 2*k);
      SendMessage(hWndComboMRI,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboMRI, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // PSA
  // ================================================================
  hWndLabelPSA = CreateWindow(TEXT("STATIC"), TEXT("Preoperative PSA (ng/ml):"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 230, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboPSA = CreateWindow(WC_COMBOBOX, TEXT(""),
         CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 250, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 0; k <= 50; ++k)
  {
	  swprintf(A, 4, 25, L"%d\0", 2*k);
      SendMessage(hWndComboPSA,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A);
  }

  SendMessage(hWndComboPSA, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // PPC
  // ================================================================
  hWndLabelPPC = CreateWindow(TEXT("STATIC"), TEXT("Percentage of positive cores:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 300, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboPPC = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 320, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 0; k <= 10; ++k)
  {
      swprintf(A, 4, 25, L"%d\0", 10*k);
      SendMessage(hWndComboPPC,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboPPC, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
  
  // Image
  // ================================================================
  hWndIcon = CreateWindow(TEXT("STATIC"), NULL,
           WS_VISIBLE | WS_CHILD | SS_BITMAP,
           500, 20, 400, 400, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  HBITMAP hImage = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_CROSS));
  SendMessage(hWndIcon, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);

  // Total points
  // ================================================================  
  hWndLabelResult = CreateWindow(TEXT("STATIC"), TEXT("Total points:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         500, 230, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  hWndResult = CreateWindow(WC_EDIT, TEXT(""), 
         WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER,
         500, 250, 75, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  UpdateResult(hWnd);

  // Risk of LNI
  // ================================================================
  hWndLabelLni = CreateWindow(TEXT("STATIC"), TEXT("Risk of LNI (%):"),
	     WS_VISIBLE | WS_CHILD | SS_LEFT,
	     500, 300, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  hWndLni = CreateWindow(WC_EDIT, TEXT(""),
	     WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER,
	     500, 320, 75, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  UpdateResult(hWnd);

  // Reset button
  // ================================================================
  hWndButtonReset = CreateWindow(WC_BUTTON, TEXT("Reset"),
           BS_DEFPUSHBUTTON | WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER,
           300, 400, 75, 50, hWnd, (HMENU)ID_RESETBUTTON, HINST_THISCOMPONENT, NULL);

  // Load accelerators.
  hAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

  // Add "about" to the system menu.
  hSysMenu = GetSystemMenu(hWnd, FALSE);
  InsertMenu(hSysMenu, 5, MF_BYPOSITION, ID_HELP_INTERPRETATION, TEXT("Interpretation"));
  InsertMenu(hSysMenu, 6, MF_BYPOSITION, ID_HELP_REFERENCE, TEXT("Reference"));
  InsertMenu(hSysMenu, 7, MF_BYPOSITION, ID_HELP_ABOUT, TEXT("About"));

  // Show window and force a paint.
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  // Main message loop.
  while(GetMessage(&msg, NULL, 0, 0) > 0)
  {
    if (!IsDialogMessage(hWnd, &msg) && !TranslateAccelerator(msg.hwnd, hAccelerators, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int) msg.wParam;
}
