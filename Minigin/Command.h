#pragma once
#include <string>
#include "SubjectObserver.h"
namespace dae
{
	class GameObject;
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class TestCommand : public Command
	{
		std::string m_testText;
	public:
		TestCommand(const std::string& testText);
		void Execute() override;
	};
	
	class TriggerEventCommand : public Command, public Subject
	{
		Event m_event;
	public:
		TriggerEventCommand(Event m_event);
		void Execute() override;
	};
}