#include "BulletManager.h"

using namespace std;

/// static member 定義
unique_ptr<BulletManager> BulletManager::sInstance_ = nullptr;

/// Singleton Instance を取得
BulletManager* BulletManager::GetInstance() {
	if (!sInstance_) {
		sInstance_ = make_unique<BulletManager>(BulletManager::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 初期化
void BulletManager::Initialize() {
	// 弾のリストをクリア
	bullets_.clear();
}

/// 終了処理
void BulletManager::Finalize() {
	// 弾のリスト内の弾をすべて削除
	for (unique_ptr<BaseBullet>& bullet : bullets_) {
		bullet.reset();
	}
	// 弾のリストをクリア
	bullets_.clear();

	sInstance_.reset();
}

/// 更新
void BulletManager::Update() {
	// 弾のリストが空なら何もしない
	if (bullets_.empty()) return;
	// 弾の更新と死亡した弾の削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		// 弾が死亡している場合はリストから削除
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}
}

/// 描画
void BulletManager::Draw() {
	// 弾のリストが空なら何もしない
	if (bullets_.empty()) return;
	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

/// 弾の追加
void BulletManager::AddBullet(std::unique_ptr<BaseBullet> bullet) {
	bullets_.emplace_back(std::move(bullet));
}

/// プレイヤーの弾の数を取得
int BulletManager::GetPlayerBulletCount() const {
	int count = 0;
	for (const auto& bullet : bullets_) {
		if (bullet->GetOwner() == BulletOwner::Player) {
			++count;
		}
	}
	return count;
}
