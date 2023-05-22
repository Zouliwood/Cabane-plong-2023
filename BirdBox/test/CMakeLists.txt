cmake_minimum_required(VERSION 3.10)
project(BirdBox)

set(This BirdBoxTest)

# Chemin de fichier
set(HEADERS_TEST_DIR "../hrc")
set(SOURCES_TEST_DIR "../src")

# traiter les warnings comme des erreurs
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Sources, Headers et tests
file(GLOB ${This}_SRC CONFIGURE_DEPENDS
    "${HEADERS_TEST_DIR}/*.hpp"
    "${SOURCES_TEST_DIR}/*.cpp"
    "*.cpp"
)

# Exécutable
add_executable(${This} ${${This}_SRC})
target_link_libraries(${This} PUBLIC
    gtest_main
    ${OpenCV_LIBS}
)

# Ajout de la commande de test
add_test(
    NAME ${This}
    COMMAND ${This}
)
