#include "shader.h"
#include "debug.h"

#include <glad/glad.h>


static u32
compile_shader(unsigned int type, const char* source) {

	u32 id = glCreateShader(type);

	if (type == GL_VERTEX_SHADER) {
		glShaderSource(id, 1, &source, NULL);
		glCompileShader(id);
	}
	else if (type == GL_FRAGMENT_SHADER) {
		glShaderSource(id, 1, &source, NULL);
		glCompileShader(id);
	}

	i32 isCompiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		i32 length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* log = (char*)malloc(sizeof(char) * length);
		glGetShaderInfoLog(id, length, &length, log);
		printf("Failed to compile shader error: %s \n", log);
		printf("Shader: %s \n", source);


		glDeleteShader(id);
		free(log);

		return 0;
	}

	return id;

}

u32 shader_create(const char* vertex, const char* fragment) {

	u32 shader = glCreateProgram();

	u32 vertexShader = compile_shader(GL_VERTEX_SHADER, vertex);
	u32 fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragment);

	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);
	
	i32 isLinked = 0;
	glGetProgramiv(shader, GL_LINK_STATUS, (i32*)&isLinked);
	if (isLinked == GL_FALSE) {

		i32 length = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

		char* log = (char*)malloc(sizeof(char) * length);
		glGetProgramInfoLog(shader, length, &length, log);

		printf("Shader linking failed: %s \n", log);

		glDeleteShader(shader);

		free(log);

		ASSERT(0);
	}

	glValidateProgram(shader);

	glDetachShader(shader, vertexShader);
	glDetachShader(shader, fragmentShader);

	return shader;
}
