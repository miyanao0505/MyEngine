#pragma once
#include "Matrix.h"
#include "MyBase.h"

/// <summary>
/// クォータニオン操作クラス
/// </summary>
class Quaternion
{
public:	// メンバ変数
	static const int kColumnWidth = 70;			// 表示文字サイズ_横幅
	static const int kRowHeight = 20;			// 表示文字サイズ_縦幅

public:	// メンバ変数
	float x, y, z, w;

public:	// メンバ関数
	// コンストラクタ
	Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	/// 複合代入演算子
	Quaternion& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	Quaternion& operator-=(const Quaternion& q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
	Quaternion& operator+=(const Quaternion& q) { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
	Quaternion& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

	/// <summary>
	/// Quaternionの和
	/// </summary>
	/// <param name="q">Quaternion1</param>
	/// <param name="r">Quaternion2</param>
	/// <returns>加算結果のQuaternion</returns>
	static Quaternion Add(const Quaternion& q, const Quaternion& r);

	/// <summary>
	/// Quaternionの差
	/// </summary>
	/// <param name="q">Quaternion1</param>
	/// <param name="r">Quaternion2</param>
	/// <returns>減算結果のQuaternion</returns>
	static Quaternion Subtract(const Quaternion& q, const Quaternion& r);

	/// <summary>
	/// Quaternionの積
	/// </summary>
	/// <param name="s">スカラー</param>
	/// <param name="q">Quaternion</param>
	/// <returns>乗算結果のQuaternion</returns>
	static Quaternion Multiply(const float s, const Quaternion& q);

	/// <summary>
	/// Quaternionの積
	/// </summary>
	/// <param name="q">Quaternion1</param>
	/// <param name="r">Quaternion2</param>
	/// <returns>乗算結果のQuaternion</returns>
	static Quaternion Multiply(const Quaternion& q, const Quaternion& r);

	/// <summary>
	/// Quaternionの内積
	/// </summary>
	/// <param name="q">Quaternion1</param>
	/// <param name="r">Quaternion2</param>
	/// <returns>内積の値</returns>
	static float Dot(const Quaternion& q, const Quaternion& r);

	/// <summary>
	/// 単位Quaternionを返す
	/// </summary>
	/// <returns>単位Quaternion</returns>
	static Quaternion IdentityQuaternion();

	/// <summary>
	/// 共役Quaternionを返す
	/// </summary>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>共役Quaternion</returns>
	static Quaternion Conjugate(const Quaternion& quaternion);

	/// <summary>
	/// Quaternionのnormを返す
	/// </summary>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>normの値</returns>
	static float Norm(const Quaternion& quaternion);

	/// <summary>
	/// 正規化したQuaternionを返す
	/// </summary>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>正規化したQuaternion</returns>
	static Quaternion Normalize(const Quaternion& quaternion);

	/// <summary>
	/// 逆Quaternionを返す
	/// </summary>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>逆Quaternion</returns>
	static Quaternion Inverse(const Quaternion& quaternion);

	/// <summary>
	/// 任意軸回転を表すQuaternionの生成
	/// </summary>
	/// <param name="axis">任意軸</param>
	/// <param name="angle">角度</param>
	/// <returns>回転を表すQuaternion</returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const MyBase::Vector3& axis, float angle);

	/// <summary>
	/// ベクトルをQuaternionで回転させた結果のベクトルを求める
	/// </summary>
	/// <param name="vector">回転させるベクトル</param>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>回転後のベクトル</returns>
	static MyBase::Vector3 RotateVector(const MyBase::Vector3& vector, const Quaternion& quaternion);

	/// <summary>
	/// Quaternionから回転行列を求める
	/// </summary>
	/// <param name="quaternion">Quaternion</param>
	/// <returns>回転行列</returns>
	static MyBase::Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="q0">Quaternion1</param>
	/// <param name="q1">Quaternion2</param>
	/// <param name="t">媒介変数</param>
	/// <returns>補間結果のQuaternion</returns>
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
};

