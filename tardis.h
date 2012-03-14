#pragma once

#include <stdlib.h>
#include <vector>
using namespace std;

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GL\glew.h>

class Tardis
{
public:
	Tardis();
	~Tardis();

	void initialize(char* model, char* diffuseMap, char* ambiantMap);
	void process();
	void render();

private:
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	GLuint LightID;
	glm::vec3 lightPos;

	glm::mat4 ModelMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 MVP;

	GLuint Texture;
	GLuint TextureID;
	GLuint ambiantTexture;
	GLuint ambiantID;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	int indicesSize;
};
