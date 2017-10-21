#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
//#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"

#include <Vector>

// Library Includes
#include <string>

#define VALIDATE(a) if (!a) return (false)

#define SMALL_NUM = 0.00000001f;


	enum ModelType{
		Triangle,
		Quad,
		Cube,
		CubeMap,
		Custom
	};

	struct VertexData{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureSource;
	};

	struct S_Ray {
		glm::vec3 P0;
		glm::vec3 Dir;
	};

	struct S_Triangle {
		glm::vec3 V0;
		glm::vec3 V1;
		glm::vec3 V2;
	};


class Utils
{
public:
	Utils();
	~Utils();

	std::vector<VertexData> GetQuadVertexData();
	std::vector<VertexData> GetCubeVertexData();
	std::vector<VertexData> GetCubeMapVertexData();

	int intersect3D_RayTriangle(S_Ray R, S_Triangle T, glm::vec3* I);
};

