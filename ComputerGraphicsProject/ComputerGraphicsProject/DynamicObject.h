#pragma once
#include "Object.h"


class CPhysicsComponent
{
	glm::vec3 m_vec3LinearVelocity;
	glm::vec3 m_vec3AngularVelocity;
	glm::vec3 m_vec3LinearAcceleration;
	glm::vec3 m_vec3AngularAcceleration;

	float m_fMaxAcceleration;
	float m_fMaxVelocity;

	static glm::vec3 vec3GravityAcceleration;

public:
	CPhysicsComponent();
	virtual ~CPhysicsComponent();

	void simulate(CObject* target, float fElapsedTime);

	void AddLinearAcceleration(glm::vec3 accel);
	glm::vec3 GetLinearAcceleration() { return m_vec3LinearAcceleration; };
	glm::vec3 GetLinearVelocity() { return m_vec3LinearVelocity; };
	void ScaleLinearAcceleration(float scale) { m_vec3LinearAcceleration *= scale; };
	void ScaleLinearVelocity(float scale) { m_vec3LinearVelocity *= scale; };
};


class CDynamicObject : public CObject
{
	CPhysicsComponent m_PhysicsComponent;

public:
	CDynamicObject();
	virtual ~CDynamicObject();

	virtual void Update(float fElapsedTime);

	void AddAcceleration(glm::vec3 dir, float fAccel);
	CPhysicsComponent& GetPhysics() { return m_PhysicsComponent; };
	void ScaleLinearAcceleration(float scale) { m_PhysicsComponent.ScaleLinearAcceleration(scale); };
	void ScaleLinearVelocity(float scale) { m_PhysicsComponent.ScaleLinearVelocity(scale); };
};

