#include "App.h"

MMMEngine::App::App()
	: m_hWnd(nullptr)
	, onIntialize(this)
	, onRender(this)
	, onResize(this)
	, onShutdown(this)
	, onUpdate(this)
{
	m_windowTitle = L"MMM Engine Application";
}

MMMEngine::App::~App()
{
}

bool MMMEngine::App::Initialize()
{
	return false;
}

void MMMEngine::App::Run()
{
}

void MMMEngine::App::Shutdown()
{
}

LRESULT MMMEngine::App::HandleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

bool MMMEngine::App::CreateMainWindow()
{

	return false;
}

LRESULT MMMEngine::App::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
