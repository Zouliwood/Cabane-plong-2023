cmake_minimum_required(VERSION 3.10)
project(BirdBox)

set(This BirdBox)

set(CMAKE_CXX_STANDARD 17)
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
    "${SOURCES_DIR}/Bird.cpp"
    "${SOURCES_DIR}/Matrix.cpp"
    "${SOURCES_DIR}/Couleur.cpp"
    "${SOURCES_DIR}/MultilayerPerceptron.cpp"
    "${SOURCES_DIR}/KNearestNeighbors.cpp"
    "${SOURCES_DIR}/SingletonKNearestNeighbors.cpp"
)

set(Headers
    "${HEADERS_DIR}/Camera.hpp"
    "${HEADERS_DIR}/Calcul.hpp"
    "${HEADERS_DIR}/Image.hpp"
    "${HEADERS_DIR}/Bird.hpp"
    "${HEADERS_DIR}/Matrix.hpp"
    "${HEADERS_DIR}/Couleur.hpp"
    "${HEADERS_DIR}/MultilayerPerceptron.hpp"
    "${HEADERS_DIR}/KNearestNeighbors.hpp"
    "${HEADERS_DIR}/SingletonKNearestNeighbors.hpp"
)

# Exécutable
add_executable(${This} ${Sources} ${Headers})
target_link_libraries(${This} ${OpenCV_LIBS})

# Ajout des tests
option (WITH_TESTS "Activer les tests pour le projet" ON)
if (WITH_TESTS)
    add_subdirectory(test)
endif()
