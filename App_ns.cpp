#include "App_ns.h"

CApp::CApp() {
	using std::wstring;
	using std::string;

	try {
		this->init_native_window_obj();
		this->create_native_controls();
	}
	catch(const std::exception& e){
		string expt_data = e.what();
		MessageBox(nullptr, wstring(begin(expt_data), 
		end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}

int CApp::Run() {
	MSG msg{};
	ShowWindow(this->m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(this->m_hwnd);
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CApp::application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	CApp* pApp;
	if (uMsg == WM_NCCREATE) {
		pApp = static_cast<CApp*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp))) {
			if (GetLastError() != 0)
				return false;
		}

	}
	else {
		pApp = reinterpret_cast<CApp*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pApp) {
		pApp->m_hwnd = hwnd;
		return pApp->window_proc(hwnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CApp::window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_COMMAND:
	{
		switch (static_cast<CApp::CTL_ID>(LOWORD(wParam)))
		{
		case CApp::CTL_ID::CALCBUTTON_ID:
			{
			try {
				std::wstring text{};
				text.resize(MAX_PATH);
				GetWindowText(this->m_hwndEdit, &text[0], MAX_PATH);

				if (text.empty()) {
					MessageBox(this->m_hwndEdit, L"Введите число!", L"Информация", MB_ICONINFORMATION | MB_OK);
					break;
				}

				
				std::string bynary = std::bitset<16>(std::stoi(text)).to_string();
				text = std::wstring(bynary.begin(), bynary.end());
				SetWindowText(this->m_hwndEdit, text.c_str()); //выводит текст в поле ввода-вывода
			}
			catch (...) {
				MessageBox(this->m_hwnd, L"Число превышает 2 байта!", L"Предупреждение", MB_ICONWARNING | MB_OK);
				}
			}
			break;
		}
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void CApp::create_native_controls() {
	this->m_hwndButton = CreateWindowEx(0, L"BUTTON", L"Перевести", 
	WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, 56, 108, 238, 37, 
	this->m_hwnd, reinterpret_cast<HMENU>(CApp::CTL_ID::CALCBUTTON_ID),
	nullptr, nullptr);

	if (!this->m_hwndButton)
		throw std::runtime_error("Error, can't create button!");

	this->m_hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"2567",
	WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 56, 66, 238, 24, this->m_hwnd,
	reinterpret_cast<HMENU>(CApp::CTL_ID::RESULTEDIT_ID), nullptr, nullptr);
	if (!this->m_hwndEdit)
		throw std::runtime_error("Error, can't create edit!");

	HFONT hFont = CreateFont(18, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET,
	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto");
	SendMessage(this->m_hwndEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
	SendMessage(this->m_hwndButton, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
}

void CApp::init_native_window_obj() {
	//using std::runtime_error;
	//using namespace std::string_literals;

	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(nullptr); //GetModuleHandle возвращает дескриптор файла, используемого для создания вызывающего процесса.
	wc.lpfnWndProc = CApp::application_proc;
	wc.lpszClassName = this->m_szClassName.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
		throw std::runtime_error("Error, can't register main window class!");

	RECT windowRC{ 0, 0, this->m_nAppWindth, this->m_nAppHeight };
	AdjustWindowRect(&windowRC, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE, false);

	this->m_hwnd = CreateWindowEx(0, this->m_szClassName.c_str(),
		this->m_szAppName.c_str(), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		(GetSystemMetrics(SM_CXSCREEN) - windowRC.right) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowRC.bottom) / 2,
		windowRC.right, windowRC.bottom, nullptr, nullptr, nullptr, this);

	if (!this->m_hwnd)
		throw std::runtime_error("Error, can't create main window!");

}
