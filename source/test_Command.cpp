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

// additional class to access to member of tested class
class Test_cSpaceShip
{
public:
    cMovable m;
    cFuelTank t;
    cFuelConsumableOperation op;
    
public:
    // add here members for free access.
    Test_cSpaceShip(const cVector& pos, const cVector& vel, double f, double operaionCost ) :
        m(pos, vel), t(f), op( operaionCost)
    {
    }
};

class cNoMovable : public cMovable
{
public:
  cNoMovable() : cMovable(cVector(), cVector()) {}
  void Velocity(const cVector& vel) override
  {
    throw cException("No moving object");
  }
};



// gTest grouping class
class test_cMoveWithFuelConsumation : public ::testing::Test
{
public:
    // additional class to access to member of tested class
    class Test_cMoveWithFuelConsumation : public cMoveWithFuelConsumation
    {
    public:
        // add here members for free access.
        using cMoveWithFuelConsumation::cMoveWithFuelConsumation; // delegate constructors
        using cMacroCommand::commands;
    };
};

// gTest grouping class
class test_cChangeVelocityCommand : public ::testing::Test
{
public:
    // additional class to access to member of tested class
    class Test_cChangeVelocityCommand : public cChangeVelocityCommand
    {
    public:
        // add here members for free access.
        using cChangeVelocityCommand::cChangeVelocityCommand; // delegate constructors
    };
};


// gTest grouping class
class test_cRotateMovable : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cRotateMovable : public cRotateMovable
  {
  public:
    // add here members for free access.
    using cRotateMovable::cRotateMovable; // delegate constructors
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


TEST_F(test_cMoveWithFuelConsumation, test_Execute)
{
    Test_cSpaceShip t0(cVector(12, 5), cVector(-7, 3), 22, 10 );
    Test_cMoveWithFuelConsumation t(t0.m, t0.op, t0.t);

    t.Execute();

    EXPECT_EQ(cVector(5, 8), t0.m.Position());
    EXPECT_EQ(12, t0.t.Fuel());
}

TEST_F(test_cMoveWithFuelConsumation, test_Execute_NoEnoughFuel)
{
    Test_cSpaceShip t0(cVector(12, 5), cVector(-7, 3), 22, 24);
    Test_cMoveWithFuelConsumation t(t0.m, t0.op, t0.t);

    try
    {
        t.Execute();
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ("Macro command,Fuel is not enough", e.what());
        EXPECT_EQ(cVector(12, 5), t0.m.Position());
        EXPECT_EQ(22, t0.t.Fuel());
    }
}

TEST_F(test_cChangeVelocityCommand, test_Execute)
{
    Test_cSpaceShip t0(cVector(12, 5), cVector(-7, 3), 22, 10);
    cVector n(-6, 4);
    Test_cChangeVelocityCommand t(t0.m, n);

    t.Execute();

    EXPECT_EQ(cVector(12, 5), t0.m.Position());
    EXPECT_EQ(cVector(-6, 4), t0.m.Velocity());
}

TEST_F(test_cChangeVelocityCommand, test_Execute_throw_expection )
{
    cNoMovable m;
    cVector n(-6, 4);
    Test_cChangeVelocityCommand t(m, n);

    try
    {
        t.Execute();
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ("Change velocity command,No moving object", e.what());
    }
}

TEST_F(test_cRotateMovable, test_Execute)
{
  cMovable m(cVector(12, 5), cVector(3, 4));
  cRotatable r( 30, 15 + 90 );

  Test_cRotateMovable t(r, m);

  t.Execute();

  EXPECT_EQ(135, r.Direction());
  EXPECT_NEAR(-5.0/sqrt(2), m.Velocity().x, 1.0e-12);
  EXPECT_NEAR( 5.0/sqrt(2), m.Velocity().y, 1.0e-12);
}

TEST_F(test_cRotateMovable, test_Execute_trow_Exception )
{
  cNoMovable m;
  cRotatable r(30, 15 + 90);

  Test_cRotateMovable t(r, m);

  try
  {
    t.Execute();
  }
  catch (const std::exception& e)
  {
    ASSERT_STREQ("Change velocity command,No moving object", e.what());
  }
}
