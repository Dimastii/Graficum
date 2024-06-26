#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Callbacks.h"
#include "GrDebug.h"
#include "Shader.h"
#include "Globals.h"

#include <stddef.h>
#include <stdio.h>
#include <iostream>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		gl_log("%s:GLFW_KEY_F5\n", __FUNCTION__);
		Shader::getInstance().init(VERTEX_FILE, FRAGMENT_FILE);
	}
}