#include "key.h"
#include "my_string.h"

KeyCode
keycode_from_string(String& keystr) {

	if (keystr ==  "a")
			return KEY_A;
	if (keystr ==  "b")
			return KEY_B;
	if (keystr ==  "c")
			return KEY_C;
	if (keystr ==  "d")
			return KEY_D;
	if (keystr ==  "e")
			return KEY_E;
	if (keystr ==  "f")
			return KEY_F;
	if (keystr ==  "g")
			return KEY_G;
	if (keystr ==  "h")
			return KEY_H;
	if (keystr ==  "i")
			return KEY_I;
	if (keystr ==  "j")
			return KEY_J;
	if (keystr ==  "k")
			return KEY_K;
	if (keystr ==  "l")
			return KEY_L;
	if (keystr ==  "m")
			return KEY_M;
	if (keystr ==  "n")
			return KEY_N;
	if (keystr ==  "o")
			return KEY_O;
	if (keystr ==  "p")
			return KEY_P;
	if (keystr ==  "q")
			return KEY_Q;
	if (keystr ==  "r")
			return KEY_R;
	if (keystr ==  "s")
			return KEY_S;
	if (keystr ==  "t")
			return KEY_T;
	if (keystr ==  "u")
			return KEY_U;
	if (keystr ==  "v")
			return KEY_V;
	if (keystr ==  "w")
			return KEY_W;
	if (keystr ==  "x")
			return KEY_X;
	if (keystr ==  "y")
			return KEY_Y;
	if (keystr ==  "z")
			return KEY_Z;
	if (keystr ==  ";")
			return KEY_Semicolon;
	if (keystr ==  "=")
			return KEY_Equal;

	return KEY_Unknown;
}
