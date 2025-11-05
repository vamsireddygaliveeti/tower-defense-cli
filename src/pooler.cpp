//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"

#include <algorithm>

Pooler::Pooler(size_t minimumPoolSize) : minimumPoolSize(minimumPoolSize) {
    // Initialize pool with some enemies ready to go
    MaintainPoolSize();
}

Pooler::~Pooler() {
    // Clean up memory - delete all enemy pointers
    for (auto* e : enemies) {
        delete e;
    }
    enemies.clear();
}

size_t Pooler::PoolSize() {
    return enemies.size();
}

Enemy* Pooler::CreateEnemy() {
    return new Enemy();
}

void Pooler::Clear() {
    // Instead of deleting, just deactivate all enemies so we can reuse them
    for (auto* e : enemies) {
        if (e->IsActive()) {
            e->Deactivate();
        }
    }
}

Enemy* Pooler::SpawnEnemy(const Vector2D& position) {
    Enemy* availableEnemy = nullptr;
    
    // Try to find an inactive enemy we can reuse
    for (auto* e : enemies) {
        if (!e->IsActive()) {
            availableEnemy = e;
            break;
        }
    }
    
    // If no inactive enemy found, create a new one
    if (availableEnemy == nullptr) {
        availableEnemy = CreateEnemy();
        enemies.push_back(availableEnemy);
    }
    
    // Activate enemy at spawn position
    availableEnemy->Activate(position);
    
    // Keep pool stocked with inactive enemies
    MaintainPoolSize();
    
    return availableEnemy;
}

void Pooler::DespawnEnemy(Enemy* enemy) {
    // Safety check
    if (enemy == nullptr) {
        return;
    }
    
    // Just deactivate - don't delete, so we can reuse later
    enemy->Deactivate();
}

std::vector<Enemy*> Pooler::GetActiveEnemies() {
    std::vector<Enemy*> result;
    
    // Go through all enemies and collect the active ones
    for (auto* e : enemies) {
        if (e->IsActive()) {
            result.push_back(e);
        }
    }
    
    return result;
}

void Pooler::MaintainPoolSize() {
    // Count how many inactive enemies we have
    size_t inactiveCount = 0;
    for (auto* e : enemies) {
        if (!e->IsActive()) {
            inactiveCount++;
        }
    }
    
    // Add more if we're below minimum
    while (inactiveCount < minimumPoolSize) {
        Enemy* newEnemy = CreateEnemy();
        enemies.push_back(newEnemy);
        inactiveCount++;
    }
}