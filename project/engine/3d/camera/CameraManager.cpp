#include "CameraManager.h"
#include <imgui.h>

using namespace std;

CameraManager* CameraManager::instance = nullptr;

// シングルトンインスタンスの取得
CameraManager* CameraManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new CameraManager;
	}
	return instance;
}

// 終了
void CameraManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

// 初期化
void CameraManager::Initialize()
{

}

// カメラのセット
void CameraManager::SetCamera(const string& cameraName)
{
	// 読み込み済みカメラを検索
	if (cameras_.contains(cameraName)) {
		// 読み込み済みなら早期return
		return;
	}

	// カメラの生成と初期化
	unique_ptr<Camera> camera = make_unique<Camera>();
	
	// カメラをmapコンテナに格納する
	cameras_.insert(make_pair(cameraName, move(camera)));
}

// カメラの検索
void CameraManager::FindCamera(const string& cameraName)
{
	if(cameraName_ == cameraName){
		// 同じ名前なら早期return
		return;
	}

	// 読み込み済みカメラを検索
	if (cameras_.contains(cameraName)) {
		// 読み込みカメラを現在のカメラとしてセット
		camera_ = cameras_.at(cameraName).get();
		cameraName_ = cameraName;
	}
}

#ifdef _DEBUG
// デバック用の描画
void CameraManager::DebugDraw() {
	// カメラ
	if (ImGui::CollapsingHeader("Camera"))
	{
		// セットされている全カメラの名前取得
		vector<string> cameraNames = GetAllName();
		string cameraNowVlue;

		// 変更するための変数
		MyBase::Transform transformCamera{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
		// 現在指定しているカメラデータがある時
		if (camera_) {
			// transformをセット
			transformCamera = camera_->GetTransform();
			// 指定されているカメラの名前をセット
			cameraNowVlue = cameraName_;
		}
		// カメラが1つでもセットされている時
		else if(cameraNames.size() != 0) {
			// 最初のカメラのtransformをセット
			transformCamera = cameras_.at(cameraNames[0])->GetTransform();
			// 最初のカメラの名前をセット
			cameraNowVlue = cameraNames[0];
		}

		static ImGuiComboFlags flags = 0;
		static int cameraIndex = 0;

		if (ImGui::BeginCombo("Now Camera", cameraNowVlue.c_str(), flags))
		{
			for (int i = 0; i < cameraNames.size(); i++)
			{
				const bool isSelected = (cameraIndex == i);
				if (ImGui::Selectable(cameraNames[i].c_str(), isSelected)) {
					cameraIndex = i;
					CameraManager::GetInstance()->FindCamera(cameraNames[i]);
					transformCamera = CameraManager::GetInstance()->GetCamera()->GetTransform();
				}

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::DragFloat3("translate", &transformCamera.translate.x, 0.05f);
		ImGui::DragFloat3("rotate", &transformCamera.rotate.x, 0.05f);
		CameraManager::GetInstance()->GetCamera()->SetTransform(transformCamera);

		ImGui::Text("\n");

		ImGui::Text((shakeState_.active) ? "shakeState_.active : true" : "shakeState_.active : false");
		ImGui::Text("\n");

	}
}
#endif // _DEBUG

vector<string> CameraManager::GetAllName()
{
	vector<string> keys;
	keys.reserve(cameras_.size());

	for (const auto& pair : cameras_) {
		keys.push_back(pair.first);
	}

	return keys;
}

// 更新
void CameraManager::Update(float deltaTime)
{
	if (!camera_) return;

	// シェイク中でなければ何もしない
	if (!shakeState_.active) {
		return;
	}

	shakeState_.timer += deltaTime;
	// 経過割合（0..1）
	const float t = shakeState_.timer / max(1e-6f, shakeState_.duration);
	// 減衰（線形）。必要なら ease 関数に変更可
	const float damp = 1.0f - std::min(1.0f, t);

	// ランダムな方向を作って揺らす
	float rx = shakeState_.dist(shakeState_.rng);
	float ry = shakeState_.dist(shakeState_.rng);
	float rz = shakeState_.dist(shakeState_.rng);

	// 周波数による揺れ要素（正弦で揺らす）
	float phase = shakeState_.timer * shakeState_.frequency;
	float wave = std::sin(phase);

	MyBase::Vector3 offset{
		rx * shakeState_.amplitude * damp * wave,
		ry * shakeState_.amplitude * damp * wave,
		rz * shakeState_.amplitude * damp * wave
	};

	MyBase::Vector3 newTranslate{
		shakeState_.originalTranslate.x + offset.x,
		shakeState_.originalTranslate.y + offset.y,
		shakeState_.originalTranslate.z + offset.z
	};

	// カメラに適用
	camera_->SetTranslate(newTranslate);

	// 終了判定
	if (shakeState_.timer >= shakeState_.duration) {
		// 復帰
		StopShake();
	}
}

/// シェイクを開始
void CameraManager::StartShake(float amplitude, float duration, float frequency, float rotationAmplitude)
{
	if (!camera_) return;
	// シェイク状態を初期化
	shakeState_.active = true;
	shakeState_.duration = max(0.0f, duration);
	shakeState_.timer = 0.0f;
	shakeState_.amplitude = amplitude;
	shakeState_.frequency = frequency;
	shakeState_.rotationAmplitude = rotationAmplitude;

	// オリジナルの位置と回転を保存
	shakeState_.originalTranslate = camera_->GetTranslate();
}

/// 即時停止(オリジナルに復帰)
void CameraManager::StopShake()
{
	if (!camera_) return;
	if (shakeState_.active) {
		camera_->SetTranslate(shakeState_.originalTranslate);
	}
	// シェイク状態をリセット
	shakeState_.active = false;
	shakeState_.timer = 0.0f;
}