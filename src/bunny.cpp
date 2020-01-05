#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

#include <shader.h>
#include <camera.h>
#include <display.h>

#define MAX_NUM_INDICES 100005

bool ShouldClose = false;

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

glm::vec3 InitPos = glm::vec3(0.0f, 0.3f, 1.0f);
glm::vec3 InitFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 InitUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.4f, 0.6f, 3.0f);

float InitYaw = -90.0f;
float InitPitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int drawMode = 0;
int mouse_x = -1;
int mouse_y = -1;
std::vector<unsigned int> selectedIndices;

bool isAltPressed = false;

std::vector<float> bunnyVertices;
std::vector<unsigned int> bunnyElemFaces;

glm::mat4 bunnyProjection;
glm::mat4 bunnyModel;
glm::mat4 bunnyView;

glm::vec3 bunnyPosition = glm::vec3(0.0f, 0.0f, 0.0f);   

void processInput();
void selectIndex();
void keyboard_callback(SDL_KeyboardEvent* event, float deltaTime);

Camera camera(InitPos, InitUp, InitYaw, InitPitch);
Display display(SCREEN_WIDTH, SCREEN_HEIGHT, "bunny", SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN);

float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void calculateNormal(int* vertex)
{
	vertex[0] *= 6;
	vertex[1] *= 6;
	vertex[2] *= 6;
	glm::vec3 vector1 = glm::vec3(bunnyVertices[vertex[1]] - bunnyVertices[vertex[0]], bunnyVertices[vertex[1]+1] - bunnyVertices[vertex[0]+1], bunnyVertices[vertex[1]+2] - bunnyVertices[vertex[0]+2]);
	glm::vec3 vector2 = glm::vec3(bunnyVertices[vertex[2]] - bunnyVertices[vertex[1]], bunnyVertices[vertex[2]+1] - bunnyVertices[vertex[1]+1], bunnyVertices[vertex[2]+2] - bunnyVertices[vertex[1]+2]);
	glm::vec3 norm = glm::normalize(glm::cross(vector1, vector2));
	for (int i = 0; i < 3; i++)
	{
		bunnyVertices[vertex[i]+3] += norm.x;
		bunnyVertices[vertex[i]+4] += norm.y;
		bunnyVertices[vertex[i]+5] += norm.z;
	}
}

