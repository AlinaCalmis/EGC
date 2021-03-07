#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;
	float rotations;
	bool direction, ok;
	float cx, cy;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	glm::ivec2 resolution;
	glm::vec2 mouseCoordinates;
	glm::vec2 arrowCoordinates;
	glm::vec2 archCoordinates;
	glm::ivec2 baloonCoordinates;
	glm::ivec2 shurikCoordinates;

	std::vector <int> redBaloons;
	std::vector <int> yellowBaloons;
	std::vector <int> shurikens;
	std::vector <int> shurikCollCount;

	float arch_len, arrow_len;
	int press_left_click, stop_dragging;
	float drag ,scale;
	float move_forward;
	float c;
	float radians;
	int i, init_arrow_x, init_arrow_y, count, factor, something;
	int score, lifes, shown;
};
