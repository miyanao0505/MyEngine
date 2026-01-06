#pragma once

/// <summary>
/// ゲーム全体で使用する時間やタイマー関係を管理するクラス
/// </summary>
class TimeManager
{
public:	// メンバ関数
	static TimeManager* GetInstance();

	void Initialize();

	void Finalize();

public:	// getter
	float GetDeltaTime() { return kDeltaTime; }

public:	// setter


public:	// シングルトンインスタンス
	static TimeManager* sInstance;

	TimeManager() = default;
	~TimeManager() = default;
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

private:	// メンバ変数
	static constexpr float kDeltaTime = 1.0f / 60.f;
};

