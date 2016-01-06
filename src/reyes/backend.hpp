#pragma once

// <options> ---------------------------------------------------------------------------
#define USE_CONSOLE_BACKEND
#define USE_NVTX_BACKEND
//#define USE_ANTTWBAR_BACKEND
//#define ANIMATE_BACKEND
// </options> --------------------------------------------------------------------------

// <user functions> --------------------------------------------------------------------
#define INPUT_f_mouseMove(fn)   (__mouseMoveF=fn)
#define INPUT_f_mouseDown(fn)   (__mouseDownF=fn)
#define INPUT_f_mouseUp(fn)     (__mouseUpF=fn)
#define INPUT_f_mouseDouble(fn) (__mouseDoubleF=fn)
#define INPUT_f_keyDown(fn)     (__keyDownF=fn)
#define INPUT_f_keyUp(fn)       (__keyUpF=fn)
#define INPUT_f_resize(fn)      (__resizeF=fn)

#ifdef ANIMATE_BACKEND
void InitApp();
void RenderApp();
void CleanupApp();
#else
void mainApp();
#endif
// </user functions> -------------------------------------------------------------------

// commonly used keys
#define INPUT_KEY_ESC 0x1B
#define INPUT_KEY_BACK 0x08
#define INPUT_KEY_TAB 0x09
#define INPUT_KEY_ENTER 0x0D
#define INPUT_KEY_SPACE 0x20
#define INPUT_KEY_PAUSE 0x13
#define INPUT_KEY_CAPS 0x14
#define INPUT_KEY_INSERT 0x2D
#define INPUT_KEY_DELETE 0x2E
// system keys
#define INPUT_KEY_SHIFT 0x10
#define INPUT_KEY_CTRL 0x11
#define INPUT_KEY_ALT 0x12
#define INPUT_KEY_LSHIFT 0xA0
#define INPUT_KEY_RSHIFT 0xA1
#define INPUT_KEY_LCTRL 0xA2
#define INPUT_KEY_RCTRL 0xA3
#define INPUT_KEY_LALT 0xA4
#define INPUT_KEY_RALT 0xA5
// arrow keys
#define INPUT_KEY_LEFT 0x25
#define INPUT_KEY_UP 0x26
#define INPUT_KEY_RIGHT 0x27
#define INPUT_KEY_DOWN 0x28
// number keys
#define INPUT_KEY_0 0x30
#define INPUT_KEY_1 0x31
#define INPUT_KEY_2 0x32
#define INPUT_KEY_3 0x33
#define INPUT_KEY_4 0x34
#define INPUT_KEY_5 0x35
#define INPUT_KEY_6 0x36
#define INPUT_KEY_7 0x37
#define INPUT_KEY_8 0x38
#define INPUT_KEY_9 0x39
// letter keys
#define INPUT_KEY_A 0x41
#define INPUT_KEY_B 0x42
#define INPUT_KEY_C 0x43
#define INPUT_KEY_D 0x44
#define INPUT_KEY_E 0x45
#define INPUT_KEY_F 0x46
#define INPUT_KEY_G 0x47
#define INPUT_KEY_H 0x48
#define INPUT_KEY_I 0x49
#define INPUT_KEY_J 0x4A
#define INPUT_KEY_K 0x4B
#define INPUT_KEY_L 0x4C
#define INPUT_KEY_M 0x4D
#define INPUT_KEY_N 0x4E
#define INPUT_KEY_O 0x4F
#define INPUT_KEY_P 0x50
#define INPUT_KEY_Q 0x51
#define INPUT_KEY_R 0x52
#define INPUT_KEY_S 0x53
#define INPUT_KEY_T 0x54
#define INPUT_KEY_U 0x55
#define INPUT_KEY_V 0x56
#define INPUT_KEY_W 0x57
#define INPUT_KEY_X 0x58
#define INPUT_KEY_Y 0x59
#define INPUT_KEY_Z 0x5A
// function keys
#define INPUT_KEY_F1 0x70
#define INPUT_KEY_F2 0x71
#define INPUT_KEY_F3 0x72
#define INPUT_KEY_F4 0x73
#define INPUT_KEY_F5 0x74
#define INPUT_KEY_F6 0x75
#define INPUT_KEY_F7 0x76
#define INPUT_KEY_F8 0x77
#define INPUT_KEY_F9 0x78
#define INPUT_KEY_F10 0x79
#define INPUT_KEY_F11 0x7A
#define INPUT_KEY_F12 0x7B
// mouse buttons
#define INPUT_MOUSE_LEFT 0x0001
#define INPUT_MOUSE_MIDDLE 0x0010
#define INPUT_MOUSE_RIGHT 0x0002
// input functions
extern void(*__mouseMoveF)(int, int, unsigned int, unsigned int);
extern void(*__mouseDownF)(int);
extern void(*__mouseUpF)(int);
extern void(*__mouseDoubleF)(int);
extern void(*__keyDownF)(unsigned int);
extern void(*__keyUpF)(unsigned int);
extern void(*__resizeF)(unsigned int, unsigned int);

