#define NOMINMAX
#include <iostream>

#include "GlobalRegistry.h"
#include "EditorRegistry.h"
#include "App.h"

#include "InputManager.h"
#include "ResourceManager.h"

using namespace MMMEngine;
using namespace MMMEngine::Utility;

void Init()
{
	InputManager::Get().StartUp(MMMEngine::g_pApp->GetWindowHandle());

	ResourceManager::Get().SetResolver(&Editor::g_resolver);
	ResourceManager::Get().SetBytesProvider(&Editor::g_filesProvider);
}

void Update()
{
	InputManager::Get().Update();
}

int main()
{
	App app;
	MMMEngine::g_pApp = &app;

	app.SetProcessHandle(GetModuleHandle(NULL)); //winmain으로 진입하는 경우 hIntance물려주기
	app.OnInitialize.AddListener<&Init>();
	app.OnUpdate.AddListener<&Update>();
	app.Run();
}