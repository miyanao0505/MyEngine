#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "SceneTransition.h"

/// <summary>
/// ゲームシーン管理クラス
/// シーンの生成・更新・描画・切り替えを管理するマネージャークラス。
/// </summary>
class SceneManager
{
public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>SceneManager</returns>
	static SceneManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コンストラクタを渡すための鍵
	class ConstructoKey {
	private:
		ConstructoKey() = default;
		friend class SceneManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit SceneManager(ConstructoKey) {}

	/// コピー禁止
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	/// <summary>
	/// 終了
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
	/// 次のシーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(SceneName sceneName);

public:	// setter
	/// <summary>
	/// シーンファクトリーのセット
	/// </summary>
	/// <param name="sceneFactory">シーンファクトリーのポインタ</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:	// Singleton Instance
	static std::unique_ptr<SceneManager> sInstance_;

private:	// メンバ変数
	// 今のシーン(実行中シーン)
	std::unique_ptr<BaseScene> scene_ = nullptr;

	// 次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	// シーン切り替え
	SceneTransition transition_;

	// シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};