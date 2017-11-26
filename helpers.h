#include <GL\glew.h>
#include <vector>
#include <glm\glm.hpp>

GLuint LoadShaders(char* vertex, char* fragment, char* geometry=NULL);
bool loadOBJ(const char *path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);
GLuint loadDDS(const char * imagepath);
GLuint loadBMP(const char * imagepath);
void indexVBO(std::vector<glm::vec3> & in_vertices,	std::vector<glm::vec2> & in_uvs, std::vector<glm::vec3> & in_normals, std::vector<unsigned int> & out_indices, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs,	std::vector<glm::vec3> & out_normals);
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();