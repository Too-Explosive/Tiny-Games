#include <Windows.h>
#include <tchar.h>
#include <random>
#include <time.h>
#include <string>
#define RESET_MENU 98

LRESULT CALLBACK WndProc(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);
inline bool playerWon();
inline bool compWon();
inline bool catGame();
int compNearWin();
int playerNearCenterWin();
char board[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
bool ended = false;
int wins = 0, losses = 0, ties = 0;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	srand(time(0));
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("Tic-Tac-Toe");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Tic-Tac-Toe"), NULL);
		return 1;
	}
	static TCHAR szWindowClass[] = _T("Tic-Tac-Toe");
	static TCHAR szTitle[] = _T("Tic-Tac-Toe");
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Tic-Tac-Toe"), NULL);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	const unsigned center = 5, topLeft = 1, topMid = 2, topRight = 3, midLeft = 4, midRight = 6, botLeft = 7, botMid = 8, botRight = 9;
	static HWND centerButton, topLeftButton, topMidButton, topRightButton, midLeftButton, midRightButton, botLeftButton, botMidButton, botRightButton, victory, reset, title;
	std::string eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
	std::wstring good = std::wstring(eh.begin(), eh.end());
	switch (message)
	{
		case WM_CREATE:

			centerButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13, 200 - 13, 26, 26, hWnd, (HMENU)center, NULL, NULL);
			midLeftButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13 - 26, 200 - 13, 26, 26, hWnd, (HMENU)midLeft, NULL, NULL);
			midRightButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 + 13, 200 - 13, 26, 26, hWnd, (HMENU)midRight, NULL, NULL);
			topLeftButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13 - 26, 200 - 13 - 26, 26, 26, hWnd, (HMENU)topLeft, NULL, NULL);
			topMidButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13, 200 - 13 - 26, 26, 26, hWnd, (HMENU)topMid, NULL, NULL);
			topRightButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 + 13, 200 - 13 - 26, 26, 26, hWnd, (HMENU)topRight, NULL, NULL);
			botLeftButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13 - 26, 200 + 13, 26, 26, hWnd, (HMENU)botLeft, NULL, NULL);
			botMidButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 - 13, 200 + 13, 26, 26, hWnd, (HMENU)botMid, NULL, NULL);
			botRightButton = CreateWindow(TEXT("BUTTON"), (LPCWSTR)"", WS_CHILD | WS_VISIBLE, 200 + 13, 200 + 13, 26, 26, hWnd, (HMENU)botRight, NULL, NULL);
			title = CreateWindow(TEXT("STATIC"), good.c_str(), WS_CHILD | WS_VISIBLE, 0, 20, 140, 60, hWnd, NULL, NULL, NULL);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case RESET_MENU:
					ShowWindow(reset, WS_MINIMIZE);
					ShowWindow(reset, SW_HIDE);
					ShowWindow(victory, SW_MINIMIZE);
					ShowWindow(victory, SW_HIDE);
					for (int i = 0; i < 10; i++)
						board[i] = ' ';
					SetWindowText(topLeftButton, _T(" "));
					SetWindowText(topMidButton, _T(" "));
					SetWindowText(topRightButton, _T(" "));
					SetWindowText(midLeftButton, _T(" "));
					SetWindowText(centerButton, _T(" "));
					SetWindowText(midRightButton, _T(" "));
					SetWindowText(botLeftButton, _T(" "));
					SetWindowText(botMidButton, _T(" "));
					SetWindowText(botRightButton, _T(" "));
					UpdateWindow(hWnd);
					ended = false;
				case center:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						ShowWindow(victory, SW_HIDE);
						SetWindowText(centerButton, _T("X"));
						board[center] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case midLeft:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(midLeftButton, _T("X"));
						board[midLeft] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case midRight:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(midRightButton, _T("X"));
						board[midRight] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case topLeft:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(topLeftButton, _T("X"));
						board[topLeft] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case topMid:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(topMidButton, _T("X"));
						board[topMid] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case topRight:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(topRightButton, _T("X"));
						board[topRight] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case botLeft:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(botLeftButton, _T("X"));
						board[botLeft] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case botMid:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(botMidButton, _T("X"));
						board[botMid] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
					}
					if (compWon())
					{
						ended = true;
						eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
						good = std::wstring(eh.begin(), eh.end());
						SetWindowText(title, good.c_str());
						victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
						reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
					}
					break;
				case botRight:
					if (board[LOWORD(wParam)] == ' ' && !ended)
					{
						SetWindowText(botRightButton, _T("X"));
						board[botRight] = 'X';
						if (playerWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You won!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (catGame())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("Cat's game!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
						else if (compNearWin() != 0)
						{
							if (compNearWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (compNearWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (compNearWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (compNearWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (compNearWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (compNearWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (compNearWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (compNearWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else if (playerNearCenterWin() != 0)
						{
							if (playerNearCenterWin() == 1)
							{
								SetWindowText(topLeftButton, _T("O"));
								board[1] = 'O';
							}
							else if (playerNearCenterWin() == 2)
							{
								SetWindowText(topMidButton, _T("O"));
								board[2] = 'O';
							}
							else if (playerNearCenterWin() == 3)
							{
								SetWindowText(topRightButton, _T("O"));
								board[3] = 'O';
							}
							else if (playerNearCenterWin() == 4)
							{
								SetWindowText(midLeftButton, _T("O"));
								board[4] = 'O';
							}
							else if (playerNearCenterWin() == 5)
							{
								SetWindowText(centerButton, _T("O"));
								board[5] = 'O';
							}
							else if (playerNearCenterWin() == 6)
							{
								SetWindowText(midRightButton, _T("O"));
								board[6] = 'O';
							}
							else if (playerNearCenterWin() == 7)
							{
								SetWindowText(botLeftButton, _T("O"));
								board[7] = 'O';
							}
							else if (playerNearCenterWin() == 8)
							{
								SetWindowText(botMidButton, _T("O"));
								board[8] = 'O';
							}
							else
							{
								SetWindowText(botRightButton, _T("O"));
								board[9] = 'O';
							}
						}
						else
						{
							int x = (rand() % 9) + 1;
							while (board[x] != ' ')
								x = (rand() % 9) + 1;
							board[x] = 'O';
							if (x == 1)
								SetWindowText(topLeftButton, _T("O"));
							else if (x == 2)
								SetWindowText(topMidButton, _T("O"));
							else if (x == 3)
								SetWindowText(topRightButton, _T("O"));
							else if (x == 4)
								SetWindowText(midLeftButton, _T("O"));
							else if (x == 5)
								SetWindowText(centerButton, _T("O"));
							else if (x == 6)
								SetWindowText(midRightButton, _T("O"));
							else if (x == 7)
								SetWindowText(botLeftButton, _T("O"));
							else if (x == 8)
								SetWindowText(botMidButton, _T("O"));
							else if (x == 9)
								SetWindowText(botRightButton, _T("O"));
						}
						if (compWon())
						{
							ended = true;
							eh = "Wins:\t" + std::to_string(wins) + "\nLosses:\t" + std::to_string(losses) + "\nTies:\t" + std::to_string(ties);
							good = std::wstring(eh.begin(), eh.end());
							SetWindowText(title, good.c_str());
							victory = CreateWindow(TEXT("STATIC"), TEXT("You lost!"), WS_CHILD | WS_VISIBLE, 0, 0, 140, 20, hWnd, NULL, NULL, NULL);
							reset = CreateWindow(TEXT("BUTTON"), TEXT("Reset?"), WS_CHILD | WS_VISIBLE, 200 - 76 / 2, 0, 76, 30, hWnd, (HMENU)RESET_MENU, NULL, NULL);
						}
					}
					break;
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline bool playerWon()
{
	if (board[1] == 'X' && board[2] == 'X' && board[3] == 'X')
	{
		wins++;
		return true;
	}
	if (board[4] == 'X' && board[5] == 'X' && board[6] == 'X')
	{
		wins++;
		return true;
	}
	if (board[7] == 'X' && board[8] == 'X' && board[9] == 'X')
	{
		wins++;
		return true;
	}
	if (board[1] == 'X' && board[4] =='X' && board[7] == 'X')
	{
		wins++;
		return true;
	}
	if (board[2] == 'X' && board[5] == 'X' && board[8] == 'X')
	{
		wins++;
		return true;
	}
	if (board[3] == 'X' && board[6] == 'X' && board[9] == 'X')
	{
		wins++;
		return true;
	}
	if (board[1] == 'X' && board[5] == 'X' && board[9] == 'X')
	{
		wins++;
		return true;
	}
	if (board[3] == 'X' && board[5] == 'X' && board[7] == 'X')
	{
		wins++;
		return true;
	}
	return false;
}

inline bool compWon()
{
	if (board[1] == 'O' && board[2] == 'O' && board[3] == 'O')
	{
		losses++;
		return true;
	}
	if (board[4] == 'O' && board[5] == 'O' && board[6] == 'O')
	{
		losses++;
		return true;
	}
	if (board[7] == 'O' && board[8] == 'O' && board[9] == 'O')
	{
		losses++;
		return true;
	}
	if (board[1] == 'O' && board[4] == 'O' && board[7] == 'O')
	{
		losses++;
		return true;
	}
	if (board[2] == 'O' && board[5] == 'O' && board[8] == 'O')
	{
		losses++;
		return true;
	}
	if (board[3] == 'O' && board[6] == 'O' && board[9] == 'O')
	{
		losses++;
		return true;
	}
	if (board[1] == 'O' && board[5] == 'O' && board[9] == 'O')
	{
		losses++;
		return true;
	}
	if (board[3] == 'O' && board[5] == 'O' && board[7] == 'O')
	{
		losses++;
		return true;
	}
	return false;
}

int playerNearCenterWin()
{
	if (board[1] == 'X' && board[9] == ' ' && board[5] == 'X')
		return 9;
	if (board[9] == 'X' && board[1] == ' ' && board[5] == 'X')
		return 1;
	if (board[8] == 'X' && board[2] == ' ' && board[5] == 'X')
		return 2;
	if (board[7] == 'X' && board[3] == ' ' && board[5] == 'X')
		return 3;
	if (board[6] == 'X' && board[4] == ' ' && board[5] == 'X')
		return 4;
	if (board[5] == ' ')
	{
		if ((board[4] == 'X' && board[6] == 'X') || (board[2] == 'X' && board[8] == 'X') || (board[1] == 'X' && board[9] == 'X') || (board[3] == 'X' && board[7] == 'X'))
			return 5;
	}
	if (board[4] == 'X' && board[6] == ' ' && board[5] == 'X')
		return 6;
	if (board[2] == 'X' && board[8] == ' ' && board[5] == 'X')
		return 8;
	if (board[3] == 'X' && board[7] == ' ' && board[5] == 'X')
		return 7;
	if (board[1] == 'X' && board[7] == 'X' && board[4] == ' ')
		return 4;
	if (board[8] == ' ' && board[7] == 'X' && board[9] == 'X')
		return 8;
	if (board[9] == 'X' && board[3] == 'X' && board[6] == ' ')
		return 6;
	if (board[3] == 'X' && board[1] == 'X' && board[2] == ' ')
		return 2;
	if ((board[1] == ' ' && board[2] == 'X' && board[3] == 'X') || (board[1] == ' ' && board[4] == 'X' && board[7] == 'X'))
		return 1;
	if ((board[7] == ' ' && board[1] == 'X' && board[4] == 'X') || (board[7] == ' ' && board[8] == 'X' && board[9] == 'X'))
		return 7;
	if ((board[9] == ' ' && board[7] == 'X' && board[8] == 'X') || (board[9] == ' ' && board[3] == 'X' && board[6] == 'X'))
		return 9;
	if ((board[3] == ' ' && board[6] == 'X' && board[9] == 'X') || (board[3] == ' ' && board[1] == 'X' && board[2] == 'X'))
		return 3;
	return 0;
}

int compNearWin()
{
	if (board[1] == 'O' && board[9] == ' ' && board[5] == 'O')
		return 9;
	if (board[9] == 'O' && board[1] == ' ' && board[5] == 'O')
		return 1;
	if (board[8] == 'O' && board[2] == ' ' && board[5] == 'O')
		return 2;
	if (board[7] == 'O' && board[3] == ' ' && board[5] == 'O')
		return 3;
	if (board[6] == 'O' && board[4] == ' ' && board[5] == 'O')
		return 4;
	if (board[5] == ' ')
	{
		if ((board[4] == 'O' && board[6] == 'O') || (board[2] == 'O' && board[8] == 'O') || (board[1] == 'O' && board[9] == 'O') || (board[3] == 'O' && board[7] == 'O'))
			return 5;
	}
	if (board[4] == 'O' && board[6] == ' ' && board[5] == 'O')
		return 6;
	if (board[2] == 'O' && board[8] == ' ' && board[5] == 'O')
		return 8;
	if (board[3] == 'O' && board[7] == ' ' && board[5] == 'O')
		return 7;
	if (board[1] == 'O' && board[7] == 'O' && board[4] == ' ')
		return 4;
	if (board[8] == ' ' && board[7] == 'O' && board[9] == 'O')
		return 8;
	if (board[9] == 'O' && board[3] == 'O' && board[6] == ' ')
		return 6;
	if (board[3] == 'O' && board[1] == 'O' && board[2] == ' ')
		return 2;
	if ((board[1] == ' ' && board[2] == 'O' && board[3] == 'O') || (board[1] == ' ' && board[4] == 'O' && board[7] == 'O'))
		return 1;
	if ((board[7] == ' ' && board[1] == 'O' && board[4] == 'O') || (board[7] == ' ' && board[8] == 'O' && board[9] == 'O'))
		return 7;
	if ((board[9] == ' ' && board[7] == 'O' && board[8] == 'O') || (board[9] == ' ' && board[3] == 'O' && board[6] == 'O'))
		return 9;
	if ((board[3] == ' ' && board[6] == 'O' && board[9] == 'O') || (board[3] == ' ' && board[1] == 'O' && board[2] == 'O'))
		return 3;
	return 0;
}

bool catGame()
{
	for (int i = 1; i < 10; i++)
	{
		if (board[i] == ' ')
			return false;
	}
	ties++;
	return true;
}
