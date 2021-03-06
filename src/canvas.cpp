#include "canvas.hpp"
#include <Windows.h>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <locale>
#include <codecvt>

namespace gui
{
    LRESULT CALLBACK Canvas::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Canvas* canvas = (Canvas*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

        // sort through and find what code to run for the message given
        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_MOUSEMOVE:
            if (canvas)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                canvas->OnMouseMove(x, y);
            }
            break;

        case WM_LBUTTONDOWN:
            if (canvas)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                canvas->OnLeftButtonDown(x, y);
            }
            break;

        case WM_LBUTTONUP:
            if (canvas)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                canvas->OnLeftButtonUp(x, y);
            }
            break;

        case WM_RBUTTONDOWN:
            if (canvas)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                canvas->OnRightButtonDown(x, y);
            }
            break;

        case WM_RBUTTONUP:
            if (canvas)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                canvas->OnRightButtonUp(x, y);
            }
            break;

        case WM_MOUSEWHEEL:
            if (canvas)
            {
                std::int16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
                canvas->OnMouseWheel(delta);
            }

        default:
            // Handle any messages the switch statement didn't
            return DefWindowProc(hwnd, message, wParam, lParam);
        }

        return 0;

    }

    ID2D1Factory* Canvas::GetD2DFactory()
    {
        static ID2D1Factory* d2d_factory_ = nullptr;

        if (!d2d_factory_)
        {
            HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory_);
            if (FAILED(hr))
            {
                throw std::runtime_error("Failed to create d2d1 factory!");
            }
        }

        return d2d_factory_;
    }

    IDWriteFactory* Canvas::GetDWriteFactory()
    {
        static IDWriteFactory* dwrite_factory_ = nullptr;

        if (!dwrite_factory_)
        {
            HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                __uuidof(dwrite_factory_), reinterpret_cast<IUnknown**>(&dwrite_factory_));

            if (FAILED(hr))
            {
                throw std::runtime_error("Failed to create dwrite factory!");
            }
        }

        return dwrite_factory_;
    }

    Canvas::Canvas(int x, int y, int width, int height, std::uint32_t id, HWND parent)
        : x_(x), y_(y), width_(width), height_(height), id_(id), parent_(parent)
    {
        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &Canvas::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszClassName = "MyCanvas";

        RegisterClassEx(&wc);

        // �reate the window
        hwnd_ = CreateWindowEx(0,
            wc.lpszClassName,                   // name of the window class
            "",                                 // title of the window
            WS_CHILDWINDOW | WS_VISIBLE,        // window style
            x_,                                 // x-position of the window
            y_,                                 // y-position of the window
            width_,                             // width of the window
            height_,                            // height of the window
            parent_,                            // parent window
            (HMENU)id_,                         // id
            hInstance,                          // application handle
            nullptr);                           // used with multiple windows

        SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)this);


        RECT rc;
        GetClientRect(hwnd_, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        FLOAT dpiX, dpiY;
        GetD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

        // Create a Direct2D render target.
        HRESULT hr = GetD2DFactory()->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
                dpiX, dpiY,
                D2D1_RENDER_TARGET_USAGE_NONE),
            D2D1::HwndRenderTargetProperties(hwnd_, size),
            &render_target_
        );

        render_target_->SetDpi(dpiX, dpiY);

        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create canvas rendertarget!");
        }


        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to create DirectWrite factory!");
        }

        hr = GetDWriteFactory()->CreateTextFormat(
            L"Verdana",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            12,
            L"", //locale
            &text_format_);

    }

    void Canvas::DrawBegin()
    {
        render_target_->BeginDraw();
        render_target_->SetTransform(D2D1::Matrix3x2F::Identity());
    }

    void Canvas::DrawEnd()
    {
        HRESULT hr = render_target_->EndDraw();
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to end drawing!");
        }
    }

    void Canvas::DrawPoint(int x, int y, std::uint32_t r, std::uint32_t g, std::uint32_t b)
    {
        assert(!"Not implemented!");
    }

    ID2D1SolidColorBrush* Canvas::GetBrush(Color const& color)
    {
        auto it = brush_cache_.find(color);
        if (it == brush_cache_.end())
        {
            ID2D1SolidColorBrush* brush = nullptr;
            D2D1_COLOR_F c = D2D1::ColorF(color.r, color.g, color.b);
            HRESULT hr = render_target_->CreateSolidColorBrush(c, &brush);
            if (FAILED(hr))
            {
                throw std::runtime_error("Failed to create color brush!");
            }

            brush_cache_.emplace(color, brush);
            return brush;
        }
        else
        {
            return it->second;
        }
    }

    void Canvas::DrawLine(int x1, int y1, int x2, int y2, Color const& color)
    {
        D2D1_POINT_2F p1 = D2D1::Point2F(x1 + 0.5f, y1 + 0.5f);
        D2D1_POINT_2F p2 = D2D1::Point2F(x2 + 0.5f, y2 + 0.5f);

        render_target_->DrawLine(p1, p2, GetBrush(color), 1.0f);
    }

    void Canvas::DrawRectangle(float x1, float y1, float x2, float y2, Color const& color)
    {
        D2D1_RECT_F rect = D2D1::RectF(x1 + 0.5f, y1 + 0.5f, x2 + 0.5f, y2 + 0.5f);

        render_target_->FillRectangle(&rect, GetBrush(color));
    }

    void Canvas::Clear(Color const& color)
    {
        render_target_->Clear(D2D1::ColorF(color.r, color.g, color.b));
    }

    void Canvas::DrawText(std::string const& text, int x, int y, Color const& color)
    {
        // Convert to wstring
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wtext = converter.from_bytes(text);

        render_target_->DrawTextA(wtext.c_str(), wtext.size(), text_format_, D2D1::RectF(x, y, width_, height_), GetBrush(color));
    }

}
