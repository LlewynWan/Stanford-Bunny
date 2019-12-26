#include <display.h>
#include <GL/glew.h>
#include <iostream>

Display::Display(unsigned int width, unsigned int height, const std::string& title, Uint32 flags)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	m_glContext = SDL_GL_CreateContext(m_window);

	if (m_window == NULL)
		std::cerr << "Could not create window: " << SDL_GetError() << std::endl;

	GLenum status = glewInit();

	if (status != GLEW_OK)
		std::cerr << "Glew failed to initialise!" << std::endl;

	m_isClosed = false;
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Clear()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::Clear(GLbitfield mask)
{
	glClearColor(0,0,0,1);
	glClear(mask);
}

void Display::Clear(float r, float g, float b, float a, GLbitfield mask)
{
	glClearColor(r,g,b,a);
	glClear(mask);
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::GetSize(int* w, int* h)
{
	SDL_GetWindowSize(m_window, w, h);
}

void Display::SwapBuffer()
{
	SDL_GL_SwapWindow(m_window);
}

void Display::Update()
{
	SDL_GL_SwapWindow(m_window);
	processInput();
}

void Display::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		switch (event.type)
		{
			case SDL_QUIT:
				m_isClosed = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					int w,h;	
					GetSize(&w,&h);
					glViewport(0,0,w,h);
				}
				break;
			default:
				break;
		}
}
