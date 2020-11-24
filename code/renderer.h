#pragma once

#include "math.h"

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define GREEN_TEXTURE_INDEX 0
#define WHITE_TEXTURE_INDEX 1
#define FONT_TEXTURE_INDEX 2
#define NO_TEXTURE -1

typedef struct Vertex {
  
	Vec4 posData;
	Vec4 color;
	i32 texIndex;

} Vertex;

typedef struct GlyphData {
  
	f32 advanceX;
	f32 advanceY;

	f32 width;
	f32 height;

	f32 bearingX;
	f32 bearingY;

	f32 offsetX;

} GlyphData;

typedef struct Renderer {

	u32 program;
	f32 projection[4][4];

	u32 texIDs[3];
	u32 VBO;
	u32 VAO;

	Vertex* vertexArray;
	Vertex* vertexArrayIndex;

	u32 vertexCount;

	FT_Library ftLib;
	FT_Face fontFace;
	GlyphData glyphs[128];
	f32 bitmapW;
	f32 bitmapH;
	i32 fontSize;

} Renderer;


GLFWwindow* renderer_create_window();

void renderer_initialize(Renderer* ren, f32 width, f32 height);
void renderer_load_font(Renderer* ren, const char* fontFile, i32 fontSize);
void renderer_begin(Renderer* ren);
void renderer_end(Renderer* ren);

void render_quad(Renderer* ren, Vec2 position, Vec2 size, Vec4 color);
void render_textured_quad(Renderer* ren, Vec2 position, Vec2 size, u32 texID);
void render_text(Renderer* ren, const char* text, sizet length, Vec2 position, Vec4 color);
