#pragma once
#include <string>
#include "glm.hpp"
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
}