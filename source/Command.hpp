#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

#include <exception>
#include <memory>

#include <math.h>

static constexpr double DPI = 3.1415926535897932;
static constexpr double D0o5PI = 3.1415926535897932 / 2;
static constexpr double D2o0PI = 2 * 3.1415926535897932;

static constexpr double RAD2DEG = (180 / DPI);
static constexpr double DEG2RAD = (DPI / 180);
static constexpr double rad2deg(const double r) { return r * RAD2DEG; }
static constexpr double deg2rad(const double d) { return d * DEG2RAD; }

// interface class of command
class iCommand
{
public:
	virtual ~iCommand() = default;

	virtual void Execute() = 0;
	virtual const char* Type() = 0;
};


class cVector
{
public:
    using T = double;
    using data_type = double;

    cVector() = default;
    cVector(const cVector& other) = default;
    cVector(T x_, T y_) : x(x_), y(y_) {}

    template< typename S >
    cVector(const S& other) : x(data_type(other.x)),
        y(data_type(other.y))
    {
    }

    cVector& operator=(const cVector& rhs) = default;
    cVector& operator+=(const cVector& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    cVector& operator-=(const cVector& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    cVector& reverse() { x = -x; y = -y; return *this; };
    const cVector operator-() const { return cVector(-x, -y); }
    const cVector operator+(const cVector& other) const { return cVector(x + other.x, y + other.y); }
    const cVector operator-(const cVector& other) const { return cVector(x - other.x, y - other.y); }

    bool operator==(const cVector& rhs) const { return (x == rhs.x) && (y == rhs.y); }
    bool operator!=(const cVector& rhs) const { return (x != rhs.x) || (y != rhs.y); }

    static double dot(const cVector& lhv, const cVector& rhv) { return lhv.x * rhv.x + lhv.y * rhv.y; }
    double LengthSqr() const { return dot(*this, *this);  }
    double Length() const { return sqrt(LengthSqr()); }


public:
    T x, y;
};

// base class of exception used in task. It has name.
class cException : public std::exception
{
public:
    cException(const char* sz) : szWhat(sz) {}

    const char* what() const noexcept { return szWhat; }

protected:
    const char* szWhat;
};

class iMovable //something capable to move in a straight line
{
public:
    virtual ~iMovable() {}

    // access
    virtual const cVector& Position() const = 0;
    virtual const cVector& Velocity() const = 0;

    // change
    virtual void Position(const cVector& pos) = 0;
    virtual void Velocity(const cVector& vel) = 0;
};

class cMovable : public iMovable
{
public:
    cMovable(const cVector& pos, const cVector& vel) : position(pos), velocity(vel) {}

    // access
    const cVector& Position() const override { return position; }
    const cVector& Velocity() const override { return velocity; }

    // change
    void Position(const cVector& pos) override { position = pos; }
    void Velocity(const cVector& vel) override { velocity = vel; }

protected:
    cVector position, velocity;
};

class cMove  : public iCommand // class performing moving along stright line
{
private:
    cMove(const cMove&) = delete;
    cMove& operator=(const cMove&) = delete;

public:
    cMove(iMovable& m_) : m(m_) {}

public:
    void Execute() override
    {
        // new position as old position + velocity.
        m.Position(m.Position() + m.Velocity());
    }

    const char* Type() override { return "Move"; }

protected:
    iMovable& m;
};


class iRotatable //interface class of something capable to rotate around a axis
{
public:
    virtual ~iRotatable() {}

    // access
    virtual double AngularVelocity() const = 0;
    virtual double Direction() const = 0;

    // change
    virtual void AngularVelocity(double val) = 0;
    virtual void Direction(double val) = 0;
};

class cRotatable : public iRotatable
{
public:
    cRotatable(double dir, double vel) : direction(dir), angularVelocity(vel) {}
    // access
    double AngularVelocity() const override { return angularVelocity; }
    double Direction() const override { return direction; }

    // change
    void AngularVelocity(double val) override { angularVelocity = val; }
    void Direction(double val) override { direction = val; }

protected:
    double direction, angularVelocity; // degrees 
};


class cRotate : public iCommand //class to perform rotation
{
private:
    cRotate(const cRotate&) = delete;
    cRotate& operator=(const cRotate&) = delete;

public:
    cRotate(iRotatable& m_) : m(m_) {}

public:
    void Execute() override
    {
        m.Direction(m.Direction() + m.AngularVelocity());
    }

    const char* Type() override { return "Rotate"; }

public:
    iRotatable& m;
};

class iFuelTank
{
public:
    virtual double Fuel() const = 0;
    virtual void Fuel( double f ) = 0;
};

class cFuelTank : public iFuelTank
{
public:
    cFuelTank(double f) : fuel(f) {}

    virtual double Fuel() const override { return fuel;  }
    virtual void Fuel(double f) override { fuel = f; }

protected:
    double fuel;
};

class iFuelConsumableOperation //something capable to ñonsume fuel
{
public:
    virtual ~iFuelConsumableOperation() {}

    // access
    virtual const double Fuel() const = 0;
};

class cFuelConsumableOperation : public iFuelConsumableOperation
{
public:
    cFuelConsumableOperation(double f) : fuel(f) {}

    // access
    virtual const double Fuel() const { return fuel;  }

protected:
    double fuel;
};


class cCheckFuelCommand : public iCommand // class check fuel consuming
{
public:
    cCheckFuelCommand(const iFuelConsumableOperation& o, const iFuelTank &t) : op(&o), tank(&t) {}

public:
    void Execute() override
    {
        // new fuel amount = preious - operation cost should be >= 0
        if ((tank->Fuel() - op->Fuel() ) < 0)
            throw( cException("Fuel is not enough"));
    }

    const char* Type() override { return "Check fuel"; }

protected:
    const iFuelConsumableOperation* op;
    const iFuelTank* tank;
};


class cBurnFuelCommand : public iCommand // class burn fuel consuming
{
public:
    cBurnFuelCommand(const iFuelConsumableOperation& o, iFuelTank& t) : op(&o), tank(&t) {}

public:
    void Execute() override
    {
        // new fuel amount = preious - operation cost.
        tank->Fuel(tank->Fuel() - op->Fuel());
    }

    const char* Type() override { return "Burn fuel"; }

protected:
    const iFuelConsumableOperation* op;
    iFuelTank* tank;
};

class cMacroCommand : public iCommand
{
public:
    cMacroCommand();
    cMacroCommand(iCommand& a);
    cMacroCommand(iCommand& a, iCommand& b);
    cMacroCommand(iCommand& a, iCommand& b, iCommand& c);

    void Execute() override;
    const char* Type() override { return type.c_str(); }

protected:    
    std::vector<iCommand *> commands;
    std::string type;
};

// base class exception thrown by command execution
class cCommandException : public cException
{
public:
    cCommandException(const char* sz);
    cCommandException(const char *sz, const std::exception &e);

    const char* what() const noexcept { return sWhat.c_str(); }

protected:
    std::string sWhat;
};


// create commands depended on input parameters.
// all create commands will be stored as shared ptr in static container and 
// to avoid provlems with memory allocation.
class cCommandsFactory
{
public:
    static std::vector< std::unique_ptr<iCommand> > storage;

    static iCommand* push_back(iCommand* cmd)
    {
        storage.push_back(std::unique_ptr<iCommand>(cmd));
        return cmd;
    }

    template<typename COMMAND_TYPE> 
    static COMMAND_TYPE* Create()
    {
        return push_back(new COMMAND_TYPE);
    }

    template<typename COMMAND_TYPE, typename ARG_TYPE1 > 
    static iCommand* Create(ARG_TYPE1 &arg1 )
    {
        return push_back(new COMMAND_TYPE(arg1));
    }

    template<typename COMMAND_TYPE, typename ARG_TYPE1, typename ARG_TYPE2 > 
    static iCommand* Create(ARG_TYPE1& arg1, ARG_TYPE2& arg2 )
    {
        return push_back(new COMMAND_TYPE(arg1, arg2));
    }

    template<typename COMMAND_TYPE, typename ARG_TYPE1, typename ARG_TYPE2, typename ARG_TYPE3 >
    static iCommand* Create(ARG_TYPE1& arg1, ARG_TYPE2& arg2, ARG_TYPE3& arg3)
    {
        return push_back(new COMMAND_TYPE(arg1, arg2, arg3));
    }
};


class cMoveWithFuelConsumation : public cMacroCommand
{
public:
    cMoveWithFuelConsumation(iMovable &m, iFuelConsumableOperation& o, iFuelTank& t) :
        cMacroCommand(
            *cCommandsFactory::Create<cCheckFuelCommand, iFuelConsumableOperation, iFuelTank>(o,t),
            *cCommandsFactory::Create<cMove, iMovable>(m),
            *cCommandsFactory::Create<cBurnFuelCommand, iFuelConsumableOperation, iFuelTank>(o, t))
    {}
};

class cChangeVelocityCommand : public iCommand
{
public:    
    cChangeVelocityCommand(iMovable& m_, const cVector &vel_) : m(&m_), vel(vel_) {}

    void Execute() override;

    const char* Type() override { return "Change velocity"; }

protected:
    iMovable *m;
    cVector vel;
};


class cRotateMovable : public iCommand
{
public:
  cRotateMovable(iRotatable& r_, iMovable& m_);

public:
    void Execute() override;
    const char* Type() override { return "Rotate a movable object"; }

public:
    iRotatable *r;
    iMovable *m;
};



#endif //#ifndef COMMAND_HPP
