#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>

namespace gui
{
    class Window
    {
    public:
        Window(const char* caption, HINSTANCE hinstance);
        void Show();
        std::string const& GetCaption() const { return caption_; }

        template <class T, typename ...Args>
        T* CreateChildControl(int x, int y, int width, int height, Args&& ... args)
        {
            T* control = new T(x, y, width, height, next_child_control_id_, hwnd_, std::forward<Args>(args)...);
            child_controls_.emplace(next_child_control_id_++, control);
            return control;
        }

        virtual void OnMouseMove(std::uint32_t x, std::uint32_t y) {}
        virtual void OnLeftButtonDown(std::uint32_t x, std::uint32_t y) {}
        virtual void OnLeftButtonUp(std::uint32_t x, std::uint32_t y) {}
        virtual void OnRightButtonDown(std::uint32_t x, std::uint32_t y) {}
        virtual void OnRightButtonUp(std::uint32_t x, std::uint32_t y) {}

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        void OnButtonPress(std::uint32_t button_id);
        void OnTextboxChange(std::uint32_t text_id);

        std::unordered_map<std::uint32_t, void*> child_controls_;
        static std::uint32_t next_child_control_id_;
        HWND hwnd_;
        std::string caption_;

    };
}