int main()
{
	std::ifstream PLYFile;
	PLYFile.open("res/bunny.ply", std::ios::in);
	int numVertices, numFaces;
	for (;;)
	{
		std::string PLYLine;
		getline(PLYFile, PLYLine);
		int pos = (int)PLYLine.find("element vertex");
		if (pos != -1)
			numVertices = std::stoi(PLYLine.substr(pos+14));
		pos = (int)PLYLine.find("element face");
		if (pos != -1)
			numFaces = std::stoi(PLYLine.substr(pos+12));
		if (PLYLine.compare("end_header")==0) break;
	}
	for (int i = 0; i < numVertices; i++)
	{
		float x,y,z,confidence,intensity;
		PLYFile >> x >> y >> z >> confidence >> intensity;
		bunnyVertices.push_back(x);
		bunnyVertices.push_back(y);
		bunnyVertices.push_back(z);
		bunnyVertices.push_back(0.0);
		bunnyVertices.push_back(0.0);
		bunnyVertices.push_back(0.0);
	}
	for (int i = 0; i < numFaces; i++)
	{
		int num_vertices;
		PLYFile >> num_vertices;
		int vertex[num_vertices];
		for (int j = 0; j < num_vertices; j++)
		{
			PLYFile >> vertex[j];
			bunnyElemFaces.push_back(vertex[j]);
		};
		calculateNormal(vertex);
	}
	PLYFile.close();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	Shader bunnyShader("res/bunny");
	Shader lampShader("res/lamp");

	GLuint bunnyVAO;
	glGenVertexArrays(1, &bunnyVAO);
	glBindVertexArray(bunnyVAO);

	GLuint bunnyVBO;
	glGenBuffers(1, &bunnyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bunnyVertices.size(), bunnyVertices.data(), GL_STATIC_DRAW);

	GLuint bunnyEBO;
	glGenBuffers(1, &bunnyEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*bunnyElemFaces.size(), bunnyElemFaces.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);

	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	GLuint lampVBO;
	glGenBuffers(1, &lampVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (!ShouldClose)
	{
		float currentFrame = (float)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		display.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bunnyShader.use();

		int w, h;
		display.GetSize(&w, &h);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)w/(float)h, 0.1f, 100.0f);
		bunnyShader.setMat4("projection", projection);
		bunnyProjection = projection;

		glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		bunnyShader.setMat4("view", view);
		bunnyView = view;

		glm::mat4 model = glm::mat4(1.0f);
		bunnyShader.setMat4("model", model);
		bunnyModel = model;
		
		bunnyShader.setVec3("objectColor", 0.843f, 0.863f, 0.867f);
		bunnyShader.setVec3("pointLight.position", lightPos);
		bunnyShader.setVec3("viewPos", camera.Position);
		bunnyShader.setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
		bunnyShader.setVec3("pointLight.diffuse", 0.4f, 0.4f, 0.4f);
		bunnyShader.setVec3("pointLight.specular", 0.7f, 0.7f, 0.7f);
		bunnyShader.setFloat("pointLight.constant", 1.0f);
		bunnyShader.setFloat("pointLight.linear", 0.09f);
		bunnyShader.setFloat("pointLight.quadratic", 0.032);

		bunnyShader.setVec3("dirLight.direction", -0.5f, -0.5f, -0.5f);
		bunnyShader.setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		bunnyShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
		bunnyShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		bunnyShader.setVec3("material.ambient", 0.4f, 0.4f, 0.4f);
		bunnyShader.setVec3("material.diffuse", 0.7f, 0.7f, 0.7f);
		bunnyShader.setVec3("material.specular", 0.3f, 0.3f, 0.3f);
		bunnyShader.setFloat("material.shininess", 8.0f);

		glBindVertexArray(bunnyVAO);
		
		bunnyShader.setBool("isDrawPoint", 0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*bunnyElemFaces.size(), bunnyElemFaces.data(), GL_STATIC_DRAW);
		switch (drawMode)
		{
			case 0: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case 1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
			case 2: glPointSize(3.0f); glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
		}
		glDrawElements(GL_TRIANGLES, 3*numFaces, GL_UNSIGNED_INT, 0);

		if (selectedIndices.size())
		{
			bunnyShader.setBool("isDrawPoint", 1);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, selectedIndices.size()*sizeof(unsigned int), selectedIndices.data(), GL_DYNAMIC_DRAW);
			glPointSize(5.0f);
			glDrawElements(GL_POINTS, selectedIndices.size(), GL_UNSIGNED_INT, 0);

		}

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		lampShader.setMat4("model", model);
		glBindVertexArray(lampVAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		display.SwapBuffer();
		processInput();

		SDL_Delay(20);
	}

	glDeleteVertexArrays(1, &lampVAO);
	glDeleteVertexArrays(1, &bunnyVAO);
	glDeleteBuffers(1, &lampVBO);
	glDeleteBuffers(1, &bunnyVBO);
	return 0;
}

void processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		switch (event.type)
		{
			case SDL_QUIT:
				ShouldClose = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event = SDL_WINDOWEVENT_RESIZED)
				{
					int w,h;
					display.GetSize(&w, &h);
					glViewport(0,0,w,h);
				}
				break;
			case SDL_KEYDOWN:
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RALT])
				{
					isAltPressed = true;
					SDL_SetRelativeMouseMode(SDL_FALSE);
				} else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_TAB])
					drawMode = (drawMode+1) % 3;
				else if (!isAltPressed)
					keyboard_callback(&event.key, deltaTime);
				break;
			case SDL_KEYUP:
				if (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] && !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RALT])
				{
					isAltPressed = false;
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				int x,y;
				if (SDL_BUTTON(SDL_GetMouseState(&x, &y)) == SDL_BUTTON_LEFT)
				{
					mouse_x = x;
					mouse_y = y;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				selectIndex();
				break;
			case SDL_MOUSEMOTION:
				if (!isAltPressed)
					camera.ProcessMouseMovement((float)event.motion.xrel, -(float)event.motion.yrel);
				break;
			case SDL_MOUSEWHEEL:
				camera.ProcessMouseScroll(event.wheel.y);
				break;
			default:
				break;
		}
}

