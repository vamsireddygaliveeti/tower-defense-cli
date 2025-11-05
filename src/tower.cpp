//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "tower.h"
#include "logger.h"

#include <limits>
#include <cmath>

Tower::Tower(int x, int y, int rateOfFire)
    : position({x, y}),
      rateOfFire(rateOfFire),
      lastFireTime(std::chrono::steady_clock::now()),
      bAutoFire(true)
    {}

void Tower::Update(std::vector<Enemy*> enemies,
                   std::vector<Bullet>& bullets) {
    if (bAutoFire) {
        FireAtEnemy(enemies, bullets);
    }
}

void Tower::FireAtEnemy(const std::vector<Enemy*>& enemies,
                        std::vector<Bullet>& bullets) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFireTime);

    // Check if enough time has passed to fire again
    if (elapsed >= std::chrono::milliseconds(rateOfFire)) {
        Enemy* target = FindClosestEnemy(enemies);

        if (target != nullptr) {
            if (bAutoFire) {
                int dist = CalculateDistance(target->GetPosition(), position);

                // Only fire if enemy is within range
                if (dist <= ENEMYRANGE) {
                    FireBulletAtEnemy(*target, bullets);
                    lastFireTime = now;
                }
            } else {
                FireBulletAtEnemy(*target, bullets);
                lastFireTime = now;
            }
        }
    }
}

bool Tower::GetAutoFire() const { return bAutoFire; }

void Tower::SetAutoFire(bool autoFire) { bAutoFire = autoFire; }

void Tower::ManualFire(std::vector<Enemy*> enemies,
                       std::vector<Bullet>& bullets) {
    FireAtEnemy(enemies, bullets);
}

void Tower::FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets) {
    Vector2D direction = {0, 0};
    
    // Figure out which direction to shoot
    if (enemy.GetPosition().x > position.x) direction.x = 1;
    if (enemy.GetPosition().x < position.x) direction.x = -1;
    if (enemy.GetPosition().y > position.y) direction.y = 1;
    if (enemy.GetPosition().y < position.y) direction.y = -1;

    bullets.emplace_back(position, direction);
}

int Tower::CalculateDistance(const Vector2D& pos1, const Vector2D& pos2) {
    // Using Manhattan distance because enemies move on a grid
    // This gives us the actual walking distance, not straight-line
    int dx = std::abs(pos1.x - pos2.x);
    int dy = std::abs(pos1.y - pos2.y);
    return dx + dy;
}

Enemy* Tower::FindClosestEnemy(const std::vector<Enemy*>& enemies) {
    // Handle empty list
    if (enemies.empty()) {
        return nullptr;
    }
    
    Enemy* closest = nullptr;
    int shortestDist = std::numeric_limits<int>::max();
    
    // Loop through all enemies and find the nearest one
    for (Enemy* e : enemies) {
        // Make sure enemy exists and is active
        if (e != nullptr && e->IsActive()) {
            int dist = CalculateDistance(e->GetPosition(), position);
            
            if (dist < shortestDist) {
                shortestDist = dist;
                closest = e;
            }
        }
    }
    
    return closest;
}