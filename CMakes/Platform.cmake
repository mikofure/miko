# Platform validation
if(APPLE)
    message(FATAL_ERROR "macOS is not supported. This project targets Windows and Linux only.")
endif()

# Platform detection
if(WIN32)
    set(MIKO_PLATFORM "windows")
elseif(UNIX)
    set(MIKO_PLATFORM "linux")
endif()

# Optional: Memory leak detection
if(MIKO_ENABLE_LEAK_DETECTION)
    if(WIN32)
        # Use Windows CRT debug heap
    else()
        # Use AddressSanitizer or Valgrind on Linux
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
    endif()
endif()