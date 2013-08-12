#include "stdafx.h"
#include <string>
using std::string;
using std::wstring;
#include "../../../Source/ASCEvents.h"
#include "../../../Source/ASCDevice.h"
#include "../../../Source/DX9/ASCDX9Provider.h"
#include "../../../Source/ASCIntVector2D.h"
#include "../../../Source/ASCWindowsConnector.h"

const wchar_t* WINDOW_CLASS = L"Asphyre_Sphinx_for_C++_Basic";
const wchar_t* WINDOW_TITLE = L"ASC(Asphyre Sphinx for C++) Basic Example";

HINSTANCE	g_hInstance	= NULL;
HWND		g_hWnd		= NULL;

CASCDevice*		G_pASCDevice = 0;
CASCCanvas*		G_pASCCanvas = 0;
CASCIntVector2D	G_DisplaySize;

int		WINAPI		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
LRESULT	CALLBACK	TheWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void				Setup();
void				Cleanup();

LRESULT CALLBACK TheWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
				DestroyWindow(hWnd);
			return 0;
		}
		break;
	default:
		return (DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
}

wstring wsClassName		= L"Asphyre_Sphinx_for_C++_Basic";
wstring wsWndCaption	= L"ASC(Asphyre Sphinx for C++) Basic Example";

void Render()
{
	G_pASCCanvas->RenderLine(100, 100, 200, 200, 0xFFFF0000);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// redirect stderr and stdout output
	freopen("stderr.txt", "w", stderr);
	freopen("stdout.txt", "w", stdout);

	g_hInstance = hInstance;

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;                                           
	wc.cbSize = sizeof(WNDCLASSEX);                              
	wc.cbWndExtra = 0;                                           
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);      
	wc.hCursor = NULL;                                           
	wc.hIcon = NULL;                                             
	wc.hIconSm = NULL;                                           
	wc.hInstance = g_hInstance;                                  
	wc.lpfnWndProc = TheWindowProc;                              
	wc.lpszClassName = WINDOW_CLASS;                             
	wc.lpszMenuName = NULL;                                      
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;  

	RegisterClassEx(&wc);

	// g_hWnd = CreateWindowEx(0, WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, g_hInstance, NULL);
	g_hWnd = CreateWindowEx(0, wsClassName.c_str(), wsWndCaption.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, g_hInstance, NULL);
	
	ShowWindow(g_hWnd, nShowCmd);

	Setup();

	MSG msg;
	for ( ; ; )
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!ASCWindowsConnector()->Init())
			{
				continue;
			}

			if ((!G_pASCDevice) || (!G_pASCDevice->Initialize()))
			{
				continue;
			}

			if (G_pASCDevice && G_pASCCanvas)
			{
				// Render();
				G_pASCDevice->Render(Render, 0x000050);
			}
		}
	}

	Cleanup();

	return (msg.wParam);
}

void OnASCCreate(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	// Create ASC components
	G_pASCDevice = ASCFactory()->CreateDevice();
	G_pASCCanvas = ASCFactory()->CreateCanvas();
}

void OnASCDestroy(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	delete G_pASCDevice;
	G_pASCDevice = 0;

	delete G_pASCCanvas;
	G_pASCCanvas = 0;
}

void OnASCDeviceInit(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	G_DisplaySize = CASCIntVector2D(800, 600);
	G_pASCDevice->GetSwapChains()->Add((ASCUInt)g_hWnd, G_DisplaySize);
}

void Setup()
{
	/*
	fprintf(stdout, "Setup Done.\n");
	fprintf(stderr, "Error.\n");
	*/
	
	ASCDX9Provider();

	// Specify that DirectX 9 provider is to be used
	ASCFactory()->UseProvider(ASC_PROVIDERID_DX9);

	// This event is called when ASC components should be created
	ASCCreateEvent()->Subscribe(wsClassName.c_str(), OnASCCreate);

	// This event is called when ASC components are to be freed
	ASCDestroyEvent()->Subscribe(wsClassName.c_str(), OnASCDestroy);

	// This event is callled before creating Asphyre device to initialize its parameters
	ASCDeviceInitEvent()->Subscribe(wsClassName.c_str(), OnASCDeviceInit);
}

void Cleanup()
{
	// fprintf(stdout, "Cleanup Done.\n");

	if (G_pASCDevice)
	{
		G_pASCDevice->Finalize();
	}

	ASCWindowsConnector()->Done();

	ASCEventProviders()->UnSubscribe(wsClassName.c_str());
}