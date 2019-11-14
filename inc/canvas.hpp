#pragma once

#define NOMINMAX
#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <string>
#include <functional>
#include <map>

#pragma comment(lib, "d2d1.lib")

namespace gui
{
    struct Color
    {
        float r;
        float g;
        float b;

        bool operator < (Color const& other) const
        {
            return r < other.r || (r == other.r) && (g < other.g)
                || (r == other.r) && (g == other.g) && (b < other.b);
        }
    };

    class Canvas
    {
    public:
        Canvas(int x, int y, int width, int height, std::uint32_t id, HWND parent);

        int GetX() const { return x_; }
        int GetY() const { return y_; }
        int GetWidth() const { return width_; }
        int GetHeight() const { return height_; }

        void DrawBegin();
        void DrawEnd();
        void DrawPoint(int x, int y, std::uint32_t r, std::uint32_t g, std::uint32_t b);
        void DrawLine(int x1, int y1, int x2, int y2, Color const& color);
        void DrawRectangle(float x1, float y1, float x2, float y2, Color const& color);
        void PaintText(std::string const& text, int x, int y);
        void Clear(Color const& color);

        virtual void OnMouseMove(std::uint32_t x, std::uint32_t y) {}
        virtual void OnLeftButtonDown(std::uint32_t x, std::uint32_t y) {}
        virtual void OnLeftButtonUp(std::uint32_t x, std::uint32_t y) {}
        virtual void OnRightButtonDown(std::uint32_t x, std::uint32_t y) {}
        virtual void OnRightButtonUp(std::uint32_t x, std::uint32_t y) {}

    private:
        static LRESULT CALLBACK Canvas::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        ID2D1SolidColorBrush* GetBrush(Color const& color);

        int x_, y_;
        int width_, height_;
        std::uint32_t id_;
        HWND hwnd_;
        HWND parent_;
        ID2D1Factory* d2d_factory_ = nullptr;
        ID2D1HwndRenderTarget* render_target_ = nullptr;
        std::map<Color, ID2D1SolidColorBrush*> brush_cache_;
    };
}
