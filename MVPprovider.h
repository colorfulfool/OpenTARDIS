#pragma once

#include <glm\glm.hpp>

class MVPprovider
{
public:
	static MVPprovider* Instance();

	void calculateMatrices();

	glm::mat4 View() { return ViewMatrix; }
	glm::mat4 Projection() { return ProjectionMatrix; }

	void setScreenSize(int width, int height) {	screenWidth = width; screenHeight = height;	}

protected:
	MVPprovider(void);
	~MVPprovider(void);

private:
	static MVPprovider* _instance;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::vec3 position; 
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;

	float speed;
	float mouseSpeed;

	int screenHeight;
	int screenWidth;

	double lastTime;
};

