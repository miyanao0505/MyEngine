#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "SceneTransition.h"

/// <summary>
/// ゲームシーン管理クラス
/// シーンの生成・更新・描画・切り替えを管理するシングルトンクラス。
/// </summary>
class SceneManager
{
public:	// メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンスのポインタ</returns>
	static SceneManager* GetInstance();

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

private:	// シングルトンインスタンス
	static SceneManager* sInstance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = default;
	SceneManager& operator=(SceneManager&) = delete;

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