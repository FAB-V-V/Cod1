// CMakeLists.txt es el mapa de construcción del proyecto.
cmake_minimum_required(VERSION 3.16)        // Indica la version minima de CMake
project(proyecto_urls_fabiana VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)                // Usar C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)        //Buscar Qt6 instalado en el computador

qt_standard_project_setup()

qt_add_executable(proyecto_urls            // Lista de todos los archivos que se van a usar
    WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
)

target_link_libraries(proyecto_urls                // une el programa con las librerias de Qt
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts
)
