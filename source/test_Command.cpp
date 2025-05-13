
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

// gTest grouping class
class test_cMacroCommand : public ::testing::Test
{
public:
    // additional class to access to member of tested class
    class Test_cMacroCommand : public cMacroCommand
    {
    public:
        // add here members for free access.
        using cMacroCommand::cMacroCommand; // delegate constructors
        using cMacroCommand::commands;
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


TEST_F(test_cMacroCommand, test_ctor)
{
    {
        Test_cMacroCommand t;

        EXPECT_EQ(0, t.commands.size());
        ASSERT_STREQ("Macro command:", t.Type());
    }

    {
        cFuelConsumableOperation t1(22);
        cFuelTank t2(21.9);

        test_cBurnFuelCommand::Test_cBurnFuelCommand t3(t1, t2);
        test_cCheckFuelCommand::Test_cCheckFuelCommand t4(t1, t2);

        Test_cMacroCommand t(t3,t4);

        EXPECT_EQ(2, t.commands.size());
        ASSERT_STREQ("Macro command:Burn fuel,Check fuel", t.Type());
    }
}


TEST_F(test_cMacroCommand, test_Execute)
{
    {
        cFuelConsumableOperation t1(22);
        cFuelTank t2(21.9);

        test_cCheckFuelCommand::Test_cCheckFuelCommand t3(t1, t2);
        test_cBurnFuelCommand::Test_cBurnFuelCommand t4(t1, t2);

        Test_cMacroCommand t(t3, t4);

        try
        { 
            t.Execute();
        }
        catch (const cCommandException& e)
        {
            ASSERT_STREQ("Macro command,Fuel is not enough", e.what());
        }
    }

    {
        cFuelConsumableOperation t1(22);
        cFuelTank t2(24);

        test_cCheckFuelCommand::Test_cCheckFuelCommand t3(t1, t2);
        test_cBurnFuelCommand::Test_cBurnFuelCommand t4(t1, t2);

        Test_cMacroCommand t(t3, t4);

        try
        {
            t.Execute();
            EXPECT_EQ(2, t2.Fuel());
        }
        catch (const cCommandException& )
        {
            FAIL() << "No exception expected";
        }
    }

}
