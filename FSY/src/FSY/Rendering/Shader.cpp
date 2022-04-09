#include "Shader.h"
#include <fstream>
#include <sstream>
#include "../../vendor/OpenGL/include/glad.h"
#include "../Editor/Console.h"

namespace FSY {

    Shader::Shader() {

    }

    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        //Store the Shader Code 
        std::string vertexCode;
        std::string fragmentCode;
        //the file in which the code is located
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            //the code gets temporarly stored here
            std::stringstream vShaderStream, fShaderStream;
            // reads the file and copies it to the string streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert streams into strings
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            Console::Error("[SHADER::FILE_NOT_SUCCESFULLY_READ]: " + std::string("--> Check locations: ") + std::string(vertexPath) + " or " + std::string(fragmentPath) );
        }

        //convert to C-Strings (OpenGL needs it, because it's written in C)
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        ////////////////
        //COMPILING & LINKING
        ////////////////

        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            Console::Error("[SHADER::VERTEX::COMPILATION_FAILED]:\n" + std::string(infoLog));
            return;
        };

        //fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        int  success_;
        char infoLog_[512];
        //again, print compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success_);
        if (!success_)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog_);
            Console::Error("[SHADER::FRAGMENT::COMPILATION_FAILED]:\n" + std::string(infoLog_));
        }

        // shader Program & Linking
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            Console::Error("[SHADER::PROGRAM::LINKING_FAILED]:\n" + std::string(infoLog));
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader() {
        
    }

    //Uses the shader
    void Shader::Use() {
        glUseProgram(ID);
    }

    void Shader::Delete() {
        glDeleteProgram(ID);
    }

    ////////////////////////////
    /// Setting Values in the shader
    ////////////////////////////

    void Shader::setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setColorValues3(const std::string& name, float r, float g, float b) const{
        glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
    }

    void Shader::setColorValues4(const std::string& name, float r, float g, float b, float a) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
        glUniform1f(glGetUniformLocation(ID, "Transparency"), a);
    }

    void Shader::setVec3(const std::string& name, float x, float y, float z) const{
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

}