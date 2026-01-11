#include "InputManager.h"
#include "TimeManager.h"

void MMMEngine::InputManager::InitKeyCodeMap()
{
    // KeyCode와 Windows Virtual Key Code 매핑
// 알파벳
    m_keyCodeMap.emplace(KeyCode::A, 'A');
    m_keyCodeMap.emplace(KeyCode::B, 'B');
    m_keyCodeMap.emplace(KeyCode::C, 'C');
    m_keyCodeMap.emplace(KeyCode::D, 'D');
    m_keyCodeMap.emplace(KeyCode::E, 'E');
    m_keyCodeMap.emplace(KeyCode::F, 'F');
    m_keyCodeMap.emplace(KeyCode::G, 'G');
    m_keyCodeMap.emplace(KeyCode::H, 'H');
    m_keyCodeMap.emplace(KeyCode::I, 'I');
    m_keyCodeMap.emplace(KeyCode::J, 'J');
    m_keyCodeMap.emplace(KeyCode::K, 'K');
    m_keyCodeMap.emplace(KeyCode::L, 'L');
    m_keyCodeMap.emplace(KeyCode::M, 'M');
    m_keyCodeMap.emplace(KeyCode::N, 'N');
    m_keyCodeMap.emplace(KeyCode::O, 'O');
    m_keyCodeMap.emplace(KeyCode::P, 'P');
    m_keyCodeMap.emplace(KeyCode::Q, 'Q');
    m_keyCodeMap.emplace(KeyCode::R, 'R');
    m_keyCodeMap.emplace(KeyCode::S, 'S');
    m_keyCodeMap.emplace(KeyCode::T, 'T');
    m_keyCodeMap.emplace(KeyCode::U, 'U');
    m_keyCodeMap.emplace(KeyCode::V, 'V');
    m_keyCodeMap.emplace(KeyCode::W, 'W');
    m_keyCodeMap.emplace(KeyCode::X, 'X');
    m_keyCodeMap.emplace(KeyCode::Y, 'Y');
    m_keyCodeMap.emplace(KeyCode::Z, 'Z');

    // 숫자
    m_keyCodeMap.emplace(KeyCode::Alpha0, '0');
    m_keyCodeMap.emplace(KeyCode::Alpha1, '1');
    m_keyCodeMap.emplace(KeyCode::Alpha2, '2');
    m_keyCodeMap.emplace(KeyCode::Alpha3, '3');
    m_keyCodeMap.emplace(KeyCode::Alpha4, '4');
    m_keyCodeMap.emplace(KeyCode::Alpha5, '5');
    m_keyCodeMap.emplace(KeyCode::Alpha6, '6');
    m_keyCodeMap.emplace(KeyCode::Alpha7, '7');
    m_keyCodeMap.emplace(KeyCode::Alpha8, '8');
    m_keyCodeMap.emplace(KeyCode::Alpha9, '9');

    // 특수 키
    m_keyCodeMap.emplace(KeyCode::Escape, VK_ESCAPE);
    m_keyCodeMap.emplace(KeyCode::Space, VK_SPACE);
    m_keyCodeMap.emplace(KeyCode::Enter, VK_RETURN);
    m_keyCodeMap.emplace(KeyCode::Tab, VK_TAB);
    m_keyCodeMap.emplace(KeyCode::Backspace, VK_BACK);
    m_keyCodeMap.emplace(KeyCode::Delete, VK_DELETE);

    m_keyCodeMap.emplace(KeyCode::LeftShift, VK_LSHIFT);
    m_keyCodeMap.emplace(KeyCode::RightShift, VK_RSHIFT);
    m_keyCodeMap.emplace(KeyCode::LeftControl, VK_LCONTROL);
    m_keyCodeMap.emplace(KeyCode::RightControl, VK_RCONTROL);
    m_keyCodeMap.emplace(KeyCode::LeftAlt, VK_LMENU);
    m_keyCodeMap.emplace(KeyCode::RightAlt, VK_RMENU);

    m_keyCodeMap.emplace(KeyCode::UpArrow, VK_UP);
    m_keyCodeMap.emplace(KeyCode::DownArrow, VK_DOWN);
    m_keyCodeMap.emplace(KeyCode::LeftArrow, VK_LEFT);
    m_keyCodeMap.emplace(KeyCode::RightArrow, VK_RIGHT);

    m_keyCodeMap.emplace(KeyCode::Comma, VK_OEM_COMMA);
    m_keyCodeMap.emplace(KeyCode::Period, VK_OEM_PERIOD);
    m_keyCodeMap.emplace(KeyCode::Slash, VK_OEM_2);
    m_keyCodeMap.emplace(KeyCode::Semicolon, VK_OEM_1);
    m_keyCodeMap.emplace(KeyCode::Quote, VK_OEM_7);
    m_keyCodeMap.emplace(KeyCode::LeftBracket, VK_OEM_4);
    m_keyCodeMap.emplace(KeyCode::RightBracket, VK_OEM_6);
    m_keyCodeMap.emplace(KeyCode::Minus, VK_OEM_MINUS);
    m_keyCodeMap.emplace(KeyCode::Equals, VK_OEM_PLUS);

    m_keyCodeMap.emplace(KeyCode::F1, VK_F1);
    m_keyCodeMap.emplace(KeyCode::F2, VK_F2);
    m_keyCodeMap.emplace(KeyCode::F3, VK_F3);
    m_keyCodeMap.emplace(KeyCode::F4, VK_F4);
    m_keyCodeMap.emplace(KeyCode::F5, VK_F5);
    m_keyCodeMap.emplace(KeyCode::F6, VK_F6);
    m_keyCodeMap.emplace(KeyCode::F7, VK_F7);
    m_keyCodeMap.emplace(KeyCode::F8, VK_F8);
    m_keyCodeMap.emplace(KeyCode::F9, VK_F9);
    m_keyCodeMap.emplace(KeyCode::F10, VK_F10);
    m_keyCodeMap.emplace(KeyCode::F11, VK_F11);
    m_keyCodeMap.emplace(KeyCode::F12, VK_F12);

    // 마우스 버튼
    m_keyCodeMap.emplace(KeyCode::MouseLeft, VK_LBUTTON);
    m_keyCodeMap.emplace(KeyCode::MouseRight, VK_RBUTTON);
    m_keyCodeMap.emplace(KeyCode::MouseMiddle, VK_MBUTTON);
}

