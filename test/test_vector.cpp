#include <gtest/gtest.h>
#include "../math/vector.h"

using namespace cry::math;

TEST(CryTest, Vector2) {
    {
        Vector2f v;
        ASSERT_FLOAT_EQ(v.getX(), 0.0f);
        ASSERT_FLOAT_EQ(v.getY(), 0.0f);

        ASSERT_FLOAT_EQ(v.getLength(), 0.0f);
    }

    {
        Vector2f v(1.0f, 2.0f);
        ASSERT_FLOAT_EQ(v.getX(), 1.0f);
        ASSERT_FLOAT_EQ(v.getY(), 2.0f);

        ASSERT_FLOAT_EQ(v.getLength(), sqrtf(5.0f));

        auto normalized = v.normalize();
        ASSERT_FLOAT_EQ(normalized.getX(), 1.0f / sqrtf(5.0f));
        ASSERT_FLOAT_EQ(normalized.getY(), 2.0f / sqrtf(5.0f));
    }

    {
        // Test operator+
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        Vector2f v3 = v1 + v2;
        ASSERT_FLOAT_EQ(v3.getX(), 4.0f);
        ASSERT_FLOAT_EQ(v3.getY(), 6.0f);

        // Test operator+ with scalar
        Vector2f v4 = v1 + 2.0f;
        ASSERT_FLOAT_EQ(v4.getX(), 3.0f);
        ASSERT_FLOAT_EQ(v4.getY(), 4.0f);
    }

    {
        // Test operator-
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        Vector2f v3 = v1 - v2;
        ASSERT_FLOAT_EQ(v3.getX(), -2.0f);
        ASSERT_FLOAT_EQ(v3.getY(), -2.0f);

        // Test operator- with scalar
        Vector2f v4 = v1 - 2.0f;
        ASSERT_FLOAT_EQ(v4.getX(), -1.0f);
        ASSERT_FLOAT_EQ(v4.getY(), 0.0f);
    }

    {
        // Test operator*
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        Vector2f v3 = v1 * v2;
        ASSERT_FLOAT_EQ(v3.getX(), 3.0f);
        ASSERT_FLOAT_EQ(v3.getY(), 8.0f);

        // Test operator* with scalar
        Vector2f v4 = v1 * 2.0f;
        ASSERT_FLOAT_EQ(v4.getX(), 2.0f);
        ASSERT_FLOAT_EQ(v4.getY(), 4.0f);
    }

    {
        // Test operator/
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        Vector2f v3 = v1 / v2;
        ASSERT_FLOAT_EQ(v3.getX(), 1.0f / 3.0f);
        ASSERT_FLOAT_EQ(v3.getY(), 2.0f / 4.0f);

        // Test operator/ with scalar
        Vector2f v4 = v1 / 2.0f;
        ASSERT_FLOAT_EQ(v4.getX(), 1.0f / 2.0f);
        ASSERT_FLOAT_EQ(v4.getY(), 2.0f / 2.0f);
    }

    {
        // Test operator==
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        Vector2f v3(1.0f, 2.0f);
        ASSERT_TRUE(v1 == v3);
        ASSERT_FALSE(v1 == v2);

        // Test operator!=
        ASSERT_FALSE(v1 != v3);
        ASSERT_TRUE(v1 != v2);
    }

    {
        // Test operator+=
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        v1 += v2;
        ASSERT_FLOAT_EQ(v1.getX(), 4.0f);
        ASSERT_FLOAT_EQ(v1.getY(), 6.0f);
    }

    {
        // Test operator-=
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        v1 -= v2;
        ASSERT_FLOAT_EQ(v1.getX(), -2.0f);
    }

    {
        // Test operator*=
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        v1 *= v2;
        ASSERT_FLOAT_EQ(v1.getX(), 3.0f);
        ASSERT_FLOAT_EQ(v1.getY(), 8.0f);

        // Test operator*= with scalar
        v1 *= 2.0f;
        ASSERT_FLOAT_EQ(v1.getX(), 6.0f);
        ASSERT_FLOAT_EQ(v1.getY(), 16.0f);
    }

    {
        // Test operator/=
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        v1 /= v2;
        ASSERT_FLOAT_EQ(v1.getX(), 1.0f / 3.0f);
        ASSERT_FLOAT_EQ(v1.getY(), 2.0f / 4.0f);

        // Test operator/= with scalar
        v1 = Vector2f{1.0f, 2.0f};
        v1 /= 2.0f;
        ASSERT_FLOAT_EQ(v1.getX(), 1.0f / 2.0f);
        ASSERT_FLOAT_EQ(v1.getY(), 2.0f / 2.0f);
    }

    {
        // Test Dot Product
        Vector2f v1(1.0f, 2.0f);
        Vector2f v2(3.0f, 4.0f);
        ASSERT_FLOAT_EQ(v1.dot(v2), 11.0f);
    }

}

TEST(CryTest, Vector3) {
    {
        Vector3f v;
        ASSERT_FLOAT_EQ(v.getX(), 0.0f);
        ASSERT_FLOAT_EQ(v.getY(), 0.0f);
        ASSERT_FLOAT_EQ(v.getZ(), 0.0f);
    }

    {
        // Test Dot Product
        Vector3f v1(1.0f, 2.0f, 3.0f);
        Vector3f v2(3.0f, 4.0f, 5.0f);
        ASSERT_FLOAT_EQ(v1.dot(v2), 26.0f);
    }

    {
        // Test Cross Product
        Vector3f v1(1.0f, 2.0f, 3.0f);
        Vector3f v2(3.0f, 4.0f, 5.0f);
        Vector3f v3 = v1.cross(v2);
        ASSERT_FLOAT_EQ(v3.getX(), -2.0f);
        ASSERT_FLOAT_EQ(v3.getY(), 4.0f);
        ASSERT_FLOAT_EQ(v3.getZ(), -2.0f);
    }
}