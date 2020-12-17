#include "renderer.h"
#include "fileio.h"
#include "shader.h"
#include "debug.h"
#include "cursor.h"
#include "config.h"

#include <glad/glad.h>

#define MAX_VERTICES 100000
#define VERTICES_PER_QUAD 6

static Vec4 global_Colors[TOK_TOTAL];
static Vec4 global_CursorColor = {1.0f, 1.0f, 1.0f, 0.5f};
static Renderer g_Renderer;

static void
error_callback(int code, const char* description) {

	printf("Error %i : %s \n", code, description);

}


GLFWwindow*
renderer_create_window() {

	Vec4 white = {0.8f, 0.8f, 0.8f, 1.0f};
	Vec4 green = {0.8f, 1.0f, 0.8f, 1.0f};
	Vec4 blue = {0.6f, 0.6f, 1.0f, 1.0f};
	Vec4 orange = {0.9f, 0.6f, 0.1f, 1.0f};
	Vec4 green2 = {0.2f, 0.8f, 0.5f, 0.8f};
	Vec4 redish = {0.7f, 0.3f, 0.1f, 0.8f};
	Vec4 purple = {0.5f, 0.2f, 0.6f, 0.8f};

	global_Colors[TOK_IDENTIFIER] = white;
	global_Colors[TOK_HASH] = white;
	global_Colors[TOK_NUMBER] = green;
	global_Colors[TOK_OPEN_PAREN] = orange;
	global_Colors[TOK_CLOSED_PAREN] = orange;
	global_Colors[TOK_OPEN_CURLY] = orange;
	global_Colors[TOK_CLOSED_CURLY] = orange;
	global_Colors[TOK_OPEN_SQUARE] = orange;
	global_Colors[TOK_CLOSED_SQUARE] = orange;
	global_Colors[TOK_KEYWORD] = orange;
	global_Colors[TOK_TYPE] = blue;
	global_Colors[TOK_STRING] = green2;
	global_Colors[TOK_SEMICOLON] = redish;
	global_Colors[TOK_COMMENT] = purple;

	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit()) {
		printf("Failed to initialize glfw\n");
		ASSERT(0);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 768, "codeze", NULL, NULL);
	if (!window) {
		glfwTerminate();
		printf("Failed to create window");
		ASSERT(0);
	}

	glfwMakeContextCurrent(window);

	// Glad initialization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize glad");
		ASSERT(0);
	}
	printf("Initialized glad\n");

	// Enable Vsync
	glfwSwapInterval(1);

	return window;

}

static void
texture_load(const char* path, u32* texID, u8 slot) {

	
	glActiveTexture(GL_TEXTURE0 + slot);

	i32 w, h, channels, internalFormat, dataFormat;
	u8* buffer = image_load_png(path, &w, &h, &channels);
	ASSERT(buffer != NULL);

	if (channels == 4) {
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3) {
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}


	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, dataFormat, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	image_free(buffer);
}

void
renderer_initialize(f32 width, f32 height) {

	g_Renderer.vertexArray = malloc(sizeof(Vertex) * MAX_VERTICES);
	g_Renderer.vertexArrayIndex = g_Renderer.vertexArray;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	File* vertexFile = file_open("src/vertex_shader.txt", "r");
	File* fragmentFile = file_open("src/fragment_shader.txt", "r");

	g_Renderer.program = shader_create(str_as_cstr(vertexFile->buffer), str_as_cstr(fragmentFile->buffer));
	
	glUseProgram(g_Renderer.program);

	glGenVertexArrays(1, &g_Renderer.VAO);
	glGenBuffers(1, &g_Renderer.VBO);

	glBindVertexArray(g_Renderer.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_Renderer.VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 4));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 4 * 2));

	mat_ortho(g_Renderer.projection, 0.0f, width, height, 0.0f);

	texture_load("assets/green.png", &g_Renderer.texIDs[GREEN_TEXTURE_INDEX], GREEN_TEXTURE_INDEX);
	texture_load("assets/white.png", &g_Renderer.texIDs[WHITE_TEXTURE_INDEX], WHITE_TEXTURE_INDEX);
	texture_load("assets/ryuk.png", &g_Renderer.texIDs[RYUK_TEXTURE_INDEX], RYUK_TEXTURE_INDEX);

	const i8 subscriptIndex = 10;
	char str[] = "uTextures[ ]";


	i32 location;
	for (i32 i = 0; i < TEXTURE_SLOTS; ++i) {

		str[subscriptIndex] = '0' + i;

		location = glGetUniformLocation(g_Renderer.program, str);
		ASSERT_MSG(location != -1, "invalid uniform location");

		glUniform1i(location, i);

	}

	file_close(vertexFile);
	file_close(fragmentFile);

}

