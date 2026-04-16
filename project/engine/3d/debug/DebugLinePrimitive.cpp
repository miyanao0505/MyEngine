#define _USE_MATH_DEFINES
#include "DebugLinePrimitive.h"
#include "MyTools.h"
#include "Matrix.h"
#include <cmath>

using namespace std;
using namespace MyBase;

/// ラインの生成
void DebugLinePrimitive::CreateaLine(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& start, const MyBase::Vector3& end, const MyBase::Vector4& color, bool isHit) {
	outLines.push_back({start, end, color, isHit});
}

/// ボックスの生成
void DebugLinePrimitive::CreateBox(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& center, const MyBase::Vector3& size, const MyBase::Matrix4x4& worldMatrix, const MyBase::Vector4& color, bool isHit) {
	Vector3 half = { size.x * 0.5f, size.y * 0.5f, size.z * 0.5f };

	Vector3 vertices[8] = {
		{ -half.x, -half.y, -half.z },
		{  half.x, -half.y, -half.z },
		{  half.x,  half.y, -half.z },
		{ -half.x,  half.y, -half.z },
		{ -half.x, -half.y,  half.z },
		{  half.x, -half.y,  half.z },
		{  half.x,  half.y,  half.z },
		{ -half.x,  half.y,  half.z }
	};

	// ワールド変換
	for (auto& v : vertices) {
		v = Matrix::Transform(v, worldMatrix);
		v = MyTools::Add(v, center);
	}

	int indices[][2] = {
		{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
		{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
		{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
	};

	for (auto& idx : indices) {
		outLines.push_back({ vertices[idx[0]], vertices[idx[1]], color, isHit });
	}
}

/// スフィアの生成
void DebugLinePrimitive::CreateSphere(std::vector<MyBase::DebugLine>& outLines, const MyBase::Vector3& center, float radius, uint32_t segment, const MyBase::Vector4& color, bool isHit) {
	const float step = 2.0f * M_PI / static_cast<float>(segment);

	// XY / XZ / YZ の3円描画
	for (int axis = 0; axis < 3; ++axis) {
		for (uint32_t i = 0; i < segment; ++i) {
			float t0 = step * i;
			float t1 = step * (i + 1);

			Vector3 p0{}, p1{};

			switch (axis) {
			case 0: // XY
				p0 = { radius * cosf(t0), radius * sinf(t0), 0 };
				p1 = { radius * cosf(t1), radius * sinf(t1), 0 };
				break;
			case 1: // XZ
				p0 = { radius * cosf(t0), 0, radius * sinf(t0) };
				p1 = { radius * cosf(t1), 0, radius * sinf(t1) };
				break;
			case 2: // YZ
				p0 = { 0, radius * cosf(t0), radius * sinf(t0) };
				p1 = { 0, radius * cosf(t1), radius * sinf(t1) };
				break;
			default:
				break;
			}

			outLines.push_back({ MyTools::Add(p0, center), MyTools::Add(p1, center), color, isHit });
		}
	}
}

/// OBBの生成
void DebugLinePrimitive::CreateOBB(std::vector<MyBase::DebugLine>& outLines, const MyBase::OBB& obb	, const MyBase::Vector4& color, bool isHit) {
	// 軸ベクトル
	Vector3 ax = MyTools::Multiply(obb.size.x, obb.orientations[0]);
	Vector3 ay = MyTools::Multiply(obb.size.y, obb.orientations[1]);
	Vector3 az = MyTools::Multiply(obb.size.z, obb.orientations[2]);

	Vector3 c = obb.center;

	Vector3 cSubAx = MyTools::Subtract(c, ax);
	Vector3 cAddAx = MyTools::Add(c, ax);
	Vector3 aySubAz = MyTools::Subtract(ay, az);
	Vector3 ayAddAz = MyTools::Add(ay, az);

	// 8頂点
	Vector3 v[8] = {
		MyTools::Subtract(cSubAx, aySubAz),	// 0
		MyTools::Subtract(cAddAx, aySubAz),	// 1
		MyTools::Add(cAddAx, aySubAz),		// 2
		MyTools::Add(cSubAx, aySubAz),		// 3
		MyTools::Subtract(cSubAx, ayAddAz),	// 4
		MyTools::Subtract(cAddAx, ayAddAz),	// 5
		MyTools::Add(cAddAx, ayAddAz),		// 6
		MyTools::Add(cSubAx, ayAddAz)		// 7
	};

	int idx[][2] = {
		{ 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
		{ 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
		{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
	};

	for (auto& i : idx) {
		outLines.push_back({ v[i[0]], v[i[1]], color, isHit });
	}
}

/// 曲線の生成
void DebugLinePrimitive::CreateSpline(std::vector<MyBase::DebugLine>& outLines, const std::vector<MyBase::Vector3>& controlPoints, uint32_t division, const MyBase::Vector4& color, bool isHit) {
	if (controlPoints.size() < 4) return;

	for (size_t i = 0; i < controlPoints.size() - 3; ++i) {
		Vector3 prev = controlPoints[i + 1]; // 最初の点は制御点の2番目

		for (uint32_t j = 1; j <= division; ++j) {
			float t = float(j) / division;

			Vector3 curr = MyTools::CatmullRomInterpolation(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);

			outLines.push_back({ prev, curr, color, isHit });
			prev = curr;
		}
	}
}