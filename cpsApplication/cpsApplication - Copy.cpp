// cpsApplication.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "cpsApplication.h"

std::wstring windowName;

//declare window precedure
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

//Make the class name to a global variable
char szClassName[] = "cpsApplication";

HWND clicksField;
HWND timeField;

//counter for all the clicks
int clicks;

//string for the text labels
std::string clickLabelText;
std::string timeLabelText;

clock_t timer;

std::wstring s2ws(const std::string& s)
{
	int len;
	int slenght = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slenght, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slenght, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR IpszArgument,
	int nCmdShow)
{
	HWND hwnd;			//This is the handle for the window
	MSG messages;		//Here messages to the application are saved, these are a sort of window events
	WNDCLASSEX wincl;	//Data structure for the windowclass

	windowName = s2ws("Elite Fun Window"); //Set the name of the window

	clicks = 0;

	//The window structure

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = (LPCWSTR) szClassName;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);

	//Use default icon and mouse pointer
	wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_CPSAPPLICATION));
	wincl.hIconSm = LoadIcon(wincl.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wincl.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wincl.lpszMenuName = nullptr;		//No menu
	wincl.cbClsExtra = 0;				//No extra bytes after the window class
	wincl.cbWndExtra = 0;				//Strucuture of the window instance

	//Use window's default color as the background of the window
	wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

	//Register the window class, and if it fails, quit the program
	if (!RegisterClassEx(&wincl))
		return 0;

	//The class is registered, let's create the program
	hwnd = CreateWindowEx(
		0,								//Extended possibilites for variation
		(LPCWSTR) szClassName,			//Classname
		windowName.c_str(),				//Title text
		WS_MINIMIZEBOX | WS_SYSMENU,	/*Default window, - maximize and resize
										*to have it compeletly default: WS_OVERLAPPEDWINDOW
										*for multitple options, use pipe symbol*/
		CW_USEDEFAULT,					//Windows decides the position... --> x or y
		CW_USEDEFAULT,					//...where the window ends up on the screen --> x or y
		WIDTH,							//The program width...
		HEIGHT,							//and height in pixels
		HWND_DESKTOP,					//The window is child-window to desktop
		nullptr,						//No menu
		hThisInstance,					//Program Instance handler
		nullptr							//No window creation data
	);

	//makethe window visible on the screen
	ShowWindow(hwnd, nCmdShow);

	//Run the message loop. It will run until GetMessage() returns 0
	while (GetMessage(&messages, nullptr, 0, 0))
	{
		//Translate virutal-key messages into character messages
		TranslateMessage(&messages);
		//Send message to WindowProcedure
		DispatchMessage(&messages);

		//check if any mouse button was pressed
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
		{
			clicks++;
		}

		clickLabelText = "Clicks: ";
		clickLabelText += std::to_string(clicks);
		SetWindowText(clicksField, s2ws(clickLabelText).c_str());

		timer = clock() / 1000;

		timeLabelText = "Time (seconds): ";
		timeLabelText += std::to_string(timer);
		SetWindowText(timeField, s2ws(timeLabelText).c_str());
	}

	//The program return-value is 0 - The value that PostQuitMessage() gave
	return messages.wParam;
}

//This function si called by the Windows function DispatchMessage();
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{//Message handler
	switch (message)			//handle the messages
	{
	case WM_CREATE:
		clicksField = CreateWindow(
			s2ws("STATIC").c_str(),
			s2ws("Left mouse button not yet clicked").c_str(),
			WS_VISIBLE | WS_CHILD,
			0, 0, 300, 20,
			hwnd,
			nullptr, nullptr, nullptr);

		timeField = CreateWindow(
			s2ws("STATIC").c_str(),
			s2ws("Time not yet updated").c_str(),
			WS_VISIBLE | WS_CHILD,
			0, 20, 300, 20,
			hwnd,
			nullptr, nullptr, nullptr);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);		//send a WM_QUIT to the message queue
		break;
	//case WM_MBUTTONCLick: 
		//std::cout << "Mouse button clicked!"; //break;
		//return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}