void
renderer_load_font(const char* fontFile, i32 fontSize) {

	g_Renderer.fontSize = fontSize;

	i32 success = FT_Init_FreeType(&g_Renderer.ftLib);
	ASSERT(success == 0);
	success = FT_New_Face(g_Renderer.ftLib, fontFile, 0, &g_Renderer.fontFace);
	ASSERT(success == 0);

	FT_Set_Pixel_Sizes(g_Renderer.fontFace, 0, g_Renderer.fontSize);

	i32 w = 0, h = 0;
	FT_GlyphSlot glyph = g_Renderer.fontFace->glyph;

	for (u8 i = 0; i < 128; i++) {

		i32 success = FT_Load_Char(g_Renderer.fontFace, i, FT_LOAD_RENDER);
		ASSERT(success == 0);
		w += glyph->bitmap.width;
		glyph->bitmap.rows > h ? h = glyph->bitmap.rows : h;
	}

	g_Renderer.bitmapW = w;
	g_Renderer.bitmapH = h;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0 + FONT_TEXTURE_INDEX);

	glGenTextures(1, &g_Renderer.texIDs[FONT_TEXTURE_INDEX]);
	glBindTexture(GL_TEXTURE_2D, g_Renderer.texIDs[FONT_TEXTURE_INDEX]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);


	i32 texOffset = 0;

	for (u8 i = 0; i < 128; ++i) {

		i8 success = FT_Load_Char(g_Renderer.fontFace, i, FT_LOAD_RENDER);
		ASSERT(success == 0);

		glTexSubImage2D(GL_TEXTURE_2D, 0, texOffset, 0, 
			glyph->bitmap.width, glyph->bitmap.rows, 
			GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

		g_Renderer.glyphs[i].advanceX = glyph->advance.x >> 6;
		g_Renderer.glyphs[i].advanceY = glyph->advance.y >> 6;

		g_Renderer.glyphs[i].width = glyph->bitmap.width;
		g_Renderer.glyphs[i].height = glyph->bitmap.rows;

		g_Renderer.glyphs[i].bearingX = glyph->bitmap_left;
		g_Renderer.glyphs[i].bearingY = glyph->bitmap_top;
	
		g_Renderer.glyphs[i].offsetX = (f32)texOffset / w;

		texOffset += glyph->bitmap.width;
	}


	g_Renderer.glyphs['\t'].advanceX = g_Renderer.glyphs[' '].advanceX * 4;


	FT_Done_Face(g_Renderer.fontFace);
	FT_Done_FreeType(g_Renderer.ftLib);

  
}

void
renderer_begin() {

	glClearColor(0.1f, 0.1f, 0.13, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	i32 projectionLocation = glGetUniformLocation(g_Renderer.program, "uProjection");
	ASSERT(projectionLocation != -1);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &g_Renderer.projection[0][0]);

}

