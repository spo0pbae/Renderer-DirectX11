// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <conio.h>

#include "renderer.h"
#include "dev_app.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Engine Development");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void create_console();
void destroy_console(int exit_code);
MSG begin_main_loop();

namespace
{
	HWND  main_hwnd = NULL;
	end::dev_app_t dev_app{};
}

int CALLBACK WinMain
(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	// Contains window class information. Used with RegisterClassEx and GetClassInfoEx funcs.
	// Contains cdSize (size of struct) and hIconSm (handle to small icon associated with window)
	// Whereas WNDCLASS does not.
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);						 // Size of struct					
	wcex.style			= CS_HREDRAW | CS_VREDRAW;					 // Class Style, can be any combination
	wcex.lpfnWndProc	= WndProc;									 // Pointer to window procedure. You must use CallWindowProc to call the winproc.
	wcex.cbClsExtra		= 0;										 // Number of extra bytes to allocate following the window-class struct. Initialized to 0.
	wcex.cbWndExtra		= 0;										 // Number of extra bytes to allocate following the window instance. Initialized to 0.
	wcex.hInstance		= hInstance;								 // A handle to the instance that contains the window proc for the class. 
	wcex.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);		 // Handle to class icon. If null, system provides a default.
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);				 // Handle to class cursor. must be a handle to a cursor resource. 
	wcex.hbrBackground	= NULL;										 // Handle to the class background brush. Can be a handle to the brush to be used for painging. // CHANGED FROM (HBRUSH)(COLOR_WINDOW + 1) TO REMOVE FLICKER
	wcex.lpszMenuName	= NULL;										 // Pointer to null terminated string that specifies the resource name of the class menu, as the name appears in the resource file.
	wcex.lpszClassName	= szWindowClass;							 // Pointer to null terminated string, or an ATOM.
	wcex.hIconSm		= LoadIcon(wcex.hInstance, IDI_APPLICATION); // Handle to a small icon that is associated with the window class

	// Registers a window class for subsequent use in calls to CreateWindow or CreateWindowEx
	//	IN:
	//		unnamedParam1: Pointer to WNDCLASSEX struct. You must fill the struct before passing it to this function.
	//	RETURNS:
	//		ATOM: this uniquely identifies the class being registered. 
	if (!RegisterClassEx(&wcex))
	{
		/* Displays modal dialog box that contains a system icon, set of buttons, a
			brief app-specific message, and returns an int that indicates which button was clicked
			Note: All parameters are optional except the last 
		*/
		// RETURNS:
		//		SUCCESS: Return code
		//		FAILURE: 0 (GetLastError)
		MessageBox(NULL,							// hWnd:	  Type: HWND 		handle to the owner window of the message box created. If NULL, the message box has no owner.
			_T("Call to RegisterClassEx failed!"),	// lpText:    Type: LPCTSTR		The message to be displayed
			_T("Engine Development"),				// lpCaption: Type: LPCTSTR		The dialog box title
			NULL);									// uType	  Type: UINT		Contents and behavior of dialog box. Can be a combination of flags.
		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	// RETURNS:
	//		SUCCESS: A new window on success
	//		FAILURE: NULL on failure (use GetLastError)
	main_hwnd = CreateWindow
	(
		szWindowClass,								// lpClassName		- Name of the application. Null terminated string or class atom created by a previous call to RegisterClass or RegisterClassEx.
		szTitle,									// lpWindowName		- window name
		WS_POPUPWINDOW | WS_CAPTION,				// dwStyle			- Style of window being created. (more in MSDN) // CHANGED FROM WS_OVERLAPPEDWINDOW TO DISABLE SIZING
		CW_USEDEFAULT, CW_USEDEFAULT,				// X,Y				- Initial position
		1280, 720,									// nWidth, nHeight	- Width/height of window (in device units)
		NULL,										// hWndParent		- Handle to parent or owner window of the window being created. To create a child or owned window, supply a valid window handle.
		NULL,										// hMenu			- Handle to a menu, or specifies a child-window identifier depending on the window style.
		hInstance,									// hInstance		- Handle to the instance of the module to be associated with the window
		NULL										// lpParam			- Pointer to a value to be passed to the window through the CREATESTRUCT struct, pointed to by the lParam param of WM_CREATE.
	);

	// If window failed to create
	if (!main_hwnd)
	{
		// RETURNS:
		//		SUCCESS: Return code
		//		FAILURE: 0 (GetLastError)
		MessageBox
		(
			NULL,								
			_T("Call to CreateWindow failed!"),	
			_T("Engine Development"),			
			NULL								
		);									

		return 1;
	}

	create_console();

	// IN:
	//		hWnd: the value returned from CreateWindow
	//		nCmdShow: the fourth parameter from WinMain
	ShowWindow(main_hwnd, nCmdShow);

	// IN:
	//		hWnd: handle to window to be updated
	// RETURNS:
	//		SUCCESS: non-zero value
	//		FAILURE: 0
	UpdateWindow(main_hwnd);

	// MSG - Struct that contains message information from a thread's message queue
	MSG msg = begin_main_loop();
	destroy_console((int)msg.wParam);

	return (int)msg.wParam;
}

