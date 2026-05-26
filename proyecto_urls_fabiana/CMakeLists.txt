# CMakeLists.txt es el mapa de construcción del proyecto.
cmake_minimum_required(VERSION 3.16)        # indica la version minima de CMake
project(proyecto_urls_fabiana VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)                # usar C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)        #buscar Qt6 instalado en el computador

qt_standard_project_setup()

qt_add_executable(proyecto_urls            # lista de todos los archivos que se van a usar
    WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    data/DataStructures.h
    data/DataStructures.cpp
    data/DataLoader.h
    data/DataLoader.cpp
)

target_link_libraries(proyecto_urls                # une el programa con las librerias de Qt
    PRIVATE Qt6::Core Qt6::Widgets Qt6::Charts
)

# copiar los archivos CSV al compilador para que el ejecutable los encuentre
configure_file(data/base1_features.csv ${CMAKE_CURRENT_BINARY_DIR}/data/base1_features.csv COPYONLY)
configure_file(data/base2_features.csv ${CMAKE_CURRENT_BINARY_DIR}/data/base2_features.csv COPYONLY)
