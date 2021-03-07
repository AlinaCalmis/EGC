#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
int var, pos,obj;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* mesh1 = new Mesh("teapot");
		mesh1->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh1->GetMeshID()] = mesh1;

		Mesh* mesh2 = new Mesh("sphere");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	//glClearColor(0.3, 0.67, 0.8, 1);

	// daca variabila este setata se calculeaza random parametrii rgb
	if (var == 1) {
		float r = (float)((rand)() / (RAND_MAX + 0.1f));
		float g = (float)((rand)() / (RAND_MAX + 0.1f));
		float b = (float)((rand)() / (RAND_MAX + 0.1f));

		glClearColor(r, g, b, 1);
	};

	var = 0;

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(2, 0.5f, 0), glm::vec3(0.5f));
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));
	// pentru exercitiul 4
	// am setat 3 pozitii care ar simula miscarea unei sfere intre trei obiecte
	// apasand tasta b
	
	if (obj == 0) {
		RenderMesh(meshes["sphere"], glm::vec3(0.5f, 1, 0));
	}
	else if (obj == 1) {
		RenderMesh(meshes["teapot"], glm::vec3(0.5f, 1, 0));
	}
	else if (obj == 2) {
		RenderMesh(meshes["box"], glm::vec3(0.5f, 1, 0), glm::vec3(0.5f));
	}
	//RenderMesh(meshes["teapot"], glm::vec3(0.5f, 1, 0));
	
	RenderMesh(meshes["teapot"], glm::vec3(x, y, z));
		
	
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	// am setat coordonatele/directiile de miscare pentru fiecare tasta separat
	if (window->KeyHold(GLFW_KEY_W)) {
		x = x;
		y = y + deltaTime * (0.2);
		z = y;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		x = x + deltaTime * (-0.1);
		y = y;
		z = z;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		x = x;
		y = y + deltaTime * (0.1);
		z = z + deltaTime * (0.34);
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		x = x + deltaTime * (0.1);
		y = y;
		z = z;
	}
	if (window->KeyHold(GLFW_KEY_E)) {
		x = x;
		y = y + deltaTime * (-0.1);
		z = z + deltaTime * (0.3);
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
		x = x + deltaTime*(-0.1);
		y = y + deltaTime * (0.1);
		z = z + deltaTime* (0.04);
	}
	
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	// a setat o variabila globala care imi verifica dacatasta H(pentru stergerea culorii)
	// este setata
	if (key == GLFW_KEY_H) {
		var = 1;
	}
	
	if (key == GLFW_KEY_B) {
		obj += 1;
		if (obj > 2) {
			obj = 0;
		}
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
