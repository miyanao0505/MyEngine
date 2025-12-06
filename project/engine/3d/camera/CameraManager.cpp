#include "CameraManager.h"
#include <imgui.h>

using namespace std;

CameraManager* CameraManager::sInstance = nullptr;

// シングルトンインスタンスの取得
CameraManager* CameraManager::GetInstance() {
	if (sInstance == nullptr) {
		sInstance = new CameraManager;
	}
	return sInstance;
}

// 終了
void CameraManager::Finalize() {
	delete sInstance;
	sInstance = nullptr;
}

// 初期化
void CameraManager::Initialize() {

}

// 更新
void CameraManager::Update(float deltaTime) {
	if (!activeCamera_) return;

	// シェイク中でなければ何もしない
	if (!shakeState_.active) {
		return;
	}

	// 経過時間を蓄積し、シェイク進行度を算出
	shakeState_.timer += deltaTime;
	const float t = shakeState_.timer / max(1e-6f, shakeState_.duration);

	// シェイクを徐々に収束のため減衰(線形)
	const float damp = 1.0f - std::min(1.0f, t);

	// ランダムな方向へ揺らすための乱数
	float rx = shakeState_.dist(shakeState_.rng);	// X方向ランダムオフセット
	float ry = shakeState_.dist(shakeState_.rng);	// Y方向ランダムオフセット
	float rz = shakeState_.dist(shakeState_.rng);	// Z方向ランダムオフセット

	// 周波数による揺れ要素(正弦で揺らす)
	float phase = shakeState_.timer * shakeState_.frequency;
	float wave = std::sin(phase);

	// 実際の揺れ量を計算
	MyBase::Vector3 offset{
		rx * shakeState_.amplitude * damp * wave,
		ry * shakeState_.amplitude * damp * wave,
		rz * shakeState_.amplitude * damp * wave
	};

	// 元の位置にオフセットを加えた新しい位置を計算
	MyBase::Vector3 newTranslate{
		shakeState_.originalTranslate.x + offset.x,
		shakeState_.originalTranslate.y + offset.y,
		shakeState_.originalTranslate.z + offset.z
	};

	// カメラに適用
	activeCamera_->SetTranslate(newTranslate);

	// シェイク終了判定
	if (shakeState_.timer >= shakeState_.duration) {
		// 復帰
		StopShake();
	}
}

// 新しいカメラを追加し、cameras_ に登録する
void CameraManager::AddCamera(const std::string& cameraName) {
	// 同名カメラが既に登録済みなら重複生成を避け早期return
	if (cameras_.contains(cameraName)) {
		return;
	}

	// 新しいカメラの生成
	unique_ptr<Camera> camera = make_unique<Camera>();

	// カメラをmapコンテナに格納する
	cameras_.insert(make_pair(cameraName, move(camera)));
}

/// シェイクを開始
void CameraManager::StartShake(float amplitude, float duration, float frequency, float rotationAmplitude) {
	if (!activeCamera_) return;

	// シェイク状態を初期化
	shakeState_.active = true;
	shakeState_.duration = max(0.0f, duration);
	shakeState_.timer = 0.0f;
	shakeState_.amplitude = amplitude;
	shakeState_.frequency = frequency;
	shakeState_.rotationAmplitude = rotationAmplitude;

	// オリジナルの位置と回転を保存
	shakeState_.originalTranslate = activeCamera_->GetTranslate();
}

/// 即時停止(オリジナルに復帰)
void CameraManager::StopShake() {
	if (!activeCamera_) return;

	// アクティブの時のみ復帰処理
	if (shakeState_.active) {
		activeCamera_->SetTranslate(shakeState_.originalTranslate);
	}

	// シェイク状態をリセット
	shakeState_.active = false;
	shakeState_.timer = 0.0f;
}

#ifdef _DEBUG
// デバック用の描画
void CameraManager::DebugDraw() {
	if (ImGui::CollapsingHeader("Camera"))
	{
		// 登録されているカメラ名一覧を取得
		vector<string> cameraNames = GetAllNames();
		string cameraNowValue;

		// 編集用の Transform 一時変数
		MyBase::Transform transformCamera{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

		// 現在指定しているカメラデータがある時
		if (activeCamera_) {
			// transformをセット
			transformCamera = activeCamera_->GetTransform();
			// 指定されているカメラの名前をセット
			cameraNowValue = activeCameraName_;
		}
		// 指定のカメラデータがない時
		else if (cameraNames.size() != 0) {
			// 最初のカメラのtransformをセット
			transformCamera = cameras_.at(cameraNames[0])->GetTransform();
			// 最初のカメラの名前をセット
			cameraNowValue = cameraNames[0];
		}

		static ImGuiComboFlags flags = 0;
		static int cameraIndex = 0;

		if (ImGui::BeginCombo("Now Camera", cameraNowValue.c_str(), flags))
		{
			for (int i = 0; i < cameraNames.size(); i++)
			{
				const bool isSelected = (cameraIndex == i);
				if (ImGui::Selectable(cameraNames[i].c_str(), isSelected)) {
					cameraIndex = i;

					// 選択変更したら CameraManager からカメラを探索
					CameraManager::GetInstance()->SetCamera(cameraNames[i]);

					// 編集用の Transform を取得
					transformCamera = CameraManager::GetInstance()->GetCamera()->GetTransform();
				}

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		// 移動・回転の編集
		ImGui::DragFloat3("translate", &transformCamera.translate.x, 0.05f);
		ImGui::DragFloat3("rotate", &transformCamera.rotate.x, 0.05f);
		// 編集された Transform を即反映
		CameraManager::GetInstance()->GetCamera()->SetTransform(transformCamera);

		ImGui::Text("\n");

		// シェイクの有効状態をデバック表示
		ImGui::Text((shakeState_.active) ? "shakeState_.active : true" : "shakeState_.active : false");
		ImGui::Text("\n");

	}
}
#endif // _DEBUG

// 登録されているすべてのカメラ名を取得
vector<string> CameraManager::GetAllNames() {
	vector<string> keys;
	keys.reserve(cameras_.size());

	for (const auto& pair : cameras_) {
		keys.push_back(pair.first);
	}

	return keys;
}

// 既存のカメラの中からアクティブカメラを選択する
void CameraManager::SetCamera(const string& cameraName) {
	// 既にそのカメラを選択している場合は早期return
	if (activeCameraName_ == cameraName) {
		return;
	}

	// 登録済みのカメラのみ選択可能
	if (cameras_.contains(cameraName)) {
		// 指定したカメラを現在のカメラとしてセット
		activeCamera_ = cameras_.at(cameraName).get();
		activeCameraName_ = cameraName;
	}
}