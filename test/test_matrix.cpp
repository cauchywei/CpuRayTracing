#include <gtest/gtest.h>
#include "../math/matrix.h"

using namespace cry::math;

TEST(CryTest, Matrix2) {
    {
        Matrix2f m;
        ASSERT_FLOAT_EQ(m(0, 0), 0.0f);
        ASSERT_FLOAT_EQ(m(1, 0), 0.0f);
        ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
        ASSERT_FLOAT_EQ(m(1, 1), 0.0f);
    }
    {
        // Test identity
        Matrix2f m = Matrix2f::MakeIdentity();
        ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
        ASSERT_FLOAT_EQ(m(1, 0), 0.0f);
        ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
        ASSERT_FLOAT_EQ(m(1, 1), 1.0f);
    }

    {
        // Test transpose
        Matrix2f m = Matrix2f::MakeIdentity();
        m.transpose();
        ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
        ASSERT_FLOAT_EQ(m(1, 0), 0.0f);
        ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
        ASSERT_FLOAT_EQ(m(1, 1), 1.0f);
    }

    {
        // Test inverse
        Matrix2f m = Matrix2f::MakeIdentity();
        m.inverse();
        ASSERT_FLOAT_EQ(m(0, 0), 1.0f);
        ASSERT_FLOAT_EQ(m(1, 0), 0.0f);
        ASSERT_FLOAT_EQ(m(0, 1), 0.0f);
        ASSERT_FLOAT_EQ(m(1, 1), 1.0f);
    }
}

TEST(CryTest, Matrix3f) {
    auto m1 = Matrix3f(
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f
    );
    auto m2 = Matrix3f(
            9.0f, 8.0f, 7.0f,
            6.0f, 5.0f, 4.0f,
            3.0f, 2.0f, 1.0f
    );

    ASSERT_TRUE(m1 != m2);
    auto m3 = m1 * m2;
    ASSERT_EQ(m3, Matrix3f(30.0f, 24.0f, 18.0f,
                           84.0f, 69.0f, 54.0f,
                           138.0f, 114.0f, 90.0f
    ));
}