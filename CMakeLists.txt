cmake_minimum_required(VERSION 3.19)
project(qt6-tcp-client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Network)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/qt6-tcp-client)

qt_standard_project_setup()

qt_add_executable(qt6-tcp-client
    main.cpp
    emulator.h emulator.cpp

)

target_link_libraries(qt6-tcp-client
    PRIVATE
        Qt::Core
        Qt::Network
)

install(TARGETS qt6-tcp-client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
)
qt_generate_deploy_app_script(
    TARGET qt6-tcp-client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(CODE "
    set(QT_DEPLOY_BIN_DIR .)
    include(${deploy_script})
    file(WRITE ${CMAKE_INSTALL_PREFIX}/qt.conf [Paths]\nPrefix = .\n)
")
