//
//  ShaderIO.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ShaderIO__
#define __ShaderIO__

#include <string>

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <map>

typedef std::map<std::string, GLuint> ProgramMap;

class programManager
{
	ProgramMap _programs;
	
public:
	static programManager& sharedInstance();
	
public:
	GLuint createProgram(std::string program_id,
						 const char* vertex_shader_filename,
						 const char* fragment_shader_filename);
public:
	GLuint programWithID(std::string program_id) const;
};

#endif /* defined(__ShaderIO__) */
