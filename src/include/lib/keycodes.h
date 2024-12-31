#ifndef KEYCODES_H
#define KEYCODES_H

namespace KeyCodes {
    enum ChromeCodes {
        VKEY_A = 0x00,
        VKEY_S = 0x01,
        VKEY_D = 0x02,
        VKEY_F = 0x03,
        VKEY_H = 0x04,
        VKEY_G = 0x05,
        VKEY_Z = 0x06,
        VKEY_X = 0x07,
        VKEY_C = 0x08,
        VKEY_V = 0x09,
        VKEY_OEM_3 = 0x0A,  // Section key.
        VKEY_B = 0x0B,
        VKEY_Q = 0x0C,
        VKEY_W = 0x0D,
        VKEY_E = 0x0E,
        VKEY_R = 0x0F,
        VKEY_Y = 0x10,
        VKEY_T = 0x11,
        VKEY_1 = 0x12,
        VKEY_2 = 0x13,
        VKEY_3 = 0x14,
        VKEY_4 = 0x15,
        VKEY_6 = 0x16,
        VKEY_5 = 0x17,
        VKEY_OEM_PLUS = 0x18,  // =+
        VKEY_9 = 0x19,
        VKEY_7 = 0x1A,
        VKEY_OEM_MINUS = 0x1B,  // -_
        VKEY_8 = 0x1C,
        VKEY_0 = 0x1D,
        VKEY_OEM_6 = 0x1E,  // ]}
        VKEY_O = 0x1F,
        VKEY_U = 0x20,
        VKEY_OEM_4 = 0x21,  // {[
        VKEY_I = 0x22,
        VKEY_P = 0x23,
        VKEY_RETURN = 0x24,  // Return
        VKEY_L = 0x25,
        VKEY_J = 0x26,
        VKEY_OEM_7 = 0x27,  // '"
        VKEY_K = 0x28,
        VKEY_OEM_1 = 0x29,      // ;:
        VKEY_OEM_5 = 0x2A,      // \|
        VKEY_OEM_COMMA = 0x2B,  // ,<
        VKEY_OEM_2 = 0x2C,      // /?
        VKEY_N = 0x2D,
        VKEY_M = 0x2E,
        VKEY_OEM_PERIOD = 0x2F,  // .>
        VKEY_TAB = 0x30,
        VKEY_SPACE = 0x31,
        //VKEY_OEM_3 = 0x32,    // `~
        VKEY_BACK = 0x33,     // Backspace
        // VKEY_UNKNOWN = 0x34,  // n/a
        VKEY_ESCAPE = 0x35,
        VKEY_APPS = 0x36,     // Right Command
        VKEY_LWIN = 0x37,     // Left Command
        VKEY_LSHIFT = 0x38,    // Left Shift
        VKEY_CAPITAL = 0x39,  // Caps Lock
        VKEY_MENU = 0x3A,     // Left Option
        VKEY_CONTROL = 0x3B,  // Left Ctrl
        VKEY_RSHIFT = 0x3C,    // Right Shift
        VKEY_RALT = 0x3D,     // Right Option
        VKEY_RCONTROL = 0x3E,  // Right Ctrl
        VKEY_FN = 0x3F,  // fn
        VKEY_F17 = 0x40,
        VKEY_DECIMAL = 0x41,   // Num Pad .
        //VKEY_UNKNOWN = 0x42,   // n/a
        VKEY_MULTIPLY = 0x43,  // Num Pad *
        //VKEY_UNKNOWN = 0x44,   // n/a
        VKEY_ADD = 0x45,       // Num Pad +
        //VKEY_UNKNOWN = 0x46,   // n/a
        VKEY_CLEAR = 0x47,     // Num Pad Clear
        VKEY_VOLUME_UP = 0x48,
        VKEY_VOLUME_DOWN = 0x49,
        VKEY_VOLUME_MUTE = 0x4A,
        VKEY_DIVIDE = 0x4B,    // Num Pad /
        VKEY_NUMPAD_RETURN = 0x4C,    // Num Pad Enter
        // VKEY_UNKNOWN = 0x4D,   // n/a
        VKEY_SUBTRACT = 0x4E,  // Num Pad -
        VKEY_F18 = 0x4F,
        VKEY_F19 = 0x50,
        VKEY_NUMPAD_OEM_PLUS = 0x51,  // Num Pad =.
        VKEY_NUMPAD0 = 0x52,
        VKEY_NUMPAD1 = 0x53,
        VKEY_NUMPAD2 = 0x54,
        VKEY_NUMPAD3 = 0x55,
        VKEY_NUMPAD4 = 0x56,
        VKEY_NUMPAD5 = 0x57,
        VKEY_NUMPAD6 = 0x58,
        VKEY_NUMPAD7 = 0x59,
        VKEY_F20 = 0x5A,
        VKEY_NUMPAD8 = 0x5B,
        VKEY_NUMPAD9 = 0x5C,
        //    VKEY_UNKNOWN = 0x5D,  // Yen (JIS Keyboard Only)
        //    VKEY_UNKNOWN = 0x5E,  // Underscore (JIS Keyboard Only)
        //    VKEY_UNKNOWN = 0x5F,  // KeypadComma (JIS Keyboard Only)
        VKEY_F5 = 0x60,
        VKEY_F6 = 0x61,
        VKEY_F7 = 0x62,
        VKEY_F3 = 0x63,
        VKEY_F8 = 0x64,
        VKEY_F9 = 0x65,
        //    VKEY_UNKNOWN = 0x66,  // Eisu (JIS Keyboard Only)
        VKEY_F11 = 0x67,
        //    VKEY_UNKNOWN = 0x68,  // Kana (JIS Keyboard Only)
        VKEY_F13 = 0x69,
        VKEY_F16 = 0x6A,
        VKEY_F14 = 0x6B,
        //    VKEY_UNKNOWN = 0x6C,  // n/a
        VKEY_F10 = 0x6D,
        //    VKEY_APPS = 0x6E,  // Context Menu key
        VKEY_F12 = 0x6F,
        //    VKEY_UNKNOWN = 0x70,  // n/a
        VKEY_F15 = 0x71,
        VKEY_INSERT = 0x72,  // Help
        VKEY_HOME = 0x73,    // Home
        VKEY_PRIOR = 0x74,   // Page Up
        VKEY_DELETE = 0x75,  // Forward Delete
        VKEY_F4 = 0x76,
        VKEY_END = 0x77,  // End
        VKEY_F2 = 0x78,
        VKEY_NEXT = 0x79,  // Page Down
        VKEY_F1 = 0x7A,
        VKEY_LEFT = 0x7B,    // Left Arrow
        VKEY_RIGHT = 0x7C,   // Right Arrow
        VKEY_DOWN = 0x7D,    // Down Arrow
        VKEY_UP = 0x7E,      // Up Arrow
        VKEY_UNKNOWN = 0x7F  // n/a
    };

