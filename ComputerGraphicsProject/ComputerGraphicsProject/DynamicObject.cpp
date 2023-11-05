#include "DynamicObject.h"

glm::vec3 CPhysicsComponent::vec3GravityAcceleration = glm::vec3(0, -9.8f / 2, 0);
  
CDynamicObject::CDynamicObject()
{
	CObject::CObject();
}

CDynamicObject::~CDynamicObject()
{
}

void CDynamicObject::Update(float fElapsedTime)
{
	m_PhysicsComponent.simulate(this, fElapsedTime);
}

void CDynamicObject::AddAcceleration(glm::vec3 dir, float fAccel)
{
	m_PhysicsComponent.AddLinearAcceleration(dir * fAccel);
}

void CDynamicObject::SetLinearAcceleration(glm::vec3 accel)
{
	m_PhysicsComponent.SetLinearAcceleration(accel);
}

void CDynamicObject::SetRotateAxis(glm::vec3 axis)
{
	m_PhysicsComponent.SetRotateAxis(axis);
}

void CDynamicObject::AddTorqueAcceleration(float fAccel)
{
	m_PhysicsComponent.AddTorqueAcceleration(fAccel);
}

CPhysicsComponent::CPhysicsComponent()
{
	m_vec3AngularAxis = glm::vec3(0, 1, 0);

	m_fAngularAcceleration = 0.0f;
	m_vec3LinearAcceleration = glm::vec3(0);
	m_fAngularVelocity = 0.0f;
	m_vec3LinearVelocity = glm::vec3(0);

	m_fMaxAcceleration = 1000.0f;
	m_fMaxVelocity = 5000.0f;

	m_fMaxTorqueAcceleration = 360.0f;
	m_fMaxTorque = 720.0f;
}

CPhysicsComponent::~CPhysicsComponent()
{
}

void CPhysicsComponent::simulate(CObject* target, float fElapsedTime)
{
	m_vec3LinearAcceleration += vec3GravityAcceleration * fElapsedTime;
	if (glm::length(m_vec3LinearAcceleration) > m_fMaxAcceleration) {
		m_vec3LinearAcceleration = glm::normalize(m_vec3LinearAcceleration) * m_fMaxAcceleration;
	}

	m_vec3LinearVelocity += m_vec3LinearAcceleration * fElapsedTime;
	if (glm::length(m_vec3LinearVelocity) > m_fMaxVelocity) {
		m_vec3LinearVelocity = glm::normalize(m_vec3LinearVelocity) * m_fMaxVelocity;
	}

	glm::vec3 pos = target->GetPosition();
	target->SetPosition(pos + m_vec3LinearVelocity * fElapsedTime);

	//m_fAngularAcceleration += vec3GravityAcceleration * fElapsedTime;
	if (m_fAngularAcceleration > m_fMaxTorqueAcceleration) {
		m_fAngularAcceleration = m_fMaxTorqueAcceleration;
	}

	m_fAngularVelocity += m_fAngularAcceleration * fElapsedTime;
	if (m_fAngularVelocity > m_fMaxTorque) {
		m_fAngularVelocity = m_fMaxTorque;
	}
	
	glm::quat rotate = target->GetRotation();
	rotate = glm::rotate(rotate, glm::radians(m_fAngularVelocity * fElapsedTime), m_vec3AngularAxis);
	target->SetRotate(rotate);
}

void CPhysicsComponent::AddLinearAcceleration(glm::vec3 accel)
{
	m_vec3LinearAcceleration += accel;
}

void CPhysicsComponent::SetLinearAcceleration(glm::vec3 accel)
{
	m_vec3LinearAcceleration = accel;
}

void CPhysicsComponent::AddTorqueAcceleration(float Accel)
{
	m_fAngularAcceleration += Accel;
}

float CPhysicsComponent::GetTorqueAcceleration()
{
	return m_fAngularAcceleration;
}

void CPhysicsComponent::SetRotateAxis(glm::vec3 Axis)
{
	m_vec3AngularAxis = Axis;
}
