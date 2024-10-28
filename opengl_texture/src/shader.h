#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath) {
            // get source code
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vertexShaderFile;
            std::ifstream fragmentShaderFile;

            vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

            try {
                vertexShaderFile.open(vertexPath);
                fragmentShaderFile.open(fragmentPath);
                std::stringstream vertexShaderStream, fragmentShaderStream;

                vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf();

                vertexShaderFile.close();
                fragmentShaderFile.close();

                vertexCode = vertexShaderStream.str();
                fragmentCode = fragmentShaderStream.str();
            } 
            catch (std::ifstream::failure e) {
                std::cout<<"error shader file not succesfully read\n"<<std::endl;
            }

            const char* vertexShaderCode = vertexCode.c_str();
            const char* fragmentShaderCode = fragmentCode.c_str();
            
            // compile shaders
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vertexShaderCode, NULL);
            glCompileShader(vertex);
            
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout<<"error vertex shader compilation failed\n"<< infoLog <<std::endl;
            }

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
            glCompileShader(fragment);

            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout<<"error fragment shader compilation failed\n"<< infoLog <<std::endl;
            }

            // shader program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout<<"error shader program linking failed\n"<< infoLog <<std::endl;
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use() 
        {
            glUseProgram(ID);
        }

        void setBool(const std::string &name, bool value) const 
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const 
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const 
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
};