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

	glActiveTexture(GL_TEXTURE1);

	i32 w, h, bpp;
	u8* buffer = image_load_png("code/white.png", &w, &h, &bpp);
	ASSERT(buffer != NULL);


	glGenTextures(1, &ren->texIDs[0]);
	glBindTexture(GL_TEXTURE_2D, ren->texIDs[0]);

	glGenTextures(1, &ren->texIDs[1]);
	glBindTexture(GL_TEXTURE_2D, ren->texIDs[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	i32 whiteLocation = glGetUniformLocation(ren->program, "uWhiteTexture");
	ASSERT(whiteLocation != -1);

	glUniform1i(whiteLocation, 1);

	file_close(vertexFile);
	file_close(fragmentFile);

	image_free(buffer);
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
			ren->vertexArrayIndex->texIndex = 1;
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
