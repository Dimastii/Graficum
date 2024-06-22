#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "GrDebug.h"

#include <time.h>
#include <stdarg.h>
#include <iostream>



#define GL_LOG_FILE "gl.log"

void glfw_error_callback(int error, const char* description) {
	gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

bool gl_log_err(const char* message, ...) {
	va_list argptr;
	FILE* file;
	fopen_s(&file, GL_LOG_FILE, "a");
	if (!file) {
		fprintf(stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE);
		return false;
	}
	fprintf(file, "%s|%s|", __DATE__, __TIME__);
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool gl_log(const char* message,  ...) {
	va_list argptr;
	FILE* file;
	fopen_s(&file, GL_LOG_FILE, "a");
	if (!file) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}
	fprintf(file, "%s|%s|", __DATE__, __TIME__);
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

void gl_log_shader_info(GLuint shader_index) {
	int max_length = 2048;
	int actual_length = 0;
	char shader_log[2048];
	glGetShaderInfoLog(shader_index, max_length, &actual_length, shader_log);
	gl_log("shader info log for GL index %u:\n%s\n", shader_index, shader_log);
}

void gl_log_params() {
	GLenum params[] = {
	  GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
	  GL_MAX_CUBE_MAP_TEXTURE_SIZE,
	  GL_MAX_DRAW_BUFFERS,
	  GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
	  GL_MAX_TEXTURE_IMAGE_UNITS,
	  GL_MAX_TEXTURE_SIZE,
	  GL_MAX_VARYING_FLOATS,
	  GL_MAX_VERTEX_ATTRIBS,
	  GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
	  GL_MAX_VERTEX_UNIFORM_COMPONENTS,
	  GL_MAX_VIEWPORT_DIMS,
	  GL_STEREO,
	};
	const char* names[] = {
	  "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
	  "GL_MAX_DRAW_BUFFERS",
	  "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
	  "GL_MAX_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_TEXTURE_SIZE",
	  "GL_MAX_VARYING_FLOATS",
	  "GL_MAX_VERTEX_ATTRIBS",
	  "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
	  "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
	  "GL_MAX_VIEWPORT_DIMS",
	  "GL_STEREO",
	};
	gl_log("GL Context Params:\n");
	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++) {
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], (unsigned int)s);
	gl_log("-----------------------------\n");
}