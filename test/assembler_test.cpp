#include <string>
#include <gtest/gtest.h>

#include "ast.h"
#include "checker.h"


using namespace tua;


TEST(CheckerTest, simple_test) {
        auto visitor=new AssemblerVisitor();
        EXPECT_EQ(true,true);
}

