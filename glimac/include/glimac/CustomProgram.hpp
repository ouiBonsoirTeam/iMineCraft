#pragma once

#include <glimac/Program.hpp>

namespace glimac
{

	struct GeneralProgram
	{
		Program m_Program;

		GLint uMVPMatrix;
		GLint uMVMatrix;
		GLint uNormalMatrix;

		GeneralProgram(const FilePath& applicationPath):
			m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/normals.fs.glsl"))
		{
			uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
			uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
			uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
		}
	};

	struct pointLightProgram
	{
		Program m_Program;

		GLint uMVPMatrix;
		GLint uMVMatrix;
		GLint uNormalMatrix;
		GLint uShininess;
		GLint uKs;
		GLint uKd;
		GLint uLightPos_vs;
		GLint uLightIntensity;

		pointLightProgram(const FilePath& applicationPath):
			m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/pointlight.fs.glsl"))
		{
			uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
			uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
			uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
			uShininess = glGetUniformLocation(m_Program.getGLId(), "uShininess");
			uKs = glGetUniformLocation(m_Program.getGLId(), "uKs");
			uKd = glGetUniformLocation(m_Program.getGLId(), "uKd");
			uLightPos_vs = glGetUniformLocation(m_Program.getGLId(), "uLightPos_vs");
			uLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
		}
	};
	
}