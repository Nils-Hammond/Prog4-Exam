#include "Command.h"
//#include "MoveComponent.h"
//#include "LivesAndPoints.h"
#include <iostream>

dae::TestCommand::TestCommand(const std::string& testText)
	: m_testText{testText}
{
}

void dae::TestCommand::Execute()
{
	std::cout << m_testText << std::endl;
}

dae::TriggerEventCommand::TriggerEventCommand(Event event) :
	m_event{ event }
{
}

void dae::TriggerEventCommand::Execute()
{
	Subject::Notify(m_event);
}
