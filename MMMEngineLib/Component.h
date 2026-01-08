#pragma once
#include "Object.h"
#include "rttr/type"

namespace MMMEngine
{
	class Component : public Object
	{
	private:
		RTTR_ENABLE(Object)
		RTTR_REGISTRATION_FRIEND
		friend class App;
		friend class ObjectManager;
		friend class GameObject;

		ObjPtr<GameObject> m_gameObject;
		inline void SetGameObject(ObjPtr<GameObject> owner) { m_gameObject = owner; }
	protected:
		Component() = default;
		virtual void Initialize() {};
		virtual void BeforeDestroy() override;
	public:
		virtual ~Component() = default;

		inline ObjPtr<GameObject> GetGameObject() { return m_gameObject; };
	};
}