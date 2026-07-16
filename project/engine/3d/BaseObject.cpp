#include "BaseObject.h"
#include <imgui.h>
#include <numbers>
#include "ModelManager.h"
#include "CollisionManager.h"

using namespace std;
using namespace numbers;

// 定数の定義と初期化
const MyBase::Vector3 BaseObject::kZeroVector{ 0.0f, 0.0f, 0.0f };
#ifdef _DEBUG
const float BaseObject::kPi = pi_v<float>;
const float BaseObject::kImGuiDragSpeed = 0.01f;
const MyBase::ScopeF BaseObject::kTranslateScope{ -100.0f, 100.0f };
const MyBase::ScopeF BaseObject::kRotateScope{ -kPi, kPi };
const MyBase::ScopeF BaseObject::kScaleScope{ 0.01f, 10.0f };
const MyBase::ScopeF BaseObject::kMaterialScope{ 0.0f, 1.0f };
#endif // _DEBUG

BaseObject::~BaseObject() {
	if (CollisionManager::Exists()) {
		CollisionManager::GetInstance()->Unregister(collider_.get());
	}
}

// 初期化
void BaseObject::Initialize(const std::string& folderPath, const std::string& filePath) {
	// モデル読み込み
	ModelManager::GetInstance()->LoadModel(folderPath, filePath);

	// Object3d の生成と初期化
	object_ = make_unique<Object3d>();
	object_->Initialize(filePath);
}

// 更新
void BaseObject::Update() {
	object_->Update();
}

// 描画
void BaseObject::Draw() {
	if (isDisabled_) return;
	object_->Draw();
}

#ifdef _DEBUG
// デバック描画
void BaseObject::DebugDraw() {
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("object")) {
		if (ImGui::TreeNode("Transform")) {
			// 現在の変換情報を取得
			MyBase::Transform transform = { object_->GetScale(), object_->GetRotate(), object_->GetTranslate() };

			// 移動
			ImGui::DragFloat3("Translate", &transform.translate.x, kImGuiDragSpeed, kTranslateScope.min, kTranslateScope.max);
			// 回転
			ImGui::DragFloat3("Rotate", &transform.rotate.x, kImGuiDragSpeed, kRotateScope.min, kRotateScope.max);
			// 拡縮
			ImGui::DragFloat3("Scale", &transform.scale.x, kImGuiDragSpeed, kScaleScope.min, kScaleScope.max);
			
			// 変換情報をオブジェクトに設定
			object_->SetTransform(transform);
			
			ImGui::TreePop();
		}
		//ImGui::Text("\n");
		if (ImGui::TreeNode("Material")) {
			// 現在のマテリアル情報を取得
			MyBase::ModelMaterial* materialData = object_->GetMaterialData();

			// 色
			ImGui::ColorEdit4("color", &materialData->color.x);
			// 光沢度
			ImGui::SliderFloat("shininess", &materialData->shininess, kMaterialScope.min, kMaterialScope.max);
			// 反射強度
			ImGui::SliderFloat("reflectivity", &materialData->reflectivity, kMaterialScope.min, kMaterialScope.max);
			// 有効かどうか
			bool isEnabled = materialData->enableLighting;
			ImGui::Checkbox("enableLighting", &isEnabled);
			materialData->enableLighting = isEnabled;

			// 変更したマテリアル情報をオブジェクトに設定
			object_->SetMaterialData(materialData);

			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}
#endif // _DEBUG


// 衝突判定処理
void BaseObject::OnCollision(Collider* other) {
	if (collider_) {
		collider_->OnCollision(other);
	}
}

/// オブジェクトのワールド座標を取得
MyBase::Vector3 BaseObject::GetWorldPosition() const {
	return object_ ? object_->GetTranslate() : kZeroVector;
}

/// Collider インスタンスを設定
void BaseObject::SetCollider(std::unique_ptr<Collider> collider) {
	collider_ = std::move(collider);

	CollisionManager::GetInstance()->Register(collider_.get());
}
