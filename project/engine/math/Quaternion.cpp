#include "Quaternion.h"
#include "MyTools.h"

// Quaternionの和
Quaternion Quaternion::Add(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(0.0f, 0.0f, 0.0f, 0.0f);

	ans = { q.x + r.x, q.y + r.y, q.z + r.z, q.w + r.w };

	return ans;
}

// Quaternionの差
Quaternion Quaternion::Subtract(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(0.0f, 0.0f, 0.0f, 0.0f);

	ans = { q.x - r.x, q.y - r.y, q.z - r.z, q.w - r.w };

	return ans;
}

// Quaternionの積
Quaternion Quaternion::Multiply(const float s, const Quaternion& q)
{
	Quaternion ans(0.0f, 0.0f, 0.0f, 0.0f);

	ans = { s * q.x, s * q.y, s * q.z, s * q.w };

	return ans;
}

// Quaternionの積
Quaternion Quaternion::Multiply(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(0.0f, 0.0f, 0.0f, 0.0f);
	
	float w = q.w * r.w - MyTools::Dot({ q.x, q.y, q.z }, { r.x, r.y, r.z });
	MyBase::Vector3 result = MyTools::Cross({ q.x, q.y, q.z }, { r.x, r.y, r.z });
	result = MyTools::Add(result, MyTools::Multiply(r.w, { q.x, q.y, q.z }));
	result = MyTools::Add(result, MyTools::Multiply(q.w, { r.x, r.y, r.z }));
	
	ans = { result.x, result.y, result.z, w };

	return ans;
}

// Quaternionの内積
float Quaternion::Dot(const Quaternion& q, const Quaternion& r)
{
	return q.x * r.x + q.y * r.y + q.z * r.z + q.w * r.w;
}

// 単位Quaternionを返す
Quaternion Quaternion::IdentityQuaternion()
{
	return Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f };
}

// 共役Quaternionを返す
Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	return Quaternion{ -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// Quaternionのnormを返す
float Quaternion::Norm(const Quaternion& quaternion)
{
	float ans = 0.0f;

	ans = sqrtf(Dot(quaternion, quaternion));

	return ans;
}

// 正規化したQuaternionを返す
Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	float norm = Norm(quaternion);
	float x = quaternion.x, y = quaternion.y, z = quaternion.z, w = quaternion.w;
	if (norm != 0) {
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}

	return Quaternion{ x, y, z, w };
}

//  逆Quaternionを返す
Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
	Quaternion identity = IdentityQuaternion();
	Quaternion conjugate = Conjugate(quaternion);
	float normPow2 = powf(Norm(quaternion),2);

	if (quaternion.x == identity.x && quaternion.y == identity.y && quaternion.z == identity.z && quaternion.w == identity.w) {
		return conjugate;
	}

	return { conjugate.x / normPow2, conjugate.y / normPow2, conjugate.z / normPow2, conjugate.w / normPow2 };
}

// 任意軸回転を表すQuaternionの生成
Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const MyBase::Vector3& axis, float angle)
{
	Quaternion q(0.0f, 0.0f, 0.0f, 0.0f);

	q.w = cosf(angle / 2.0f);
	q.x = axis.x * sinf(angle / 2.0f);
	q.y = axis.y * sinf(angle / 2.0f);
	q.z = axis.z * sinf(angle / 2.0f);

	return q;
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
MyBase::Vector3 Quaternion::RotateVector(const MyBase::Vector3& vector, const Quaternion& quaternion)
{
	Quaternion ans(0.0f, 0.0f, 0.0f, 0.0f);
	Quaternion r(vector.x, vector.y, vector.z, 0.0f);
	
	ans = Multiply(quaternion, r);
	ans = Multiply(ans, Conjugate(quaternion));
	
	return MyBase::Vector3{ ans.x, ans.y, ans.z };
}

// Quaternionから回転行列を求める
Matrix::Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix::Matrix4x4 ans{ 0 };

	ans.m[0][0] = powf(quaternion.w, 2) + powf(quaternion.x, 2) - powf(quaternion.y, 2) - powf(quaternion.z, 2);
	ans.m[0][1] = 2 * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
	ans.m[0][2] = 2 * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);

	ans.m[1][0] = 2 * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
	ans.m[1][1] = powf(quaternion.w, 2) - powf(quaternion.x, 2) + powf(quaternion.y, 2) - powf(quaternion.z, 2);
	ans.m[1][2] = 2 * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);

	ans.m[2][0] = 2 * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
	ans.m[2][1] = 2 * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
	ans.m[2][2] = powf(quaternion.w, 2) - powf(quaternion.x, 2) - powf(quaternion.y, 2) + powf(quaternion.z, 2);

	ans.m[3][3] = 1.0f;

	return ans;
}

// 球面線形補間
Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion q2 = q0;
	// q0 と q1 の内積
	float dot = Dot(q2, q1);
	if (dot < 0) {
		q2 = Multiply(-1.0f, q2);	// もう片方の回転を利用する
		dot = -dot;					// 内積も反転
	}

	if (dot >= 1.0f - DBL_EPSILON) {
		return Add(Multiply((1.0f - t), q2), Multiply(t, q1));
	}

	// なす角を求める
	float theta = std::acosf(dot);

	// theta と sin を使って補間係数 scale0, scale1 を求める
	float scale0 = sinf((1.0f - t) * theta) / sinf(theta);
	float scale1 = sinf(t * theta) / sinf(theta);

	// それぞれの補間係数を利用して補間後の Quaternion を求める
	Quaternion ans = Add(Multiply(scale0, q2), Multiply(scale1, q1));

	return ans;
}