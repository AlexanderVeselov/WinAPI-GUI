#include "canvas.hpp"
#include <Windows.h>
#include <cassert>

namespace gui
{
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Canvas* canvas = (Canvas*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        // sort through and find what code to run for the message given
        switch (message)
        {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
            break;
        case WM_LBUTTONDOWN:
            break;
        case WM_LBUTTONUP:
            break;
        case WM_RBUTTONDOWN:
            break;
        case WM_RBUTTONUP:
            break;
        case WM_COMMAND:
            break;
        case WM_PAINT:
            canvas->RenderImpl();
            break;
        default:
            // Handle any messages the switch statement didn't
            return DefWindowProc(hwnd, message, wParam, lParam);
        }

        return 0;

    }

    Canvas::Canvas(int x, int y, int width, int height, std::uint32_t id, HWND parent)
        : x_(x), y_(y), width_(width), height_(height), id_(id), parent_(parent)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszClassName = "CANVAS";

        RegisterClassEx(&wc);

        // Ñreate the window
        hwnd_ = CreateWindowEx(0,
            wc.lpszClassName,                   // name of the window class
            "",                                 // title of the window
            WS_CHILD | WS_VISIBLE,              // window style
            x_,                                 // x-position of the window
            y_,                                 // y-position of the window
            width_,                             // width of the window
            height_,                            // height of the window
            parent_,                            // parent window
            (HMENU)id_,                         // id
            hInstance,                          // application handle
            nullptr);                           // used with multiple windows

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);

        HDC hdc = GetDC(hwnd_);
        back_hdc_ = CreateCompatibleDC(hdc);
        back_bitmap_ = CreateCompatibleBitmap(hdc, width_, height_);
        SelectObject(back_hdc_, back_bitmap_);
        ReleaseDC(hwnd_, hdc);

        font_ = CreateFont(12, 0, 0, 0, 300, false, false, false,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    }

    void Canvas::Flush()
    {
        InvalidateRect((HWND)hwnd_, NULL, false);
    }

    void Canvas::RenderImpl()
    {
        // Copy data from the backbuffer
        PAINTSTRUCT ps;
        BeginPaint(hwnd_, &ps);
        BitBlt(ps.hdc, 0, 0, width_, height_, back_hdc_, 0, 0, SRCCOPY);
        EndPaint(hwnd_, &ps);
    }

    void Canvas::DrawPoint(int x, int y, std::uint32_t r, std::uint32_t g, std::uint32_t b)
    {
        SetPixel(back_hdc_, x, y, RGB(r, g, b));
    }

    void Canvas::DrawLine(int x1, int y1, int x2, int y2)
    {
        MoveToEx(back_hdc_, x1, y1, 0);
        LineTo(back_hdc_, x2, y2);
    }

    void Canvas::DrawRectangle(int x1, int y1, int x2, int y2)
    {
        Rectangle(back_hdc_, x1, y1, x2, y2);
    }

    void Canvas::Clear(std::uint32_t r, std::uint32_t g, std::uint32_t b)
    {
        RECT client_rect = { 0, 0, width_, height_ };
        FillRect(back_hdc_, &client_rect, CreateSolidBrush(RGB(r, g, b)));
    }

    void Canvas::PaintText(std::string const& text, int x, int y)
    {
        RECT client_rect = { x, y, width_, height_ };
        SetBkMode(back_hdc_, TRANSPARENT);

        HFONT old_font = (HFONT)SelectObject(back_hdc_, font_);
        DrawText(back_hdc_, text.c_str(), text.size(), &client_rect, DT_TOP | DT_LEFT);
        SelectObject(back_hdc_, old_font);
    }

}
