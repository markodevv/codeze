#pragma once
#include "types.h"

typedef struct Vec2 {
	union {
		struct {
			
			f32 x, y;
		};
		struct {
			
			f32 start, end;
		};
		struct {
			
			f32 w, h;
		};
	};

} Vec2;

typedef struct Vec3 {

	f32 x, y, z;

} Vec3;

typedef struct Vec4 {

	f32 x, y, z, w;

} Vec4;

typedef struct Vec2i {
	union {
		struct {
			
			i32 x, y;
		};
		struct {
			
			i32 start, end;
		};
		struct {
			
			i32 w, h;
		};
	};

} Vec2i;

typedef struct Vec3i {

	i32 x, y, z;

} Vec3i;

typedef struct Vec4i {

	i32 x, y, z, w;

} Vec4i;

void mat_ortho(f32 mat[4][4], f32 left, f32 right, f32 bottom, f32 top);
b8 is_point_in_rect(Vec2 point, Vec2 rectPos, Vec2 rectSize);
b8 is_point_in_rect_i(Vec2i point, Vec2i rectPos, Vec2i rectSize);

void vec4_print(Vec4 vec);
void vec3_print(Vec3 vec);
void vec2_print(Vec2 vec);

void vec4i_print(Vec4i vec);
void vec3i_print(Vec3i vec);
void vec2i_print(Vec2i vec);
int random_int();
