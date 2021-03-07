#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "Tema2Camera.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));
//	void RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix);
	bool isACollision(glm::vec3 road_coord, glm::vec3 player_coord);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	Tema::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;

	glm::vec3 lightPosition;
	glm::vec3 objectColor;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;

	glm::mat4 modelMatrix;
	float translateX, translateY, translateZ;
	float scaleX, scaleY, scaleZ;
	float angularStepOX, angularStepOY, angularStepOZ;
	GLenum polygonMode;

	float foV;
	float x;
	std::vector <glm::vec3> roads;
	std::vector <glm::vec3> colors;
	std::vector <glm::vec3> roads_colors;
	std::vector <int> draw;

	glm::vec3 red;
	glm::vec3 green;
	glm::vec3 orange;
	glm::vec3 yellow;
	glm::vec3 purple;
	glm::vec3 standart;
	glm::vec3 player_coordinates;

	int space_click,color_idx, speed, temp_speed;
	int count_reds, count_yellows, count_oranges, count_greens;
	bool start, stop_jumping, go_right, go_left, too_far, allow_press, switch_person, power_UP, GAME_OVER;
	float jump, distance_Y, color_X, color_Y, color_Z;
	float fuel, time;
};
