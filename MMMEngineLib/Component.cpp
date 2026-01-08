#include "Component.h"
#include "GameObject.h"

void MMMEngine::Component::BeforeDestroy()
{
	GetGameObject()->UnRegisterComponent(SelfPtr(this));
}
