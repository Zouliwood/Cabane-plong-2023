cmake_minimum_required(VERSION 3.24)

set(This BirdBoxTest)

# Fichiers de test
set(TestSources
        CalculTest.cpp MatrixTest.cpp)

# Sources et Headers
set(Sources
    "${SOURCES_TEST_DIR}/Camera.cpp"
    "${SOURCES_TEST_DIR}/Calcul.cpp"
    "${SOURCES_TEST_DIR}/Image.cpp"
    "${SOURCES_TEST_DIR}/Bird.cpp"
    "${SOURCES_TEST_DIR}/Matrix.cpp"
    "${SOURCES_TEST_DIR}/Couleur.cpp"
    "${SOURCES_TEST_DIR}/MultilayerPerceptron.cpp"
)

set(Headers
    "${HEADERS_TEST_DIR}/Camera.hpp"
    "${HEADERS_TEST_DIR}/Calcul.hpp"
    "${HEADERS_TEST_DIR}/Image.hpp"
    "${HEADERS_TEST_DIR}/Bird.hpp"
    "${HEADERS_TEST_DIR}/Matrix.hpp"
    "${HEADERS_TEST_DIR}/Couleur.hpp"
    "${HEADERS_TEST_DIR}/MultilayerPerceptron.hpp"
)

# Exécutable
add_executable(${This} ${Headers} ${Sources} ${TestSources})
target_link_libraries(${This} PUBLIC
    gtest_main
    ${OpenCV_LIBS}
)

# Ajout de la commande de test
add_test(
    NAME ${This}
    COMMAND ${This}
)
