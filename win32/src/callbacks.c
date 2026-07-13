#include "callbacks.h"
#include "resource.h"


// Window procedure for our main window.
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static HINSTANCE hInstance;

  switch (msg)
  {
    case WM_COMMAND:
    {
        if(HIWORD(wParam) == CBN_SELCHANGE)
        // If the user makes a selection from the list:
        //   Send CB_GETCURSEL message to get the index of the selected list item.
        //   Send CB_GETLBTEXT message to get the item.
        //   Display the item in a messagebox.
        {
            UpdateResult(hWnd);
            /*
            int ItemIndex = SendMessage((HWND) lParam, (UINT) CB_GETCURSEL,
                                        (WPARAM) 0, (LPARAM) 0);
            TCHAR  ListItem[256];
            (TCHAR) SendMessage((HWND) lParam, (UINT) CB_GETLBTEXT,
                                (WPARAM) ItemIndex, (LPARAM) ListItem);
            MessageBox(hWnd, (LPCWSTR) ListItem, TEXT("Item Selected"), MB_OK);
            */
        }
      switch (LOWORD(wParam))
      {
        case ID_HELP_ABOUT:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }

        case ID_HELP_INTERPRETATION:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_INTERPRETATIONDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }

        case ID_FILE_EXIT:
        {
          DestroyWindow(hWnd);
          return 0;
        }
      }
      break;
    }

    case WM_GETMINMAXINFO:
    {
      MINMAXINFO *minMax = (MINMAXINFO*) lParam;
      minMax->ptMinTrackSize.x = 220;
      minMax->ptMinTrackSize.y = 110;

      return 0;
    }

    case WM_SYSCOMMAND:
    {
      switch (LOWORD(wParam))
      {
        case ID_HELP_ABOUT:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }
        case ID_HELP_INTERPRETATION:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_INTERPRETATIONDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }
      }
      break;
    }
    
    case WM_CREATE:
    {
      hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
      return 0;
    }

    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Dialog procedure for our "about" dialog.
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case IDOK:
        case IDCANCEL:
        {
          EndDialog(hwndDlg, (INT_PTR) LOWORD(wParam));
          return (INT_PTR) TRUE;
        }
      }
      break;
    }

    case WM_INITDIALOG:
      return (INT_PTR) TRUE;
  }

  return (INT_PTR) FALSE;
}

int GetSelectedIndex(HWND hWndMain, HWND hWndCombo)
{
    int itemIndex = -1;
    if (!hWndCombo)
    {
        MessageBox(hWndMain, TEXT("ComboBox missing."), TEXT("Error"), MB_ICONERROR | MB_OK);
        return -1;
    }
    itemIndex = SendMessage((HWND) hWndCombo, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
    return itemIndex;
}


int GetAndValidateIndex(HWND hWndMain, HWND hWndCombo, int upperBound, LPCTSTR message)
{
    int index = GetSelectedIndex(hWndMain, hWndCombo);
    if (index < 0 || upperBound <= index)
    {
        MessageBox(hWndMain, message, TEXT("Error"), MB_ICONERROR | MB_OK);
    }
    return index;
}


void UpdateResult(HWND hWndMain)
{
    int indexPSA, indexCS, indexPGG, indexSGG, indexPPC;
    TCHAR A[4];
    int totalpoints = 0;
    const int psapoints[] = {3,5,8,11,13,16,19,22,25,27,29,32,35,38,40,42,45,48};
    indexPSA = GetAndValidateIndex(hWndMain, hWndComboPSA, sizeof(psapoints)/sizeof(int), TEXT("PSA out of range."));
    
    indexCS  = GetAndValidateIndex(hWndMain, hWndComboCS, 3, TEXT("CS out of range."));
    
    indexPGG = GetAndValidateIndex(hWndMain, hWndComboPGG, 2, TEXT("PGG out of range."));
    
    indexSGG = GetAndValidateIndex(hWndMain, hWndComboSGG, 2, TEXT("SGG out of range."));
    
    indexPPC = GetAndValidateIndex(hWndMain, hWndComboPPC, 10, TEXT("PPC out of range."));

    totalpoints += psapoints[indexPSA];
    switch(indexCS)
    {
        case 0: totalpoints += 0; break;
        case 1: totalpoints += 22; break;
        case 2: totalpoints += 74; break;
    }
    if (1==indexPGG)
        totalpoints += 48;
    if (1==indexSGG)
        totalpoints += 13;
    totalpoints += 10*(indexPPC+1);
    
    swprintf(A, 4, L"%d", totalpoints);
    SendMessage(hWndResult, (UINT) WM_SETTEXT, (WPARAM) FALSE, (LPARAM) A); 
}
