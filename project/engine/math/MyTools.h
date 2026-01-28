#pragma once
#include <cmath>
#include <numbers>
#include "MyBase.h"

/// <summary>
/// ゲームエンジン・3Dグラフィックス向けユーティリティクラス
/// </summary>
class MyTools
{
public:	// メンバ変数
	static constexpr int kColumnWidth = 60;			// 表示文字サイズ_横幅
	static constexpr int kRowHeight = 20;			// 表示文字サイズ_縦幅

private:
#pragma region 定数
	static constexpr uint32_t kOBBVertexCount = 8;
	static constexpr uint32_t kSeparationAxisCount = 15;
#pragma endregion

public:	// メンバ関数
#pragma region ツール関数	
	/// <summary>
	/// 範囲内の値を返す関数
	/// </summary>
	/// <param name="num">確認する値</param>
	/// <param name="min">最低値</param>
	/// <param name="max">最大値</param>
	/// <returns>範囲内の値</returns>
	static float Clamp(float num, float min, float max);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="num1">数値1</param>
	/// <param name="num2">数値2</param>
	/// <param name="t">媒介変数</param>
	/// <returns>補間後の値</returns>
	static float Lerp(float num1, float num2, float t);

	/// <summary>
	/// 三角形の存在する平面情報を求める関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <returns>三角形の定義する平面情報</returns>
	static MyBase::Plane TriangleToPlane(const MyBase::Triangle& triangle);

	/// <summary>
	/// 2つの球の衝突判定を行う関数
	/// </summary>
	/// <param name="sphere1">球A</param>
	/// <param name="sphere2">球B</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::Sphere& sphere1, const MyBase::Sphere& sphere2);

	/// <summary>
	/// 球と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::Sphere& sphere, const MyBase::Plane& plane);

	/// <summary>
	/// 直線と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="line">直線</param>
	/// <param name="plane">平面</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Line& line, const MyBase::Plane& plane);

	/// <summary>
	/// 半直線と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="ray">半直線</param>
	/// <param name="plane">平面</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Ray& ray, const MyBase::Plane& plane);

	/// <summary>
	/// 線分と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="segment">線分</param>
	/// <param name="plane">平面</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Segment& segment, const MyBase::Plane& plane);

	/// <summary>
	/// 三角形と直線の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="line">直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Triangle& triangle, const MyBase::Line& line);

	/// <summary>
	/// 三角形と半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="ray">半直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Triangle& triangle, const MyBase::Ray& ray);

	/// <summary>
	/// 三角形と線分の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="segment">線分</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::Triangle& triangle, const MyBase::Segment& segment);

	/// <summary>
	/// AABB同士の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb1">aabbA</param>
	/// <param name="aabb2">aabbB</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb1, const MyBase::AABB& aabb2);

	/// <summary>
	/// AABBと球の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="sphere">球</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb, const MyBase::Sphere& sphere);

	/// <summary>
	/// AABBと直線の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="line">直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb, const MyBase::Line& line);

	/// <summary>
	/// AABBと半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="ray">半直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb, const MyBase::Ray& ray);

	/// <summary>
	/// AABBと線分の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="segment">線分</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb, const MyBase::Segment& segment);

	/// <summary>
	/// AABBと点の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="point">点</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::AABB& aabb, const MyBase::Vector3& point);

	/// <summary>
	/// OBBと球の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="sphere">球</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::OBB& obb, const MyBase::Sphere& sphere);

	/// <summary>
	/// OBBと直線の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="line">直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::OBB& obb, const MyBase::Line& line);

	/// <summary>
	/// OBBと半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="ray">半直線</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::OBB& obb, const MyBase::Ray& ray);

	/// <summary>
	/// OBBと線分の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="segment">線分</param>
	/// <returns>交差する場合 true</returns>
	static bool IsCollision(const MyBase::OBB& obb, const MyBase::Segment& segment);

	/// <summary>
	/// OBBとOBBの衝突判定を返す関数
	/// </summary>
	/// <param name="obb1">OBB1</param>
	/// <param name="obb2">OBB2</param>
	/// <returns>衝突している場合 true</returns>
	static bool IsCollision(const MyBase::OBB& obb1, const MyBase::OBB& obb2);

	/// <summary>
	/// 分離軸に投影された軸成分から投影線分長を算出
	/// </summary>
	/// <param name="seg">各軸方向ベクトル</param>
	/// <param name="e1">軸1</param>
	/// <param name="e2">軸2</param>
	/// <param name="e3">軸3</param>
	/// <returns>投影された線分長</returns>
	static float LenSegOnSeparateAxis(const MyBase::Vector3* seg, const MyBase::Vector3* e1, const MyBase::Vector3* e2, const MyBase::Vector3* e3);
#pragma endregion ツール関数

#pragma region 2次元ベクトル
	/// <summary>
	/// 2次元ベクトルの加算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>加算結果のベクトル</returns>
	static MyBase::Vector2 Add(const MyBase::Vector2& v1, const MyBase::Vector2& v2);

	/// <summary>
	/// 2次元ベクトルの減算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>減算結果のベクトル</returns>
	static MyBase::Vector2 Subtract(const MyBase::Vector2& v1, const MyBase::Vector2& v2);

	/// <summary>
	/// 2次元ベクトルのスカラー倍
	/// </summary>
	/// <param name="scalar">スカラー倍</param>
	/// <param name="v">ベクトル</param>
	/// <returns>スカラー倍したベクトル</returns>
	static MyBase::Vector2 Multiply(float scalar, const MyBase::Vector2 v);

	/// <summary>
	/// 2次元ベクトルの内積を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>内積の値</returns>
	static float Dot(const MyBase::Vector2& v1, const MyBase::Vector2& v2);

