#pragma once

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

	void initialize();
	void process();
	void render();
private:
	particle* parts;

	glm::mat4 ModelMatrix;
	MVPprovider* mvp;

	void createParticle(particle* init);
	void evolveParticle(particle* evolve);
};

