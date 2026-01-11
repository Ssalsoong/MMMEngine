#pragma once
#include "MUID.h"
#include "rttr/type"
#include "rttr/registration_friend.h"

namespace MMMEngine
{
	class Resource
	{
	private:
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
		friend class ResourceManager;

		Utility::MUID m_muid;

		inline void SetMUID(Utility::MUID muid) { m_muid = muid; }
	protected:
		virtual bool LoadFromBytes(const void* data, size_t size) = 0;
	public:
		virtual ~Resource() = default;

		inline const Utility::MUID& GetMUID() const { return m_muid; }
	};
}