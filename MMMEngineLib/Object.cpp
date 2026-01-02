#include "Object.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"

uint64_t MMMEngine::Object::s_nextInstanceID = 1;


RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<MMMEngine::Object>("Object")
		.constructor<>()
			(rttr::policy::ctor::as_raw_ptr)
		.property("Name", &MMMEngine::Object::GetName, &MMMEngine::Object::SetName)
		.property("GUID", &MMMEngine::Object::GetGUID, &MMMEngine::Object::SetGUID)
		.property_readonly("InstanceID", &MMMEngine::Object::GetInstanceID)
		.property_readonly("isDestroyed", &MMMEngine::Object::IsDestroyed);
}