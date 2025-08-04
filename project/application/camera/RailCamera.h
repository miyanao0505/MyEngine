#pragma once
#include "Camera.h"

class RailCamera : public Camera
{
public:	// メンバ関数
	// コンストラクタ
	RailCamera();
	~RailCamera();

	// 初期化
	void Initialize();

	// 更新
	void Update() override;

public:	// getter


public:	// setter


private:	// メンバ変数
	// オフセット
	MyBase::Vector3 ofSet_;
	// カメラの方向
	MyBase::Vector3 directional_;
};

