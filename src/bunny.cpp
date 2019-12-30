#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include <shader.h>
#include <camera.h>
#include <display.h>

bool ShouldClose = false;

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

glm::vec3 InitPos = glm::vec3(0.0f, 0.1f, 0.7f);
glm::vec3 InitFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 InitUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(0.5f, 0.7f, 1.0f);

float InitYaw = -90.0f;
float InitPitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<float> bunnyVertices;
std::vector<float> bunnyElemFaces;

glm::vec3 bunnyPosition = glm::vec3(0.0f, 0.0f, 0.0f);   

void processInput();
void pickup(SDL_Event *event);
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
	vertex[0] *= 5;
	vertex[1] *= 5;
	vertex[2] *= 5;
	float va[3], vb[3], vr[3], val;
	va[0] = bunnyVertices[vertex[0]] - bunnyVertices[vertex[1]];
	va[1] = bunnyVertices[vertex[0]+1] - bunnyVertices[vertex[1]+1];
	va[2] = bunnyVertices[vertex[0]+2] - bunnyVertices[vertex[1]+2];
	vb[0] = bunnyVertices[vertex[0]] - bunnyVertices[vertex[2]];
	vb[1] = bunnyVertices[vertex[0]+1] - bunnyVertices[vertex[2]+1];
	vb[2] = bunnyVertices[vertex[0]+2] - bunnyVertices[vertex[2]+2];

	vr[0] = va[1]*vb[2] - vb[1]*va[2];
   	vr[1] = vb[0]*va[2] - va[0]*vb[2];
   	vr[2] = va[0]*vb[1] - vb[0]*va[1];

	val = sqrt(vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2]);

	for (int i = 0; i < 3; i++)	
	{
		bunnyElemFaces.push_back(bunnyVertices[vertex[i]]);
		bunnyElemFaces.push_back(bunnyVertices[vertex[i]+1]);
		bunnyElemFaces.push_back(bunnyVertices[vertex[i]+2]);
		bunnyElemFaces.push_back(vr[0]/val);
		bunnyElemFaces.push_back(vr[1]/val);
		bunnyElemFaces.push_back(vr[2]/val);
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
		bunnyVertices.push_back(confidence);
		bunnyVertices.push_back(intensity);
	}
	for (int i = 0; i < numFaces; i++)
	{
		int num_vertices;
		PLYFile >> num_vertices;
		int vertex[num_vertices];
		for (int j = 0; j < num_vertices; j++)
			PLYFile >> vertex[j];
		calculateNormal(vertex);
	}

	glEnable(GL_DEPTH_TEST);
	Shader bunnyShader("res/bunny");
	Shader lampShader("res/lamp");

	GLuint bunnyVAO;
	glGenVertexArrays(1, &bunnyVAO);
	glBindVertexArray(bunnyVAO);

	GLuint bunnyVBO;
	glGenBuffers(1, &bunnyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bunnyElemFaces.size(), bunnyElemFaces.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	GLuint lampVAO;
	glGenVertexArrays(1, &lampVAO);
	glBindVertexArray(lampVAO);

	GLuint lampVBO;
	glGenBuffers(1, &lampVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lampVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		bunnyShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		bunnyShader.setMat4("model", model);
		
		bunnyShader.setVec3("objectColor", 0.843f, 0.863f, 0.867f);
		bunnyShader.setVec3("pointLight.position", lightPos);
		bunnyShader.setVec3("viewPos", camera.Position);
		bunnyShader.setVec3("pointLight.ambient", 0.1f, 0.1f, 0.1f);
		bunnyShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		bunnyShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		bunnyShader.setFloat("pointLight.constant", 1.0f);
		bunnyShader.setFloat("pointLight.linear", 0.09f);
		bunnyShader.setFloat("pointLight.quadratic", 0.032);

		bunnyShader.setVec3("dirLight.direction", -0.2f, -1.0f, 0.3f);
		bunnyShader.setVec3("dirLight.ambient", 0.6f, 0.6f, 0.6f);
		bunnyShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
		bunnyShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

		bunnyShader.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
		bunnyShader.setVec3("material.diffuse", 0.02f, 0.02f, 0.02f);
		bunnyShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		bunnyShader.setFloat("material.shininess", 8.0f);

		glBindVertexArray(bunnyVAO);
		glDrawArrays(GL_TRIANGLES, 0, bunnyElemFaces.size());

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		lampShader.setMat4("model", model);
		glBindVertexArray(lampVAO);
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
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT])
					pickup(&event);
				else keyboard_callback(&event.key, deltaTime);
				break;
			case SDL_MOUSEMOTION:
				camera.ProcessMouseMovement((float)event.motion.xrel, -(float)event.motion.yrel);
				break;
			case SDL_MOUSEWHEEL:
				camera.ProcessMouseScroll(event.wheel.y);
				break;
			default:
				break;
		}
}

void pickup(SDL_Event *event)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);

	bool isPressed = false;
	for (;;)
	{
		SDL_PollEvent(event);
		
		int x, y;
		if (!isPressed && event->type == SDL_MOUSEBUTTONDOWN && SDL_BUTTON(SDL_GetMouseState(&x, &y)) == SDL_BUTTON_LEFT)
		{
			isPressed = true;
			//std::cout << std::endl << "-------------------------++++++++++++++++++++++++++++++-------------------------" << std::endl;
			std::cout << "Cursor Position (relative to window): (" << std::fixed << std::setprecision(5) << x << ", " << y << ")" << std::endl;
		
			GLint viewport[4];
    			GLdouble modelview[16];
    			GLdouble projection[16];
    			GLfloat winX, winY, winZ;
    			GLdouble object_x, object_y, object_z;
    			int mouse_x = x;
    			int mouse_y = y;
    			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    			glGetDoublev(GL_PROJECTION_MATRIX, projection);
    			glGetIntegerv(GL_VIEWPORT, viewport);

 			winX = (float)mouse_x;
    			winY = (float)viewport[3] - (float)mouse_y - 1.0f;
    			glReadBuffer(GL_BACK);
    			glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    			gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);

			std::cout <<"Cursor Position (in viewport): (" << std::fixed << std::setprecision(5) << object_x << ", " << object_y << ", " << object_z << ")" << std::endl;

			glm::vec3 pp(object_x, object_y, object_z);
			glm::vec3 U, V, N;
			N = glm::normalize(camera.Position - camera.Front);
			U = glm::normalize(glm::cross(N, camera.WorldUp));
			V = glm::normalize(glm::cross(U, N));

			GLdouble worldpos_x, worldpos_y, worldpos_z;
			worldpos_x = U.x*object_x + V.x*object_y + N.x*object_z + camera.Position.x;
			worldpos_y = U.y*object_x + V.y*object_y + N.y*object_z + camera.Position.y;
			worldpos_z = U.z*object_x + V.z*object_y + N.z*object_z + camera.Position.z;
			std::cout << "Cursor Position (in world coordinate): (" << std::fixed << std::setprecision(5) << worldpos_x << ", " << worldpos_y << ", " << worldpos_z << ")" << std::endl;
			//std::cout << "-------------------------++++++++++++++++++++++++++++++-------------------------" << std::endl;
			std::cout << std::endl;
		}
		if (event->type == SDL_MOUSEBUTTONUP)
			isPressed = false;
		if (event->type == SDL_KEYUP && event->key.keysym.scancode == 226)
			break;
	}
	SDL_SetRelativeMouseMode(SDL_TRUE);
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
