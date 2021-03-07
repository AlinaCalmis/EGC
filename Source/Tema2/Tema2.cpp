#include "Tema2.h"
#include "Transform3D.h"
#include "Object2D1.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	projectionMatrix = glm::perspective(RADIANS(90), 0.0f, 100.01f, 0.0f);
	//projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	Mesh* mesh; 
	mesh = Object2D1::CreateRectangle("rectangle", glm::vec3(0, 0, 0), 5, 1, glm::vec3(1, 1, 1), true);
	AddMeshToList(mesh);

	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("SphereDef");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SphereDef.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SphereDef.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		
		materialShininess = 100;
		materialKd = 4;
		materialKs = 2;
	}

	translateX = 0;
	translateY = 0;
	translateZ = -1;

	scaleX = 1;
	scaleY = 1;
	scaleZ = 5;

	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	

	
	speed = 1;
	color_idx = 0;
	space_click = 0;

	time = 0.0f;
	fuel = 0.0f;
	color_X = 0.0f;
	color_Y = 0.0f;
	color_Z = 0.0f;
	
	distance_Y = 0;
	

	start = FALSE;
	too_far = FALSE;
	go_left = FALSE;
	go_right = FALSE;
	stop_jumping = FALSE;
	switch_person = FALSE;
	GAME_OVER = FALSE;

	power_UP = FALSE;
	allow_press = TRUE;

	float factor = 0.0f;
	for (int road = 0; road <= 27; road += 3, factor += 5) {
		glm::vec3 newRoad;
		newRoad.x = -3.0f;
		newRoad.y = -1.0f;
		newRoad.z = 0.0f - factor;
		roads.push_back(newRoad);
		newRoad.x = 0.0f;
		newRoad.y = -1.0f;
		newRoad.z = 0.0f - factor;
		roads.push_back(newRoad);
		newRoad.x = 3.0f;
		newRoad.y = -1.0f;
		newRoad.z = 0.0f - factor;

		roads.push_back(newRoad);
	}
	for (int i = 0; i < 27; i++) {
		roads_colors.push_back(glm::vec3(0.678, 0.847, 0.902));
	}

	for (int i = 0; i < 27; i++) {
		draw.push_back(1);
	}

	red = glm::vec3(0.698, 0.133, 0.133);
	green = glm::vec3(0.420, 0.557, 0.137);
	yellow = glm::vec3(1.000, 1.000, 0.000);
	orange = glm::vec3(1.000, 0.647, 0.000);
	purple = glm::vec3(0.902, 0.902, 0.980);
	standart = glm::vec3(0.678, 0.847, 0.902);
	player_coordinates = glm::vec3(0.0f, 0.0f, 0.0f);

	colors.push_back(standart);
	colors.push_back(purple);
	colors.push_back(red);
	colors.push_back(green);
	colors.push_back(yellow);
	colors.push_back(orange);

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.961, 0.961, 0.961, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// Randeaza dreptunghiurile de viteza
	{
		if (start && !GAME_OVER) {
			fuel -= 0.0001;
			if (fuel < -95) GAME_OVER = TRUE;
		}
		glm::mat3 modelMatrix;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform3D::Translate2D(-2.225f, 2.013f);
		modelMatrix *= Transform3D::Scale2D(0.95f + fuel, 0.02f);
		RenderMesh2D(meshes["rectangle"], modelMatrix, glm::vec3(0.804, 0.522f, 0.247f));


		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform3D::Translate2D(-2.25f, 2.0f);
		modelMatrix *= Transform3D::Scale2D(1.0f, 0.025f);
		RenderMesh2D(meshes["rectangle"],  modelMatrix, glm::vec3(1, 1, 1));

		
	}
	// Randeaza placile
	{
		for (int i = 0; i < 27; i++) {

			// deseneaza placile daca inca nu a inceput jocul
			// adica inca nu a fost apasat pentru prima data space
			if (space_click == 0) {
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(roads[i].x, roads[i].y, roads[i].z);
				modelMatrix *= Transform3D::Scale(2.5f, 0.1f, 5.0f);
				RenderMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, roads_colors[0]);
			}
			// daca jocul a inceput, adica a fost apasat space o data sau de mai multe ori
			// placile incep a se misca
			else if (start) {

				if (speed >= 20) speed = 20;
				if (speed <= 5) speed = 5;

				// calculeaza pozitia pe z a fiecarei placi la fiecare frame
				roads[i].z += deltaTimeSeconds * 6 * speed / 10;
			 
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(roads[i].x, roads[i].y, roads[i].z);
				modelMatrix *= Transform3D::Scale(2.5f, 0.1f, 5.0f);

				

				// alege random o placa pe care sa nu o deseneze la un moment dat de timp
				// dupa ce placa a iesit din scena
				if (roads[i].z >= 2) {
					roads[i].z = -45;
					draw[i] = rand() % 3;
					color_idx = 2 + rand() % 4;

					glm::vec3 current_color = colors[color_idx];

					if (current_color == red) count_reds++;
					else if (current_color == green) count_greens++;
					else if (current_color == yellow) count_yellows++;
					else if (current_color == orange) count_oranges++;

					if (count_reds == 5 && ((i + 1) % 3 == 0) && ((draw[i - 1] == 0 && draw[i - 2] == 0) || (draw[i - 1] != 0 && draw[i - 2] == 0)
						|| (draw[i - 1] == 0 && draw[i - 2] != 0))) {
						count_reds = 0; roads_colors[i] = red;
					}
					else if (count_reds == 5 && ((i + 1) % 3 == 1) && draw[i - 1] != 0) { count_reds = 0; roads_colors[i] = red; }
					else if (count_greens == 4) { count_greens = 0; roads_colors[i] = green; }
					else if (count_yellows == 2) { count_yellows = 0; roads_colors[i] = yellow; }
					else if (count_oranges == 2) { count_oranges = 0; roads_colors[i] = orange; }
					else current_color = standart;

					roads_colors[i] = current_color;
				}

				// verifica sa nu fie prea mult spatiu intre placile care pot fi desenate
				if (i > 3 && i < 27) { 
					if (draw[i] == 0) {

						// A treia placa de pe rand
						if (i % 3 == 0) {
							if (draw[i - 4] == 0 && draw[i - 1] == 0 && draw[i - 3] != 0)
								draw[i] = 0;
							else if (draw[i - 4] == 0 && draw[i - 1] != 0)
								draw[i] = rand() % 2;
							else if (draw[i - 1] != 0 && draw[i - 4] != 0)
								draw[i] = rand() % 2;
							else if (draw[i - 3] != 0 && draw[i - 4] != 0)
								draw[i] = rand() % 2;
						}
						// A doua placa de pe rand
						if (i % 3 == 1) {
							if (draw[i - 3] != 0 && draw[i + 1] == 0)
								draw[i] = 1;
							else if (draw[i - 2] != 0 && draw[i - 3] == 0)
								draw[i] = 1;
							else if (draw[i - 3] == 0 && draw[i - 2] == 0)
								draw[i] = rand() % 2;
						}
						// Prima placa de pe rand
						if (i % 3 == 2) {
							if (draw[i - 4] != 0 && draw[i - 1] == 0)
								draw[i] = 1;
							else if (draw[i - 4] != 0 && draw[i - 1] != 0)
								draw[i] = rand() % 2;
							else if (draw[i - 3] != 0 && draw[i - 1] != 0)
								draw[i] = rand() % 2;
							else if (draw[i - 3] != 0 && draw[i - 1] == 0)
								draw[i] = 1;
							else if (draw[i - 2] != 0)
								draw[i] = 1;
						}
					}
					
				}
				else draw[i] = 4;
				

				if (isACollision(roads[i], player_coordinates) && draw[i] != 0 && !GAME_OVER) {
					
					if (roads_colors[i] == red && allow_press) GAME_OVER = TRUE;
					else if (roads_colors[i] == yellow && allow_press) fuel -= 0.003;
					else if (roads_colors[i] == orange && allow_press) allow_press = FALSE;
					else if (roads_colors[i] == green && allow_press){
						fuel += 0.0015;
						if (fuel >= 0) fuel = 0;
					}

					roads_colors[i] = purple;
				}
				else if (isACollision(roads[i], player_coordinates) && draw[i] == 0 && !GAME_OVER && allow_press) GAME_OVER = TRUE;
				
				if (!allow_press) {
					power_UP = TRUE;
					time += deltaTimeSeconds;
					temp_speed = speed;
					speed = 30;
					if (time >= 100) {
						time = 0;
						speed = temp_speed;
						temp_speed = 0;
						allow_press = TRUE;
						power_UP = FALSE;
					}
				}

				// daca la random nu a nimerit 0 atunci desenaeaza placa
				if (draw[i] != 0) {
					RenderMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, roads_colors[i]);
				}
			}
		}
	}
	
	// Randeaza jucatorul / bila
	{
		// daca se apasa continuu pe space , adica jucatorul sare
		if (!stop_jumping && jump > 0) {
			distance_Y += deltaTimeSeconds * 4;
			if (distance_Y > 1.5f) {
				distance_Y = 1.5f;
			}
		}

		// inaltimea maxima pana unde poate sari mingea
		if (distance_Y == 1.5)
			stop_jumping = TRUE;

		// daca s-a oprit din a apasa tasta space, adica nu mai sare jucatorul
		// si inaltimea la care se afla inca nu e cea initiala
		if (distance_Y >= 0 && stop_jumping) {

			// scade din inaltime
			distance_Y -= deltaTimeSeconds * 5;

			// verifica sa nu fie dus jucatorul sub pozitia initiala
			if (distance_Y < 0) {
				distance_Y = 0;
				stop_jumping = FALSE;
			}

			jump = 0;
		}

		// calculeaza coordonatale pe OX daca jucatorul se duce in stanga apasand tasta A
		if (go_left) {
			translateX -= deltaTimeSeconds * 4;
			if (translateX < -2) {
				translateX = -2;
			}
		}

		// caluleaza coordonatele pe OX daca jucatorul se duce la dreapta apasand tasta D
		if (go_right) {
			translateX += deltaTimeSeconds * 4;
			if (translateX > 2) {
				translateX = 2;
			}
		}

		{
			player_coordinates.x = translateX;
			player_coordinates.y = distance_Y;
			player_coordinates.y = 0;

			if (GAME_OVER) 
				distance_Y -= deltaTimeSeconds;

			if (power_UP) {
				angularStepOY += deltaTimeSeconds * 50;
				angularStepOX += deltaTimeSeconds * 50;

				color_X = (rand() % 100) / 10;
				color_Y = (rand() % 100) / 10;
				color_Z = (rand() % 100) / 10;

				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(translateX, distance_Y, 0);
				modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
				modelMatrix *= Transform3D::RotateOX(RADIANS(angularStepOX));
				modelMatrix *= Transform3D::RotateOY(RADIANS(angularStepOY));
				RenderMesh(meshes["sphere"], shaders["SphereDef"], modelMatrix, glm::vec3(color_X, color_Y, color_Z));
			}
			else {
				modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(translateX, distance_Y, 0);
				modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
				RenderMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1.000, 0.843, 0.000));
			}
			
		}
	}

	{
		if (fuel > 0) fuel = 0;

		if (GAME_OVER) speed -=deltaTimeSeconds *10 ;
	}
}

