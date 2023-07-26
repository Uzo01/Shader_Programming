#include "BasicScene.h"
#include "Scene.h"
#include "FPSCamera.h"
#include <stb_image.h>
#include "../../STB image/STB image/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using  namespace std;
BasicScene::BasicScene(GLFWwindow* window, std::shared_ptr<InputHandler> H) : Scene(window, H)
{
	m_camera = new FirstPersonCamera();
	m_camera->attachHandler(window, H);
	m_shader = new Shader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
	m_floorShader = new Shader("..\\shaders\\plainVert.vs", "..\\shaders\\floorFrag.fs");
	//load textures
	cubeDiff = loadTexture("..\\resources\\metalPlate\\diffuse.jpg");
	cubeSpec = loadTexture("..\\resources\\metalPlate\\specular.jpg");
	normCubeMap = loadTexture("..\\resources\\metalPlate\\normal.jpg");


	floorDiff = loadTexture("..\\resources\\metalPlate\\diffuse.jpg");
	floorSpec = loadTexture("..\\resources\\metalPlate\\specular.jpg");
	normFloorMap = loadTexture("..\\resources\\metalPlate\\normal.jpg");
	


	
	createBuffers();
	dirLight = glm::vec3(0, -1, 0);
	ColLight = glm::vec3(1.0, 1.0, 1.0); //colour of lighting
	ColCube = glm::vec3(1.0, 0.0, 0.0);// cube color
	ColFloor = glm::vec3(3.0, 1.0, 0.0); //floor colour
	pointLigPos = glm::vec3(2.0, 3.0, 4.0);
	pointLigPos2 = glm::vec3(6.0, 3.0, 2.0);
	poinLigCol = glm::vec3(1.0, 0.0, 0.0);
	

	

	glEnable(GL_DEPTH_TEST);

}

void BasicScene::update(float dt)
{
	m_camera->update(dt); // check for input

	// update transforms //MVP
	
	m_projection = glm::perspective(m_camera->getFOV(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);   // near plane, far plane - these could/should be global properties or member variables
	m_view = m_camera->getViewMatrix();
	m_model = glm::mat4(1.0f);
	
	setUniforms(m_shader);
	/*
	
	*/

	m_shader->setInt("CubeDiffTex", 0);
	m_shader->setInt("CubeSpecTex", 1);
	m_shader->setInt("CubeNormalMap", 2);
	//Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeDiff);//Diffuse Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubeSpec);//Specular Texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normCubeMap);//Specular Texture

	// set uniforms - why do we set this each frame?
	//Cube shader
	
	// create additional cubes
	m_model = glm::translate(m_model, glm::vec3(0.0, 0.0, 0.0)); //update the model matrix
	m_shader->setMat4("model", m_model);
	m_shader->setMat4("projection", m_projection);
	m_shader->setMat4("view", m_view);
	m_shader->setVec3("viePos", m_camera->getPosition());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?
	glBindVertexArray(m_cubeVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Second cube 
	m_model = glm::translate(m_model, glm::vec3(0.0, 0.0, 5.0));//update model matrix to a different point
	m_shader->setMat4("model", m_model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// rotate cube 
	m_model = glm::mat4(1.0f); //reset the model matrix
	m_model = glm::translate(m_model, glm::vec3(0.0, 0.0, -5.0)); //translate
	m_model = glm::rotate(m_model,(float)(glfwGetTime ()), glm::vec3(2.0, 2.0, 2.0)); //roate by each frame
	m_model = glm::scale(m_model, glm::vec3(1.5)); //scale
	m_shader->setMat4("model", m_model); // update the uniform in shader
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // then draw shape

	pointLigPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	pointLigPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

	//floor shader

	setUniforms(m_floorShader);

	
	m_floorShader->setInt("floorDiffText", 0);
	m_floorShader->setInt("floorSpecText", 1);
	m_floorShader->setInt("floorNormalMap", 2);
		//Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorDiff);//Diffuse Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorSpec);//Specular Texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normFloorMap);//Specular Texture

	m_model = glm::mat4(1.0f);
	m_floorShader->setMat4("model", m_model);
	m_floorShader->setMat4("projection", m_projection);
	m_floorShader->setMat4("view", m_view);
	m_floorShader->setVec3("viePos", m_camera->getPosition());
	glBindVertexArray(m_floorVAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	
	//pass to FBO
	/*glBindFramebuffer(GL_FRAMEBUFFER, myFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);*/
	


}

void BasicScene::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (map == 1) map == 0;
		else map = 1;
	};
	
	
}

void BasicScene::setFBOcolour()
{
	glGenFramebuffers(1, &myFBO);//Generate 1 frambuffer myFBO
	glBindFramebuffer(GL_FRAMEBUFFER, myFBO);//Bind framebuffer
	glGenTextures(1, &colourAttachment);//Generate 1 texture colourAttachment
	glBindTexture(GL_TEXTURE_2D, colourAttachment);//Bind colourAttachment
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);//Set parameter for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//Take minimum of nearest
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Take mag of nearest
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);//Bind colourAttachment to framebuffer
}


