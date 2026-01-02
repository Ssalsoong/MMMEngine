#pragma once
#include "rttr/type"
#include "rttr/registration_friend.h"
#include "GUID.h"

namespace MMMEngine
{
	class Object
	{
	private:
		friend class ObjectManager;

		template<typename T>
		friend class ObjectPtr;
		
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

		static uint64_t s_nextInstanceID;

		uint64_t		m_instanceID;
		std::string		m_name;
		GUID			m_guid;

		bool			m_isDestroyed = false;

		void MarkDestory() { m_isDestroyed = true; }

		inline void					SetGUID(const GUID& guid) { m_guid = guid; }
	protected:
		Object() : m_instanceID(s_nextInstanceID++)
		{
			m_name = "<Unnamed> [ Instance ID : " + std::to_string(m_instanceID) + " ]";
			m_guid = GUID::NewGuid();
		}
	public:

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;

		virtual ~Object() = default;

		inline uint64_t				GetInstanceID() const { return m_instanceID; }

		inline const GUID&			GetGUID()		const { return m_guid; }

		inline const std::string&	GetName()		const { return m_name; }
		inline void					SetName(const std::string& name) { m_name = name; }

		inline const bool&			IsDestroyed()	const { return m_isDestroyed; }
	};

}