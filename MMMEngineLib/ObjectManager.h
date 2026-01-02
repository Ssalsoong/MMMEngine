#pragma once
#include "Singleton.hpp"
#include "Object.h"
#include <vector>
#include <queue>

namespace MMMEngine
{
	template<typename T>
	class ObjectPtr
	{
	private:
		friend class ObjectManager;
		T* m_ptr;
		uint32_t m_handleID;
		uint32_t m_handleGeneration;
	public:
		T* Get() const
		{
			if (ObjectManager::Get()->IsValidObjectPtr(*this))
			{
				return m_ptr;
			}
			return nullptr;
		}

		T& operator*() const
		{
			return *Get();
		}

		T* operator->() const
		{
			return Get();
		}

		bool IsValid() const
		{
			return ObjectManager::Get()->IsValidObjectPtr(*this);
		}
	};

	class ObjectManager : public Singleton<ObjectManager>
	{
	private:
		friend class Object;

		template <typename T>
		friend class ObjectPtr;
		
		std::vector<Object*> m_objects;
		std::vector<uint32_t> m_handleGenerations;
		std::queue<uint32_t> m_freeHandleIDs;	

		void RegisterObject(Object* obj);
		void UnregisterObject(Object* obj);

		template<typename T>
		ObjectPtr<T> CreateObject()
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
			T* newObj = new T();
			RegisterObject(newObj);
			ObjectPtr<T> objPtr;
			objPtr.m_ptr = newObj;
			if (m_freeHandleIDs.empty())
			{
				m_objects.push_back(newObj);
				m_handleGenerations.push_back(0);
				objPtr.m_handleID = static_cast<uint32_t>(m_objects.size() - 1);
				objPtr.m_handleGeneration = m_handleGenerations[objPtr.m_handleID];
			}
			else
			{
				uint32_t freeID = m_freeHandleIDs.front();
				m_freeHandleIDs.pop();
				m_objects[freeID] = newObj;
				objPtr.m_handleID = freeID;
				objPtr.m_handleGeneration = ++m_handleGenerations[freeID];
			}
			return objPtr;
		}

		template<typename T>
		ObjectPtr<T> CreateObject(std::string name)
		{
			static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");
			T* newObj = new T();
			newObj->SetName(name);
			RegisterObject(newObj);
			ObjectPtr<T> objPtr;
			objPtr.m_ptr = newObj;

			if (m_freeHandleIDs.empty())
			{
				m_objects.push_back(newObj);
				m_handleGenerations.push_back(0);
				objPtr.m_handleID = static_cast<uint32_t>(m_objects.size() - 1);
				objPtr.m_handleGeneration = m_handleGenerations[objPtr.m_handleID];
			}
			else
			{
				uint32_t freeID = m_freeHandleIDs.front();
				m_freeHandleIDs.pop();
				m_objects[freeID] = newObj;
				objPtr.m_handleID = freeID;
				objPtr.m_handleGeneration = ++m_handleGenerations[freeID];
			}

			return objPtr;
		}

		template<typename T>
		void DestroyObject(ObjectPtr<typename T> obj)
		{

		}

	public:
		bool IsValidObjectPtr(const ObjectPtr<Object>& objPtr)
		{
			if (objPtr.m_handleID >= m_objects.size())
				return false;
			if (m_objects[objPtr.m_handleID] != objPtr.m_ptr)
				return false;
			if (m_handleGenerations[objPtr.m_handleID] != objPtr.m_handleGeneration)
				return false;
			if (objPtr.m_ptr->IsDestroyed())
				return false;
			return true;
		}

		ObjectManager();
		~ObjectManager();
	};
} 