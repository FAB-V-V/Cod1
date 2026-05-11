cmake_minimum_required(VERSION 3.16)
project(proyecto_urls_fabiana VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(proyecto_urls_fabiana
    WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
)

target_link_libraries(proyecto_urls_fabiana
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts
)