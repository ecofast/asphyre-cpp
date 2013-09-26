#include "stdafx.h"
#include <string>
using std::wstring;
#include "../../../Source/ASCEvents.h"
#include "../../../Source/ASCDevice.h"
#include "../../../Source/DX9/ASCDX9Provider.h"
#include "../../../Source/ASCUtils.h"
#include "../../../Source/ASCIntVector2D.h"
#include "../../../Source/ASCWindowsConnector.h"
#include "../../../Source/ASCWindowsTimer.h"
#include "../../../Source/ASCTextures.h"
#include "../../../Source/ASCImages.h"
#include "../../../Source/ASCBitmaps.h"
#include "../../../Source/ASCBitmapsBMP.h"

const wchar_t* WINDOW_CLASS = L"Asphyre_Sphinx_for_C++_Basic";
const wchar_t* WINDOW_TITLE = L"ASC(Asphyre Sphinx for C++) Basic Example";

const int CLIENT_WIDTH  = 1280;
const int CLIENT_HEIGHT = 720;

HINSTANCE	G_hInstance		= NULL;
HWND		G_hWnd			= NULL;
int			G_nWndWidth, G_nWndHeight;
int			G_nLeft, G_nTop;

CASCDevice*		G_pASCDevice = 0;
CASCCanvas*		G_pASCCanvas = 0;
CASCIntVector2D	G_DisplaySize;
CASCImage*		G_pASCImage = 0;
int				G_nGameTicks = 0;

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// redirect stderr and stdout output
	// freopen("stderr.txt", "w", stderr);
	// freopen("stdout.txt", "w", stdout);

	G_hInstance = hInstance;

	WNDCLASSEX wc;
	wc.cbClsExtra = 0;                                           
	wc.cbSize = sizeof(WNDCLASSEX);                              
	wc.cbWndExtra = 0;                                           
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);      
	wc.hCursor = LoadCursor(G_hInstance, MAKEINTRESOURCE(IDC_ARROW));                                           
	wc.hIcon = NULL;                                             
	wc.hIconSm = NULL;                                           
	wc.hInstance = G_hInstance;                                  
	wc.lpfnWndProc = TheWindowProc;                              
	wc.lpszClassName = WINDOW_CLASS;                             
	wc.lpszMenuName = NULL;                                      
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;  

	RegisterClassEx(&wc);

	G_nWndWidth = CLIENT_WIDTH + GetSystemMetrics(SM_CXSIZEFRAME) * 2;
	G_nWndHeight = CLIENT_HEIGHT + GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	G_nLeft = (GetSystemMetrics(SM_CXSCREEN) - G_nWndWidth) / 2;
	G_nTop = (GetSystemMetrics(SM_CYSCREEN) - G_nWndHeight) / 2;
	// G_hWnd = CreateWindowEx(0, WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, g_hInstance, NULL);
	G_hWnd = CreateWindowEx(0, wsClassName.c_str(), wsWndCaption.c_str(), WS_OVERLAPPEDWINDOW, 
		G_nLeft, G_nTop, G_nWndWidth, G_nWndHeight, NULL, NULL, G_hInstance, NULL);
	
	ShowWindow(G_hWnd, nShowCmd);

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
			ASCWindowsTimer()->AppIdle();
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
	ASCWindowsTimer()->SetEnabled(false);

	delete G_pASCImage;
	G_pASCImage = 0;

	delete G_pASCCanvas;
	G_pASCCanvas = 0;

	delete G_pASCDevice;
	G_pASCDevice = 0;
}

void OnASCDeviceInit(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	G_DisplaySize = CASCIntVector2D(CLIENT_WIDTH, CLIENT_HEIGHT);
	G_pASCDevice->GetSwapChains()->Add((ASCUInt)G_hWnd, G_DisplaySize);
}

void OnASCDeviceCreate(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{

}

void OnASCTimerReset(const void* pSender, const ASCPointer pParam, ASCBoolean* bHandled)
{
	ASCWindowsTimer()->Reset();
}

RECT G_Rc = {500, 400, 700, 550};
CASCColor4 G_Color4 = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF};

