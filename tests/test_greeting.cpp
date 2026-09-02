#include <gtest/gtest.h>

#include "learn_opengl/learn_opengl.hpp"

TEST(Greeting, IsNotEmpty)
{
    EXPECT_FALSE(learn_opengl::greeting().empty());
}
