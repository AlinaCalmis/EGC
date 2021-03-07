#include "Object2D1.h"

#include <Core/Engine.h>

// creeaza sageata
Mesh* Object2D1::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, length/100 , 0), color),
		VertexFormat(corner + glm::vec3(length , length/100, 0), color),
		VertexFormat(corner + glm::vec3(length , 0 , 0), color),
		VertexFormat(corner + glm::vec3(length , -(length /8) , 0),  color),
		VertexFormat(corner + glm::vec3(length + length/8 + length/200, length / 200, 0), color),
		VertexFormat(corner + glm::vec3(length , length / 8 + length / 200, 0), color)

	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 2, 3 , 0, 
											0, 1, 2,
											6,5,2,
											3,2,5,
											5,4,3
											};	

	
	indices.push_back(0);
	indices.push_back(2);
	

	square->InitFromData(vertices, indices);
	return square;
}

// creeaza shuriken
Mesh* Object2D1::CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, -length , 0), color),
		VertexFormat(corner + glm::vec3(-length/2 , -length / 2, 0), color),
		VertexFormat(corner + glm::vec3(-length , 0 , 0), color),
		VertexFormat(corner + glm::vec3(-length /2, (length / 2) , 0),  color),
		VertexFormat(corner + glm::vec3(0, length, 0), color),
		VertexFormat(corner + glm::vec3(length /2, length / 2 , 0), color),
		VertexFormat(corner + glm::vec3(length , 0 , 0), color),
		VertexFormat(corner + glm::vec3(length / 2, -length / 2 , 0), color)

	};

	Mesh* shurik = new Mesh(name);
	std::vector<unsigned short> indices = { 0,1,2,
											3,4,0,
		                                    5,6,0,
											0,7,8
	};

	shurik->InitFromData(vertices, indices);
	return shurik;
}

// creeaza balon
Mesh* Object2D1::CreateBaloon(std::string name, glm::vec3 leftBottomCorner, float w, float h, int numberOfSides, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices;


	Mesh* baloon = new Mesh(name);
	//baloon->SetDrawMode(GL_LINE_LOOP);
	vertices.push_back(VertexFormat(corner, color));

	for (int o = 0; o < numberOfSides; o++)
	{
		float theta = 2.0f * 3.1415926f * float(o) / float(numberOfSides);
		float x = w/2 * cos(theta);
		float y = h/2 * sin(theta);

		vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}
	
	std::vector<unsigned short> indices;

	for (unsigned short i = 1; i < numberOfSides; i++)
	{
			indices.push_back(0);
			for (int j = i; j <= i + 1; j++)
			{
				if (j == numberOfSides) 
				{
					indices.push_back(1);
					break;
				}
				indices.push_back(j);
			}
		
	}

	vertices.push_back(VertexFormat(glm::vec3(0, -h / 2, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(w / 6, -h / 2 + 1 - w / 8, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(-w / 6, -h / 2 + 1 - w / 8, 0), color));

	// triunghiul balonului
	indices.push_back(numberOfSides + 1);
	indices.push_back(numberOfSides + 2);
	indices.push_back(numberOfSides + 3);
	

	baloon->InitFromData(vertices, indices);
	return baloon;
}

// creeaza arc
Mesh* Object2D1::CreateArch(std::string name, glm::vec3 leftBottomCorner, float w, float h, int numberOfSides, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices;


	Mesh* arch = new Mesh(name);
	arch->SetDrawMode(GL_LINE_STRIP);
	//vertices.push_back(VertexFormat(corner, color));

	for (int o = numberOfSides /4; o <= 3  *  numberOfSides / 4; o++)
	{
		float theta = 2.0f * 3.1415926f * float(o) / float(numberOfSides);
		float x = w / 2 * cos(theta);
		float y = h / 2 * sin(theta);

		vertices.push_back(VertexFormat( glm::vec3(x, y, 0), color));
	}

	std::vector<unsigned short> indices;

	for (unsigned short i = 0; i <= numberOfSides /2 ; i++)
	{
		indices.push_back(i);

	}
	indices.push_back(0);
	arch->InitFromData(vertices, indices);
	return arch;
}

// creeaza dreptunghi
Mesh* Object2D1::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float w, float h, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(0, w , 0), color),
		VertexFormat(corner + glm::vec3(h , w, 0), color),
		VertexFormat(corner + glm::vec3(h, 0 , 0), color)

	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices;
	if (fill) {
		indices = { 0,1,2,
				    2,3,0
		};
	}
	else {
		rectangle->SetDrawMode(GL_LINE_STRIP);
		
		for (int i = 0; i < 4; i++) {
			indices.push_back(i);
		}
		indices.push_back(0);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

// urmatoarele functii creeaza literele G,A,M,E,O,V,R
Mesh* Object2D1::StrG(std::string name, glm::vec3 leftBottomCorner, float w,  glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(w, w , 0), color),                   // 0
		VertexFormat(corner + glm::vec3(3 * w / 4, w + w / 4, 0), color),    // 1
		VertexFormat(corner + glm::vec3(w / 4, w + w / 4 , 0), color),       // 2
		VertexFormat(corner + glm::vec3(0, w, 0), color),                    // 3
		VertexFormat(corner + glm::vec3(0, w / 2 , 0), color),               // 4
		VertexFormat(corner + glm::vec3(w / 4, w / 4 , 0), color),           // 5
		VertexFormat(corner + glm::vec3(3 * w / 4, w / 4 , 0), color),       // 6
		VertexFormat(corner + glm::vec3(w, w / 2, 0), color),                // 7
		VertexFormat(corner + glm::vec3(w,  (3 * w / 4)  , 0), color),       // 8
		VertexFormat(corner + glm::vec3(3 * w / 5, (3 * w / 4)  , 0), color) // 9
	};

	Mesh* g = new Mesh(name);
	g->SetDrawMode(GL_LINE_STRIP);
	std::vector<unsigned short> indices;
	
	for (int i = 0; i < 10; i++) {
		indices.push_back(i);
	}

	g->InitFromData(vertices, indices);
	return g;

}

