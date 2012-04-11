#pragma once

#include <vector>
using namespace std;

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "MVPprovider.h"

struct particle
{
	float lifetime;
	glm::vec3 rgb;
	glm::vec3 position;
	glm::vec3 speed;
};

class Vortex
{
public:
	Vortex(void);
	~Vortex(void);

	float distance;

	void initialize(char* texturePath = "particle.dds", char* alphaTexture = "particle-alpha.dds");
	void process();
	void render();
private:
	vector<glm::vec3> particles;
	vector<glm::vec3> speeds;
	vector<glm::vec2> sprites;

	glm::mat4 ModelMatrix;
	MVPprovider* mvp;

	GLuint ProgramID;
	GLuint ProcessingProgram;

	GLuint GeneratorStateID;
	float generatorState;
	GLuint DistanceID;

	GLuint ModelViewID;
	GLuint ProjectionID;
	GLuint ParticleSizeID;
	GLuint Texture;
	GLuint TextureSamplerID;
	GLuint AlphaTexture;
	GLuint AlphaTextureSamplerID;

	GLuint ParticleBufferID;
	GLuint SpritesBufferID;
	GLuint ParticleChanged;
	GLuint SpeedBufferID;
	GLuint SpeedChanged;

	glm::vec3 initParticle();

	GLuint preapareProcessingShader();
};

