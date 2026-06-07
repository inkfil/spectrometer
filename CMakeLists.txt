cmake_minimum_required(VERSION 3.16)
project(SpectrometerMVP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Charts)
find_package(Eigen3 REQUIRED)

add_executable(app
    src/core/spectrum.hpp

    src/processing/signalprocessor.hpp
    src/processing/processingpipeline.hpp

    src/processing/movingaverage.hpp
    src/processing/movingaverage.cpp

    src/acquisition/spectrometer.hpp

    src/acquisition/mockspectrometer/mockspectrometer.hpp
    src/acquisition/mockspectrometer/mockspectrometer.cpp

    src/app/spectrometercontroller.hpp
    src/app/spectrometercontroller.cpp
    src/main.cpp
    resource.qrc
    src/app/spectrumview.hpp
    src/app/spectrumview.cpp
)

target_link_libraries(app PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Charts
    Eigen3::Eigen
)
