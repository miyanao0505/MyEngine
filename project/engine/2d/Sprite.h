#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "MyBase.h"

// 前方宣言
class SpriteBase;

/// <summary>
/// 2Dスプライトを描画するためのクラス
/// スプライトの位置・回転・スケール・色・テクスチャ範囲などを設定し、
/// シンプルなUIや2D演出、タイトル画面などでの画像表示に利用できる。
/// </summary>
class Sprite
{
public:	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureFilePath">使用するテクスチャファイルのパス</param>
	void Initialize(std::string textureFilePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

#ifdef _DEBUG
	/// <summary>
	/// デバック描画
	/// </summary>
	void DebugDraw();
#endif // _DEBUG

public:	// getter
	/// <summary>
	/// スプライトの現在の位置(2D座標)を取得
	/// </summary>
	/// <returns>スプライトの位置を表す MyBase::Vector2 への参照</returns>
	const MyBase::Vector2& GetPosition() const { return position_; }

	/// <summary>
	/// スプライトの現在の回転角度を取得
	/// </summary>
	/// <returns>回転角度(ラジアン単位)</returns>
	float GetRotation() const { return rotation_; }

	/// <summary>
	/// スプライトに適用されているカラー情報を取得
	/// </summary>
	/// <returns>RGBA のカラー値を表す MyBase::Vector4 への参照</returns>
	const MyBase::Vector4& GetColor() const { return materialData_->color; }

	/// <summary>
	/// スプライトの現在の表示サイズを取得
	/// </summary>
	/// <returns>スプライトの幅と高さを表す MyBase::Vector2 への参照</returns>
	const MyBase::Vector2& GetSize() const { return size_; }

	/// <summary>
	/// スプライトのアンカーポイント(基準座標)を取得
	/// </summary>
	/// <returns>アンカーポイントを表す MyBase::Vector2 への参照。(0,0) が左上、(1,1) が右下を示します</returns>
	const MyBase::Vector2& GetAnchorPoint() const { return anchroPoint_; }

	/// <summary>
	/// スプライトが左右反転されているかを取得
	/// </summary>
	/// <returns>左右反転フラグの参照。true の場合、スプライトは左右に反転されています</returns>
	const bool& GetIsFlipX() const { return isFlipX_; }

	/// <summary>
	/// スプライトが上下反転されているかを取得
	/// </summary>
	/// <returns>上下反転フラグの参照。true の場合、スプライトは上下に反転されています</returns>
	const bool& GetIsFlipY() const { return isFlipY_; }

	/// <summary>
	/// スプライトで使用するテクスチャの描画範囲における、左上座標を取得
	/// </summary>
	/// <returns>テクスチャ範囲の左上位置を示す MyBase::Vector2 への参照。テクスチャのピクセル座標で指定されます。</returns>
	const MyBase::Vector2& GetTextureLeftTop() const { return textureLeftTop_; }

	/// <summary>
	/// スプライトで使用するテクスチャの切り出しサイズを取得
	/// </summary>
	/// <returns>テクスチャ範囲の幅と高さ(ピクセル単位)を表す MyBase::Vector2 への参照</returns>
	const MyBase::Vector2& GetTextureSize() const { return textureSize_; }

	/// <summary>
	/// スプライトが持つ本来のサイズ(テクスチャの解像度に基づく値)を取得
	/// </summary>
	/// <returns>スプライトの元となるテクスチャサイズを表す MyBase::Vector2 への参照</returns>
	const MyBase::Vector2& GetSpriteSize() const { return spriteSize_; }

public:	// setter
	/// <summary>
	/// スプライトの表示位置を設定
	/// </summary>
	/// <param name="position">スプライトに適用する新しい位置(2D座標)</param>
	void SetPosition(const MyBase::Vector2& position) { position_ = position; }

	/// <summary>
	/// スプライトの回転角度を設定
	/// </summary>
	/// <param name="rotation">新しい回転角度(ラジアン単位)</param>
	void SetRotation(float rotation) { rotation_ = rotation; }

	/// <summary>
	/// スプライトに適用するカラー値(RGBA)を設定
	/// </summary>
	/// <param name="color">新しく適用するカラー値。MyBase::Vector4 で RGBA を指定します</param>
	void SetColor(const MyBase::Vector4& color) { materialData_->color = color; }

	/// <summary>
	/// スプライトの表示サイズ(幅・高さ)を設定
	/// </summary>
	/// <param name="size">新しく適用するサイズ。MyBase::Vector2 で幅と高さを指定します</param>
	void SetSize(const MyBase::Vector2& size) { size_ = size; }

	/// <summary>
	/// スプライトに使用するテクスチャを変更
	/// </summary>
	/// <param name="textureFilePath">新しく適用するテクスチャファイルのパス</param>
	void SetTexture(std::string textureFilePath);

	/// <summary>
	/// スプライトのアンカーポイント(基準位置)を設定
	/// </summary>
	/// <param name="anchorPoint">新しく適用するアンカーポイント。(0,0) が左上、(1,1) が右下を示します</param>
	void SetAnchorPoint(const MyBase::Vector2& anchorPoint) { anchroPoint_ = anchorPoint; }
	
	/// <summary>
	/// スプライトの左右反転状態を設定
	/// </summary>
	/// <param name="isFlipX">左右反転を行う場合は true、通常表示の場合は false を指定します</param>
	void SetIsFlipX(const bool& isFlipX) { isFlipX_ = isFlipX; }

	/// <summary>
	/// スプライトの上下反転状態を設定
	/// </summary>
	/// <param name="isFlipY">上下反転を行う場合は true、通常表示の場合は false を指定します</param>
	void SetIsFlipY(const bool& isFlipY) { isFlipY_ = isFlipY; }

	/// <summary>
	/// スプライトで使用するテクスチャの描画範囲における左上座標を設定
	/// </summary>
	/// <param name="textureLeftTop">新しく設定するテクスチャ範囲の左上座標(ピクセル単位)</param>
	void SetTextureLeftTop(const MyBase::Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	/// <summary>
	/// スプライトで使用するテクスチャの切り出しサイズを設定
	/// </summary>
	/// <param name="textureSize">新しく設定するテクスチャ範囲の幅と高さ(ピクセル単位)</param>
	void SetTextureSize(const MyBase::Vector2& textureSize) { textureSize_ = textureSize; }				// 切り出しサイズ

private:	// メンバ関数
	/// <summary>
	/// スプライト描画用の頂点データを生成
	/// </summary>
	void CreateVertexData();

	/// <summary>
	/// スプライト描画用のマテリアルデータ(カラーやテクスチャ情報など)を生成
	/// </summary>
	void CreateMaterialData();

	/// <summary>
	/// スプライトの座標変換行列データ(World/View/Projection行列)を生成
	/// </summary>
	void CreateTransformationMatrixData();

	/// <summary>
	/// スプライトのサイズをテクスチャイメージのサイズに合わせて調整
	/// </summary>
	void AdjustTextureSize();

private:	// メンバ変数
	SpriteBase* spriteBase_ = nullptr;
	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;				// vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;				// index
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;				// マテリアル
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;	// 座標変換行列
	// バッファリソース内のデータを指すポインタ
	MyBase::SpriteVertexData* vertexData_ = nullptr;										// vertex
	uint32_t* indexData_ = nullptr;													// index
	MyBase::SpriteMaterial* materialData_ = nullptr;										// マテリアル
	MyBase::TransformationMatrix* transformationMatrixData_ = nullptr;				// 座標変換行列
	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};									// vertex
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};										// index

	// ファイルパス
	std::string filePath_;
	// スプライトのサイズ
	MyBase::Vector2 spriteSize_ = { 0.0f, 0.0f };

	// 座標
	MyBase::Vector2 position_ = { 0.0f, 0.0f };
	// 回転
	float rotation_ = 0.0f;
	// サイズ
	MyBase::Vector2 size_ = { 640.f, 360.f };

	// アンカーポイント
	MyBase::Vector2 anchroPoint_ = { 0.0f, 0.0f };
	// フリップ
	bool isFlipX_ = false;		// 左右
	bool isFlipY_ = false;		// 上下
	// テクスチャ範囲指定
	MyBase::Vector2 textureLeftTop_ = { 0.0f, 0.0f };	// 左上座標
	MyBase::Vector2 textureSize_ = { 100.0f, 100.0f };	// 切り出しサイズ

};