Mesh* Object2D1::StrA(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, w /4 , 0), color),                    // 0
		VertexFormat(corner + glm::vec3(w / 4 , 3 * w / 4, 0), color),           // 1
		VertexFormat(corner + glm::vec3(w / 2, w + w / 4 , 0), color),           // 2
		VertexFormat(corner + glm::vec3(3 * w / 4, 3 * w /4 , 0), color),        // 3
		VertexFormat(corner + glm::vec3(w, w / 4 , 0), color),                   // 4
	};

	Mesh* a = new Mesh(name);
	a->SetDrawMode(GL_LINE_STRIP); 

	std::vector<unsigned short> indices = { 0,1,2,3,4,4,3,1,0 };

	a->InitFromData(vertices, indices);
	return a;
}

Mesh* Object2D1::StrM(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3( 0, w / 4 , 0), color),                    // 0
		VertexFormat(corner + glm::vec3(w / 6 , w + w / 4, 0), color),             // 1
		VertexFormat(corner + glm::vec3( w / 2, w / 2 , 0), color),                // 2
		VertexFormat(corner + glm::vec3(5 *  w / 6, w +  w / 4 , 0), color),       // 3
		VertexFormat(corner + glm::vec3(w, w / 4 , 0), color),                     // 4
	};

	Mesh* m = new Mesh(name);
	m->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices ;
	for (int i = 0; i < 5; i++) {
		indices.push_back(i);
	}

	m->InitFromData(vertices, indices);
	return m;
}

Mesh* Object2D1::StrE(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, w / 4 , 0), color),                     // 0
		VertexFormat(corner + glm::vec3(0, 3 * w / 4, 0),color),                   // 1
		VertexFormat(corner + glm::vec3(0 , w + w / 4, 0), color),                 // 2
		VertexFormat(corner + glm::vec3(3 * w / 4, w + w / 4 , 0), color),         // 3
		VertexFormat(corner + glm::vec3( w / 2, 3 * w / 4 , 0), color),            // 4
		VertexFormat(corner + glm::vec3(3 * w / 4, w / 4 , 0), color),             // 5
	};

	Mesh* e = new Mesh(name);
	e->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices = {0,1,2,3,2,1,4,1,0,5};

	e->InitFromData(vertices, indices);
	return e;
}

Mesh* Object2D1::StrO(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, w / 2 , 0), color),                 // 0
		VertexFormat(corner + glm::vec3(0, w , 0),color),                      // 1
		VertexFormat(corner + glm::vec3(w / 4 , w + w / 4, 0), color),         // 2
		VertexFormat(corner + glm::vec3(3 * w / 4, w + w / 4 , 0), color),     // 3
		VertexFormat(corner + glm::vec3(w, w, 0), color),                      // 4
		VertexFormat(corner + glm::vec3(w, w / 2, 0), color),                  // 5
		VertexFormat(corner + glm::vec3(3 * w / 4,  w / 4, 0), color),         // 6
		VertexFormat(corner + glm::vec3(w / 4, w / 4, 0), color),              // 7
	};

	Mesh* o = new Mesh(name);
	o->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices ;
	for (int i = 0; i < 8; i++) {
		indices.push_back(i);
	}
	indices.push_back(0);

	o->InitFromData(vertices, indices);
	return o;
}

Mesh* Object2D1::StrV(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, w + w / 4 , 0), color),        // 0
		VertexFormat(corner + glm::vec3(w / 2, w / 4 , 0),color),         // 1
		VertexFormat(corner + glm::vec3(w, w + w / 4, 0), color),         // 2
	};

	Mesh* v = new Mesh(name);
	v->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices;
	for (int i = 0; i < 3; i++) {
		indices.push_back(i);
	}

	v->InitFromData(vertices, indices);
	return v;
}

Mesh* Object2D1::StrR(std::string name, glm::vec3 leftBottomCorner, float w, glm::vec3 color) {

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, w / 4, 0), color),                      // 0
		VertexFormat(corner + glm::vec3(0, 3 * w / 4, 0),color),                   // 1
		VertexFormat(corner + glm::vec3(0, w / 4 + w  , 0),color),                 // 2
		VertexFormat(corner + glm::vec3(w / 2 + w / 8, w + w / 4, 0), color),      // 3
		VertexFormat(corner + glm::vec3(3 * w / 4, w + w / 8, 0), color),          // 4
		VertexFormat(corner + glm::vec3(3 * w / 4, 3 * w / 4 + w / 8, 0), color),  // 5
		VertexFormat(corner + glm::vec3(w / 2 + w / 8, 3 * w / 4, 0), color),      // 6
		VertexFormat(corner + glm::vec3(3 * w / 4 + w / 8, w / 4, 0), color),      // 7
	};

	Mesh* r = new Mesh(name);
	r->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices = { 0,1,2,3,4,5,6,1,6,7 };

	r->InitFromData(vertices, indices);
	return r;
}
