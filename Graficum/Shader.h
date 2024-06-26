#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
	Shader() = default;
public:
	static Shader& getInstance();
	unsigned int ID;

	void init(const char* vertexPath, const char* fragmentPath);

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float val) const;
};
