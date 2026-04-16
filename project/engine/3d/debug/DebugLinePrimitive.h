#pragma once
#include <vector>
#include "MyBase.h"

/// <summary>
/// デバッグラインプリミティブクラス
/// 各種形状をラインの集合として生成するユーティリティ。
/// </summary>
class DebugLinePrimitive{
public:	// メンバ関数
	/// <summary>
	/// ラインの生成
	/// </summary>
	/// <param name="outLines">生成されたラインを格納する配列</param>
	/// <param name="start">ラインの開始位置</param>
	/// <param name="end">ラインの終了位置</param>
	/// <param name="color">ラインの色</param>
	/// <param name="isHit">ラインがヒットしているかどうか</param>
	static void CreateaLine(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& start, const MyBase::Vector3& end, const MyBase::Vector4& color, bool isHit);

	/// <summary>
	/// ボックスの生成
	/// </summary>
	/// <param name="outLines">生成されたラインを格納する配列</param>
	/// <param name="center">ボックスの中心位置</param>
	/// <param name="size">ボックスのサイズ</param>
	/// <param name="worldMatrix">ボックスのワールド行列</param>
	/// <param name="color">ボックスの色</param>
	/// <param name="isHit">ボックスがヒットしているかどうか</param>
	static void CreateBox(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& center, const MyBase::Vector3& size, const MyBase::Matrix4x4& worldMatrix, const MyBase::Vector4& color, bool isHit);

	/// <summary>
	/// スフィアの生成
	/// </summary>
	/// <param name="outLines">生成されたラインを格納する配列</param>
	/// <param name="center">スフィアの中心位置</param>
	/// <param name="radius">スフィアの半径</param>
	/// <param name="segment">スフィアの分割数</param>
	/// <param name="color">スフィアの色</param>
	/// <param name="isHit">スフィアがヒットしているかどうか</param>
	static void CreateSphere(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& center, float radius, uint32_t segment, const MyBase::Vector4& color, bool isHit);

	/// <summary>
	/// OBBの生成
	/// </summary>
	/// <param name="outLines">生成されたラインを格納する配列</param>
	/// <param name="obb">OBBの情報</param>
	/// <param name="color">OBBの色</param>
	/// <param name="isHit">OBBがヒットしているかどうか</param>
	static void CreateOBB(std::vector<MyBase::DebugLine>& outLines, const MyBase::OBB& obb, const MyBase::Vector4& color, bool isHit);

	/// <summary>
	/// 曲線の生成
	/// </summary>
	/// <param name="outLines">生成されたラインを格納する配列</param>
	/// <param name="controlPoints">曲線の制御点</param>
	/// <param name="division">曲線の分割数</param>
	/// <param name="color">曲線の色</param>
	/// <param name="isHit">曲線がヒットしているかどうか</param>
	static void CreateSpline(std::vector<MyBase::DebugLine>& outLines, const std::vector<MyBase::Vector3>& controlPoints, uint32_t division, const MyBase::Vector4& color, bool isHit);
};