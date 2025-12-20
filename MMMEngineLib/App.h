#pragma once
#include <Windows.h>
#include <string>

#include "Delegates.h"

namespace MMMEngine
{
	class App
	{
	public:
		App();
		~App();
		bool Initialize();
		void Run();
		void Shutdown();

		Event<App, void(void)> onIntialize;
		Event<App, void(void)> onShutdown;
		Event<App, void(void)> onUpdate;
		Event<App, void(void)> onRender;
		Event<App, void(void)> onResize;

	protected:
		LRESULT HandleWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		std::wstring m_windowTitle;

		bool CreateMainWindow();
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}