void Tema2::FrameEnd()
{
}

// RenderMesh din laboratorul 7
void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
	glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	if (switch_person)
		viewMatrix = glm::lookAt(glm::vec3(player_coordinates.x, player_coordinates.y + 1, -0.25), glm::vec3(0, 0, -100), glm::vec3(0, 2, 0));
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


bool Tema2::isACollision(glm::vec3 road_coord, glm::vec3 player_coord) {

	float x = std::fmax(road_coord.x - 1.25f, std::fmin(player_coord.x, road_coord.x + 1.25f));
	//float y = std::fmax(road_coord.y - 0.05f, std::fmin(player_coord.y, road_coord.y + 0.05f));
	float z = std::fmax(road_coord.z - 2.5f, std::fmin(player_coord.z, road_coord.z + 2.5f));

	float distance = std::sqrt(std::pow(x - player_coord.x, 2) + std::pow(z - player_coord.z, 2));

	return distance < 0.5;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_SPACE) && start) {
		jump++;
		space_click++;
	}
	if (window->KeyHold(GLFW_KEY_W) && allow_press)
		speed++;
	if (window->KeyHold(GLFW_KEY_S) && allow_press)
		speed--;
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	// daca se incepe jocul si se apasa prima data SPACE
	if (key == GLFW_KEY_SPACE && space_click == 0) {
		start = TRUE;
		space_click = 1;
	}
	if (key == GLFW_KEY_A)
		go_left = TRUE;
	if (key == GLFW_KEY_D)
		go_right = TRUE;
	if (key == GLFW_KEY_C)
		switch_person = !switch_person;
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// jocul e inceput si se aopreste apasarea tastei SPACE
	if (key == GLFW_KEY_SPACE && start) {
		stop_jumping = TRUE;
	}
	if (key == GLFW_KEY_A)
		go_left = FALSE;
	if (key == GLFW_KEY_D)
		go_right = FALSE;
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
