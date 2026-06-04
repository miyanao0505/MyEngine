#pragma once
#include <memory>

/// <summary>
/// ゲーム全体で使用する時間やタイマー関係を管理するクラス
/// </summary>
class TimeManager
{
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>TimeManager</returns>
	static TimeManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class TimeManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit TimeManager(ConstructorKey) {}

	/// コピー禁止
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

	/// <summary>
	///  初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

public:	// getter
	/// <summary>
	/// 経過時間の取得
	/// </summary>
	/// <returns>経過時間</returns>
	float GetDeltaTime() { return kDeltaTime; }

public:	// setter


public:	// Singleton Instance
	static std::unique_ptr<TimeManager> sInstance_;

private:	// メンバ変数
	/// 経過時間
	static constexpr float kDeltaTime = 1.0f / 60.f;
};