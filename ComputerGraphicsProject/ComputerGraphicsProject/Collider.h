#pragma once
#include "stdafx.h"
#include <variant>
#include <concepts>

template <class COLLISONSHAPE>
concept IsBoundingable = std::same_as<COLLISONSHAPE, DirectX::BoundingBox> || std::same_as<COLLISONSHAPE, DirectX::BoundingFrustum>
|| std::same_as<COLLISONSHAPE, DirectX::BoundingOrientedBox> || std::same_as<COLLISONSHAPE, DirectX::BoundingSphere>;

template <class COLLISONSHAPE>
class CCollider
{
	enum class COLLIDER_TYPE {
		TYPE_NONE = -1,
		TYPE_AABB,
		TYPE_FRUSTUM,
		TYPE_OBB,
		TYPE_SPHERE,
		TYPE_LENGTH
	};

	COLLIDER_TYPE m_eCollierType = COLLIDER_TYPE::TYPE_NONE;

	COLLISONSHAPE m_CollisionShape;

public:
	CCollider() = default;
	virtual ~CCollider();
	CCollider(COLLIDER_TYPE colliderType, COLLISONSHAPE shape);

	bool Collision(CCollider& other);
};

template <class COLLISONSHAPE1, class COLLISONSHAPE2>
extern bool Collision(COLLISONSHAPE1& lhs, COLLISONSHAPE2& rhs);

template<class COLLISONSHAPE>
inline CCollider<COLLISONSHAPE>::~CCollider()
{
}

template<class COLLISONSHAPE>
inline CCollider<COLLISONSHAPE>::CCollider(COLLIDER_TYPE colliderType, COLLISONSHAPE shape)
{
	m_eCollierType = colliderType;

	m_CollisionShape = shape;
}

template<class COLLISONSHAPE>
inline bool CCollider<COLLISONSHAPE>::Collision(CCollider& other)
{
	return m_CollisionShape.intersects(other);
}

template<class COLLISONSHAPE1, class COLLISONSHAPE2>
inline bool Collision(COLLISONSHAPE1& lhs, COLLISONSHAPE2& rhs)
{
	return lhs.intersects(rhs);
}
