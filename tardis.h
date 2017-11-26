#pragma once

#include <stdlib.h>
#include <vector>
using namespace std;

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GL\glew.h>

#include "MVPprovider.h"

class Tardis
{
public:
	Tardis();
	~Tardis();

	void initialize(char* model="tardis.object", char* diffuseMap="tardis.dds", char* ambiantMap="tardis-ambiant.dds");
	void process();
	void render();

	glm::mat4 ModelMatrix;

private:
	GLuint programID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	GLuint LightID;
	glm::vec3 lightPos;

	MVPprovider* mvp;

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

