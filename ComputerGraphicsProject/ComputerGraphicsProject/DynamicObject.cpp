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

CPhysicsComponent::CPhysicsComponent()
{
	m_vec3AngularAcceleration = glm::vec3(0);
	m_vec3LinearAcceleration = glm::vec3(0);
	m_vec3AngularVelocity = glm::vec3(0);
	m_vec3LinearVelocity = glm::vec3(0);

	m_fMaxAcceleration = 1000.0f;
	m_fMaxVelocity = 5000.0f;
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
}

void CPhysicsComponent::AddLinearAcceleration(glm::vec3 accel)
{
	m_vec3LinearAcceleration += accel;
}
