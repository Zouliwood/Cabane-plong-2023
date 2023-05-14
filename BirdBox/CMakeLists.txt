cmake_minimum_required(VERSION 3.10)
project(BirdBox)

set(This BirdBox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Chemin de fichier
set(HEADERS_DIR "hrc")
set(SOURCES_DIR "src")

# traiter les warnings comme des erreurs
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Googletest
enable_testing()
add_subdirectory(googletest)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Sources and Headers
file(GLOB ${This}_SRC CONFIGURE_DEPENDS
    "${HEADERS_DIR}/*.hpp"
    "${SOURCES_DIR}/*.cpp"
    "*.cpp"
)

# Exécutable
add_executable(${This} ${${This}_SRC})
target_link_libraries(${This} ${OpenCV_LIBS})

# Ajout des tests
option (WITH_TESTS "Activer les tests pour le projet" ON)
if (WITH_TESTS)
    add_subdirectory(test)
endif()
