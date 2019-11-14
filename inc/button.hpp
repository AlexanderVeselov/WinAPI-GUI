#pragma once

#include <Windows.h>
#include <string>
#include <functional>

namespace gui
{
    class Button
    {
    public:
        Button(int x, int y, int width, int height, std::uint32_t id, HWND parent, char const* label);
        void SetCallback(std::function<void()> const& callback) { callback_ = callback; }
        void OnCallback() const { callback_(); }

    private:
        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        std::function<void()> callback_;

    };
}
