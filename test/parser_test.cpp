#include <gtest/gtest.h>

#include "parser.h"

TEST(Parser, simpleTest) {
        int x=2;
        int y=2;
        EXPECT_EQ(add(x,y),x+y);
}

