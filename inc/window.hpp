#pragma once

#include "button.hpp"
#include "textbox.hpp"
#include "label.hpp"
#include "combobox.hpp"
#include "canvas.hpp"

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
            T* control = new T(x, y, width, height, next_child_control_id_, hwnd_);
            child_controls_.emplace(next_child_control_id_, control);
            ++next_child_control_id_;
            return control;
        }

        void OnButtonPress(std::uint32_t button_id);
        void OnTextboxChange(std::uint32_t text_id);

    private:
        std::unordered_map<std::uint32_t, void*> child_controls_;
        static std::uint32_t next_child_control_id_;
        HWND hwnd_;
        std::string caption_;


    };
}
