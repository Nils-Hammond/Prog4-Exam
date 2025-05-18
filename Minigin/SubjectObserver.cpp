#include "SubjectObserver.h"

void dae::Subject::AddObserver(Observer* observer)
{
	m_Observers.emplace_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void dae::Subject::Notify(Event event)
{
	for (auto observer : m_Observers)
	{
		observer->OnNotify(event);
	}
}
