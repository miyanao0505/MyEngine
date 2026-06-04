#pragma once
#include "BaseBullet.h"
#include <memory>
#include <list>

/// <summary>
/// 弾管理クラス
/// </summary>
class BulletManager {
public:	// クラス関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>BulletManager のインスタンス</returns>
	static BulletManager* GetInstance();

	/// ------ Passkey Idiom ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class BulletManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit BulletManager(ConstructorKey) {}

	/// コピー禁止
	BulletManager(const BulletManager&) = delete;
	BulletManager& operator=(const BulletManager&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾の追加
	/// </summary>
	/// <param name="bullet">追加する弾のポインタ</param>
	void AddBullet(std::unique_ptr<BaseBullet> bullet);

public:	//getter
	/// <summary>
	/// プレイヤーの弾の数を取得
	/// </summary>
	/// <returns>プレイヤーの弾の数</returns>
	int GetPlayerBulletCount() const;

public:	// Singleton Instance
	static std::unique_ptr<BulletManager> sInstance_;

private:	// メンバ変数
	std::list<std::unique_ptr<BaseBullet>> bullets_;	// 弾のリスト
};