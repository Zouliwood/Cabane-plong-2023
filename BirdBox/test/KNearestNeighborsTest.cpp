#include "../hrc/KNearestNeighbors.hpp"
#include <gtest/gtest.h>

class KNearestNeighborsTest :
        public ::testing::Test,
        public ::testing::WithParamInterface<Bird> {

protected:

    KNearestNeighbors knn;

    vector<Bird> list;

    KNearestNeighborsTest()
            : knn{vector<Bird>()} {
    }

    void SetUp() override {

        list.insert(list.end(), {
            Bird(Vec3b(255, 55, 0), 45, "Cockatiel"),
            Bird(Vec3b(255, 55, 0), 50, "Triton cockatoo"),
            Bird(Vec3b(255, 0, 0), 15, "Gouldian finch"),
            Bird(Vec3b(255, 255, 255), 32, "Columba"),
            Bird(Vec3b(255, 245, 250), 35, "Columba")
        });

        knn = KNearestNeighbors(list);

    }


    void TearDown() override {

        list.clear();

    }

};

INSTANTIATE_TEST_SUITE_P(
        KNNValues,
        KNearestNeighborsTest,
        testing::Values(
            Bird(Vec3b(255, 10, 0), 45),
            Bird(Vec3b(255, 255, 250), 50),
            Bird(Vec3b(255, 250, 250), 15),
            Bird(Vec3b(255, 245, 255), 32)
        )
);

TEST_F(KNearestNeighborsTest, ValueWindow) {
    vector<Bird> result = knn.getKNNWindow(Bird(Vec3b(255, 55, 0), 49.7), 1);

    EXPECT_EQ(result.size(), 1);
}

TEST_F(KNearestNeighborsTest, ValueWindowEmpty) {
    vector<Bird> result = knn.getKNNWindow(Bird(Vec3b(255, 55, 0), 47.5), 1);

    EXPECT_TRUE(result.empty());
}

TEST_F(KNearestNeighborsTest, MostCloseValue) {
    String result = KNearestNeighbors::getMostCommonType(knn.getKNNDistance(Bird(Vec3b(255, 55, 0), 47.5), 1));

    EXPECT_EQ(result, "Cockatiel");
}

TEST_P(KNearestNeighborsTest, MostCommonValue) {
    String result = KNearestNeighbors::getMostCommonType(knn.getKNNDistance(GetParam(), 5));

    EXPECT_EQ(result, "Columba");
}

TEST_P(KNearestNeighborsTest, ErrorEmptyField) {
    KNearestNeighbors knn(vector<Bird>{});

    EXPECT_THROW(knn.getMostCommonType(knn.getKNNDistance(GetParam(), 5)), domain_error);
}