int MMMEngine::InputManager::GetNativeKeyCode(KeyCode keyCode) const
{
    auto it = m_keyCodeMap.find(keyCode);
    if (it != m_keyCodeMap.end())
    {
        return it->second;
    }
    return -1; // KeyCode가 매핑되지 않은 경우 -1 반환
}

void MMMEngine::InputManager::StartUp(HANDLE windowHandle)
{
    m_hWnd = static_cast<HWND>(windowHandle); // 윈도우 핸들
    InitKeyCodeMap();
}

void MMMEngine::InputManager::ShutDown()
{
    m_hWnd = NULL;
    m_keyCodeMap.clear();
}

void MMMEngine::InputManager::Update()
{
    // 마우스 좌표
    ::GetCursorPos(&m_mouseClient);
    ::ScreenToClient(m_hWnd, &m_mouseClient);
    ::GetClientRect(m_hWnd, &m_clientRect);

    // 키보드 상태 업데이트
    memcpy_s(m_prevState, sizeof(m_prevState), m_currState, sizeof(m_currState));
    for (int i = 0; i < 256; i++) {
        m_currState[i] = GetAsyncKeyState(i);
    }


    //// 게임패드 상태 업데이트
    //for (auto& gamepad : m_gamepads)
    //{
    //    if (gamepad)
    //    {
    //        gamepad->Update();
    //    }
    //}
}


Vector2 MMMEngine::InputManager::GetMousePos() 
{
    //(타겟 스크린 사이즈 / 윈도우 클라이언트 사이즈) 비율로 처리하도록 변경
    //float screenFactorX = Screen::GetWidth() / static_cast<float>(RenderManager::Get()->GetWindow()->GetWidth());
    //float screenFactorY = Screen::GetHeight() / static_cast<float>(RenderManager::Get()->GetWindow()->GetHeight());

    return Vector2{ (float)m_mouseClient.x, (float)m_mouseClient.y };
}
bool MMMEngine::InputManager::GetKey(KeyCode keyCode)
{
    auto nativeKeyCode = GetNativeKeyCode(keyCode);
    if (nativeKeyCode == -1) return false;
    return (m_currState[nativeKeyCode] & KEY_PRESSED) != 0;
}
bool MMMEngine::InputManager::GetKeyDown(KeyCode keyCode)
{
    auto nativeKeyCode = GetNativeKeyCode(keyCode);
    if (nativeKeyCode == -1) return false;
    return (!(m_prevState[nativeKeyCode] & KEY_PRESSED) && (m_currState[nativeKeyCode] & KEY_PRESSED));
}
bool MMMEngine::InputManager::GetKeyUp(KeyCode keyCode)
{
    auto nativeKeyCode = GetNativeKeyCode(keyCode);
    if (nativeKeyCode == -1) return false;
    return ((m_prevState[nativeKeyCode] & KEY_PRESSED) && !(m_currState[nativeKeyCode] & KEY_PRESSED));
}
