
#include <gtest/gtest.h>

#include "Command.hpp"


// gTest grouping class
class test_cCheckFuelCommand : public ::testing::Test
{
public:
    // additional class to access to member of tested class
    class Test_cCheckFuelCommand : public cCheckFuelCommand
    {
    public:
        // add here members for free access.
        using cCheckFuelCommand::cCheckFuelCommand; // delegate constructors
        using cCheckFuelCommand::op;
        using cCheckFuelCommand::tank;
    };

};

TEST_F(test_cCheckFuelCommand, test_ctor)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(50);

    Test_cCheckFuelCommand t(t1, t2);

    EXPECT_EQ(&t1, t.op);
    EXPECT_EQ(&t2, t.tank);
}

TEST_F(test_cCheckFuelCommand, test_Execute_throws_no_exception)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(22.1);

    Test_cCheckFuelCommand t(t1, t2);

    try
    {
        t.Execute();
    }
    catch (const std::exception&)
    {
        FAIL() << "No exception expected";
    }
}


TEST_F(test_cCheckFuelCommand, test_Execute_throws_exception)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(21.9);

    Test_cCheckFuelCommand t(t1, t2);

    try
    {
        t.Execute();
    }
    catch (const std::exception& expected)
    {
        ASSERT_STREQ("Fuel is not enough", expected.what());
    }
}

TEST_F(test_cCheckFuelCommand, test_Type)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(21.9);

    Test_cCheckFuelCommand t(t1, t2);

    ASSERT_STREQ("Check fuel", t.Type());
}



// gTest grouping class
class test_cBurnFuelCommand : public ::testing::Test
{
public:
    // additional class to access to member of tested class
    class Test_cBurnFuelCommand : public cBurnFuelCommand
    {
    public:
        // add here members for free access.
        using cBurnFuelCommand::cBurnFuelCommand; // delegate constructors
        using cBurnFuelCommand::op;
        using cBurnFuelCommand::tank;
    };

};

TEST_F(test_cBurnFuelCommand, test_ctor)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(50);

    Test_cBurnFuelCommand t( t1, t2 );

    EXPECT_EQ(&t1, t.op);
    EXPECT_EQ(&t2, t.tank);
}

TEST_F(test_cBurnFuelCommand, test_Execute)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(23);

    Test_cBurnFuelCommand t(t1, t2);

    try
    {
        t.Execute();
        EXPECT_EQ(1.0, t2.Fuel());
    }
    catch (const std::exception& )
    {
        FAIL() << "No exception expected";
    }
}

TEST_F(test_cBurnFuelCommand, test_Type)
{
    cFuelConsumableOperation t1(22);
    cFuelTank t2(21.9);

    Test_cBurnFuelCommand t(t1, t2);

    ASSERT_STREQ("Burn fuel", t.Type());
}

