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

        Button* CreateButton(char const* label, int x, int y, int width, int height);
        void OnButtonPress(std::uint32_t button_id);

        Textbox* CreateTextbox(int x, int y, int width, int height);
        void OnTextboxChange(std::uint32_t text_id);

        Label* CreateLabel(int x, int y, int width, int height);
        Combobox* CreateCombobox(int x, int y, int width, int height);

        Canvas* CreateCanvas(int x, int y, int width, int height);

    private:
        std::unordered_map<std::uint32_t, void*> child_controls_;
        static std::uint32_t next_child_control_id_;
        HWND hwnd_;
        std::string caption_;


    };
}
