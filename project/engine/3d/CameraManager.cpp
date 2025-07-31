#include "CameraManager.h"
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


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
	// 読み込み済みカメラを検索
	if (cameras_.contains(cameraName)) {
		// 読み込みカメラを現在のカメラとしてセット
		camera_ = cameras_.at(cameraName).get();
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
			for (const auto& pair : cameras_) {
				if (pair.second.get() == camera_) {
					cameraNowVlue = pair.first;
				}
			}
			
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
