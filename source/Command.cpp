#include "Command.hpp"

std::vector< std::unique_ptr<iCommand> > cCommandsFactory::storage;

cMacroCommand::cMacroCommand()
{
	type = "Macro command:";
}

cMacroCommand::cMacroCommand(iCommand& a)
{
	type = "Macro command:";

	commands.push_back(&a); type += a.Type();
}

cMacroCommand::cMacroCommand(iCommand& a, iCommand& b)
{
	type = "Macro command:";

	commands.push_back(&a); type += a.Type();
	commands.push_back(&b); type += ","; type += b.Type();
}

cMacroCommand::cMacroCommand(iCommand &a, iCommand& b, iCommand& c)
{
	type = "Macro command:";

	commands.push_back(&a); type += a.Type();
	commands.push_back(&b); type += ","; type += b.Type();
	commands.push_back(&c); type += ","; type += c.Type();
}

void cMacroCommand::Execute()
{
	try
	{
		for (int i = 0; i < commands.size(); ++i)
			commands[i]->Execute();
	}
	catch (const std::exception& e)
	{
		throw cCommandException("Macro command", e);
	}
}

cCommandException::cCommandException(const char* sz) : cException(nullptr)
{
	sWhat = sz;
}

cCommandException::cCommandException(const char *sz, const std::exception& e) : cException(nullptr) 
{
	sWhat = sz;
	sWhat += ",";
	sWhat += e.what();
}

void cChangeVelocityCommand::Execute()
{
	try
	{
		m->Velocity(vel);
	}
	catch (const std::exception& e)
	{
		throw cCommandException("Change velocity command", e);
	}
}

void cRotateMovable::Execute()
{
	cVector v;
	cMacroCommand m(
		*cCommandsFactory::Create<cRotate, iRotatable>(r),
		*cCommandsFactory::Create<cChangeVelocityCommand, iMovable, cVector>(m, m->Velocity()));

}
