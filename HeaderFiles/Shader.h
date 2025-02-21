#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ShaderID;

	//constructor
	Shader(const char* vertexPath, const char *fragmentPath){
		//get source code from filePaths
		std::string VertexCode;
		std::string FragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions (std::ifstream::failbit |std::ifstream::badbit);
		fShaderFile.exceptions (std::ifstream::failbit |std::ifstream::badbit);
		try{
			///open
			vShaderFile.open(vertexPath);
			std::stringstream vShaderStream;
			//read
			vShaderStream<<vShaderFile.rdbuf();
			vShaderFile.close();
			VertexCode=vShaderStream.str();

		}
		catch(std::ifstream::failure& e){
			std::cerr<<"Error: Vertex shader file not succesfully read\n"<<e.what()<<std::endl;
		}
		try{
			///open
			fShaderFile.open(fragmentPath);
			std::stringstream fShaderStream;
			//read
			fShaderStream<<fShaderFile.rdbuf();
			fShaderFile.close();
			FragmentCode=fShaderStream.str();

		}
		catch(std::ifstream::failure& e){
			std::cerr<<"Error: Fragment shader file not succesfully read\n"<<e.what()<<std::endl;
		}
		const char* vShaderCode=VertexCode.c_str();
		const char* fShaderCode=FragmentCode.c_str();
		unsigned int vertexShader,fragmentShader;
		compile_shader_source(vShaderCode, GL_VERTEX_SHADER, &vertexShader);
		compile_shader_source(fShaderCode, GL_FRAGMENT_SHADER, &fragmentShader);

		link_program(vertexShader, fragmentShader, &ShaderID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use(){
		glUseProgram(ShaderID);
	}
	//utility uniform functions
	void setBool(const std::string &name, bool value) const{         
		glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), (int)value); 
	}
	// ------------------------------------------------------------------------
		void setInt(const std::string &name, int value) const{ 
		glUniform1i(glGetUniformLocation(ShaderID, name.c_str()), value); 
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const{ 
		glUniform1f(glGetUniformLocation(ShaderID, name.c_str()), value); 
	}    // ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{ 
		glUniform2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]); 
	}
	void setVec2(const std::string &name, float x, float y) const
	{ 
		glUniform2f(glGetUniformLocation(ShaderID, name.c_str()), x, y); 
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{ 
		glUniform3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]); 
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{ 
		glUniform3f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z); 
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{ 
		glUniform4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, &value[0]); 
	}
	void setVec4(const std::string &name, float x, float y, float z, float w) const
	{ 
		glUniform4f(glGetUniformLocation(ShaderID, name.c_str()), x, y, z, w); 
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:

	const char *shader_type_str(GLuint shader){
	    switch (shader) {
		case GL_VERTEX_SHADER:
		    return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER:
		    return "GL_FRAGMENT_SHADER";
		default:
		    return "Unkown shader";
	    }
	}
	bool compile_shader_source(const char* source, GLenum shader_type, unsigned int *shader){
	    
	    *shader=glCreateShader(shader_type);
	    glShaderSource(*shader,1,&source,NULL);
	    glCompileShader(*shader);

	    GLint compiled=0;
	    glGetShaderiv(*shader,GL_COMPILE_STATUS,&compiled);

	    if(!compiled){
		char InfoLog[1024];
		glGetShaderInfoLog(*shader,1024,NULL,InfoLog);
		std::cerr<<"Error: could not compile "<< shader_type_str(*shader)<<"\n"<<InfoLog<<std::endl;
		return false;
	    }
	    return true;
	}
	bool link_program(GLuint vertex_shader, GLuint fragment_shader, GLuint* program){

	    *program= glCreateProgram();

	    glAttachShader(*program,vertex_shader);
	    glAttachShader(*program,fragment_shader);

	    glLinkProgram(*program);

	    GLint linked=0;
	    glGetProgramiv(*program,GL_LINK_STATUS,&linked);
	    if(!linked){
		char InfoLog[1024];
		glGetProgramInfoLog(*program,1024,NULL,InfoLog);
		std::cerr<<"Error: Program linking failed\n"<<InfoLog<<std::endl;
		return false;

	    }
	    return true;


	}
};


#endif
