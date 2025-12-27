#include "App.h"

MMMEngine::App::App()
	: m_hWnd(NULL)
	, m_hInstance(NULL)
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
	onRender(this);
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
