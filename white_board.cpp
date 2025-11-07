// whiteboard_win32.cpp
// Minimal whiteboard using Win32 + GDI only (no external libraries).
// Build (MinGW): g++ whiteboard_win32.cpp -municode -lgdi32 -luser32 -o whiteboard.exe
// Build (MSVC): cl /EHsc whiteboard_win32.cpp user32.lib gdi32.lib

#include <windows.h>
#include <vector>

// Global handles for simplicity
HBITMAP g_hBitmap = NULL;   // offscreen bitmap that stores the drawing
HDC     g_hMemDC  = NULL;   // memory DC for the bitmap
int     g_clientW = 800, g_clientH = 600;
bool    g_drawing = false;
POINT   g_lastPt  = {0,0};

// Create an offscreen bitmap sized to the client area and initialize to white
void CreateBackBuffer(HWND hwnd)
{
    // Release previous resources if any
    if (g_hBitmap) { DeleteObject(g_hBitmap); g_hBitmap = NULL; }
    if (g_hMemDC)  { DeleteDC(g_hMemDC); g_hMemDC = NULL; }

    HDC hdc = GetDC(hwnd);
    g_hMemDC = CreateCompatibleDC(hdc);
    g_hBitmap = CreateCompatibleBitmap(hdc, g_clientW, g_clientH);
    SelectObject(g_hMemDC, g_hBitmap);

    // Fill with white background
    HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
    RECT r = {0,0,g_clientW,g_clientH};
    FillRect(g_hMemDC, &r, white);

    ReleaseDC(hwnd, hdc);
}

// Clear the back buffer to white
void ClearBackBuffer()
{
    if (!g_hMemDC) return;
    HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
    RECT r = {0,0,g_clientW,g_clientH};
    FillRect(g_hMemDC, &r, white);
}

// Draw a line on the back buffer (thick, anti-aliased is limited in GDI)
void DrawLineToBackBuffer(POINT fromPt, POINT toPt)
{
    if (!g_hMemDC) return;
    // Choose pen (black, width 6)
    HPEN hPen = CreatePen(PS_SOLID, 6, RGB(0,0,0));
    HPEN hOld = (HPEN)SelectObject(g_hMemDC, hPen);

    MoveToEx(g_hMemDC, fromPt.x, fromPt.y, NULL);
    LineTo(g_hMemDC, toPt.x, toPt.y);

    SelectObject(g_hMemDC, hOld);
    DeleteObject(hPen);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        {
            // create initial back buffer
            CreateBackBuffer(hwnd);
        }
        return 0;

    case WM_SIZE:
        {
            // When window resizes, recreate the back buffer and copy old content if desired.
            // For simplicity we recreate blank buffer on resize.
            g_clientW = LOWORD(lParam);
            g_clientH = HIWORD(lParam);
            CreateBackBuffer(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_LBUTTONDOWN:
        {
            g_drawing = true;
            g_lastPt.x = LOWORD(lParam);
            g_lastPt.y = HIWORD(lParam);
            SetCapture(hwnd); // capture mouse until LBUTTONUP
        }
        return 0;

    case WM_MOUSEMOVE:
        {
            if (g_drawing)
            {
                POINT cur;
                cur.x = LOWORD(lParam);
                cur.y = HIWORD(lParam);
                DrawLineToBackBuffer(g_lastPt, cur);
                // prepare for next segment
                g_lastPt = cur;
                // invalidate only the region around the line for efficiency (simple approach: whole window)
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        return 0;

    case WM_LBUTTONUP:
        {
            if (g_drawing)
            {
                POINT cur;
                cur.x = LOWORD(lParam);
                cur.y = HIWORD(lParam);
                DrawLineToBackBuffer(g_lastPt, cur);
                g_drawing = false;
                ReleaseCapture();
                InvalidateRect(hwnd, NULL, FALSE);
            }
        }
        return 0;

    case WM_KEYDOWN:
        {
            if (wParam == 'C' || wParam == 'c')
            {
                ClearBackBuffer();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (g_hBitmap && g_hMemDC)
            {
                BitBlt(hdc, 0, 0, g_clientW, g_clientH, g_hMemDC, 0, 0, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_DESTROY:
        {
            if (g_hBitmap) { DeleteObject(g_hBitmap); g_hBitmap = NULL; }
            if (g_hMemDC)  { DeleteDC(g_hMemDC); g_hMemDC = NULL; }
            PostQuitMessage(0);
        }
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// WinMain - program entry (Unicode)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    // Register window class
    const wchar_t CLASS_NAME[] = L"SimpleWhiteboardClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Simple Whiteboard - draw with left mouse. Press C to clear, Esc to quit",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, g_clientW, g_clientH,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