void selectIndex()
{
	if (mouse_x < 0 || mouse_y < 0)
		return;

	int WIDTH,HEIGHT;
	display.GetSize(&WIDTH, &HEIGHT);

	float x = (2.0f * mouse_x) / WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / HEIGHT;
	float z = 1.0f;
	glm::fvec3 ray_nds(x, y, z);
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 0.0f);
	glm::fvec4 ray_camera = glm::inverse(bunnyProjection) * ray_clip;
	ray_camera = glm::dvec4(ray_camera.x, ray_camera.y, -1.0f, 0.0f);
	glm::fvec4 tmp = glm::inverse(bunnyView) * ray_camera;
	glm::fvec3 ray_world(tmp.x, tmp.y, tmp.z);
	ray_world = glm::normalize(ray_world);

	std::vector<glm::vec3> resultFaces;
	std::vector<int> resultIndices;
	glm::vec3 p(1.0f, 1.0f, 1.0f);

	for (unsigned int i = 0; i < bunnyElemFaces.size(); i += 3)
	{
		glm::vec3 position[3];
		for (int j = 0; j < 3; j++)
			position[j] = glm::vec3(bunnyModel * glm::vec4(bunnyVertices[bunnyElemFaces[i+j]*6], bunnyVertices[bunnyElemFaces[i+j]*6+1], bunnyVertices[bunnyElemFaces[i+j]*6+2], 1.0f));
		if (glm::intersectLineTriangle(camera.Position, ray_world, position[0], position[1], position[2], p))
		{
			glm::vec3 resultPoint = camera.Position + ray_world * p.x;
			resultFaces.push_back(resultPoint);
			resultIndices.push_back(i);
		}
	}

	if (resultFaces.size())
	{
		float min_dst = 0;
		unsigned int min_index = 0;
		for (unsigned int i = 0; i < resultFaces.size(); i++)
		{
			glm::vec3 tmp = resultFaces[i];
			float dst = glm::distance(camera.Position, tmp);
			if (i == 0)
			{
				min_index = i;
				min_dst = dst;
			} else {
				if (min_dst > dst)
				{
					min_dst = dst;
					min_index = i;
				}
			}
		}

		int towardIndex = resultIndices[min_index];
		glm::vec3 intersectPoint = resultFaces[min_index];
		for (int i = 0; i < 3; i++)
		{
			int vertexIndex = bunnyElemFaces[towardIndex+i];
			glm::vec3 tmp = glm::vec3(bunnyModel * glm::vec4(bunnyVertices[vertexIndex*6], bunnyVertices[vertexIndex*6+1], bunnyVertices[vertexIndex*6+2], 1.0f));
			float dst = glm::distance(intersectPoint, tmp);
			if (i == 0)
			{
				min_index = vertexIndex;
				min_dst = dst;
			} else {
				if (min_dst > dst)
				{
					min_dst = dst;
					min_index = vertexIndex;
				}
			}
		}

		std::vector<unsigned int>::iterator pos = std::find(selectedIndices.begin(), selectedIndices.end(), min_index);
		if (pos != selectedIndices.end())
			selectedIndices.erase(pos);
		else
			selectedIndices.push_back(min_index);
	}
}

void keyboard_callback(SDL_KeyboardEvent *event, float deltaTime)
{
	deltaTime *= 0.1;

	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

	if (keyboard_state_array[SDL_SCANCODE_ESCAPE])
		ShouldClose = true;

	Camera_Movement direction;

	if (keyboard_state_array[SDL_SCANCODE_W] && !keyboard_state_array[SDL_SCANCODE_S]) direction = FORWARD;
        else if (!keyboard_state_array[SDL_SCANCODE_W] && keyboard_state_array[SDL_SCANCODE_S]) direction = BACKWARD;

	camera.ProcessKeyboard(direction, deltaTime);

        if (keyboard_state_array[SDL_SCANCODE_D] && !keyboard_state_array[SDL_SCANCODE_A]) direction = RIGHT;
        else if (!keyboard_state_array[SDL_SCANCODE_D] && keyboard_state_array[SDL_SCANCODE_A]) direction = LEFT;

	camera.ProcessKeyboard(direction, deltaTime);
}
