#include "helpers.h"
#include <GL\glfw.h>

#include <stdio.h>
#include <fstream>
#include <string>

#include "Vortex.h"

Vortex::Vortex(void)
{
	mvp = MVPprovider::Instance();

	distance = 40;
	generatorState = 0;
}

Vortex::~Vortex(void)
{
}

glm::vec3 Vortex::initParticle()
{
	return glm::vec3((rand() % 5)/5.0f, (rand() % 5)/5.0f, 0);
}

void Vortex::initialize(char* texturePath, char* alphaTexturePath)
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	ProgramID = LoadShaders("vortexVertex.glsl", "vortexFragment.glsl", "vortexGeometry.glsl");

	ModelViewID = glGetUniformLocation(ProgramID, "MV");
	ProjectionID = glGetUniformLocation(ProgramID, "P");
	ParticleSizeID = glGetUniformLocation(ProgramID, "particleSize");
	
	//Texture = loadDDS(texturePath);
	//TextureSamplerID = glGetUniformLocation(ProgramID, "textureSampler");
	AlphaTexture = loadDDS(alphaTexturePath);
	AlphaTextureSamplerID = glGetUniformLocation(ProgramID, "alphaTextureSampler");

	for (int i=0; i < 1000; i++)
	{
		particles.push_back(glm::vec3(0.0f, 0.0f, distance+1));
		speeds.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		sprites.push_back(glm::vec2(rand() % 4, rand() % 4));
	}
	
	glGenBuffers(1, &ParticleBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferID);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(glm::vec3), &particles[0], GL_STREAM_DRAW);

	glGenBuffers(1, &ParticleChanged); //изменившиеся позиции
	glBindBuffer(GL_ARRAY_BUFFER, ParticleChanged);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(glm::vec3), 0, GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &SpeedBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, SpeedBufferID);
	glBufferData(GL_ARRAY_BUFFER, speeds.size() * sizeof(glm::vec3), &speeds[0], GL_STREAM_DRAW);

	glGenBuffers(1, &SpeedChanged); //изменившиеся скорости
	glBindBuffer(GL_ARRAY_BUFFER, SpeedChanged);
	glBufferData(GL_ARRAY_BUFFER, speeds.size() * sizeof(glm::vec3), 0, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &SpritesBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, SpritesBufferID);
	glBufferData(GL_ARRAY_BUFFER, sprites.size() * sizeof(glm::vec2), &sprites[0], GL_STATIC_DRAW);

	ProcessingProgram = preapareProcessingShader();

	ModelMatrix = glm::mat4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);
}

GLuint Vortex::preapareProcessingShader()
{
	GLuint ProgramID = glCreateProgram();
	GLuint ProcessingShaderID = glCreateShader(GL_VERTEX_SHADER);

	std::string VertexShaderCode;
	std::ifstream VertexShaderStream("particleVertex.glsl", std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	const char* ShaderSourcePointer = VertexShaderCode.c_str();
	glShaderSource(ProcessingShaderID, 1, &ShaderSourcePointer, NULL);
	glCompileShader(ProcessingShaderID);
	glAttachShader(ProgramID, ProcessingShaderID);

	const char* state[2] = { "particlePosition", "particleSpeed" };
	glTransformFeedbackVaryings(ProgramID, 2, state, GL_SEPARATE_ATTRIBS);

	glLinkProgram(ProgramID);
	
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	GeneratorStateID = glGetUniformLocation(ProgramID, "state");
	DistanceID = glGetUniformLocation(ProgramID, "maxDistance");

	glDeleteShader(ProcessingShaderID);

	return ProgramID;
}

void Vortex::process()
{
	glDisable(GL_DEPTH_TEST);

	generatorState++; //для начала обновляю состояние генератора частиц
	if (generatorState > 100) generatorState = 1;

	glUseProgram(ProcessingProgram);

	glEnable(GL_RASTERIZER_DISCARD);

	glUniform1f(GeneratorStateID, generatorState);
	glUniform1f(DistanceID, distance);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferID); //буфер с центрами частиц
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, SpeedBufferID); //буфер с их скоростями
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleChanged);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, SpeedChanged);

	glBeginTransformFeedback(GL_POINTS); //теперь происходит собственно обработка
	glDrawArrays(GL_POINTS, 0, particles.size());
	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glDisable(GL_RASTERIZER_DISCARD);

	std::swap(ParticleBufferID, ParticleChanged); //меняю буферы местами
	std::swap(SpeedBufferID, SpeedChanged);
}

void Vortex::render()
{
	glUseProgram(ProgramID); //выбираю программу с шейдерами

	glm::mat4 ModelViewMatrix = getViewMatrix() * ModelMatrix; //подаю матрицы в шейдер
	glUniformMatrix4fv(ModelViewID, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &(getProjectionMatrix())[0][0]);
	//glm::mat4 ModelViewMatrix = ModelMatrix * mvp->View();
	//glUniformMatrix4fv(ModelViewID, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	//glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &(mvp->Projection())[0][0]);
	glUniform2f(ParticleSizeID, 0.8f, 0.8f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferID); //буфер с центрами частиц
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, SpritesBufferID); //буфер с центрами частиц
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)NULL);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture); //текстура будет в нулевом модуле
	//glUniform1i(TextureSamplerID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, AlphaTexture);
	glUniform1i(AlphaTextureSamplerID, 1);

	glDrawArrays(GL_POINTS, 0, particles.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
