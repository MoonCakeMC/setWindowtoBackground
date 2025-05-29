#include <windows.h>
#include <stdio.h>
#include <conio.h>
// #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int state=0;
boolean hiden = FALSE;
MSG msg = { 0 };
HWND now_background=NULL;
LONG now_background_long;
HWND hWorkerW = NULL;
HWND hProgman;
RECT rctA;

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowExW(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) {
		HWND hWorkerw = FindWindowExW(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, state);
		
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK MinimizeVisibleWindowsProc(HWND hwnd, LPARAM lParam)  
{  
     DWORD dwStyle = GetWindowLong(hwnd, GWL_EXSTYLE);  
    if ((dwStyle & WS_EX_TOOLWINDOW) == 0 && IsWindowVisible(hwnd)) { // 判断窗口是否为非ToolWindow且可见的窗口  
        ShowWindow(hwnd, SW_MINIMIZE); // 如果是非ToolWindow且可见的窗口，则最小化它  
	}
    return TRUE; // 继续枚举窗口  
}  


void ShowBackground() {
	
    SetParent(now_background, NULL);
	SetWindowLong(now_background, GWL_STYLE, now_background_long);
	int width = rctA.right - rctA.left;
	int height = rctA.bottom - rctA.top;
	MoveWindow(now_background,rctA.left,rctA.top,width,height,FALSE);
    now_background = NULL;
}

void send_to_PM() {
	hWorkerW = FindWindowW(L"WorkerW",0);
	if (hProgman == NULL){
		hProgman = FindWindowW(L"Progman", 0);				// 找到PM窗口
		SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	// 给它发特殊消息
	}
}

void win_d(){
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_LWIN,0,0,0);
	keybd_event(77,0,0,0);
	keybd_event(77,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_LWIN,0,KEYEVENTF_KEYUP,0);
}

void set_to_bg() {
	send_to_PM();
	POINT p;
	GetCursorPos(&p);
	HWND hFfplay = WindowFromPoint(p);
	if (GetParent(hFfplay) != NULL){
		hFfplay = GetParent(hFfplay);
	}
    if (now_background != NULL){
        ShowBackground();
	}
	now_background = hFfplay;
	now_background_long = GetWindowLong(hFfplay, GWL_STYLE);
	GetWindowRect(hFfplay,&rctA);
	HWND hProgman = FindWindowW(L"Progman", 0);				// 找到PM窗口
	printf("%lld %lld\n", hFfplay, hProgman) ;
	SetParent(hFfplay, hProgman);							// 将视频窗口设置为PM的子窗口
	state = SW_HIDE;
	EnumWindows(EnumWindowsProc, 0);						// 找到第二个WorkerW窗口并隐藏它
	ShowWindow(hFfplay,SW_MAXIMIZE);
	// MoveWindow(hFfplay,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN), FALSE);
	SetWindowLong(hFfplay, GWL_STYLE,GetWindowLong(hFfplay, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));
	// EnumWindows(MinimizeVisibleWindowsProc, 0); //枚举所有顶级窗口，并将它们最小化
	win_d();
}

void show_bg() {
	if (now_background != NULL){
		ShowBackground();
		state = SW_SHOW;
		EnumWindows(EnumWindowsProc, 0);
	}
}

void py_exit(){exit(0);}

HWND x15;
void hide_win() {
	if (hiden) {
		if (x15) {
			ShowWindow(x15, SW_MAXIMIZE);
			hiden = FALSE;
		}
	}
	else {
		POINT p;
		GetCursorPos(&p);
		x15 = WindowFromPoint(p);
		if (GetParent(x15) != NULL){
			x15 = GetParent(x15);
		}
		if (x15) {
			ShowWindow(x15, SW_HIDE);
			hiden = TRUE;
		}
	}
}

int main()
{
    // HideWindow();

    // 注册两个热键 Ctrl+F1 , Ctrl+F2
    if (0 == RegisterHotKey(NULL, 1, MOD_CONTROL, VK_F1)) {
        // printf("%s\n",GetLastError());
    }
    if (0 == RegisterHotKey(NULL, 2, MOD_CONTROL, VK_F2)) {
        // printf("%s\n",GetLastError());
    }
    if (0 == RegisterHotKey(NULL, 3, MOD_CONTROL, VK_F3)) {
        // printf("%s\n",GetLastError());
    }
    if (0 == RegisterHotKey(NULL, 4, MOD_CONTROL, VK_F4)) {
        // printf("%s\n",GetLastError());
    }

    // 消息循环
	
    while (GetMessage(&msg, NULL, 0, 0)){
        switch (msg.message) {
        case WM_HOTKEY:
        {
            if (1 == msg.wParam) {
				set_to_bg();
			}
            if (2 == msg.wParam) {
            	show_bg();
			}
			if (3 == msg.wParam) {
				show_bg();
				exit(0);
			}
			if (4 == msg.wParam) {
				hide_win();
			}
            break;
        }
        default:
            break;
        }
    }
    return 0;
}
