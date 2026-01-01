#pragma once
#include "rttr/type.h"
#include "json/json.hpp"

using namespace nlohmann::json_abi_v3_12_0;

#include <type_traits>

using namespace rttr;

namespace MMMEngine
{
	class Object;
	class JsonSerializer
	{
	private:
		std::optional<json> SerializeVariant(const variant& var);
		bool DeserializeVariant(const json& j, variant& var);
	public:
		json Serialize(const Object& obj);
		bool Deserialize(const json& j, Object& obj);
	};
}
