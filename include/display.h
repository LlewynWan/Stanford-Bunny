#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

class Display
{
public:
	Display(unsigned int width, unsigned int height, const std::string& title, Uint32 flags = SDL_WINDOW_OPENGL);

	void Clear();
	void Clear(GLbitfield mask);
	void Clear(float r, float g, float b, float a, GLbitfield mask = GL_COLOR_BUFFER_BIT);
	void Update();
	bool IsClosed();
	void GetSize(int* w, int* h);
	void processInput();
	void SwapBuffer();

	virtual ~Display();
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isClosed;
};

#endif
