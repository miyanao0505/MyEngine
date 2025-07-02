#include "Collider.h"
#include "CollisionManager.h"

Collider::Collider() {
	CollisionManager::GetInstance()->Register(this);
}

Collider::~Collider() {
	CollisionManager::GetInstance()->Unregister(this);
}
