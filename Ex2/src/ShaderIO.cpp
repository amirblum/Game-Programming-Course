//
//  ShaderIO.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <vector>

#include <unistd.h>

static programManager _sharedInstance;

namespace ShaderIO {
	
    std::string FindFileOrThrow( const std::string &strBasename )
    {
        std::string strFilename = strBasename;
        std::ifstream testFile(strFilename.c_str());
        if (testFile.is_open())
            return strFilename;
        else {
            throw std::runtime_error("Could not open the file " + strBasename);
        }
    }
    
    // Compile the shader of the specified type from the provided string.
    // If compilation was nor successful, report any errors.
    GLuint CompileShader(GLenum eShaderType, const std::string &strShaderSource)
    {
        GLuint shader = glCreateShader(eShaderType);
        const char *shaderSource = strShaderSource.c_str();
        glShaderSource(shader, 1, &shaderSource, NULL);
        
        glCompileShader(shader);
        
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
            
            const char *strShaderType = NULL;
            switch(eShaderType)
            {
                case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
                case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
                case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
            }
            
            fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
            delete[] strInfoLog;
        }
        
        return shader;
    }
    
    // Link the (already compiled) shaders in the provided list into a program.
    // If linking was not successful, report any errors.
    GLuint LinkProgram(const std::vector<GLuint> &shaderList)
    {
        GLuint program = glCreateProgram();
        
        for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
            glAttachShader(program, shaderList[iLoop]);
        
        glLinkProgram(program);
        
        GLint status;
        glGetProgramiv (program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
            fprintf(stderr, "Linker failure: %s\n", strInfoLog);
            delete[] strInfoLog;
        }
        
        for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
            glDetachShader(program, shaderList[iLoop]);
        
        return program;
    }
    
    GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename)
    {
        try
        {
            std::string strFilename = FindFileOrThrow(strShaderFilename);
            std::ifstream shaderFile(strFilename.c_str());
            std::stringstream shaderData;
            shaderData << shaderFile.rdbuf();
            shaderFile.close();
            return CompileShader(eShaderType, shaderData.str());
        }
        catch(std::exception &e)
        {
            fprintf(stderr, "%s\n", e.what());
            throw;            
        }
    }
    
    GLuint CreateProgram(const std::vector<GLuint> &shaderList)
    {
        try
        {
            GLuint prog = LinkProgram(shaderList);
            std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
            return prog;
        }
        catch(std::exception &e)
        {
            std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
            fprintf(stderr, "%s\n", e.what());
            throw;
        }
    }
    
}
	
programManager& programManager::sharedInstance()
{
	return _sharedInstance;
}
	
GLuint programManager::createProgram(std::string program_id,
									 const char* vertex_shader_filename,
									 const char* fragment_shader_filename)
{
	// Create a list of all the shaders that constitute our program
	std::vector<GLuint> shaderList;
	shaderList.push_back(ShaderIO::LoadShader(GL_VERTEX_SHADER, vertex_shader_filename));
	shaderList.push_back(ShaderIO::LoadShader(GL_FRAGMENT_SHADER, fragment_shader_filename));
	
	// Link the program
	GLuint theProgram = ShaderIO::CreateProgram(shaderList);
	
	// Store program handle for program id:
	_programs[program_id] = theProgram;
	
	return theProgram;
}

GLuint programManager::programWithID(std::string program_id) const
{
	ProgramMap::const_iterator iterator_to_program = _programs.find(program_id);
	if (iterator_to_program == _programs.end())
	{
		return 0;
	}
	else
	{
		return iterator_to_program->second;
	}
}