void BasicScene::createBuffers()
{

	/*VAO stuff  - when you are comfortable what all of this is and what it is for - abstract to classes:
	  cube and plane class ( can use for platforms, ceilings, floors,  walls, etc.)
	  
	  Should abstract these buffer classes too VAO class, VBO class, etc. You will learn more about this in your Game Engine class.
	  */
	//
	//normMapper m_normMap;
	//m_normMap.calcTanandBitan(m_cubeVertices, 192, m_cubeIndices, 36);
	//std::vector<float> updatedCubeVert = m_normMap.getUpdatedVerexData(); //updated cuber vert in a vector
	normMapper m_normCubeMapper;
	m_normCubeMapper.calcTanandBitan(m_cubeVertices, 192, m_cubeIndices, 36);
	std::vector<float> updateCubeVertices = m_normCubeMapper.getUpdatedVerexData();

	  // Cube
	glGenVertexArrays(1, &m_cubeVAO);  // generate 1 VAO buffer called m_cubeVAO
	glGenBuffers(1, &m_cubeVBO);       // generate buffer called m_cubeVBO  - this is for vertex data
	glGenBuffers(1, &m_cubeEBO);       // another called EBO for index info

	glBindVertexArray(m_cubeVAO);  // bind VAO
	// fill VBO with vertex data

	
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);  // bind EBO
	glBufferData(GL_ARRAY_BUFFER, updateCubeVertices.size() * sizeof(GLfloat), updateCubeVertices.data(), GL_STATIC_DRAW);   // 'send' or copy the data in m_cubeVertices from CPU to GPU, 'STATIC_DRAW' is a hint to OpenGL that we will not be updating the data in this buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_cubeIndices), m_cubeIndices, GL_STATIC_DRAW);

    // tell OpenGl how to interpret the data in VBO
	// This is 'stored' in VAO along with VBO so we only need to do this once and whenever we bind m_cubeVAO we bind the data and instructions on how to interpret
	// Whenever we bind m_cubeVAO we are now dealing with m_cubeVBO, m_cubeEBO, and the following instructions. All with one command: glBindVertexArray(m_cubeVAO);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0); //params: at position 0, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 0 here)
	glEnableVertexAttribArray(0);  // first attribute
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));  //params: at position 2; position of uv, length 2; UV has 2 values  
	glEnableVertexAttribArray(2);
	//// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));  //params: at position 3, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(3);
	//// biTan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));  //params: at position 2; position of uv, length 2; UV has 2 values  
	glEnableVertexAttribArray(4);
	
	normMapper m_normFloorMapper;
	m_normFloorMapper.calcTanandBitan(m_floorVertices, 32, m_floorIndices, 6);
	std::vector<float> updateFloorVertices = m_normFloorMapper.getUpdatedVerexData();
	//Floor
	glGenVertexArrays(1, &m_floorVAO);
	glGenBuffers(1, &m_floorVBO);//floor VBO
	glGenBuffers(1, &m_floorEBO);//floor EBO

	glBindVertexArray(m_floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
	glBufferData(GL_ARRAY_BUFFER, updateFloorVertices.size() * sizeof(GLfloat), updateFloorVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_floorIndices), m_floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//// tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));  //params: at position 1, length , type (so here, 3 floats), not normalised, stride (6 floats in total per vertex), position to start - start at 3 here)
	glEnableVertexAttribArray(3);
	//// biTan attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));  //params: at position 2; position of uv, length 2; UV has 2 values  
	glEnableVertexAttribArray(4);


}

void BasicScene::setUniforms(Shader* shader)
{
	//Cube 
	shader->use();  // do we need this command each frame? - Probably not if we only have one shader
	
	
	shader->setVec3("ColLight", ColLight);
	shader->setVec3("OBjCol", ColCube);
	shader->setVec3("dirLight", dirLight);

	shader->setInt("map", map);

	//Pointlight
	shader->setVec3("pointLig.position", pointLigPos);
	shader->setVec3("pointLig.position", pointLigPos2);
	shader->setVec3("pointLig.colour", poinLigCol);
	
	shader->setFloat("pointLig.Kc", Kc);
	shader->setFloat("pointLig.Kl", Kl);
	shader->setFloat("pointLig.Ke", Ke);

	
	
	//spotlight
	
	shader->setVec3("spotLig.colour", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setFloat("spotLig.Kc", 1.0f);
	shader->setFloat("spotLig.Kl", 0.27f);
	shader->setFloat("spotLig.Ke", 0.028f);
	shader->setFloat("spotLig.innerRad", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spotLig.outterRad", glm::cos(glm::radians(17.5f)));
	//cube texture 

	
	
	
}





unsigned int BasicScene::loadTexture(char const* path)
{
	glGenTextures(1, &textureID);// Generates 1 texture
	unsigned char* m_data = stbi_load(path, &m_width, &m_height, &m_nrComponents, 0);
	if (m_data)
	{
		/**
		* if recieved data successful load 
		* if 1 component GL_RED
		* if 3 component GL_RGB
		* if 4 component GL_RGBA
		*/
		GLenum format;
		if (m_nrComponents == 1)
			format = GL_RED;
		else if (m_nrComponents == 3)
			format = GL_RGB;
		else if (m_nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID); //Bind texture GL_TEXTURE_2D type
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data); //
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// S = x axis axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T == y axis, R if 3D
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //zoom in
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //zoom out
		stbi_image_free(m_data);
		cout << "Loaded texture at path: " << path << " width " << m_width << " id " << textureID << endl;
		
	}
	else
	{
		cout << "Texture failed to load at path: " << path << " width " << m_width << " id " << textureID << endl;
		stbi_image_free(m_data);
	}
	
	return textureID;
}
