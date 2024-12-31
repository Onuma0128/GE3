#include "CollisionManager.h"

#include "Vector3.h"

void CollisionManager::Reset()
{
    colliders_.clear();
}

void CollisionManager::AddCollider(Collider* collider)
{
    colliders_.push_back(collider);
}

void CollisionManager::CheckAllCollisions()
{
    std::list<Collider*>::iterator itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA) {
        Collider* colliderA = *itrA;

        std::list<Collider*>::iterator itrB = itrA;
        itrB++;
        for (; itrB != colliders_.end(); ++itrB) {
            Collider* colliderB = *itrB;

            CheckCollisionPair(colliderA, colliderB);
        }
    }
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
    Vector3 PosA = colliderA->GetCenterPosition();
    Vector3 PosB = colliderB->GetCenterPosition();
    Vector3 subtract = PosB - PosA;

    float distance = subtract.Length();

    float radiusA = colliderA->GetRadius();
    float radiusB = colliderB->GetRadius();

    std::string nameA = colliderA->GetColliderName();
    std::string nameB = colliderB->GetColliderName();

    if ((nameA == "player" && nameB == "sword") ||
        (nameA == "sword" && nameB == "player")) {
        return;
    }

    // 衝突の判定
    if (distance <= (radiusA + radiusB) && nameA != nameB) {
        colliderA->OnCollision(nameB);
        colliderB->OnCollision(nameA);
    }
}