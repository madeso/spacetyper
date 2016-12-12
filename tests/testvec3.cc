#include "gtest/gtest.h"
#include "spacetyper/vec3.h"

#define GTEST(X) TEST(vec3, X)

GTEST(constructor_same) {
  const auto v = vec3i(42);
  EXPECT_EQ(42, v.x);
  EXPECT_EQ(42, v.y);
  EXPECT_EQ(42, v.z);
}

GTEST(constructor_unique) {
  const auto v = vec3i(1, 2, 3);
  EXPECT_EQ(1, v.x);
  EXPECT_EQ(2, v.y);
  EXPECT_EQ(3, v.z);
}

GTEST(constructor_vec2) {
  const auto v = vec3i(vec2i(1, 2), 3);
  EXPECT_EQ(1, v.x);
  EXPECT_EQ(2, v.y);
  EXPECT_EQ(3, v.z);
}

GTEST(equal) {
  EXPECT_TRUE(vec3i(1, 2, 3) == vec3i(1, 2, 3));
  EXPECT_FALSE(vec3i(1, 2, 3) == vec3i(3, 2, 1));
}