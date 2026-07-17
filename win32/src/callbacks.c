#include "callbacks.h"
#include "resource.h"
#include <math.h>


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

        case ID_HELP_REFERENCE:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_REFERENCEDIALOG), hWnd, &AboutDialogProc);
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
        case ID_HELP_REFERENCE:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_REFERENCEDIALOG), hWnd, &AboutDialogProc);
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
    if (index < 0 || upperBound < index)
    {
        MessageBox(hWndMain, message, TEXT("Error"), MB_ICONERROR | MB_OK);
    }
    return index;
}


void UpdateResult(HWND hWndMain)
{
    int indexBGG, indexCS, indexMRI, indexPSA, indexPPC;
    TCHAR A[7];
    double lni = 0;
    double totalpoints = 0;

    indexBGG = GetAndValidateIndex(hWndMain, hWndComboBGG,  1, TEXT("BGG out of range."));
    
    indexCS  = GetAndValidateIndex(hWndMain, hWndComboCS,   2, TEXT("CS out of range."));
    
    indexMRI = GetAndValidateIndex(hWndMain, hWndComboMRI,  45, TEXT("MRI out of range."));
    
    indexPSA = GetAndValidateIndex(hWndMain, hWndComboPSA, 100, TEXT("PSA out of range."));
    
    indexPPC = GetAndValidateIndex(hWndMain, hWndComboPPC, 100, TEXT("PPC out of range."));

    if (1 == indexBGG)
    	totalpoints += 40;
    switch(indexCS)
    {
        case 0: totalpoints += 0; break;
        case 1: totalpoints += 33; break;  // 32.5 + 2.5*(2/10) = 33
        case 2: totalpoints += 63; break;  // 62.5 + 2.5*(2/10) = 63
    }
    totalpoints += 100*indexMRI/45;
    totalpoints += 38.5*indexPSA/100;      // psa(100) = 37.5 + 2.5*(4/10) = 38.5
    totalpoints += 73.0*indexPPC/100;      // ppc( 50) = 35.0 + 2.5*(6/10) = 36.5

    if (totalpoints < 0 || 300 < totalpoints)
    {
        MessageBox(hWndMain, TEXT("Total Points exceed expected range [0,300]"), TEXT("Warning"), MB_ICONWARNING | MB_OK);
    }

    if (0 < totalpoints)
        lni = 0.409613*log(totalpoints) - 1.510997;  // lni(x) = a*log(x) + b; lni(42) = 0.02; lni(282) = 0.8
    // l(x) = a*log(x+b) + c, l(140)=0.15, l(180)=0.3, l(200)=0.4

    swprintf(A, 6, L"%.1f\0", totalpoints);
    SendMessage(hWndResult, (UINT) WM_SETTEXT, (WPARAM) FALSE, (LPARAM) A);

    swprintf(A, 7, L"%.3f\0", lni);
    SendMessage(hWndLni, (UINT) WM_SETTEXT, (WPARAM) FALSE, (LPARAM) A);
/*
    SendMessage(hWndComboBGG, WM_KILLFOCUS, (WPARAM)0, (LPARAM)0);
    SendMessage(hWndComboCS,  WM_KILLFOCUS, (WPARAM)0, (LPARAM)0);
    SendMessage(hWndComboMRI, WM_KILLFOCUS, (WPARAM)0, (LPARAM)0);
    SendMessage(hWndComboPSA, WM_KILLFOCUS, (WPARAM)0, (LPARAM)0);
    SendMessage(hWndComboPPC, WM_KILLFOCUS, (WPARAM)0, (LPARAM)0);
*/
}
