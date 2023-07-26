#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"

#include <vector>

struct  vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 biTan;
};

class normMapper
{
public:
	normMapper();
	void calcTanandBitan(float* vertexData, int length, unsigned int* indiciesData, int indLenght);
	std::vector<float>getUpdatedVerexData();
	
private:
	void unPackVertices();
	void extractVertices(float* vertexData, int length);
	void computeTanandBiTan(unsigned int* indiciesData, int indLenght);
	std::vector<vertex> vertices;
	std::vector<float> updatedVertexData;
	
};