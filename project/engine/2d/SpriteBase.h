#pragma once
#include "DirectXBase.h"
#include "Matrix.h"

/// <summary>
/// スプライト描画の共通処理を管理するクラス。
/// 各スプライト(Sprite クラス)は本クラスを通して
/// 描画パイプライン、ルートシグネチャ、ブレンドモード設定を共有する。
/// </summary>
class SpriteBase
{
public:	// 列挙型
	// ブレンドモード
	enum class BlendMode {
		kBlendModeNone,			//!< ブレンドなし
		kBlendModeNormal,		//!< 通常αブレンド。デフォルト。 Src * SrcA + Dest * (1 - SrcA)
		kBlendModeAdd,			//!< 加算。 Src * SrcA + Dest * 1
		kBlendModeSubtract,		//!< 減算。 Dest * 1 - Src * SrcA
		kBlendModeMultiply,		//!< 乗算。 Src * 0 + Dest * Src
		kBlendModeScreen,		//!< スクリーン。 Src * (1 - Dest) + Dest * 1
		kBlendModeExclusion,	//!< 除外。(1 - Dest) * Src + (1 - Src) * Dest

		kCountOfBlendMode,	//!< 内部管理用(配列サイズなどで使用)。ユーザーは使用しない
	};

public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// ルートシグネチャ・パイプライン生成に加え、
	/// スプライト描画で共通利用する View / Projection 行列を生成
	/// </summary>
	/// <param name="dxBase">DirectX の基本機能を提供する DirectXBase へのポインタ</param>
	void Initialize(DirectXBase* dxBase);

	/// <summary>
	/// 共通の画面設定(描画ターゲット、ビューポートなど)を適用
	/// </summary>
	void SetCommonScreen();

	/// <summary>
	/// スプライト描画用のグラフィックスパイプラインステートを生成
	/// </summary>
	void CreateGraphicsPipeline();

public:	// getter
	/// <summary>
	/// DirectXBase のポインタを取得
	/// </summary>
	/// <returns>DirectXBase へのポインタ</returns>
	DirectXBase* GetDxBase() const { return dxBase_; }

public:	// setter
	/// <summary>
	/// 現在のブレンドモードを設定
	/// </summary>
	/// <param name="blendMode">適用するブレンドモード。BlendMode 列挙型を使用します</param>
	void SetBlendMode(BlendMode blendMode);

private:	// メンバ関数
	/// <summary>
	/// スプライト描画用のルートシグネチャを作成
	/// </summary>
	void CreateRootSignature();

	// ブレンドモード設計
	D3D12_BLEND_DESC SetBlendModeNone();
	D3D12_BLEND_DESC SetBlendModeNormal();
	D3D12_BLEND_DESC SetBlendModeAdd();
	D3D12_BLEND_DESC SetBlendModeSubtract();
	D3D12_BLEND_DESC SetBlendModeMultiply();
	D3D12_BLEND_DESC SetBlendModeScreen();

	// ブレンドモードごとに対応するメンバ関数へのポインタテーブル
	// BlendMode の列挙値をインデックスとして対応関数を呼び出す
	static D3D12_BLEND_DESC(SpriteBase::* spFuncTable[])();

public:
	// スプライト描画で使用する共通の View 行列
	// 2D 描画ではカメラが固定のため、一度計算した行列を全スプライトで共有し、
	// 毎フレーム行列を生成するコストを削減している
	static inline MyBase::Matrix4x4 sViewMatrix = Matrix::MakeIdentity4x4();
	// スプライト描画で使用する共通の射影行列(正射影)
	// ウィンドウサイズに応じて一度だけ生成し、Sprite::Update で毎回計算しないようにする
	// DirectX12 のパフォーマンス最適化において非常に有効
	static inline MyBase::Matrix4x4 sProjectionMatrix = Matrix::MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsAPI::kClientWidth), float(WindowsAPI::kClientHeight), 0.0f, 100.0f);

private:	// メンバ変数
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;
	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

	// ブレンドモード
	BlendMode blendMode_ = BlendMode::kBlendModeNormal;

	// DirectXBase
	DirectXBase* dxBase_ = nullptr;
};

