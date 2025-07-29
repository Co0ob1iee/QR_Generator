cmake_minimum_required(VERSION 3.16)
project(QRGenerator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Znajdź wymagane pakiety
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs objdetect videoio)

# Sprawdź dostępność libqrencode
pkg_check_modules(QRENCODE REQUIRED libqrencode)

# Aktywuj Qt MOC (Meta Object Compiler)
set(CMAKE_AUTOMOC ON)

# Katalogi z plikami nagłówkowymi
include_directories(include)

# Lista plików źródłowych
set(SOURCES
    src/main.cpp
    src/qrgenerator.cpp
    src/ui_setup.cpp
    src/qr_generation.cpp
    src/wifi_handler.cpp
    src/qr_reader.cpp
    src/utils.cpp
)

# Lista plików nagłówkowych
set(HEADERS
    include/qrgenerator.h
)

# Stwórz wykonywany plik
add_executable(qrgenerator ${SOURCES} ${HEADERS})

# Linkuj z bibliotekami
target_link_libraries(qrgenerator 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Gui
    ${OpenCV_LIBS}
    ${QRENCODE_LIBRARIES}
)

# Dodaj katalogi z nagłówkami
target_include_directories(qrgenerator PRIVATE 
    ${QRENCODE_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    include/
)

# Dodaj flagi kompilacji
target_compile_options(qrgenerator PRIVATE ${QRENCODE_CFLAGS_OTHER})

# Ustaw właściwości kompilacji
set_target_properties(qrgenerator PROPERTIES
    OUTPUT_NAME "qr-generator"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Instalacja
install(TARGETS qrgenerator 
    RUNTIME DESTINATION bin
)

# Opcjonalnie - zainstaluj ikonę desktop (jeśli zostanie stworzona)
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/qrgenerator.desktop")
    install(FILES resources/qrgenerator.desktop
        DESTINATION share/applications
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/resources/qrgenerator.png")
    install(FILES resources/qrgenerator.png
        DESTINATION share/icons/hicolor/256x256/apps
    )
endif()
