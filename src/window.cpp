#include "window.hpp"

#include <Windows.h>
#include <WinUser.h>

#include <iostream>
#include <cassert>

namespace gui
{
    std::uint32_t Window::next_child_control_id_ = 0u;

#define WINDOW_CLASS "WindowClass1"

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
        case WM_LBUTTONDOWN:
            break;
        case WM_LBUTTONUP:
            break;
        case WM_RBUTTONDOWN:
            break;
        case WM_RBUTTONUP:
            break;
        case WM_COMMAND:
            switch (HIWORD(wParam))
            {
            case BN_CLICKED:
                window->OnButtonPress((std::uint32_t)LOWORD(wParam));
                break;
            case EN_CHANGE:
                window->OnTextboxChange((std::uint32_t)LOWORD(wParam));
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
        std::cout << "Window::Window()" << std::endl;
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
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
            100,                                // x-position of the window
            100,                                // y-position of the window
            1280,                               // width of the window
            720,                                // height of the window
            nullptr,                            // we have no parent window
            nullptr,                            // we aren't using menus
            hInstance,                          // application handle
            nullptr);                           // used with multiple windows

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);
    }

    void Window::Show()
    {
        ShowWindow(hwnd_, SW_SHOWNORMAL);
    }

    Button* Window::CreateButton(char const* label, int x, int y, int width, int height)
    {
        Button* button = new Button(label, x, y, width, height, next_child_control_id_, hwnd_);
        child_controls_.emplace(next_child_control_id_, button);
        ++next_child_control_id_;
        return button;
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

    Textbox* Window::CreateTextbox(int x, int y, int width, int height)
    {
        Textbox* textbox = new Textbox(x, y, width, height, next_child_control_id_, hwnd_);
        child_controls_.emplace(next_child_control_id_, textbox);
        ++next_child_control_id_;
        return textbox;
    }

}