void
render_quad(Vec2 position, Vec2 size, Vec4 color) {

		Vec4 quadVertices[] = {
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x + size.x, position.y,          1.0f, 0.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f},
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x,          position.y + size.y, 0.0f, 1.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f}
		};

		if (g_Renderer.vertexCount >= MAX_VERTICES) {
			renderer_end();
		}

		for (int i = 0; i < VERTICES_PER_QUAD; ++i) {

			g_Renderer.vertexArrayIndex->color = color;
			g_Renderer.vertexArrayIndex->posData = quadVertices[i];
			g_Renderer.vertexArrayIndex->texIndex = NO_TEXTURE;
			g_Renderer.vertexArrayIndex++;
		}

		g_Renderer.vertexCount += VERTICES_PER_QUAD;

}

void
render_textured_quad(Vec2 position, Vec2 size, Vec4 color, u32 texID) {

		Vec4 quadVertices[] = {
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x + size.x, position.y,          1.0f, 0.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f},
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x,          position.y + size.y, 0.0f, 1.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f}
		};

		if (g_Renderer.vertexCount >= MAX_VERTICES) {
			renderer_end();
		}

		for (int k = 0; k < VERTICES_PER_QUAD; ++k) {

			g_Renderer.vertexArrayIndex->color = color;
			g_Renderer.vertexArrayIndex->posData = quadVertices[k];
			g_Renderer.vertexArrayIndex->texIndex = texID;
			g_Renderer.vertexArrayIndex++;
		}

		g_Renderer.vertexCount += VERTICES_PER_QUAD;
	
}

void
render_text(Buffer* buf, Vec2 position, Vec4 color) {

	static float xpos, ypos, w, h, offsetX, texX, texY, advanceX, advanceY;
	advanceY = position.y;
	advanceX = position.x;

	string text = buffer_get_text(buf);

	for (sizet i = 0; i < STR_LENGTH(text); ++i) {

		if (text[i] == '\n') {

			advanceY += g_Renderer.fontSize;
			advanceX = 0.0f;
			continue;
		}
		else if (text[i] == '\t') {

			advanceX += g_Renderer.glyphs[text[i]].advanceX;
			continue;
		}

		xpos = advanceX + g_Renderer.glyphs[text[i]].bearingX;
		// this is stupid, idk how else to make it work
		ypos = advanceY - g_Renderer.glyphs[text[i]].bearingY + g_Renderer.fontSize;
		w = g_Renderer.glyphs[text[i]].width;
		h = g_Renderer.glyphs[text[i]].height;
		offsetX = g_Renderer.glyphs[text[i]].offsetX;
		texX = w / g_Renderer.bitmapW;
		texY = h / g_Renderer.bitmapH;

		Vec4 quadVertices[] = {
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos + w, ypos,     offsetX + texX, 0.0f},
			{xpos + w, ypos + h, offsetX + texX, texY},
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos,     ypos + h, offsetX,        texY},
			{xpos + w, ypos + h, offsetX + texX, texY}
		};

		if (g_Renderer.vertexCount >= MAX_VERTICES) {

			renderer_end();
		}

		for (int j = 0; j < VERTICES_PER_QUAD; ++j) {

			g_Renderer.vertexArrayIndex->color = color;
			g_Renderer.vertexArrayIndex->posData = quadVertices[j];
			g_Renderer.vertexArrayIndex->texIndex = FONT_TEXTURE_INDEX;
			g_Renderer.vertexArrayIndex++;
		}

		g_Renderer.vertexCount += VERTICES_PER_QUAD;
		advanceX += g_Renderer.glyphs[text[i]].advanceX;
	}
	str_release(text);
}


