#pragma once
#include <string>
#include "glm.hpp"
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

	class GameActorCommand : public Command
	{
		GameObject* m_actor;
	protected:
		GameObject* GetGameActor() const { return m_actor; }
	public:
		GameActorCommand(GameObject* actor);
		virtual ~GameActorCommand() = default;
	};

	class MoveCommand : public GameActorCommand
	{
		glm::vec3 m_direction;
	public:
		MoveCommand(GameObject* actor, glm::vec3 direction);
		void Execute() override;
	};

	class DieCommand : public GameActorCommand
	{	
	public:
		DieCommand(GameObject* actor);
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