#pragma once
#include "rttr/type"
#include "GUID.h"

namespace MMMEngine
{
	class Object
	{
	private:
		RTTR_ENABLE()
	private:
		static uint64_t s_nextInstanceID;
		uint64_t		m_instanceID;
		GUID			m_guid;
		bool			m_isDestroyed = false;

		void MarkDestory() { m_isDestroyed = true; }
	public:
		std::string m_name;

		Object() : m_instanceID(s_nextInstanceID++)
		{
			m_name = "<Unnamed> [ Instance ID : " + std::to_string(m_instanceID) + " ]";
			m_guid = GUID::NewGuid();
		}

		virtual ~Object() = default;

	public:
		inline uint64_t				GetInstanceID() const { return m_instanceID; }
		inline const std::string&	GetName()		const { return m_name; }

		inline bool					IsDestroyed()	const { return m_isDestroyed; }

		inline void					SetName(const std::string& name) { m_name = name; }
	};

}