#include "math.h"
#include "debug.h"


void
ortho(f32 mat[4][4], f32 left, f32 right, f32 bottom, f32 top) {

    mat[0][0] = 2 / (right - left);
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[0][3] = 0;

    mat[1][0] = 0;
    mat[1][1] = 2 / (top - bottom);
    mat[1][2] = 0;
    mat[1][3] = 0;

    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = -2;
    mat[2][3] = 0;

    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = 0;
    mat[3][3] = 1;
}
 
void
vec4_print(Vec4 vec) {

	printf("( ");
	printf("%f", vec.x);
	printf(", ");
	printf("%f", vec.y);
	printf(", ");
	printf("%f", vec.z);
	printf(", ");
	printf("%f", vec.w);
	printf(" )\n");
  
}
