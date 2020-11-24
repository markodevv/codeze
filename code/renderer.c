#include "renderer.h"
#include "fileio.h"
#include "shader.h"
#include "debug.h"

#include <glad/glad.h>

#define MAX_VERTICES 100000
#define VERTICES_PER_QUAD 6

static void
error_callback(int code, const char* description) {

	printf("Error %i : %s \n", code, description);

}


GLFWwindow*
renderer_create_window() {

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
renderer_initialize(Renderer* ren, f32 width, f32 height) {

	ren->vertexArray = malloc(sizeof(Vertex) * MAX_VERTICES);
	ren->vertexArrayIndex = ren->vertexArray;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	File* vertexFile = file_open("code/vertex_shader.txt", "r");
	File* fragmentFile = file_open("code/fragment_shader.txt", "r");

	ren->program = shader_create(vertexFile->buffer, fragmentFile->buffer);
	
	glUseProgram(ren->program);

	glGenVertexArrays(1, &ren->VAO);
	glGenBuffers(1, &ren->VBO);

	glBindVertexArray(ren->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, ren->VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 4));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 4 * 2));

	ortho(ren->projection, 0.0f, width, height, 0.0f);

	texture_load("code/green.png", &ren->texIDs[GREEN_TEXTURE_INDEX], GREEN_TEXTURE_INDEX);
	texture_load("code/white.png", &ren->texIDs[WHITE_TEXTURE_INDEX], WHITE_TEXTURE_INDEX);

	i32 location1 = glGetUniformLocation(ren->program, "uTextures[0]");
	ASSERT(location1 != -1);

	glUniform1i(location1, GREEN_TEXTURE_INDEX);

	i32 location2 = glGetUniformLocation(ren->program, "uTextures[1]");
	ASSERT(location2 != -1);

	glUniform1i(location2, WHITE_TEXTURE_INDEX);

	file_close(vertexFile);
	file_close(fragmentFile);

}

void
renderer_load_font(Renderer* ren, const char* fontFile, i32 fontSize) {

	ren->fontSize = fontSize;

	i32 success = FT_Init_FreeType(&ren->ftLib);
	ASSERT(success == 0);
	success = FT_New_Face(ren->ftLib, fontFile, 0, &ren->fontFace);
	ASSERT(success == 0);

	FT_Set_Pixel_Sizes(ren->fontFace, 0, ren->fontSize);

	i32 w = 0, h = 0;
	FT_GlyphSlot glyph = ren->fontFace->glyph;

	for (u8 i = 0; i < 128; i++) {
		i32 success = FT_Load_Char(ren->fontFace, i, FT_LOAD_RENDER);
		ASSERT(success == 0);
		w += glyph->bitmap.width;
		glyph->bitmap.rows > h ? h = glyph->bitmap.rows : h;

	}

	ren->bitmapW = w;
	ren->bitmapH = h;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0 + FONT_TEXTURE_INDEX);

	glGenTextures(1, &ren->texIDs[FONT_TEXTURE_INDEX]);
	glBindTexture(GL_TEXTURE_2D, ren->texIDs[FONT_TEXTURE_INDEX]);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);


	i32 texOffset = 0;

	for (u8 i = 0; i < 128; ++i) {

		i32 success = FT_Load_Char(ren->fontFace, i, FT_LOAD_RENDER);
		ASSERT(success == 0);

		glTexSubImage2D(GL_TEXTURE_2D, 0, texOffset, 0, 
			glyph->bitmap.width, glyph->bitmap.rows, 
			GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

		ren->glyphs[i].advanceX = glyph->advance.x >> 6;
		ren->glyphs[i].advanceY = glyph->advance.y >> 6;

		ren->glyphs[i].width = glyph->bitmap.width;
		ren->glyphs[i].height = glyph->bitmap.rows;

		ren->glyphs[i].bearingX = glyph->bitmap_left;
		ren->glyphs[i].bearingY = glyph->bitmap_top;
	
		ren->glyphs[i].offsetX = (f32)texOffset / w;

		texOffset += glyph->bitmap.width;
	}

	i32 location = glGetUniformLocation(ren->program, "uTextures[2]");
	ASSERT(location != -1);

	glUniform1i(location, FONT_TEXTURE_INDEX);

	// Hardcoded tab size
	ren->glyphs['\t'].advanceX = ren->glyphs[' '].advanceX * 4;

	FT_Done_Face(ren->fontFace);
	FT_Done_FreeType(ren->ftLib);

  
}

void
renderer_begin(Renderer* ren) {

	glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	i32 projectionLocation = glGetUniformLocation(ren->program, "uProjection");
	ASSERT(projectionLocation != -1);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &ren->projection[0][0]);

}

void
render_quad(Renderer* ren, Vec2 position, Vec2 size, Vec4 color) {

		Vec4 quadVertices[] = {
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x + size.x, position.y,          1.0f, 0.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f},
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x,          position.y + size.y, 0.0f, 1.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f}
		};

		if (ren->vertexCount >= MAX_VERTICES) {
			renderer_end(ren);
		}

		for (int k = 0; k < VERTICES_PER_QUAD; ++k) {
			ren->vertexArrayIndex->color = color;
			ren->vertexArrayIndex->posData = quadVertices[k];
			ren->vertexArrayIndex->texIndex = NO_TEXTURE_INDEX;
			ren->vertexArrayIndex++;
		}

		ren->vertexCount += VERTICES_PER_QUAD;

}

void render_textured_quad(Renderer* ren, Vec2 position, Vec2 size, u32 texID) {


		Vec4 quadVertices[] = {
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x + size.x, position.y,          1.0f, 0.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f},
				{position.x,          position.y,          0.0f, 0.0f},
				{position.x,          position.y + size.y, 0.0f, 1.0f},
				{position.x + size.x, position.y + size.y, 1.0f, 1.0f}
		};

		if (ren->vertexCount >= MAX_VERTICES) {
			renderer_end(ren);
		}

		Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		for (int k = 0; k < VERTICES_PER_QUAD; ++k) {
			ren->vertexArrayIndex->color = color;
			ren->vertexArrayIndex->posData = quadVertices[k];
			ren->vertexArrayIndex->texIndex = texID;
			ren->vertexArrayIndex++;
		}

		ren->vertexCount += VERTICES_PER_QUAD;
	
}

void
renderer_end(Renderer* ren) {



	static i32 count, dataSize;
	count = (ren->vertexArrayIndex - ren->vertexArray);
	dataSize = (ren->vertexArrayIndex - ren->vertexArray) * sizeof(Vertex);

	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, ren->vertexArray);

	glDrawArrays(GL_TRIANGLES, 0, ren->vertexCount);

	ren->vertexCount = 0;
	ren->vertexArrayIndex = ren->vertexArray;

}
