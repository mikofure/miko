# Install configuration
include(GNUInstallDirs)

install(TARGETS miko
    EXPORT MikoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(EXPORT MikoTargets
    FILE MikoTargets.cmake
    NAMESPACE Miko::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Miko
)

# Generate config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MikoConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MikoConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Miko
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MikoConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MikoConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/MikoConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Miko
)

# Shader installation
if(MIKO_RUNTIME_SHADERS)
    install(DIRECTORY shaders/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/miko/shaders
        FILES_MATCHING
        PATTERN "*.vert"
        PATTERN "*.frag"
        PATTERN "*.comp"
        PATTERN "*.spv"
    )
endif()