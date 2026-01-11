#define NOMINMAX
#include <iostream>

#include "GlobalRegistry.h"
#include "PlayerRegistry.h"
#include "App.h"

#include "InputManager.h"
#include "ResourceManager.h"

using namespace MMMEngine;
using namespace MMMEngine::Utility;

void Init()
{
	InputManager::Get().StartUp(MMMEngine::g_pApp->GetWindowHandle());

	ResourceManager::Get().SetResolver(&Player::g_resolver);
	ResourceManager::Get().SetBytesProvider(&Player::g_bytes);
}

void Update()
{
	InputManager::Get().Update();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	App app{L"MMMPlayer",1280,720};
	MMMEngine::g_pApp = &app;

	//if (!Player::g_pathMap.LoadFromFile(L"Data/PathMap.bin"))
	//	return -1;

	//if (!Player::g_assetIndex.LoadFromFile(L"Data/AssetIndex.bin"))
	//	return -2;

	//if (!Player::g_pak.Open(L"Data/assets.pak"))
	//	return -3;

	app.SetProcessHandle(hInstance); //winmain으로 진입하는 경우 hIntance물려주기
	app.OnInitialize.AddListener<&Init>();
	app.OnUpdate.AddListener<&Update>();
	app.Run();
}