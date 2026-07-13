#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include "resource.h"
#include "callbacks.h"

/* Briganti Nomogram EPLD 2012
 * PSA = Prostate specific antigen
 * CS  = Clinical stage
 * PGG = Primary Gleason Grade
 * SGG = Secondary Gleason Grade
 * PPC = Percentage of positive cores
 */ 

// Our application entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  INITCOMMONCONTROLSEX icc;
  WNDCLASSEX wc;
  LPCTSTR MainWndClass = TEXT("Briganti Nomogram");
  HWND hWnd; 
  HWND hWndLabelResult, hWndLabelPSA, hWndLabelCS, hWndLabelPGG, hWndLabelSGG, hWndLabelPPC;
  HACCEL hAccelerators;
  HMENU hSysMenu;
  MSG msg;
  int k;
  TCHAR A[4];

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
                        700, 450, NULL, NULL, hInstance, NULL);

  // Error if window creation failed.
  if (! hWnd)
  {
    MessageBox(NULL, TEXT("Error creating main window."), TEXT("Error"), MB_ICONERROR | MB_OK);
    return 0;
  }

  // PSA
  // ================================================================
  hWndLabelPSA = CreateWindow(TEXT("STATIC"), TEXT("Prostate specific antigen:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 20, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboPSA = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 40, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 1; k <= 18; ++k)
  {
      // Add string to combobox.
      swprintf(A,3,L"%d",2*k);
      SendMessage(hWndComboPSA,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }
      
  // Send the CB_SETCURSEL message to display an initial item in the selection field  
  SendMessage(hWndComboPSA, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // CS
  // ================================================================
  hWndLabelCS = CreateWindow(TEXT("STATIC"), TEXT("Clinical stage:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 90, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboCS = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 110, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 1; k <= 3; ++k)
  {
      swprintf(A,3,L"T%d",k);
      SendMessage(hWndComboCS,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboCS, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  
  // PGG
  // ================================================================
  hWndLabelPGG = CreateWindow(TEXT("STATIC"), TEXT("Primary Gleason grade:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 160, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboPGG = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 180, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 3; k <= 4; ++k)
  {
      swprintf(A, 4, (k==3)? L"<=%d" : L">=%d", k);
      SendMessage(hWndComboPGG,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboPGG, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // SGG
  // ================================================================
  hWndLabelSGG = CreateWindow(TEXT("STATIC"), TEXT("Secondary Gleason grade:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 230, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboSGG = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 250, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 3; k <= 4; ++k)
  {
      swprintf(A, 4, (k==3)? L"<=%d" : L">=%d", k);
      SendMessage(hWndComboSGG,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboSGG, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

  // PPC
  // ================================================================
  hWndLabelPPC = CreateWindow(TEXT("STATIC"), TEXT("Percentage of positive cores:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         40, 300, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  
  hWndComboPPC = CreateWindow(WC_COMBOBOX, TEXT(""), 
         CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP,
         40, 320, 75, 350, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  for (k = 1; k <= 10; ++k)
  {
      swprintf(A, 3, L"%d", 10*k);
      SendMessage(hWndComboPPC,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) A); 
  }

  SendMessage(hWndComboPPC, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
  

  // Total points
  // ================================================================  
  hWndLabelResult = CreateWindow(TEXT("STATIC"), TEXT("Total points:"),
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         500, 160, 200, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);

  hWndResult = CreateWindow(WC_EDIT, TEXT(""), 
         WS_CHILD | WS_TABSTOP | WS_VISIBLE,
         500, 180, 75, 20, hWnd, NULL, HINST_THISCOMPONENT, NULL);
  UpdateResult(hWnd);


  // Load accelerators.
  hAccelerators = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

  // Add "about" to the system menu.
  hSysMenu = GetSystemMenu(hWnd, FALSE);
  InsertMenu(hSysMenu, 5, MF_BYPOSITION, ID_HELP_INTERPRETATION, TEXT("Interpretation"));
  InsertMenu(hSysMenu, 6, MF_BYPOSITION, ID_HELP_ABOUT, TEXT("About"));

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