void
render_buffer(Buffer* buf, Window *window, TokenArray tokens, b8 focused) {

	static float xpos, ypos, w, h, offsetX,
		texX, texY, advanceX, advanceY, tokLen;

	string text = buffer_get_text(buf);

	advanceY = window->position.y;
	advanceX = window->position.x;

	u32 tokIndex = 0;
	Vec4 color = global_Colors[0];

	{
		Vec2 borderPos, borderSize;
		borderPos.x = window->position.x;
		borderPos.y = window->position.y;
		borderSize.x = 0.8f;
		borderSize.y = window->size.h;

		Vec4 borderColor = global_Colors[0];

		if (focused) 
			borderColor = global_Colors[5];

		render_quad(borderPos, borderSize, borderColor);

		borderPos.x = window->position.x;
		borderPos.y = window->position.y + window->size.h - 1.0f;
		borderSize.x = window->size.w;
		borderSize.y = 0.8f;

		render_quad(borderPos, borderSize, borderColor);
	}

	sizet visibleLines = (window->size.h -
						  (window->size.h % g_Renderer.fontSize) - 1) / g_Renderer.fontSize;

	window->renderView.end = window->renderView.start + visibleLines - 1;
	if (window->renderView.end > ARRAY_LENGTH(buf->lineLengths))
		window->renderView.end = ARRAY_LENGTH(buf->lineLengths);

	sizet start = buffer_index_based_on_line(buf, window->renderView.start);
	sizet end = buffer_index_based_on_line(buf, window->renderView.end);

	for (sizet i = start; i < end; ++i) {


		// TODO: can remove this and make
		// it renderable by setting the newline glyph as empthy image
		if (text[i] == '\n') {

			advanceY += g_Renderer.fontSize;
			advanceX = window->position.x;
			continue;
		}
		else if (text[i] == '\t') {

			advanceX += g_Renderer.glyphs[text[i]].advanceX;
			continue;
		}

		if (tokLen <= 0) {

			color = global_Colors[TOK_IDENTIFIER];
		}
		tokLen--;

		if (advanceY >= window->size.h + window->position.y - g_Renderer.fontSize) {
			break;
		}
		if (advanceX >= window->position.x + window->size.x) {
			// advanceX = window->position.x;
			// advanceY += g_Renderer.fontSize;
			continue;
		}

		xpos = advanceX + g_Renderer.glyphs[text[i]].bearingX;
		// this is stupid, idk how else to make it work
		ypos = advanceY - g_Renderer.glyphs[text[i]].bearingY + g_Renderer.fontSize;
		w = g_Renderer.glyphs[text[i]].width;
		h = g_Renderer.glyphs[text[i]].height;
		offsetX = g_Renderer.glyphs[text[i]].offsetX;
		texX = w / g_Renderer.bitmapW;
		texY = h / g_Renderer.bitmapH;

		Vec4 quadVertices[] = {
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos + w, ypos,     offsetX + texX, 0.0f},
			{xpos + w, ypos + h, offsetX + texX, texY},
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos,     ypos + h, offsetX,        texY},
			{xpos + w, ypos + h, offsetX + texX, texY}
		};

		if (g_Renderer.vertexCount >= MAX_VERTICES) {

			renderer_end();
		}


		if (i == tokens[tokIndex].pos) {

			color = global_Colors[tokens[tokIndex].type];
			tokLen = tokens[tokIndex].length;
			tokIndex++;
		}


		for (i32 j = 0; j < VERTICES_PER_QUAD; ++j) {

			g_Renderer.vertexArrayIndex->color = color;
			g_Renderer.vertexArrayIndex->posData = quadVertices[j];
			g_Renderer.vertexArrayIndex->texIndex = FONT_TEXTURE_INDEX;
			g_Renderer.vertexArrayIndex++;
		}


		g_Renderer.vertexCount += VERTICES_PER_QUAD;
		advanceX += g_Renderer.glyphs[text[i]].advanceX;
	}

	#ifdef DEBUG

	Vec2 quadSize = {200.0f, 140.0f};
	Vec2 pos = {window->position.x + window->size.w - 200.0f, window->position.y};
	Vec4 quadColor = {0.2f, 0.2f, 0.2f, 0.8f};
	render_quad(pos, quadSize, quadColor);

	pos.x = (f32)window->position.x + window->size.w - 200.0f;
	pos.y = (f32)window->position.y;
	DEBUG_TEXT(pos, "position: [%i, %i]", window->position.x, window->position.y); pos.y += 20.0f;
	DEBUG_TEXT(pos, "size: [%i, %i]", window->size.w, window->size.h) pos.y += 20.0f;
	DEBUG_TEXT(pos, "start line: %i", window->renderView.start) pos.y += 20.0f;
	DEBUG_TEXT(pos, "end line: %i", window->renderView.end) pos.y += 20.0f;
	DEBUG_TEXT(pos, "window ID: %i", window->id) pos.y += 20.0f;
	DEBUG_TEXT(pos, "parent: %p", window->parent) pos.y += 20.0f;
	DEBUG_TEXT(pos, "adress: %p", window) pos.y += 20.0f;

	#endif

	str_release(text);
}