void RenderEvent()
{
	G_pASCCanvas->RenderPixel(10, 10, 0xFFFF0000);
	G_pASCCanvas->RenderLine(100, 100, 200, 200, 0xFFFFFFFF);
	G_pASCCanvas->FillRect(500, 100, 200, 200, 0xFF00FF00);
	G_pASCCanvas->FrameRect(G_Rc, G_Color4);
	G_pASCCanvas->FrameRect(200, 250, 100, 100, 0xFFFFFFFF);
	G_pASCCanvas->RenderHorizLine(50, 50, 100, 0xFFFF0000, 0xFFFF0000);
	G_pASCCanvas->RenderVertLine(400, 100, 200, 0xFFFF0000, 0xFF00FF00);

	// Draw an animated hole
	G_pASCCanvas->RenderQuadHole(CASCFloatVector2D(0.0, 0.0), 
		CASCFloatVector2D((ASCSingle)G_DisplaySize.X, (ASCSingle)G_DisplaySize.Y), 
		CASCFloatVector2D((ASCSingle)(G_DisplaySize.X * 0.5 + cos(G_nGameTicks * 0.0073) * G_DisplaySize.X * 0.25), 
		(ASCSingle)(G_DisplaySize.Y * 0.5 + sin(G_nGameTicks * 0.00312) * G_DisplaySize.Y * 0.25)),
		CASCFloatVector2D(80.0f, 100.0f), 0x20FFFFFF, 0x80955BFF, 16);

	// Draw an image
	if (G_pASCImage)
	{
		G_pASCCanvas->RenderTexture(750, 100, G_pASCImage->GetTexture(0));
		// G_pASCCanvas->RenderScaledTexture(750, 100, G_pASCImage->GetTexture(0), 0.6f);
		// G_pASCCanvas->RenderPartTexture(750, 100, G_pASCImage->GetTexture(0), 0, 0, 256, 256, 1, 1, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		// G_pASCCanvas->RenderRotateTexture(950, 400, G_pASCImage->GetTexture(0), 0.2f, 1, 1, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	}
}

void HandleConnectFailure()
{
	ASCWindowsTimer()->SetEnabled(false);
	MessageBox(G_hWnd, L"Failed initializing ASC device.", L"Error", MB_OK);
	PostQuitMessage(0);
}

void ProcessEvent()
{
	G_nGameTicks++;
}

void TimerEvent()
{
	// Try to connect ASC to the application
	if (!ASCWindowsConnector()->Init())
	{
		return;
	}

	/*
	 * In case the device could not be initialized properly(in the frame before
	 * this one), show error message and close the form
	*/
	if (G_pASCDevice && (G_pASCDevice->IsAtFault()))
	{
		static ASCBoolean bFailureHandled = false;
		if (!bFailureHandled)
		{
			HandleConnectFailure();
		}
		bFailureHandled = true;
		return;
	}

	/*
	 * Initialize ASC device, if needed. If this initialization fails, the
	 * failure will be handled in the next OnTimer event
	*/
	if ((!G_pASCDevice) || (!(G_pASCDevice->Initialize())))
	{
		return;
	}

	if (!G_pASCImage)
	{
		G_pASCImage = new CASCImage();
		G_pASCImage->LoadFromFile(ExtractCurrModulePath() + L"\\1.bmp");
	}

	// Render the scene
	G_pASCDevice->Render(RenderEvent, 0x000050);

	// Execute constant time processing
	ASCWindowsTimer()->Process();
}

void Setup()
{
	/*
	fprintf(stdout, "Setup Done.\n");
	fprintf(stderr, "Error.\n");
	*/

	// Create DX9 provider explicit
	ASCDX9Provider();

	// Specify that DirectX 9 provider is to be used
	ASCFactory()->UseProvider(ASC_PROVIDERID_DX9);

	// This event is called when ASC components should be created
	ASCCreateEvent()->Subscribe(wsClassName.c_str(), OnASCCreate);

	// This event is called when ASC components are to be freed
	ASCDestroyEvent()->Subscribe(wsClassName.c_str(), OnASCDestroy);

	// This event is callled before creating ASC device to initialize its parameters
	ASCDeviceInitEvent()->Subscribe(wsClassName.c_str(), OnASCDeviceInit);

	// This event is callled upon Asphyre device creation
	ASCDeviceCreateEvent()->Subscribe(wsClassName.c_str(), OnASCDeviceCreate);

	// This event is called when creating device and loading data to let the application reset the timer so it does not stall
	ASCTimerResetEvent()->Subscribe(wsClassName.c_str(), OnASCTimerReset);

	// Initialize and prepare the timer
	ASCWindowsTimer()->SetOnTimer(&TimerEvent);
	ASCWindowsTimer()->SetOnProcess(&ProcessEvent);
	ASCWindowsTimer()->SetEnabled(true);

	// Register BMP handler
	ASCBitmapManager()->RegisterExt(L".bmp", ASCBitmapBMP());
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

	// UnRegister BMP handler
	ASCBitmapManager()->UnregisterExt(L".bmp");
}