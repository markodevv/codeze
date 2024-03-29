#include "math.h"
#include <stdlib.h>
#include "debug.h"


void
mat_ortho(f32 mat[4][4], f32 left, f32 right, f32 bottom, f32 top) {

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
 
b8
is_point_in_rect(Vec2 point, Vec2 rectPos, Vec2 rectSize) {
  
	if (point.x > rectPos.x && point.x < (rectPos.x + rectSize.w) &&
		point.y > rectPos.y && point.y < (rectPos.y + rectSize.h))
		return 1;
	else 
		return 0; 	
  
}

b8
is_point_in_rect_i(Vec2i point, Vec2i rectPos, Vec2i rectSize) {
  
	if (point.x > rectPos.x && point.x < (rectPos.x + rectSize.w) &&
		point.y > rectPos.y && point.y < (rectPos.y + rectSize.h))
		return 1;
	else
		return 0; 	
		
  
}

int random_int() {
  
	return rand();
}
void
vec4_print(Vec4 vec) {

	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%f", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.y);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.z);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.w);
	NORMAL_MSG(" )\n", NULL);
  
}

void
vec3_print(Vec3 vec) {
  
	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%f", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.y);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.z);
	NORMAL_MSG(" )\n", NULL);
}
  
void
vec2_print(Vec2 vec) {
  
	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%f", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%f", vec.y);
	NORMAL_MSG(" )\n", NULL);
}


void
vec4i_print(Vec4i vec) {

	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%i", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.y);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.z);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.w);
	NORMAL_MSG(" )\n", NULL);
  
}

void
vec3i_print(Vec3i vec) {
  
	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%i", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.y);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.z);
	NORMAL_MSG(" )\n", NULL);
}
  
void
vec2i_print(Vec2i vec) {
  
	NORMAL_MSG("( ", NULL);
	NORMAL_MSG("%i", vec.x);
	NORMAL_MSG(", ", NULL);
	NORMAL_MSG("%i", vec.y);
	NORMAL_MSG(" )\n", NULL);
}