//  PURPOSE:  Processes messages for the main window.
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			end::keys[(int)wParam] = true;
			break;
		}
		case WM_KEYUP:
		{
			end::keys[(int)wParam] = false;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			end::keys[(int)wParam] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			end::keys[(int)wParam] = false;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			end::keys[(int)wParam] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			end::keys[(int)wParam] = false;
			break;
		}
		case WM_MOUSEMOVE:
		{
			//std::cout << "mouse moved" << std::endl;
			end::currMouseX = GET_X_LPARAM(lParam);
			end::currMouseY = GET_Y_LPARAM(lParam);

			std::cout << "X: " << end::currMouseX << std::endl << "Y: " << end::currMouseY << std::endl;

			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
	return 0;
}

// Create debug console window
void create_console()
{
#ifndef NDEBUG	// if in debug mode

	// Allocates a new console for the calling process
	//	RETURNS:
	//		SUCCESS: Non-zero value
	//		FAILURE: 0 (GetLastError)
	AllocConsole();

	// File stream of some kind?
	FILE* new_std_in_out;

	// Closes file currently associated with _oldstream and reassigns _stream to specified file _filename
	//	IN:
	//		Stream:		Out prameter that will point to the reopened stream when the func returns
	//		FileName	Path of file to re-open
	//		Mode:		Mode for re-opened stream
	//		OldStream:	Stream to re-open. It is flushed and any files associated with it are closed.
	//	RETURNS:
	//		SUCCESS:	0
	//		FAILURE:	Error code. The original file is closed and NULL is written to stream unless stream is also null.
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$",  "r", stdin);

	std::cout << "Debug Console Opened.\n"; // Don’t forget to include <iostream>
#endif
}

void destroy_console(int exit_code)
{
#ifndef NDEBUG
	if (exit_code)
	{
		std::cout << "Exit Code: " << exit_code << "\n";
		std::cout << "Press any key to continue...";
		_getch();
	}

	// Detaches the callint process from its console
	// RETURNS:
	//		SUCCESS: non-zero value
	//		FAILURE: 0 (GetLastError)
	FreeConsole();
#endif
}

MSG begin_main_loop()
{
	MSG msg;
	end::renderer_t renderer(main_hwnd);

	// Main application loop:
	while (true)
	{
		// Process all messages, stop on WM_QUIT
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT does not need to be translated or dispatched
			if (msg.message == WM_QUIT)
				break;

			// Translates messages and sends them to WndProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			dev_app.cam = &renderer.default_view;
			dev_app.update();
			renderer.draw();
		}
	}
	return msg;
}