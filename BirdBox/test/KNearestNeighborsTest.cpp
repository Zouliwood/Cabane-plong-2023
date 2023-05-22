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

        Vec3b cockatiel[NB_COLOR] = {Vec3b(255, 55, 0), Vec3b(255, 55, 0)};
        Vec3b triton_cockatoo[NB_COLOR] = {Vec3b(255, 55, 0), Vec3b(255, 55, 0)};
        Vec3b gouldian_finch[NB_COLOR] = {Vec3b(255, 0, 0), Vec3b(255, 0, 0)};
        Vec3b columba[NB_COLOR] = {Vec3b(255, 255, 255), Vec3b(255, 255, 255)};
        Vec3b columba_bis[NB_COLOR] = {Vec3b(255, 245, 250), Vec3b(255, 245, 250)};

        list.insert(list.end(), {
            Bird(cockatiel, 45, "Cockatiel"),
            Bird(triton_cockatoo, 50, "Triton cockatoo"),
            Bird(gouldian_finch, 15, "Gouldian finch"),
            Bird(columba, 32, "Columba"),
            Bird(columba_bis, 35, "Columba")
        });

        knn = KNearestNeighbors(list);

    }


    void TearDown() override {

        list.clear();

    }

};

//Create helper
Vec3b colors1[NB_COLOR] = {Vec3b(255, 10, 0), Vec3b(255, 10, 0)};
Vec3b colors2[NB_COLOR] = {Vec3b(255, 255, 250), Vec3b(255, 255, 250)};
Vec3b colors3[NB_COLOR] = {Vec3b(255, 250, 250), Vec3b(255, 250, 250)};
Vec3b colors4[NB_COLOR] = {Vec3b(255, 245, 255), Vec3b(255, 245, 255)};

INSTANTIATE_TEST_SUITE_P(
        KNNValues,
        KNearestNeighborsTest,
        testing::Values(
            Bird(colors1, 45),
            Bird(colors2, 50),
            Bird(colors3, 15),
            Bird(colors4, 32)
        )
);

TEST_F(KNearestNeighborsTest, ValueWindow) {
    Vec3b color[NB_COLOR] = {Vec3b(255, 55, 0), Vec3b(255, 55, 0)};
    vector<Bird> result = knn.getKNNWindow(Bird(color, 49.7), 1);

    EXPECT_EQ(result.size(), 1);
}

TEST_F(KNearestNeighborsTest, ValueWindowEmpty) {
    Vec3b  color[NB_COLOR] = {Vec3b(255, 55, 0), Vec3b(255, 55, 0)};
    vector<Bird> result = knn.getKNNWindow(Bird(color, 47.5), 1);

    EXPECT_TRUE(result.empty());
}

TEST_F(KNearestNeighborsTest, MostCloseValue) {
    Vec3b color[NB_COLOR] = {Vec3b(255, 55, 0), Vec3b(255, 55, 0)};
    String result = KNearestNeighbors::getMostCommonType(knn.getKNNDistance(Bird(color, 47.5), 1, EUCLIDEAN, RGB));

    EXPECT_EQ(result, "Cockatiel");
}

TEST_P(KNearestNeighborsTest, MostCommonValue) {
    String result = KNearestNeighbors::getMostCommonType(knn.getKNNDistance(GetParam(), 5, EUCLIDEAN, RGB));

    EXPECT_EQ(result, "Columba");
}

TEST_P(KNearestNeighborsTest, ErrorEmptyField) {
    KNearestNeighbors knn(vector<Bird>{});

    EXPECT_THROW(knn.getMostCommonType(knn.getKNNDistance(GetParam(), 5, EUCLIDEAN, RGB)), domain_error);
}
