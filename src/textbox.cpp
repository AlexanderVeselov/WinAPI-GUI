#include "textbox.hpp"
#include <Windows.h>
#include <vector>

namespace gui
{
    Textbox::Textbox(int x, int y, int width, int height, std::uint32_t id, HWND parent)
        : x_(x), y_(y), width_(width), height_(height), id_(id), parent_(parent)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        hwnd_ = CreateWindowEx(0,
            "edit",                                     // name of the window class
            "Sample text",                              // title of the window
            WS_VISIBLE | WS_CHILD,                      // window style
            x,                                          // x-position of the window
            y,                                          // y-position of the window
            width,                                      // width of the window
            height,                                     // height of the window
            parent_,                                    // parent window
            (HMENU)id_,                                 // button ID
            hInstance,                                  // application handle
            nullptr);                                      // used with multiple windows, nullptr
    }

    std::string const& Textbox::GetText() const
    {
        int length = GetWindowTextLength(hwnd_);
        std::vector<char> raw_char_data(length + 1);
        GetWindowText(hwnd_, raw_char_data.data(), length + 1);
        text_.resize(length + 1);
        std::copy(raw_char_data.begin(), raw_char_data.end(), text_.begin());
        return text_;
    }
}
