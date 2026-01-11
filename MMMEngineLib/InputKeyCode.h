#pragma once
#include <functional>
#include <utility>

namespace MMMEngine
{
    enum class KeyCode
    {
        // 알파벳
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // 숫자
        Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

        // 특수 키
        Escape, Space, Enter, Tab, Backspace, Delete,
        LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
        UpArrow, DownArrow, LeftArrow, RightArrow,

        Comma, Period, Slash, Semicolon, Quote, LeftBracket, RightBracket, Minus, Equals,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        // 마우스 버튼
        MouseLeft, MouseRight, MouseMiddle,

        // 기타
        Unknown,
    };
}

namespace std {
    template <>
    struct hash<MMMEngine::KeyCode> {
        // 호출 연산자 오버로드: KeyCode 객체를 받아 size_t 해시 값을 반환합니다.
        size_t operator()(const MMMEngine::KeyCode& kc) const noexcept {
            // 1. KeyCode의 기반 타입(underlying type, 보통 int)을 얻습니다.
            using UnderlyingType = typename std::underlying_type<MMMEngine::KeyCode>::type;

            // 2. KeyCode 값을 해당 정수 타입으로 캐스팅합니다.
            UnderlyingType underlying_value = static_cast<UnderlyingType>(kc);

            // 3. 정수 타입의 표준 해시 함수를 호출하여 해시 값을 생성하고 반환합니다.
            return std::hash<UnderlyingType>{}(underlying_value);
        }
    };
}