#include "window.hpp"

#include "button.hpp"
#include "textbox.hpp"

#include <Windows.h>
#include <WinUser.h>

#include <iostream>
#include <cassert>

namespace gui
{
    std::uint32_t Window::next_child_control_id_ = 0u;

#define WINDOW_CLASS "WindowClass1"

    LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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

        case WM_MOUSEMOVE:
            if (window && static_cast<UINT8>(wParam) == MK_LBUTTON)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                window->OnMouseMove(x, y);
            }
            break;

        case WM_LBUTTONDOWN:
            if (window)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                window->OnLeftButtonDown(x, y);
            }
            break;

        case WM_LBUTTONUP:
            if (window)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                window->OnLeftButtonUp(x, y);
            }
            break;

        case WM_RBUTTONDOWN:
            if (window)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                window->OnRightButtonDown(x, y);
            }
            break;

        case WM_RBUTTONUP:
            if (window)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                window->OnRightButtonUp(x, y);
            }
            break;

        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
            case BN_CLICKED:
                if (window)
                {
                    window->OnButtonPress((std::uint32_t)LOWORD(wParam));
                }
                break;
            case EN_CHANGE:
                if (window)
                {
                    window->OnTextboxChange((std::uint32_t)LOWORD(wParam));
                }
            }
            break;
        default:
            // Handle any messages the switch statement didn't
            return DefWindowProc(hwnd, message, wParam, lParam);
        }

        return 0;

    }

    Window::Window(const char* caption, HINSTANCE hInstance)
        : caption_(caption)
    {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &Window::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = WINDOW_CLASS;

        RegisterClassEx(&wc);

        // Ñreate the window
        hwnd_ = CreateWindowEx(0,
            WINDOW_CLASS,                       // name of the window class
            caption_.c_str(),                   // title of the window
            WS_OVERLAPPED | WS_SYSMENU,         // window style
            CW_USEDEFAULT,                      // x-position of the window
            CW_USEDEFAULT,                      // y-position of the window
            1280,                               // width of the window
            720,                                // height of the window
            nullptr,                            // we have no parent window
            nullptr,                            // we aren't using menus
            hInstance,                          // application handle
            nullptr);                           // used with multiple windows

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);

        ShowWindow(hwnd_, SW_SHOWNORMAL);
    }

    void Window::Show()
    {
        ShowWindow(hwnd_, SW_SHOWNORMAL);
    }

    void Window::OnButtonPress(std::uint32_t button_id)
    {
        auto it = child_controls_.find(button_id);
        if (it == child_controls_.end())
        {
            return;
        }

        static_cast<Button*>(it->second)->OnCallback();
    }

    void Window::OnTextboxChange(std::uint32_t text_id)
    {
        auto it = child_controls_.find(text_id);
        if (it == child_controls_.end())
        {
            return;
        }

        static_cast<Textbox*>(it->second)->OnChangeCallback();
    }

}
