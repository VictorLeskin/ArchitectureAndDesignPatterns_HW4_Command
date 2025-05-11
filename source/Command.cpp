#include "Command.hpp"

cMacroCommand::cMacroCommand(iCommand* a, iCommand* b, iCommand* c) 
{
	type = "Macro command:";

	commands.push_back(a); type += a->Type();
	commands.push_back(b); type += ","; type += b->Type();
	commands.push_back(c); type += ","; type += c->Type();
}
