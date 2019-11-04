#pragma once

#include <Windows.h>
#include <string>

namespace gui
{
    class Label
    {
    public:
        Label(int x, int y, int width, int height, std::uint32_t id, HWND parent);
        std::string const& GetText() const;
        void SetText(std::string const& text);

    private:
        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        std::string text_;
    };
}