// loading graphics backend libraries
#include <Windows.h>
#include <stdio.h>
#include "thirdparty/glew.h"

// loading perf tracking (nv tools extension)
#ifdef USE_NVTX_BACKEND
#   include "thirdparty/nvToolsExt.h"
#   define perfMarkerStart(_text) nvtxRangePushA((_text))
#   define perfMarkerEnd(_nil) nvtxRangePop()
#   define perfMarkerStartEx(_text,_col) \
{                                                       \
    nvtxEventAttributes_t initAttrib = { 0 };           \
    initAttrib.version = NVTX_VERSION;                  \
    initAttrib.size = NVTX_EVENT_ATTRIB_STRUCT_SIZE;    \
    initAttrib.color = (_col);                          \
    initAttrib.colorType = NVTX_COLOR_ARGB;             \
    initAttrib.message.ascii = _text;                   \
    initAttrib.messageType = NVTX_MESSAGE_TYPE_ASCII;   \
    nvtxRangePushEx(&initAttrib);                       \
}
#else
#   define perfMarkerStart(_text) 
#   define perfMarkerEnd(_nil) 
#   define perfMarkerStartEx(_text,_col) 
#endif
struct PerfMarker_t
{
    PerfMarker_t(const char* name, unsigned int col=0)
    {
        if (0 == col)   perfMarkerStart(name);
        else            perfMarkerStartEx(name, col);
    }
    ~PerfMarker_t() { perfMarkerEnd(); }
};
#define NVTX_B_COMBINE1(X,Y) X##Y
#define NVTX_B_COMBINE(X,Y) NVTX_B_COMBINE1(X,Y)
#define PerfMarker(markerName,col) PerfMarker_t NVTX_B_COMBINE(marker,__LINE__) (markerName, col)


// loading antTweakBar libraries
#ifdef USE_ANTTWBAR_BACKEND
#   include "thirdparty/AntTweakBar.h"
#endif

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = nullptr;
HWND                                g_hWnd  = nullptr;

void(*__mouseMoveF)(int, int, unsigned int, unsigned int)   = NULL;
void(*__mouseDownF)(int)                                    = NULL;
void(*__mouseUpF)(int)                                      = NULL;
void(*__mouseDoubleF)(int)                                  = NULL;
void(*__keyDownF)(unsigned int)                             = NULL;
void(*__keyUpF)(unsigned int)                               = NULL;
void(*__resizeF)(unsigned int, unsigned int)                = NULL;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitBackend(HWND hWnd);
void CleanupWindow();
void CleanupBackend(HWND hWnd);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SwapBuffersBackend();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_CONSOLE_BACKEND
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    // init
    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

    if (FAILED(InitBackend(g_hWnd)))
    {
        CleanupBackend(g_hWnd);
        return 0;
    }

    ShowWindow(g_hWnd, SW_SHOW);
    SetForegroundWindow(g_hWnd);
    SetFocus(g_hWnd);

    nvtxNameOsThreadA(GetCurrentThreadId(), "MAIN_THREAD");

#ifdef USE_ANTTWBAR_BACKEND
    TwInit(TW_OPENGL, NULL);
#endif

#ifdef ANIMATE_BACKEND
    InitApp();
#else
    mainApp();
#endif

    // main loop
    static bool quit = false;
    MSG msg = { 0 };
    while (!quit)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
        {
            quit = true;
        }
#ifdef ANIMATE_BACKEND
        else
        {
            RenderApp();
#ifdef USE_ANTTWBAR_BACKEND
            TwDraw();
#endif
            SwapBuffersBackend();
        }
#endif
    }

    // cleanup
#ifdef ANIMATE_BACKEND
    CleanupApp();
#endif
#ifdef USE_ANTTWBAR_BACKEND
    TwTerminate();
#endif
    CleanupBackend(g_hWnd);
    CleanupWindow();

    return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"FrameWinClass";
    wcex.hIconSm = wcex.hIcon;
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 480, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(L"FrameWinClass", L"Frame",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
        nullptr);

    if (!g_hWnd)
        return E_FAIL;

    //ShowWindow(g_hWnd, nCmdShow);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release resources, destroy window and unregister class
//--------------------------------------------------------------------------------------
void CleanupWindow()
{
    if (g_hWnd && !DestroyWindow(g_hWnd))
    {
        MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hWnd = NULL;
    }

    if (!UnregisterClass(L"FrameWinClass", g_hInst))
    {
        MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hInst = NULL;
    }
}

// --------------------------------------------------------------------------------------
// Called every time the application receives a message
//---------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

#ifdef USE_ANTTWBAR_BACKEND
    TwEventWin(hWnd, message, wParam, lParam);