    static const std::map<char, unsigned short> table = {
        {'a', VKEY_A},
        {'A', VKEY_A},
        {'b', VKEY_B},
        {'B', VKEY_B},
        {'c', VKEY_C},
        {'C', VKEY_C},
        {'d', VKEY_D},
        {'D', VKEY_D},
        {'e', VKEY_E},
        {'E', VKEY_E},
        {'f', VKEY_F},
        {'F', VKEY_F},
        {'g', VKEY_G},
        {'G', VKEY_G},
        {'h', VKEY_H},
        {'H', VKEY_H},
        {'i', VKEY_I},
        {'I', VKEY_I},
        {'j', VKEY_J},
        {'J', VKEY_J},
        {'k', VKEY_K},
        {'K', VKEY_K},
        {'l', VKEY_L},
        {'L', VKEY_L},
        {'m', VKEY_M},
        {'M', VKEY_M},
        {'n', VKEY_N},
        {'N', VKEY_N},
        {'o', VKEY_O},
        {'O', VKEY_O},
        {'p', VKEY_P},
        {'P', VKEY_P},
        {'q', VKEY_Q},
        {'Q', VKEY_Q},
        {'r', VKEY_R},
        {'R', VKEY_R},
        {'s', VKEY_S},
        {'S', VKEY_S},
        {'t', VKEY_T},
        {'T', VKEY_T},
        {'u', VKEY_U},
        {'U', VKEY_U},
        {'v', VKEY_V},
        {'V', VKEY_V},
        {'w', VKEY_W},
        {'W', VKEY_W},
        {'x', VKEY_X},
        {'X', VKEY_X},
        {'y', VKEY_Y},
        {'Y', VKEY_Y},
        {'z', VKEY_Z},
        {'Z', VKEY_Z},
        {'1', VKEY_1},
        {'2', VKEY_2},
        {'3', VKEY_3},
        {'4', VKEY_4},
        {'5', VKEY_5},
        {'6', VKEY_6},
        {'7', VKEY_7},
        {'8', VKEY_8},
        {'9', VKEY_9},
        {'0', VKEY_0},
        {0x08, VKEY_BACK},
        {0x09, VKEY_TAB},
        {0x20, VKEY_SPACE},
        {0x0D, VKEY_RETURN},
        {0x1B, VKEY_ESCAPE},
        {0x01, VKEY_HOME},
        {0x04, VKEY_END},
        {0x0B, VKEY_PRIOR},
        {0x0C, VKEY_NEXT},
        {0x1E, VKEY_UP},
        {0x1F, VKEY_DOWN},
        {0x1C, VKEY_LEFT},
        {0x1D, VKEY_RIGHT},
        {0x7F, VKEY_DELETE},
        // U.S. Specific mappings.  Mileage may vary.
        {';', VKEY_OEM_1},
        {':', VKEY_OEM_1},
        {'=', VKEY_OEM_PLUS},
        {'+', VKEY_OEM_PLUS},
        {',', VKEY_OEM_COMMA},
        {'<', VKEY_OEM_COMMA},
        {'-', VKEY_OEM_MINUS},
        {'_', VKEY_OEM_MINUS},
        {'.', VKEY_OEM_PERIOD},
        {'>', VKEY_OEM_PERIOD},
        {'/', VKEY_OEM_2},
        {'?', VKEY_OEM_2},
        {'`', VKEY_OEM_3},
        {'~', VKEY_OEM_3},
        {'[', VKEY_OEM_4},
        {'{', VKEY_OEM_4},
        {'\\', VKEY_OEM_5},
        {'|', VKEY_OEM_5},
        {']', VKEY_OEM_6},
        {'}', VKEY_OEM_6},
        {'\'', VKEY_OEM_7},
        {'"', VKEY_OEM_7},
    };
}
#endif
