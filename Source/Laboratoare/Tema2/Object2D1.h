#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D1
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateBaloon(std::string name, glm::vec3 leftBottomCorner, float w, float h, int numberOfSides, glm::vec3 color);
	Mesh* CreateArch(std::string name, glm::vec3 leftBottomCorner, float w, float h, int numberOfSides, glm::vec3 color);
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float w, float h, glm::vec3 color, bool fill = true);
	Mesh* StrG(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrA(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrM(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrE(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrO(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrV(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
	Mesh* StrR(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color);
}

