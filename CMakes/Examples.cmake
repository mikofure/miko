# Examples
if(MIKO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(MIKO_BUILD_TESTS)
    enable_testing()
    find_package(GTest QUIET)
    if(GTest_FOUND)
        add_subdirectory(tests)
    else()
        message(WARNING "Google Test not found. Tests will not be built.")
    endif()
endif()

# Examples subdirectory helper function
if(MIKO_BUILD_EXAMPLES)
    function(add_miko_example name)
        add_executable(${name} ${name}.cpp)
        target_link_libraries(${name} PRIVATE miko)

        # Copy SDL2 DLLs on Windows
        if(WIN32 AND SDL2_RUNTIME_LIBRARIES)
            add_custom_command(TARGET ${name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL2_RUNTIME_LIBRARIES}
                $<TARGET_FILE_DIR:${name}>
            )
        endif()

        set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
        )
    endfunction()

    # Define all examples
    set(MIKO_EXAMPLES
        hello_world
        image_viewer
        input_demo
        window_events
        opacity_demo
        pattern_demo
        menu_demo
        shadow_demo
        svg_viewer
        tab_navigation
        text_editor
        text_wrapping
        tree_view
        list_view
        window_styling
    )

    foreach(example ${MIKO_EXAMPLES})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/${example}.cpp)
            add_miko_example(${example})
        endif()
    endforeach()
endif()