void
render_cursor(Buffer* buf, Window* win) {

	static Vec2 cursorPos, cursorSize;

	cursorPos = cursor_render_pos(buf, win);
	cursorSize = cursor_render_size(buf);
	// TODO: fix wierd offset
	cursorPos.y += renderer_font_size() / 5;
	render_quad(cursorPos, cursorSize, global_Colors[2]);
}



void
renderer_end() {

	static i32 count, dataSize;
	count = (g_Renderer.vertexArrayIndex - g_Renderer.vertexArray);
	dataSize = (g_Renderer.vertexArrayIndex - g_Renderer.vertexArray) * sizeof(Vertex);

	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, g_Renderer.vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, g_Renderer.vertexCount);

	g_Renderer.vertexCount = 0;
	g_Renderer.vertexArrayIndex = g_Renderer.vertexArray;

}

void
renderer_on_window_resize(f32 width, f32 height) {
  
	mat_ortho(g_Renderer.projection, 0.0f, width, height, 0.0f);
}

GlyphData* renderer_get_glyphs() {

	return g_Renderer.glyphs;
}

i32 renderer_font_size() {

	return g_Renderer.fontSize;
}

#ifdef DEBUG
void
render_text_debug(char* text, Vec2 position, Vec4 color) {

	static float xpos, ypos, w, h, offsetX, texX, texY, advanceX, advanceY;

	advanceY = position.y;
	advanceX = position.x;

	for (sizet i = 0; text[i] != '\0'; ++i) {

		xpos = advanceX + g_Renderer.glyphs[text[i]].bearingX;
		// this is stupid, idk how else to make it work
		ypos = advanceY - g_Renderer.glyphs[text[i]].bearingY + g_Renderer.fontSize;
		w = g_Renderer.glyphs[text[i]].width;
		h = g_Renderer.glyphs[text[i]].height;
		offsetX = g_Renderer.glyphs[text[i]].offsetX;
		texX = w / g_Renderer.bitmapW;
		texY = h / g_Renderer.bitmapH;

		Vec4 quadVertices[] = {
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos + w, ypos,     offsetX + texX, 0.0f},
			{xpos + w, ypos + h, offsetX + texX, texY},
			{xpos,     ypos,     offsetX,        0.0f},
			{xpos,     ypos + h, offsetX,        texY},
			{xpos + w, ypos + h, offsetX + texX, texY}
		};

		if (g_Renderer.vertexCount >= MAX_VERTICES) {

			renderer_end();
		}

		for (int j = 0; j < VERTICES_PER_QUAD; ++j) {

			g_Renderer.vertexArrayIndex->color = color;
			g_Renderer.vertexArrayIndex->posData = quadVertices[j];
			g_Renderer.vertexArrayIndex->texIndex = FONT_TEXTURE_INDEX;
			g_Renderer.vertexArrayIndex++;
		}

		g_Renderer.vertexCount += VERTICES_PER_QUAD;
		advanceX += g_Renderer.glyphs[text[i]].advanceX;
	}
}

#endif
