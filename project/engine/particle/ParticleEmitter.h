#pragma once
#include "MyBase.h"
#include "ParticleSystem.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

/// <summary>
/// パーティクルタイプ
/// </summary>
enum class ParticleType {
	kEllipse,	// 楕円
	kRing,		// リング
	kCylinder,	// 円柱
};

/// <summary>
/// パーティクルエミッタークラス(ParticleEmitter)
/// パーティクルシステムにおける「発生源」として機能するクラスです。
/// </summary>
class ParticleEmitter
{
public:	// メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleEmitter();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="textureFilePath">使用するテクスチャファイルのパス</param>
	/// <param name="type">パーティクルの形状タイプ</param>
	void Initialize(const std::string& name, const std::string& textureFilePath, const ParticleType type = ParticleType::kEllipse);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルの発生
	/// </summary>
	void Emit();

	/// <summary>
	/// 新しいパーティクルグループを作成
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="textureFilePath">使用するテクスチャファイル</param>
	/// <param name="type">パーティクルの形状タイプ</param>
	void CreateParticleGroup(const std::string& name, const std::string& textureFilePath, const ParticleType type = ParticleType::kEllipse);

#ifdef _DEBUG
	/// <summary>
	/// ImGuiを使用したデバッグ表示
	/// </summary>
	/// <param name="name">対象パーティクルグループ名</param>
	void ImGui(const std::string& name);
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// エミッターの位置を取得
	/// </summary>
	/// <returns>位置ベクトル</returns>
	const MyBase::Vector3& GetPosition() { return transform_.translate; }
	
	/// <summary>
	/// エミッターの回転を取得
	/// </summary>
	/// <returns>回転ベクトル</returns>
	const MyBase::Vector3& GetRotation() { return transform_.rotate; }
	
	/// <summary>
	/// エミッターのサイズを取得
	/// </summary>
	/// <returns>サイズベクトル</returns>
	const MyBase::Vector3& GetSize() { return transform_.scale; }
	
	/// <summary>
	/// 指定したパーティクルグループのデータを取得
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <returns>パーティクルグループデータ</returns>
	const ParticleSystem::ParticleGroupData& GetParticleGroupData(const std::string& name);
	
	/// <summary>
	/// 指定したパーティクルグループがビルボードかどうかを取得
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <returns>ビルボード設定か</returns>
	bool GetIsBillboard(const std::string& name);

public:	// setter
	/// <summary>
	/// エミッターの位置を設定
	/// </summary>
	/// <param name="position">位置ベクトル</param>
	void SetPosition(const MyBase::Vector3& position) { transform_.translate = position; }
	
	/// <summary>
	/// エミッターの回転を設定
	/// </summary>
	/// <param name="rotation">回転ベクトル</param>
	void SetRotation(const MyBase::Vector3& rotation) { transform_.rotate = rotation; }
	
	/// <summary>
	/// エミッターのサイズを設定
	/// </summary>
	/// <param name="size">サイズベクトル</param>
	void SetSize(const MyBase::Vector3& size) { transform_.scale = size; }
	
	/// <summary>
	/// 操作対象のパーティクルグループ名を設定
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	void SetParticleGroupName(const std::string& name);
	
	/// <summary>
	/// 指定パーティクルグループのデータを設定
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="particleGroupData">設定するパーティクルグループデータ</param>
	void SetParticleGroupData(const std::string& name, ParticleSystem::ParticleGroupData& particleGroupData);
	
	/// <summary>
	/// 指定パーティクルグループのビルボード設定を変更
	/// </summary>
	/// <param name="name">パーティクルグループ名</param>
	/// <param name="isBillboard">ビルボードにするか</param>
	void SetBillboard(const std::string& name, bool isBillboard);

private:	// メンバ変数
	// 発生中心
	MyBase::Transform transform_{};

	// パーティクルグループ名のリスト
	std::vector<std::string> particleGroupNames_;

	// パーティクルシステム
	std::unique_ptr<ParticleSystem> particleSystem_;

	// デルタタイム
	const float kDeltaTime_ = 1.0f / 60.0f;

#pragma region 定数
	static constexpr float kEmitThresholdTime = 0.0f;

#ifdef _DEBUG
	static constexpr float kImGuiDragSpeed = 0.1f;
	static constexpr ImVec2 kEmitButtonSize = { 100.0f, 50.0f };
#endif // _DEBUG
#pragma endregion
};