	/// <summary>
	/// 2次元ベクトルのクロス積(外積)を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>外積の値</returns>
	static float Cross(const MyBase::Vector2& v1, const MyBase::Vector2& v2);

	/// <summary>
	/// 2次元ベクトルの長さ(ノルム)を返す関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	/// <returns>ベクトルの長さ</returns>
	static float Length(float x, float y);

	/// <summary>
	/// 2次元ベクトルの長さ(ノルム)を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>ベクトルの長さ</returns>
	static float Length(const MyBase::Vector2& v);

	/// <summary>
	/// 2次元ベクトルの正規化した値を返す関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	/// <returns>正規化したベクトル</returns>
	static MyBase::Vector2 Normalize(float x, float y);

	/// <summary>
	/// 2次元ベクトルの正規化した値を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>正規化したベクトル</returns>
	static MyBase::Vector2 Normalize(const MyBase::Vector2& v);

	/// <summary>
	/// 2次元ベクトルの方向を求める関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	///	<returns>方向ベクトル</returns>
	static MyBase::Vector2 Direction(float x, float y);
#pragma endregion 2次元ベクトル

#pragma region 3次元ベクトル	
	/// <summary>
	/// 3次元ベクトルの加算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>加算結果のベクトル</returns>
	static MyBase::Vector3 Add(const MyBase::Vector3& v1, const MyBase::Vector3& v2);

	/// <summary>
	/// 3次元ベクトルの減算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>減算結果のベクトル</returns>
	static MyBase::Vector3 Subtract(const MyBase::Vector3& v1, const MyBase::Vector3& v2);

	/// <summary>
	/// 3次元ベクトルのスカラー倍
	/// </summary>
	/// <param name="scalar">スカラー値</param>
	/// <param name="v">ベクトル</param>
	/// <returns>スカラー倍されたベクトル</returns>
	static MyBase::Vector3 Multiply(float scalar, const MyBase::Vector3& v);

	/// <summary>
	/// 3次元ベクトルの内積を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>内積の値</returns>
	static float Dot(const MyBase::Vector3& v1, const MyBase::Vector3& v2);

	/// <summary>
	/// 3次元ベクトルのクロス積(外積)を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	/// <returns>外積のベクトル</returns>
	static MyBase::Vector3 Cross(const MyBase::Vector3& v1, const MyBase::Vector3& v2);

	/// <summary>
	/// 3次元ベクトル長さ(ノルム)を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>ベクトルの長さ</returns>
	static float Length(const MyBase::Vector3& v);

	/// <summary>
	/// 3次元ベクトルの正規化した値を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>正規化したベクトル</returns>
	static MyBase::Vector3 Normalize(const MyBase::Vector3& v);

	/// <summary>
	/// 3次元ベクトルを標準化して返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>標準化したベクトル</returns>
	static MyBase::Vector3 Standardization(const MyBase::Vector3& v);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="vector1">開始ベクトル</param>
	/// <param name="vector2">終了ベクトル</param>
	/// <param name="t">媒介変数</param>
	/// <returns>線形補間された3次元ベクトル</returns>
	static MyBase::Vector3 Lerp(const MyBase::Vector3& vector1, const MyBase::Vector3& vector2, float t);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="vector1">開始ベクトル</param>
	/// <param name="vector2">終了ベクトル</param>
	/// <param name="t">媒介変数</param>
	/// <returns>球面線形補間された3次元ベクトル</returns>
	static MyBase::Vector3 Slerp(const MyBase::Vector3& vector1, const MyBase::Vector3& vector2, float t);

	/// <summary>
	/// CatmullRom補間
	/// </summary>
	/// <param name="p0">点0の座標</param>
	/// <param name="p1">点1の座標</param>
	/// <param name="p2">点2の座標</param>
	/// <param name="p3">点3の座標</param>
	/// <param name="t">点1を0.0f、点2を1.0fとした割合指定</param>
	/// <returns>点1と点2の間で指定された座標</returns>
	static MyBase::Vector3 CatmullRomInterpolation(const MyBase::Vector3& p0, const MyBase::Vector3& p1, const MyBase::Vector3& p2, const MyBase::Vector3& p3, float t);

	/// <summary>
	/// CatmullRomスプライン曲線上の座標を得る
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="t">スプラインの全区間の中での割合指定[0, 1]</param>
	/// <returns>座標</returns>
	static MyBase::Vector3 CatmullRomPosition(const std::vector<MyBase::Vector3>& points, float t);

	/// <summary>
	/// 正射影ベクトル(ベクトル射影)を返す関数
	/// </summary>
	/// <param name="v1">射影されるベクトル</param>
	/// <param name="v2">射影先のベクトル</param>
	/// <returns>正射影ベクトル</returns>
	static MyBase::Vector3 Project(const MyBase::Vector3& v1, const MyBase::Vector3& v2);

	/// <summary>
	/// 最近接点を返す関数
	/// </summary>
	/// <param name="point">始点</param>
	/// <param name="segment">線分</param>
	/// <returns>最近接点</returns>
	static MyBase::Vector3 ClosestPoint(const MyBase::Vector3& point, const MyBase::Segment& segment);

	/// <summary>
	/// 垂直なベクトルを求める関数
	/// </summary>
	/// <param name="vector">基準ベクトル</param>
	/// <returns>垂直なベクトル</returns>
	static MyBase::Vector3 Perpendicular(const MyBase::Vector3& vector);

	/// <summary>
	/// 反射ベクトルを求める関数
	/// </summary>
	/// <param name="input">入射ベクトル</param>
	/// <param name="normal">法線ベクトル(正規化済み)</param>
	/// <returns>反射ベクトル</returns>
	static MyBase::Vector3 Reflect(const MyBase::Vector3& input, const MyBase::Vector3& normal);
#pragma endregion 3次元ベクトル
};