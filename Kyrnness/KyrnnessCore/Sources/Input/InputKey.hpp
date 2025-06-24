#pragma once
#ifndef KYRNNES_INPUT_KEY_HPP
#define KYRNNES_INPUT_KEY_HPP

#include "Core/Core.hpp"

enum EInputKey : int32
{
    None = 0,

    // Alphanumeric keys
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,

    // Numeric keys
    Num0 = 0x30,
    Num1 = 0x31,
    Num2 = 0x32,
    Num3 = 0x33,
    Num4 = 0x34,
    Num5 = 0x35,
    Num6 = 0x36,
    Num7 = 0x37,
    Num8 = 0x38,
    Num9 = 0x39,

    // Function keys
    F1 = 0x70,
    F2 = 0x71,
    F3 = 0x72,
    F4 = 0x73,
    F5 = 0x74,
    F6 = 0x75,
    F7 = 0x76,
    F8 = 0x77,
    F9 = 0x78,
    F10 = 0x79,
    F11 = 0x7A,
    F12 = 0x7B,

    // Special keys
    Escape = 0x1B,
    Space = 0x20,
    Enter = 257,
    Tab = 0x09,
    Backspace = 0x08,
    Shift = 0x10,
    Control = 0x11,
    Alt = 0x12,
    CapsLock = 0x14,
    NumLock = 0x90,
    ScrollLock = 0x91,
    ShiftLeft = 340,
    CtrlLeft = 341,
    AltLeft = 342,
    ShiftRight = 344,
    CtrlRight = 345,
	AltRight = 346,

    // Arrow keys
    LeftArrow = 0x25,
    UpArrow = 0x26,
    RightArrow = 0x27,
    DownArrow = 0x28,

    // Other keys
    Insert = 0x2D,
    Delete = 0x2E,
    Home = 0x24,
    End = 0x23,
    PageUp = 0x21,
    PageDown = 0x22
};

enum EInputKeyStatus
{
	IKS_None     = 0,       // No key is pressed
	IKS_Pressed  = 1,    // The key is currently pressed down
	IKS_Released = 2,   // The key has been released
};

struct TInputKey
{
	int32 KeyCode; // The key code for the input key
	int32 Modifiers; // Bitmask for modifiers (e.g., Shift, Ctrl, Alt)
	EInputKeyStatus Status; // Current status of the key
	float PressDuration; // Duration for which the key has been pressed
};;




#endif// KYRNNES_INPUT_KEY_HPP
