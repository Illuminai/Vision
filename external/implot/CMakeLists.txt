add_library(implot
        src/implot.cpp
        src/implot_demo.cpp
        src/implot_items.cpp)

target_include_directories(implot PUBLIC include)

target_link_libraries(implot PUBLIC imgui)