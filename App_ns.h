#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <string>
#include <algorithm>
#include <exception>
#include <bitset>

class CApp {
enum class CTL_ID{ CALCBUTTON_ID, RESULTEDIT_ID };//контролы

public:
	explicit CApp();
	~CApp() {};
	int Run();

private:
	void init_native_window_obj();

	static LRESULT CALLBACK application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void create_native_controls();

private:
	const std::wstring m_szAppName{ L"Bin-VV" }, m_szClassName{ L"Bin-VV_Window_class_nostyles" };
	HWND m_hwnd{}, m_hwndButton{}, m_hwndEdit{};
	const int m_nAppWindth{ 350 }, m_nAppHeight{ 250 };
};