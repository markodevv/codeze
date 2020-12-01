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

	i32 x, y;

} Vec2i;

typedef struct Vec3i {

	i32 x, y, z;

} Vec3i;

typedef struct Vec4i {

	i32 x, y, z, w;

} Vec4i;

void mat_ortho(f32 mat[4][4], f32 left, f32 right, f32 bottom, f32 top);
void vec4_print(Vec4 vec);
