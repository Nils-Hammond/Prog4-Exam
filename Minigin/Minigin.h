#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath, int windowWidth, int windowHeight, const std::string& name);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}