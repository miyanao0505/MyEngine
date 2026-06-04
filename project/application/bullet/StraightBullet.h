#pragma once
#include "BaseBullet.h"

/// <summary>
/// 直線弾クラス
/// </summary>
class StraightBullet : public BaseBullet {
public:	// クラス関数
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">弾の所有者</param>
		StraightBullet(BulletOwner owner);
		
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="position">弾の初期位置</param>
		/// <param name="direction">弾の初期方向</param>
		void Initialize(const MyBase::Vector3& position, const MyBase::Vector3& direction);

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;
};