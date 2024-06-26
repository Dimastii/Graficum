#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/linmath.h>

//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <iostream>

//
#include "GrDebug.h"
#include "GrUtils.h"
#include "Globals.h"
#include "Callbacks.h"
#include "Shader.h"

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;

static const Vertex vertices[6] =
{
    { { -0.9f, -0.1f }, { 1.f, 0.f, 0.f } },
    { { -0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { { -0.8f,  -0.6f }, { 0.f, 0.f, 1.f } },

	{ { 0.5f, -0.5f }, { 1.f, 0.f, 0.f } },
	{ { 0.7f, -0.7f }, { 1.f, 0.f, 0.f } },
	{ { 0.8f, -0.4f }, { 1.f, 0.f, 0.f } }
};

static void setWinHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);


	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	int monitor_w = mode->width;
    int monitor_h = mode->height;

	std::cout << "Resolution: " << monitor_w << "x" << monitor_h << std::endl;

	glfwWindowHint(GLFW_POSITION_X, 0/*monitor_w - WIN_W*/);
	glfwWindowHint(GLFW_POSITION_Y, monitor_h - WIN_H);

}

static GLFWwindow* createMainWin()
{
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "OpenGL Triangle", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowOpacity(window, 0.5f);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
    return window;
}

int main(void)
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    gl_log("Start\n");

    setWinHints();

    GLFWwindow* window = createMainWin();

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader::getInstance().init(VERTEX_FILE, FRAGMENT_FILE);

    const GLint mvp_location = glGetUniformLocation(Shader::getInstance().ID, "MVP");
    const GLint vpos_location = glGetAttribLocation(Shader::getInstance().ID, "vPos");
    const GLint vcol_location = glGetAttribLocation(Shader::getInstance().ID, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, col));
    while (!glfwWindowShouldClose(window))
    {     
        // see GLFW_DECORATED
        // _update_fps_counter(window); 
        float time = (float)glfwGetTime();
        float speed = time / 4;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glClear(GL_COLOR_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, speed);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

		float greenValue = (sin(speed) / 2.0f) + 0.5f;
        int green_location = glGetUniformLocation(Shader::getInstance().ID, "green");
		Shader::getInstance().use();

        glUniform1f(green_location, greenValue);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	gl_log("Stop\n");

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}