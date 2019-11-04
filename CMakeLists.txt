set(INCLUDE
    inc/window.hpp
    inc/button.hpp
    inc/textbox.hpp
)

set(SOURCES
    src/window.cpp
    src/button.cpp
    src/textbox.cpp
)

add_library(WinAPIGUI STATIC ${SOURCES} ${INCLUDE})
target_compile_features(WinAPIGUI PRIVATE cxx_std_17)
target_include_directories(WinAPIGUI PUBLIC inc/.)
