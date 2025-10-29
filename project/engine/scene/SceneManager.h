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
	// シングルトンインスタンスの取得
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
	void ChangeScene(const std::string& sceneName);

public:	// setter
	// シーンファクトリーのセット
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

private:	// シングルトンインスタンス
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = default;
	SceneManager& operator=(SceneManager&) = delete;

private:	// メンバ変数
	// 今のシーン(実行中シーン)
	BaseScene* scene_ = nullptr;

	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーン切り替え
	SceneTransition transition_;

	// シーンファクトリー(借りてくる)
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

