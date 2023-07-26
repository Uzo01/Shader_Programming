#pragma once

#include "Scene.h"
#include "Shader.h"
#include "normMapper.h"





class BasicScene : public Scene
{
public:
	BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H); // constructor
	void update(float dt) override;   // per frame update
	void ProcessInput(GLFWwindow* window);
	FirstPersonCamera* m_camera;
	void setFBOcolour();
	//a seccond test comment

private:
	
	// Load Textures
	void loadTextFiles(FirstPersonCamera *buttonPress);

	unsigned int cubeDiff,cubeSpec, floorDiff, floorSpec, normCubeMap, normFloorMap;
	unsigned int specMap, diffMap, normMap;
	unsigned int loadTexture(char const* path);
	unsigned int textureID; // declare handle ID for texture
	char const* path;
	int m_width, m_height, m_nrComponents;
	int map;
	
	//Framebuffer
	unsigned int myFBO;
	unsigned int colourAttachment;

	void createBuffers();
	unsigned int m_floorVBO, m_cubeVBO, m_cube2VBO, m_floorEBO, m_cubeEBO, m_cube2EBO, m_cubeVAO, m_cube2VAO, m_floorVAO;
	void setUniforms(Shader* shader);
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_model;

	
	Shader* m_shader;
	Shader* m_floorShader;

	float Kc = 1.0;
	float Kl = 0.22f;
	float Ke = 0.2f;

	//lighting insitiliser
	glm::vec3 dirLight;
	glm::vec3 ColLight;
	glm::vec3 ColCube;
	glm::vec3 ColFloor;
	glm::vec3 Position;
	
	//Point light
	glm::vec3 pointLigPos;
	glm::vec3 poinLigCol;
	glm::vec3 pointLigPos2;
	/////
	glm::vec3 dirLigPos;


	//////////////////// cube data
	////////
	// vertices
	float m_cubeVertices[192] = {
		//back
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,// 0 
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, //1
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

	//front
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,//4
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

	//left
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//8
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

	//right
   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,//12
   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

   //bottom
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,//16
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

   //top	
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //20
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	};
	// indices for index draw calls
	unsigned int m_cubeIndices[36] = {
	    1,2,3,
		1,3,0,

		5,6,7,
		5,7,4,

		11,8,9,
		11,9,10,

		15,12,13,
		15,13,14,

		23,22,21,
		23,21,20,

		16,17,18,
		16,18,19,

	};
	

	//////////////////// floor data

	float floorSize = 5.0f;
	float floorLevel = -2.0f;

	float m_floorVertices[32] = {
		 -floorSize, floorLevel, -floorSize, 0.0, 1.0, 0.0, 0.0f, 0.0f,
		 floorSize, floorLevel,  -floorSize, 0.0, 1.0, 0.0, 1.0f, 0.0f,
		 floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,  1.0f,1.0f,
		-floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0,  0.0f,1.0f

	};

	unsigned int m_floorIndices[6] = {
		3,2,1,
		3,1,0
	};

	



};