#pragma once
#include <vector>
#include <any>

namespace dae
{
	class GameObject;

	template <int length> struct sdbm_hash
	{
		constexpr static unsigned int _calculate(const char* const text, unsigned int& value) {
			const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
			value = character + (value << 6) + (value << 16) - value;
			return text[length - 1];
		}
		constexpr static unsigned int calculate(const char* const text) {
			unsigned int value = 0;
			const auto character = _calculate(text, value);
			return character + (value << 6) + (value << 16) - value;
		}
	};
	template <> struct sdbm_hash<1> {
		constexpr static int _calculate(const char* const text, unsigned int&) { return text[0]; }
	};
	template <size_t N> constexpr unsigned int make_sdbm_hash(const char(&text)[N]) {
		return sdbm_hash<N - 1>::calculate(text);
	}

	using EventId = unsigned int;

	struct Event
	{
		const EventId id;
		std::any data;
		explicit Event(EventId _id, std::any _data = {}) : id{ _id }, data{ std::move(_data) } {};
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event) = 0;
	};

	class Subject
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		void Notify(Event event);
	private:
		std::vector<Observer*> m_Observers{};
	};
}
