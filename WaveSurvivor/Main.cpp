#include "Renderer.h"

int main()
{
	Renderer* renderer = new Renderer();
	
	renderer->Render();

	delete renderer;
}