#pragma once

#include "math.h"
#include "my_string.h"
#include "tokenizer.h"
#include "buffer.h"
#include "window.h"

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



GLFWwindow* renderer_create_window();
void renderer_initialize(f32 width, f32 height);
void renderer_load_font(const char* fontFile, i32 fontSize);

void renderer_begin();
void renderer_end();

void render_quad(Vec2 position, Vec2 size, Vec4 color);
void render_textured_quad(Vec2 position, Vec2 size, Vec4 color, u32 texID);
void render_text(Buffer* buf, Vec2 position, Vec4 color);
void render_buffer(Buffer* buf, Window* window, Array<Token> tokens, b8 focused);
void render_cursor(Buffer* buf, Window* window);
void renderer_on_window_resize(f32 width, f32 height);
GlyphData* renderer_get_glyphs();
i32 renderer_font_size();

#ifdef DEBUG
#include "stdio.h" 

void render_text_debug(char* text, Vec2 position, Vec4 color);
static char DebugString[64];
static Vec4 DebugColor = {0.8f, 0.8f, 0.0f, 1.0f};

#define DEBUG_TEXT(pos, text, ...)	\
	sprintf(DebugString, text, __VA_ARGS__); \
	render_text_debug(DebugString, pos, DebugColor);
#else
#define DEBUG_TEXT(row, text, ...)	

#endif
  

