# Examples CMakeLists.txt

# Basic example application
add_executable(basic_example basic_example.cpp)
target_link_libraries(basic_example miko)

# Widget showcase example
add_executable(widget_showcase widget_showcase.cpp)
target_link_libraries(widget_showcase miko)

# Debug example application
add_executable(debug_example debug_example.cpp)
target_link_libraries(debug_example miko)

# Set target properties
set_target_properties(basic_example widget_showcase debug_example PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)