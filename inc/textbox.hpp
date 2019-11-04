#pragma once

#include <Windows.h>
#include <string>
#include <functional>

namespace gui
{
    class Textbox
    {
    public:
        Textbox(int x, int y, int width, int height, std::uint32_t id, HWND parent);
        std::string const& GetText() const;
        void SetChangeCallback(std::function<void(std::string const&)> const& callback) { change_callback_ = callback; }
        void OnChangeCallback() const { change_callback_(GetText()); }

    private:
        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        mutable std::string text_;
        std::function<void(std::string const&)> change_callback_;
    };
}
