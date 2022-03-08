#pragma once

#include "pgr.h"

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	// identifier for the shader program
	GLuint program;          // = 0;

	struct {
		// vertex attributes locations
		GLint pos;       // = -1;
		// uniforms locations
		GLint PVMmatrix; // = -1;
	} locations;

	// ...
} ShaderProgram;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	unsigned int  numTriangles;         // number of triangles in the mesh

	// ...
} ObjectGeometry;

class ObjectInstance {

protected:

	ObjectGeometry*	geometry;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	// dynamic objects
	// glm::vec3 direction;
	// float     speed;
	// ...

	typedef std::vector<ObjectInstance*> ObjectList;
	ObjectList children;

public:

	ObjectInstance();
	~ObjectInstance();

  /** recalculates global matrix and updates all children
  *
  * Derived classes should also call this method (using SceneNode::update()).
  */
	virtual void update(float elapsedTime, const glm::mat4* parentModelMatrix) {
		// update model transformation - localModelMatrix
		// ...
 
		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		for (auto child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/// calls draw on child nodes
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		// draw node geometry using globalModelMatrix
		// ...

		// process all children
		for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix);
		}
	}

};
