#include "ObjectManager.h"

void MMMEngine::ObjectManager::ProcessPendingDestroy()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    DestroyScope scope;

    for (uint32_t ptrID : m_pendingDestroy)
    {
        if (ptrID >= m_objects.size())
            continue;

        Object* obj = m_objects[ptrID];
        if (!obj)
            continue;

        delete obj;
        m_objects[ptrID] = nullptr;
        m_freePtrIDs.push(ptrID);
    }

    m_pendingDestroy.clear();
}

size_t MMMEngine::ObjectManager::GetObjectCount() const
{
    size_t count = 0;
    for (const Object* obj : m_objects)
        if (obj && !obj->IsDestroyed())
            count++;
    return count;
}

bool MMMEngine::ObjectManager::IsCreatingObject() const
{
    return m_isCreatingObject;
}

bool MMMEngine::ObjectManager::IsDestroyingObject() const
{
    return m_isDestroyingObject;
}

bool MMMEngine::ObjectManager::IsValidPtr(uint32_t ptrID, uint32_t generation, const Object* ptr) const
{
    if (ptrID >= m_objects.size())
        return false;

    if (m_objects[ptrID] != ptr)
        return false;

    if (m_ptrGenerations[ptrID] != generation)
        return false;

    if (ptr && ptr->IsDestroyed())
        return false;

    return true;
}

MMMEngine::ObjectManager::~ObjectManager()
{
    DestroyScope scope;
    // 모든 객체 정리
    for (Object* obj : m_objects)
    {
        if (obj)
        {
            delete obj;
        }
    }
}
