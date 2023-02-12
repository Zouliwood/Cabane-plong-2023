cmake_minimum_required(VERSION 3.24)
project(BirdBox)

set(This BirdBox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Chemin de fichier
set(HEADERS_DIR "hrc")
set(SOURCES_DIR "src")
set(HEADERS_TEST_DIR "../hrc")
set(SOURCES_TEST_DIR "../src")

# Googletest
enable_testing()
add_subdirectory(googletest)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Sources et Headers
set(Sources "main.cpp"
    "${SOURCES_DIR}/Camera.cpp"
    "${SOURCES_DIR}/Calcul.cpp"
    "${SOURCES_DIR}/Image.cpp"
)

set(Headers
    "${HEADERS_DIR}/Camera.hpp"
    "${HEADERS_DIR}/Calcul.hpp"
    "${HEADERS_DIR}/Image.hpp"
)

# Exécutable
add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} ${OpenCV_LIBS})

# Ajout des tests
option (WITH_TESTS "Activer les tests pour le projet" ON)
if (WITH_TESTS)
    add_subdirectory(test)
endif()
