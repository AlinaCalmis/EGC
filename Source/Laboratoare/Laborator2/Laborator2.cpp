#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);

void Laborator2::Init()
{
	cullFace = GL_BACK;
	polygonMode = GL_FILL;

	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a mesh box using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1, 1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-1, 1, 1), glm::vec3(0, 0.4, 0.7)),
			VertexFormat(glm::vec3(1, -1, 1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0.8, 1, 1)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(0, 0.7, 0.5)),
			VertexFormat(glm::vec3(-1, 1, -1), glm::vec3(1, 0.3, 0.3)),
			VertexFormat(glm::vec3(1, 1, -1), glm::vec3(0, 0.3, 0.6)),
			VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0, 1, 1)),
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			1, 7, 2,
			7, 2, 3,
			6, 7, 1,
			3, 6, 7,
			1, 5, 6,
			5, 1, 0,
			0, 5, 4,
			6, 4, 3,
			5, 4, 6,
			0, 2, 3,
			3, 4, 0
			

			// TODO: Complete indices data
		};

		meshes["cube1"] = new Mesh("generated cube 1");
		meshes["cube1"]->InitFromData(vertices, indices);

		// Create a new mesh from buffer data
		Mesh *cube = CreateMesh("cube3", vertices, indices);
	}

	{
		vector<VertexFormat> vertices1
		{
			VertexFormat(glm::vec3(-2, -1, 1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-4, -1, 1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-2, -1, 3), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-4, -1, 3), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-3, 3, 2), glm::vec3(0, 1, 1)),
		};
		vector<unsigned short> indices1 =
		{
			0,1,2,
			1,3,2,
			4,1,3,
			4,2,1,
			2,3,4,
			0,2,4,
			1,4,0,
		};
		meshes["piramide"] = new Mesh("generated piramide 1");
		meshes["piramide"]->InitFromData(vertices1, indices1);

		// Create a new mesh from buffer data
		Mesh* pir = CreateMesh("piram", vertices1, indices1);
	}

	{
		vector<VertexFormat> vertices2
		{
			VertexFormat(glm::vec3(2, 2, 2), glm::vec3(0.3, 0.4, 0.5)),
			VertexFormat(glm::vec3(2, 4, 2), glm::vec3(0.3, 0.4, 0.5)),
			VertexFormat(glm::vec3(4, 2, 2), glm::vec3(0.3, 0.4, 0.5)),
			VertexFormat(glm::vec3(4, 4, 2), glm::vec3(0.3, 0.4, 0.5))
		};
		vector<unsigned short> indices2 =
		{
			0,1,2,
			2,3,1
		};
		meshes["sq"] = new Mesh("generated sq 1");
		meshes["sq"]->InitFromData(vertices2, indices2);
		Mesh* sq = CreateMesh("sq", vertices2, indices2);
	}
}

Mesh* Laborator2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	GLuint VBO_ID;			// ID-ul (nume sau referinta) buffer-ului ce va fi cerut de la GPU
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Crete the IBO and bind it
	unsigned int IBO_ID = 0;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// TODO: Enable face culling
//	glEnable(GL_CULL_FACE);

	// TODO: Set face custom culling. Use the "cullFace" variable
//	if (count == 1) {
//		glCullFace(GL_FRONT);
//	}
//	else if(count == 2){
//		glCullFace(GL_BACK);
//	}
	// render an object using face normals for color
//	RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

	// render an object using colors from vertex
//	RenderMesh(meshes["cube3"], shaders["VertexColor"], glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0.25f));

//	RenderMesh(meshes["sq"], shaders["VertexColor"], glm::vec3(1, 0.5f, 0), glm::vec3(0.25f));

	// TODO: Disable face culling
//	glDisable(GL_CULL_FACE);

//	RenderMesh(meshes["cube1"], shaders["VertexColor"], glm::vec3(1.5f, 0.5f, 0), glm::vec3(0.25f));
//	RenderMesh(meshes["cube1"], shaders["VertexNormal"], glm::vec3(2.5f, 0.5f, 0), glm::vec3(0.25f));
//	RenderMesh(meshes["piram"], shaders["VertexColor"], glm::vec3(-2, 0.5f, 0), glm::vec3(0.25f));

	drawCircle( 32, 20, 0, 120, 10);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint nrOfSides)
{
	const int numberOfVertices = 12;

	GLfloat twicePi = 2.0f * M_PI;

	GLfloat circleVerticesX[numberOfVertices];
	GLfloat circleVerticesY[numberOfVertices];
	GLfloat circleVerticesZ[numberOfVertices];

	for (int i = 1; i < numberOfVertices; i++) {
		circleVerticesX[i] = x + (radius * cos(1 * twicePi / nrOfSides));
		circleVerticesY[i] = y + (radius * cos(1 * twicePi / nrOfSides));
		circleVerticesZ[i] = z;
	}
	GLfloat allCircleVertices[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}
void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	// TODO: switch between GL_FRONT and GL_BACK culling
	// Save the state in "cullFace" variable and apply it in the Update() method not here

	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
	if (key == GLFW_KEY_F2) {
		if (count == 2) {
			count = 0;
		}
		count++;
	}
	
}

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator2::OnWindowResize(int width, int height)
{
}
