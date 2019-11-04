#include "combobox.hpp"
#include <Windows.h>
#include <vector>

namespace gui
{
    Combobox::Combobox(int x, int y, int width, int height, std::uint32_t id, HWND parent)
        : x_(x), y_(y), width_(width), height_(height), id_(id), parent_(parent)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        hwnd_ = CreateWindowEx(0,
            "combobox",                                 // name of the window class
            "",                                         // title of the window
            WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL, // window style
            x,                                          // x-position of the window
            y,                                          // y-position of the window
            width,                                      // width of the window
            height,                                     // height of the window
            parent_,                                    // parent window
            (HMENU)id_,                                 // button ID
            hInstance,                                  // application handle
            nullptr);                                   // used with multiple windows, nullptr
    }

    void Combobox::AddItem(std::string const& item)
    {
        SendMessage(hwnd_, CB_ADDSTRING, (WPARAM)0, (LPARAM)item.c_str());
    }

    void Combobox::SetItems(std::vector<std::string> const& items)
    {
        ClearItems();
        for (auto const& str : items)
        {
            AddItem(str);
        }
    }

    void Combobox::ClearItems()
    {
        SendMessage(hwnd_, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
    }

    std::uint32_t Combobox::GetSelectedIndex() const
    {
        LRESULT result = SendMessage(hwnd_, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
        return result == CB_ERR ? ~0u : (std::uint32_t)result;
    }
}
