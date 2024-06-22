#pragma once



bool gl_log(const char* message, ...);
void gl_log_params();
void gl_log_shader_info(GLuint shader_index);

bool gl_log_err(const char* message, ...);
void glfw_error_callback(int error, const char* description);


