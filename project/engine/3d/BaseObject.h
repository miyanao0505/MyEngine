#pragma once
#include <memory>
#include "Object3d.h"
#include "Collider.h"
#include "MyBase.h"

/// <summary>
/// ゲーム内に存在するすべてのオブジェクトの基底クラス。
/// BaseObject クラスは、描画・更新・衝突判定など、
/// すべてのゲームオブジェクトが共通して持つ基本的な処理を定義します。
/// </summary>
class BaseObject
{
public:	// メンバ関数
	BaseObject() = default;
	virtual ~BaseObject();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="folderPath">モデルや関連リソースが格納されているフォルダパス</param>
	/// <param name="filePath">初期化するモデルファイルのパス</param>
	void Initialize(const std::string& folderPath, const std::string& filePath);
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(float deltaTime = 0.0f);
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	virtual void DebugDraw();
#endif // _DEBUG

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <param name="other">衝突相手の Collider</param>
	virtual void OnCollision([[maybe_unused]] Collider* other);

public:	// getter
	/// <summary>
	/// オブジェクトのワールド座標を取得
	/// </summary>
	/// <returns>オブジェクトのワールド座標(Vector3)</returns>
	virtual MyBase::Vector3 GetWorldPosition() const;

	/// <summary>
	/// オブジェクトに関連付けられた Collider を取得
	/// </summary>
	/// <returns>Collider へのポインタ</returns>
	Collider* GetCollider() const { return collider_.get(); }
	
	/// <summary>
	/// オブジェクトに関連付けられた 3D モデルを取得
	/// </summary>
	/// <returns>Object3d へのポインタ</returns>
	Object3d* GetObject3D() const { return object_.get(); }
	
	/// <summary>
	/// オブジェクト名を取得
	/// </summary>
	/// <returns>オブジェクト名(文字列)</returns>
	std::string GetName() const { return name_; }
	
	/// <summary>
	/// オブジェクトが無効化されているかを取得
	/// </summary>
	/// <returns>true の場合はオブジェクトが無効、false の場合は有効</returns>
	bool IsDisabled() const { return isDisabled_; }

public:	// setter
	/// <summary>
	/// オブジェクトにモデルを設定
	/// </summary>
	/// <param name="modelPath">設定するモデルファイルのパス</param>
	void SetModel(const std::string& modelPath) { if (object_) { object_->SetModel(modelPath); } }
	
	/// <summary>
	/// Object3d インスタンスを設定
	/// </summary>
	/// <param name="object">所有権を移譲する Object3d のユニークポインタ</param>
	void SetObject3D(std::unique_ptr<Object3d> object) { object_ = std::move(object); }
	
	/// <summary>
	/// Collider インスタンスを設定
	/// </summary>
	/// <param name="collider">所有権を移譲する Collider のユニークポインタ</param>
	void SetCollider(std::unique_ptr<Collider> collider);
	
	/// <summary>
	/// オブジェクト名を設定
	/// </summary>
	/// <param name="name">設定する名前</param>
	void SetName(const std::string& name) { name_ = name; }
	
	/// <summary>
	/// オブジェクトの有効/無効状態を設定
	/// </summary>
	/// <param name="isDisabled">true にするとオブジェクトを無効化、false にすると有効化</param>
	void SetDisabled(bool isDisabled) { isDisabled_ = isDisabled; }

protected:	// メンバ変数
	std::unique_ptr<Object3d> object_ = nullptr;	// モデルデータ	
	std::unique_ptr<Collider> collider_ = nullptr;	// 衝突判定用コライダー

	std::string name_;								// オブジェクト名
	bool isDisabled_ = false;						// 無効化フラグ

#pragma region 定数
protected:
	static const MyBase::Vector3 kZeroVector;

#ifdef _DEBUG
private:
	static const float kPi;
	static const float kImGuiDragSpeed;				// ImGui のドラッグ速度	
	static const MyBase::ScopeF kTranslateScope;	// 平行移動の範囲
	static const MyBase::ScopeF kRotateScope;		// 回転の範囲
	static const MyBase::ScopeF kScaleScope;		// スケールの範囲

	static const MyBase::ScopeF kMaterialScope;		// マテリアルの範囲
#endif // _DEBUG
#pragma endregion
};