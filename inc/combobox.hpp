#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace gui
{
    class Combobox
    {
    public:
        Combobox(int x, int y, int width, int height, std::uint32_t id, HWND parent);
        void AddItem(std::string const& item);
        void SetItems(std::vector<std::string> const& items);
        void ClearItems();
        std::uint32_t GetSelectedIndex() const;
        //void SetChangeCallback(std::function<void(std::string const&)> const& callback) { change_callback_ = callback; }
        //void OnChangeCallback() const { change_callback_(GetText()); }

    private:
        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        //std::function<void(std::string const&)> change_callback_;
    };
}
