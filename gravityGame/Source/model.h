#pragma once

#include "gravityEngine.h"
#include "collisionEngine.h"
#include "movementEngine.h"

class Model{
	private:
		std::shared_ptr<VertexData> vertexData{new VertexData};
		std::unique_ptr<GravityEngine> gravityEngine{ new GravityEngine };
		std::unique_ptr<CollisionEngine> collisionEngine{ new CollisionEngine };
		std::shared_ptr<MovementEngine> movementEngine{ new MovementEngine };

		float pos[2] = { 0,0 };
		float velocity[2] = { 0,0 };
		glm::vec2 deltaVelocity;
		glm::vec2 movementVec = glm::vec2(0, 0);
		glm::vec2 gravityDirection = glm::vec2(0, 0);

	public: 
		Model();
		Model(const char* modelPath,int windowSize[2], float pos[2], float velocity[2], float gravity, int locked);
		void calculateGravity(std::vector<std::shared_ptr<Model>> references);
		void calculateCollision(std::vector<std::shared_ptr<Model>> references);
		std::shared_ptr<MovementEngine> getMovementPointer();
		glm::vec2 getGravityDirection();
		void calculateMovement();
		float* calculateVelocity(std::vector<std::shared_ptr<Model>> references);
		std::vector<std::shared_ptr<VertexData>> toVertexData(std::vector<std::shared_ptr<Model>> input);
		std::shared_ptr<VertexData> getVertexDataPointer();
		void moveWithVelocity(float newV[2]);
		void moveWithPosition(float newPos[2]);
		void rotate(glm::vec2 direction);
		void render();
		void destroy();
};

