set(INCLUDE
    inc/window.hpp
    inc/button.hpp
    inc/textbox.hpp
    inc/label.hpp
    inc/combobox.hpp
    inc/canvas.hpp
)

set(SOURCES
    src/window.cpp
    src/button.cpp
    src/textbox.cpp
    src/label.cpp
    src/combobox.cpp
    src/canvas.cpp
)

add_library(WinAPIGUI STATIC ${SOURCES} ${INCLUDE})
target_compile_features(WinAPIGUI PRIVATE cxx_std_17)
target_include_directories(WinAPIGUI PUBLIC inc/.)
