#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

glm::vec3 Utils::ScaleVec3(glm::vec3 _Vec, float _Scale) {
	glm::vec3 ReturnVec = _Vec;
	ReturnVec.x *= _Scale;
	ReturnVec.y *= _Scale;
	ReturnVec.z *= _Scale;
	return ReturnVec;
}

std::vector<VertexData> Utils::GetCubeVertexData(){
	GLfloat vertices[] = {
		//Co-ords					Normal			Texture Source
		//Front
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

		//Back
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		//Up
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		//Down
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		//Left
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//Right
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	};

	std::vector<VertexData> Bananas;
	VertexData NewData;
	//Bananas.push_back()

	for (int i = 0; i < 24; i++){
		GLfloat PosX = vertices[i * 8];
		GLfloat PosY = vertices[i * 8 + 1];
		GLfloat PosZ = vertices[i * 8 + 2];

		GLfloat NormalX = vertices[i * 8 + 3];
		GLfloat NormalY = vertices[i * 8 + 4];
		GLfloat NormalZ = vertices[i * 8 + 5];

		GLfloat TexX = vertices[i * 8 + 6];
		GLfloat TexY = vertices[i * 8 + 7];

		NewData.Position = glm::vec3(PosX, PosY, PosZ);
		NewData.Normal = glm::vec3(NormalX, NormalY, NormalZ);
		NewData.TextureSource = glm::vec2(TexX, TexY);
		
		Bananas.push_back(NewData);
	}

	return Bananas;

}

std::vector<VertexData> Utils::GetQuadVertexData(){
	GLfloat vertices[] = {
		//Co-ords					Normal			Texture Source
		//Front
		-0.5f, 0.5, 0.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
	};

	std::vector<VertexData> Bananas;
	VertexData NewData;

	for (int i = 0; i < 4; i++){
		GLfloat PosX = vertices[i * 8];
		GLfloat PosY = vertices[i * 8 + 1];
		GLfloat PosZ = vertices[i * 8 + 2];

		GLfloat NormalX = vertices[i * 8 + 3];
		GLfloat NormalY = vertices[i * 8 + 4];
		GLfloat NormalZ = vertices[i * 8 + 5];

		GLfloat TexX = vertices[i * 8 + 6];
		GLfloat TexY = vertices[i * 8 + 7];

		NewData.Position = glm::vec3(PosX, PosY, PosZ);
		NewData.Normal = glm::vec3(NormalX, NormalY, NormalZ);
		NewData.TextureSource = glm::vec2(TexX, TexY);

		Bananas.push_back(NewData);
	}

	return Bananas;

}

std::vector<VertexData> Utils::GetCubeMapVertexData(){
	
		GLfloat vertices[] = {
			//Co-ords					Normal			Texture Source
			//Front
			-0.5f, -0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f, 

			//Back
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f, 
			0.5f, 0.5f, 0.5f, 
			-0.5f, 0.5f, 0.5f, 

			//Up
			-0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f, 
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f, 

			//Down
			-0.5f, -0.5f, -0.5f, 
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f, 

			//Left
			-0.5f, -0.5f, 0.5f, 
			-0.5f, 0.5f, 0.5f, 
			-0.5f, 0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f,

			//Right
			0.5f, -0.5f, -0.5f, 
			0.5f, 0.5f, -0.5f, 
			0.5f, 0.5f, 0.5f, 
			0.5f, -0.5f, 0.5f,
		};

	std::vector<VertexData> Bananas;
	VertexData NewData;

	for (int i = 0; i < 24; i++){
		GLfloat PosX = vertices[i * 3];
		GLfloat PosY = vertices[i * 3 + 1];
		GLfloat PosZ = vertices[i * 3 + 2];

		NewData.Position = glm::vec3(PosX, PosY, PosZ);
		Bananas.push_back(NewData);
	}
	return Bananas;
}

int Utils::intersect3D_RayTriangle(S_Ray R, S_Triangle T, glm::vec3* I)
{
	glm::vec3    u, v, n;              // triangle vectors
	glm::vec3    w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

									// get triangle edge vectors and plane normal
	u = T.V1 - T.V0;
	v = T.V2 - T.V0;
	n = glm::cross(u, v);              // cross product
	if (n.x == 0 && n.y == 0 && n.z == 0)             // triangle is degenerate
		return -1;                  // do not deal with this case

     // ray direction vector
	w0 = R.P0 - T.V0;
	a = -dot(n, w0);
	b = dot(n, R.Dir);
	if (fabs(b) < 0.000000001f) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			return 2;
		else return 0;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return 0;                   // => no intersect
									// for a segment, also test if (r > 1.0) => no intersect

	*I = R.P0 + r * R.Dir;            // intersect point of ray and plane

									// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u, u);
	uv = dot(u, v);
	vv = dot(v, v);
	w = *I - T.V0;
	wu = dot(w, u);
	wv = dot(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                       // I is in T
}



