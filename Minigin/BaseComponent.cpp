#include "BaseComponent.h"

dae::BaseComponent::BaseComponent(std::weak_ptr<GameObject> parentObject)
	: _parentObjectPtr(parentObject)
{
}
