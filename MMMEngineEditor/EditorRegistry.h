#pragma once
#include "EditorResolver.h"
#include "FileBytesProvider.h"
#include "ImporterRegistry.h"

namespace MMMEngine::Editor
{
	inline static MetaDB g_db;
	inline static EditorResolver g_resolver{ &g_db };
	inline static FileBytesProvider g_filesProvider;
	inline static ImporterRegistry g_importerRegistry;
}

