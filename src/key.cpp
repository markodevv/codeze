#include "key.h"
#include "my_string.h"

KeyCode
keycode_from_string(String key) {

	if (key ==  "a") return KEY_A;
	if (key ==  "b") return KEY_B;
	if (key ==  "c") return KEY_C;
	if (key ==  "d") return KEY_D;
	if (key ==  "e") return KEY_E;
	if (key ==  "f") return KEY_F;
	if (key ==  "g") return KEY_G;
	if (key ==  "h") return KEY_H;
	if (key ==  "i") return KEY_I;
	if (key ==  "j") return KEY_J;
	if (key ==  "k") return KEY_K;
	if (key ==  "l") return KEY_L;
	if (key ==  "m") return KEY_M;
	if (key ==  "n") return KEY_N;
	if (key ==  "o") return KEY_O;
	if (key ==  "p") return KEY_P;
	if (key ==  "q") return KEY_Q;
	if (key ==  "r") return KEY_R;
	if (key ==  "s") return KEY_S;
	if (key ==  "t") return KEY_T;
	if (key ==  "u") return KEY_U;
	if (key ==  "v") return KEY_V;
	if (key ==  "w") return KEY_W;
	if (key ==  "x") return KEY_X;
	if (key ==  "y") return KEY_Y;
	if (key ==  "z") return KEY_Z;
	if (key ==  ";") return KEY_Semicolon;
	if (key ==  "=") return KEY_Equal;
	if (key ==  "left") return KEY_Left;
	if (key ==  "right") return KEY_Right;
	if (key ==  "up") return KEY_Up;
	if (key ==  "down") return KEY_Down;
	if (key ==  "escape") return KEY_Escape;

	return KEY_Unknown;
}
