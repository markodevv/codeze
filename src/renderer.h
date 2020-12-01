#pragma once

#include "math.h"
#include "my_string.h"
#include "tokenizer.h"
#include "buffer.h"

#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef enum TextureIndex {

	NO_TEXTURE  = -1,
	GREEN_TEXTURE_INDEX  = 0,
	WHITE_TEXTURE_INDEX  = 1,
	FONT_TEXTURE_INDEX  = 2,
	RYUK_TEXTURE_INDEX  = 3,

	TEXTURE_SLOTS = 4

	
} TextureIndex;

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

typedef struct Window {
  
	i8 active;

	Vec2 position;
	Vec2 size;

} Window;


GLFWwindow* renderer_create_window();

void renderer_initialize(Renderer* ren, f32 width, f32 height);
void renderer_load_font(Renderer* ren, const char* fontFile, i32 fontSize);
void renderer_begin(Renderer* ren);
void renderer_end(Renderer* ren);

void render_quad(Renderer* ren, Vec2 position, Vec2 size, Vec4 color);
void render_textured_quad(Renderer* ren, Vec2 position, Vec2 size, Vec4 color, u32 texID);
void render_text(Renderer* ren, string text, Vec2 position, Vec4 color);
void render_buffer(Renderer* ren, Buffer* buffer, Window* window, Token* tokens);

#ifdef DEBUG
#include "stdio.h" 

void render_text_debug(Renderer* ren, char* text, Vec2 position, Vec4 color);
static char DebugString[64];
static Vec2 DebugPos = {1200.0f, 0.0f};
static Vec4 DebugColor = {0.8f, 0.8f, 0.0f, 1.0f};

#define DEBUG_TEXT(ren, row, text, ...)	\
	sprintf(DebugString, text, __VA_ARGS__); \
	DebugPos.y = row * ren->fontSize; \
	render_text_debug(ren, DebugString, DebugPos, DebugColor);
#else
#define DEBUG_TEXT(ren, row, text, ...)	

#endif
  

