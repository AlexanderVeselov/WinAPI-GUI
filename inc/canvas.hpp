#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <functional>

namespace gui
{
    class Canvas
    {
    public:
        Canvas(int x, int y, int width, int height, std::uint32_t id, HWND parent);

        int GetX() const { return x_; }
        int GetY() const { return y_; }
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }

        void RenderImpl();

        void DrawPoint(int x, int y, std::uint32_t r, std::uint32_t g, std::uint32_t b);
        void DrawLine(int x1, int y1, int x2, int y2);
        void DrawRectangle(int x1, int y1, int x2, int y2);
        void PaintText(std::string const& text, int x, int y);
        void Clear(std::uint32_t r, std::uint32_t g, std::uint32_t b);

        void Flush();

    private:
        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        HDC back_hdc_ = nullptr;
        HBITMAP back_bitmap_ = nullptr;
        HFONT font_ = nullptr;
    };
}