#endif

    switch (message)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (__keyDownF)
            (__keyDownF)(wParam);
        return 0;
    case WM_KEYUP:
        if (__keyUpF)
            (*__keyUpF)(wParam);
        return 0;
    case WM_MOUSEMOVE:
    {
        if (__mouseMoveF)
            (*__mouseMoveF)
            (
            wParam,
            ((wParam == INPUT_MOUSE_LEFT || wParam == INPUT_MOUSE_MIDDLE || wParam == INPUT_MOUSE_RIGHT) ? 1 : 0),
            LOWORD(lParam),
            HIWORD(lParam)
            );
        return 0;
    }
    case WM_LBUTTONDOWN:
        if (__mouseDownF)
            (*__mouseDownF)(INPUT_MOUSE_LEFT);
        return 0;
    case WM_MBUTTONDOWN:
        if (__mouseDownF)
            (*__mouseDownF)(INPUT_MOUSE_MIDDLE);
        return 0;
    case WM_RBUTTONDOWN:
        if (__mouseDownF)
            (*__mouseDownF)(INPUT_MOUSE_RIGHT);
        return 0;
    case WM_LBUTTONUP:
        if (__mouseUpF)
            (*__mouseUpF)(INPUT_MOUSE_LEFT);
        return 0;
    case WM_MBUTTONUP:
        if (__mouseUpF)
            (*__mouseUpF)(INPUT_MOUSE_MIDDLE);
        return 0;
    case WM_RBUTTONUP:
        if (__mouseUpF)
            (*__mouseUpF)(INPUT_MOUSE_RIGHT);
        return 0;
    case WM_LBUTTONDBLCLK:
        if (__mouseDoubleF)
            (*__mouseDoubleF)(INPUT_MOUSE_LEFT);
        return 0;
    case WM_MBUTTONDBLCLK:
        if (__mouseDoubleF)
            (*__mouseDoubleF)(INPUT_MOUSE_MIDDLE);
        return 0;
    case WM_RBUTTONDBLCLK:
        if (__mouseDoubleF)
            (*__mouseDoubleF)(INPUT_MOUSE_RIGHT);
        return 0;
    case WM_SIZE:
        if(__resizeF)
            (*__resizeF)(LOWORD(lParam), HIWORD(lParam));
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

HGLRC g_hRC = nullptr;
HDC g_hDC   = nullptr;

//--------------------------------------------------------------------------------------
// Initialize drawing API
//--------------------------------------------------------------------------------------
HRESULT InitBackend(HWND hWnd)
{
    if (!(g_hDC = GetDC(hWnd)))
    {
        printf("Error: Device fail.\n");
        return E_FAIL;
    }

    int format;
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
        1, // Version Number
        PFD_DRAW_TO_WINDOW | // Format Must Support Window
        PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
        PFD_DOUBLEBUFFER, // Must Support Double Buffering
        PFD_TYPE_RGBA, // Request An RGBA Format
        32, // Select Our Color Depth
        0, 0, 0, 0, 0, 0, // Color Bits Ignored
        0, // No Alpha Buffer
        0, // Shift Bit Ignored
        0, // No Accumulation Buffer
        0, 0, 0, 0, // Accumulation Bits Ignored
        16, // 16Bit Z-Buffer (Depth Buffer)
        0, // No Stencil Buffer
        0, // No Auxiliary Buffer
        PFD_MAIN_PLANE, // Main Drawing Layer
        0, // Reserved
        0, 0, 0
    };

    if (!(format = ChoosePixelFormat(g_hDC, &pfd)))
    {
        printf("Error: PixelFormat fail.\n");
        return E_FAIL;
    }
    if (!SetPixelFormat(g_hDC, format, &pfd))
    {
        printf("Error: PixelFormat not set.\n");
        return E_FAIL;
    }
    if (!(g_hRC = wglCreateContext(g_hDC)))
    {
        printf("Error: Context not creted.\n");
        return E_FAIL;
    }
    if (!wglMakeCurrent(g_hDC, g_hRC))
    {
        printf("Error: Context not set.\n");
        return E_FAIL;
    }

    // initialize glew
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        printf("Error: GLEW fail.\n");
        return E_FAIL;
    }

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Release drawing API resources
//--------------------------------------------------------------------------------------
void CleanupBackend(HWND hWnd)
{
    if (g_hRC)
    {
        if (!wglMakeCurrent(NULL, NULL))
            MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        if (!wglDeleteContext(g_hRC))
            MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hRC = NULL;
    }

    if (g_hDC && !ReleaseDC(hWnd, g_hDC))
    {
        MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        g_hDC = NULL;
    }
}

//--------------------------------------------------------------------------------------
// Swap buffers and display
//--------------------------------------------------------------------------------------
void SwapBuffersBackend()
{
    SwapBuffers(g_hDC);
}