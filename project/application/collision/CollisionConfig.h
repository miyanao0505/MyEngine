#pragma
#include <cstdint>

/// <summary>
/// コリジョン種別ID定義
/// </summary>
enum class CollisionTypeIdDef : uint32_t {
	kDefault,
	kPlayer,
	kPlayerBullet,
	kEnemy,
	kEnemyBullet,
};