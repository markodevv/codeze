#pragma once

#include "math.h"

#include <GLFW/glfw3.h>

#define GREEN_TEXTURE_INDEX 0
#define WHITE_TEXTURE_INDEX 1
#define NO_TEXTURE_INDEX -1

typedef struct Vertex {
  
	Vec4 posData;
	Vec4 color;
	i32 texIndex;

} Vertex;

typedef struct Renderer {

	u32 program;
	f32 projection[4][4];

	u32 texIDs[2];
	u32 VBO;
	u32 VAO;

	Vertex* vertexArray;
	Vertex* vertexArrayIndex;

	u32 vertexCount;

} Renderer;

GLFWwindow* renderer_create_window();

void renderer_initialize(Renderer* ren, f32 width, f32 height);
void renderer_begin(Renderer* ren);
void renderer_end(Renderer* ren);

void render_quad(Renderer* ren, Vec2 position, Vec2 size, Vec4 color);
void render_textured_quad(Renderer* ren, Vec2 position, Vec2 size, u32 texID);
