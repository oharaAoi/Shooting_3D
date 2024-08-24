#pragma once
#include <cstdint>

enum class CollisionTypeIdDef : uint32_t {
	kDefault,
	kPlayer,
	kPlayerBullet,
	kEnemy,
	kBoss,
	kEnemyBullet,
	kBossBullet
};