#include "Quaternion.h"
#include "MyTools.h"

// Quaternionの和
Quaternion Quaternion::Add(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(kZero, kZero, kZero, kZero);

	ans = { q.x + r.x, q.y + r.y, q.z + r.z, q.w + r.w };

	return ans;
}

// Quaternionの差
Quaternion Quaternion::Subtract(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(kZero, kZero, kZero, kZero);

	ans = { q.x - r.x, q.y - r.y, q.z - r.z, q.w - r.w };

	return ans;
}

// Quaternionの積
Quaternion Quaternion::Multiply(const float s, const Quaternion& q)
{
	Quaternion ans(kZero, kZero, kZero, kZero);

	ans = { s * q.x, s * q.y, s * q.z, s * q.w };

	return ans;
}

// Quaternionの積
Quaternion Quaternion::Multiply(const Quaternion& q, const Quaternion& r)
{
	Quaternion ans(kZero, kZero, kZero, kZero);
	
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
	return Quaternion{ kZero, kZero, kZero, kOne };
}

// 共役Quaternionを返す
Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	return Quaternion{ -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

// Quaternionのnormを返す
float Quaternion::Norm(const Quaternion& quaternion)
{
	float ans = kZero;

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
	float normPow2 = powf(Norm(quaternion), kDouble);

	if (quaternion.x == identity.x && quaternion.y == identity.y && quaternion.z == identity.z && quaternion.w == identity.w) {
		return conjugate;
	}

	return { conjugate.x / normPow2, conjugate.y / normPow2, conjugate.z / normPow2, conjugate.w / normPow2 };
}

// 任意軸回転を表すQuaternionの生成
Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const MyBase::Vector3& axis, float angle)
{
	Quaternion q(kZero, kZero, kZero, kZero);

	q.w = cosf(angle / kDouble);
	q.x = axis.x * sinf(angle / kDouble);
	q.y = axis.y * sinf(angle / kDouble);
	q.z = axis.z * sinf(angle / kDouble);

	return q;
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
MyBase::Vector3 Quaternion::RotateVector(const MyBase::Vector3& vector, const Quaternion& quaternion)
{
	Quaternion ans(kZero, kZero, kZero, kZero);
	Quaternion r(vector.x, vector.y, vector.z, kZero);
	
	ans = Multiply(quaternion, r);
	ans = Multiply(ans, Conjugate(quaternion));
	
	return MyBase::Vector3{ ans.x, ans.y, ans.z };
}

// Quaternionから回転行列を求める
MyBase::Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion)
{
	MyBase::Matrix4x4 ans{ kZero };

	ans.m[0][0] = powf(quaternion.w, kDouble) + powf(quaternion.x, kDouble) - powf(quaternion.y, kDouble) - powf(quaternion.z, kDouble);
	ans.m[0][1] = kDouble * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
	ans.m[0][2] = kDouble * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);

	ans.m[1][0] = kDouble * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
	ans.m[1][1] = powf(quaternion.w, kDouble) - powf(quaternion.x, kDouble) + powf(quaternion.y, kDouble) - powf(quaternion.z, kDouble);
	ans.m[1][2] = kDouble * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);

	ans.m[2][0] = kDouble * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
	ans.m[2][1] = kDouble * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
	ans.m[2][2] = powf(quaternion.w, kDouble) - powf(quaternion.x, kDouble) - powf(quaternion.y, kDouble) + powf(quaternion.z, kDouble);

	ans.m[3][3] = 1.0f;

	return ans;
}

// 球面線形補間
Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion q2 = q0;
	// q0 と q1 の内積
	float dot = Dot(q2, q1);
	if (dot < kZero) {
		q2 = Multiply(kInvert, q2);	// もう片方の回転を利用する
		dot = -dot;					// 内積も反転
	}

	if (dot >= kOne - DBL_EPSILON) {
		return Add(Multiply((kOne - t), q2), Multiply(t, q1));
	}

	// なす角を求める
	float theta = std::acosf(dot);

	// theta と sin を使って補間係数 scale0, scale1 を求める
	float scale0 = sinf((kOne - t) * theta) / sinf(theta);
	float scale1 = sinf(t * theta) / sinf(theta);

	// それぞれの補間係数を利用して補間後の Quaternion を求める
	Quaternion ans = Add(Multiply(scale0, q2), Multiply(scale1, q1));

	return ans;
}