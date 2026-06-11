cmake_minimum_required(VERSION 3.16)
project(SpectrometerMVP)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(Eigen3 REQUIRED)

add_executable(app
    src/core/appconfig.hpp
    src/core/logging.hpp
    src/core/logging.cpp
    src/core/spectrum.hpp

    src/synthetic/syntheticprofile.hpp
    src/synthetic/syntheticprofile.cpp
    src/synthetic/syntheticgenerator.hpp
    src/synthetic/syntheticgenerator.cpp

    src/processing/signalprocessor.hpp
    src/processing/processingpipeline.hpp

    src/processing/movingaverage.hpp
    src/processing/movingaverage.cpp

    src/acquisition/spectrometer.hpp

    src/acquisition/filespectrometer/filespectrometer.hpp
    src/acquisition/filespectrometer/filespectrometer.cpp
    src/acquisition/mockspectrometer/mockspectrometer.hpp
    src/acquisition/mockspectrometer/mockspectrometer.cpp

    src/app/acquisitionworker.hpp
    src/app/acquisitionworker.cpp
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
    Eigen3::Eigen
)
