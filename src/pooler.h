//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef POOLER_H
#define POOLER_H

#include <memory>
#include <list>
#include <vector>

#include "enemy.h"

// Pooler class manages a pool of enemy objects to avoid constant new/delete calls
// Using object pooling pattern for better memory management
class Pooler {
   public:
    Pooler(size_t minimumPoolSize);
    ~Pooler();
    
    size_t PoolSize();
    
    // Remove all active enemies and reset pool
    void Clear();
    
    // Get an enemy from pool or create new one if needed
    Enemy* SpawnEnemy(const Vector2D& position);
    
    // Put enemy back into pool for reuse
    void DespawnEnemy(Enemy* enemy);
    
    // Returns vector of all currently active enemies
    std::vector<Enemy*> GetActiveEnemies();

   private:
    std::list<Enemy*> enemies;
    size_t minimumPoolSize;

    Enemy* CreateEnemy();
    
    // Make sure we always have some ready enemies in pool
    void MaintainPoolSize();
};

#endif  // POOLER_H