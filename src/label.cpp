#include "label.hpp"
#include <Windows.h>

namespace gui
{
    Label::Label(int x, int y, int width, int height, std::uint32_t id, HWND parent)
        : x_(x), y_(y), width_(width), height_(height), id_(id), parent_(parent)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        hwnd_ = CreateWindowEx(0,
            "static",                                   // name of the window class
            "",                                         // title of the window
            WS_VISIBLE | WS_CHILD,                      // window style
            x,                                          // x-position of the window
            y,                                          // y-position of the window
            width,                                      // width of the window
            height,                                     // height of the window
            parent_,                                    // parent window
            (HMENU)id_,                                 // button ID
            hInstance,                                  // application handle
            nullptr);                                   // used with multiple windows, nullptr
    }

    std::string const& Label::GetText() const
    {
        return text_;
    }

    void Label::SetText(std::string const& text)
    {
        text_ = text;
        SetWindowText(hwnd_, text_.c_str());
    }
}
