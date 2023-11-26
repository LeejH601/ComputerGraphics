#pragma once
#include "Object.h"


class CPhysicsComponent
{
	glm::vec3 m_vec3AngularAxis;
	glm::vec3 m_vec3LinearVelocity;
	float m_fAngularVelocity;
	glm::vec3 m_vec3LinearAcceleration;
	float m_fAngularAcceleration;

	float m_fMaxAcceleration;
	float m_fMaxVelocity;

	float m_fMaxTorqueAcceleration;
	float m_fMaxTorque;

	static glm::vec3 vec3GravityAcceleration;

public:
	CPhysicsComponent();
	virtual ~CPhysicsComponent();

	void simulate(CObject* target, float fElapsedTime);

	void AddLinearAcceleration(glm::vec3 accel);
	void SetLinearAcceleration(glm::vec3 accel);
	glm::vec3 GetLinearAcceleration() { return m_vec3LinearAcceleration; };
	glm::vec3 GetLinearVelocity() { return m_vec3LinearVelocity; };
	void ScaleLinearAcceleration(float scale) { m_vec3LinearAcceleration *= scale; };
	void ScaleLinearVelocity(float scale) { m_vec3LinearVelocity *= scale; };

	void AddTorqueAcceleration(float fAccel);
	float GetTorqueAcceleration();
	void SetRotateAxis(glm::vec3 Axis);
};


class CDynamicObject : public CObject
{
	CPhysicsComponent m_PhysicsComponent;

public:
	CDynamicObject();
	virtual ~CDynamicObject();

	virtual void Update(float fElapsedTime);

	void AddAcceleration(glm::vec3 dir, float fAccel);
	void SetLinearAcceleration(glm::vec3 accel);
	void SetRotateAxis(glm::vec3 axis);
	void AddTorqueAcceleration(float fAccel);
	CPhysicsComponent& GetPhysics() { return m_PhysicsComponent; };
	void ScaleLinearAcceleration(float scale) { m_PhysicsComponent.ScaleLinearAcceleration(scale); };
	void ScaleLinearVelocity(float scale) { m_PhysicsComponent.ScaleLinearVelocity(scale); };
};

class CAmazingMovementObject : public CDynamicObject
{
public:
	float m_fCurrentTime;
	float m_fDeltaOffset;
	float m_fScaleValue;
	float m_fTimeScale;
	bool m_bScaleSimulate;
public:
	CAmazingMovementObject();
	virtual ~CAmazingMovementObject();

	virtual void Update(float fElapsedTime);
};
