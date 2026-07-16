#include "Input.h"
#include <cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

/// static member 定義
unique_ptr<Input> Input::sInstance_ = nullptr;

// Singleton Instance を取得
Input* Input::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<Input>(Input::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 初期化
void Input::Initialize(WindowsAPI* winApi) {
	// 狩りてきたWinApiのインスタンスを記録
	winApi_ = winApi;

	// キーボード初期化
	KeyboardInitialize();

	// マウス初期化
	MouseInitialize();
}

// 終了
void Input::Finalize() {
	keyboard_.Reset();
	mouseDevice_.Reset();
	directInput_.Reset();

	sInstance_.reset();
}

/// 更新
void Input::Update() {
	// キーボードの更新
	KeyboardUpdate();

	// マウスの更新
	MouseUpdate();
}

#pragma region キーボード
/// キーボードの初期化
void Input::KeyboardInitialize() {
	HRESULT hr;

	// DirectInputの初期化
	hr = DirectInput8Create(winApi_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);	// 標準形式
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(winApi_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

/// キーボードの更新
void Input::KeyboardUpdate() {
	HRESULT hr;

	// 前回のキー入力を保存
	keyPre_ = key_;

	// キーボード情報の取得開始
	hr = keyboard_->Acquire();
	// 全キーの入力状態を取得する
	hr = keyboard_->GetDeviceState(sizeof(key_), key_.data());

	if (FAILED(hr)) {
		keyboard_->Acquire();
	}
}
#pragma endregion

#pragma region マウス
// マウスの初期化
void Input::MouseInitialize() {
	HRESULT hr;

	// マウスデバイスの生成
	hr = directInput_->CreateDevice(GUID_SysMouse, &mouseDevice_, nullptr);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット
	hr = mouseDevice_->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのセット
	hr = mouseDevice_->SetCooperativeLevel(winApi_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));
}

/// マウスの更新
void Input::MouseUpdate() {
	// マウス情報の更新
	MouseStateUpdate();

	// マウス座標更新
	MousePosUpdate();

	// マウス移動量
	MouseMoveUpdate();

	// ホイール回転量
	MouseWheelUpdate();
}

/// マウス情報の更新
void Input::MouseStateUpdate() {
	HRESULT hr;

	// 前回のマウス状態を保存
	mouseStatePre_ = mouseState_;

	// マウス情報の取得開始
	hr = mouseDevice_->Acquire();
	// マウス状態取得
	mouseDevice_->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState_);

	if (FAILED(hr)) {
		mouseDevice_->Acquire();
	}

	// マウスボタン状態を配列に反映
	for (size_t i = 0; i < kMouseCount; ++i) {
		mouseButtonsPre_[i] = mouseButtons_[i];
		mouseButtons_[i] = (mouseState_.rgbButtons[i] & 0x80) ? 1 : 0;
	}
}

/// マウス座標更新
void Input::MousePosUpdate() {
	mousePosPre_ = mousePos_;

	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(winApi_->GetHwnd(), &pos);

	mousePos_ = { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}

/// マウス移動量更新
void Input::MouseMoveUpdate() {
	mouseMove_.x = static_cast<float>(mouseState_.lX);
	mouseMove_.y = static_cast<float>(mouseState_.lY);
}

/// マウスホイールの更新
void Input::MouseWheelUpdate() {
	wheelDelta_ = static_cast<int>(mouseState_.lZ);
}
#pragma endregion

#pragma region getter
#pragma region キーボード関係
/// キーの押下をチェック
bool Input::PushKey(BYTE keyNumber) {
	assert(keyNumber < key_.size());

	// 指定キーを押していればtrueを返す
	if (key_[keyNumber]) return true;

	// そうでなければfalseを返す
	return false;
}

/// キーのトリガーをチェック
bool Input::TriggerKey(BYTE keyNumber) {
	assert(keyNumber < key_.size());

	// 指定キーを押した時にtrueを返す
	if (!keyPre_[keyNumber] && key_[keyNumber]) return true;

	// そうでなければfalseを返す
	return false;
}

/// キーのリリースをチェック
bool Input::ReleaseKey(BYTE keyNumber) {
	assert(keyNumber < key_.size());

	// 指定キーをリリースした時にtrueを返す
	if (keyPre_[keyNumber] && !key_[keyNumber]) return true;
	
	// そうでなければfalseを返す
	return false;
}
#pragma endregion

#pragma region マウス関係
/// マウスボタンの押下をチェック
bool Input::PushMouse(MouseButton button) {
	const uint8_t index = static_cast<uint8_t>(button);

	assert(index < mouseButtons_.size());

	// 指定マウスボタンを押していればtrueを返す
	if (mouseButtons_[index]) return true;
	
	// そうでなければfalseを返す
	return false;
}

/// マウスボタンのトリガーをチェック
bool Input::TriggerMouse(MouseButton button) {
	const uint8_t index = static_cast<uint8_t>(button);

	assert(index < mouseButtons_.size());

	// 指定マウスボタンを押した時にtrueを返す
	if (!mouseButtonsPre_[index] && mouseButtons_[index]) return true;
	
	// そうでなければfalseを返す
	return false;
}

/// マウスボタンのリリースをチェック
bool Input::ReleaseMouse(MouseButton button) {
	const uint8_t index = static_cast<uint8_t>(button);

	assert(index < mouseButtons_.size());

	// 指定マウスボタンをリリースした時にtrueを返す
	if (mouseButtonsPre_[index] && !mouseButtons_[index]) return true;
	
	// そうでなければfalseを返す
	return false;
}

/// マウスカーソルの座標取得
MyBase::Vector2 Input::GetMousePosition() {
	return mousePos_;
}

/// マウスカーソルの移動距離取得
MyBase::Vector2 Input::GetMouseMove() {
	return mouseMove_;
}

/// マウスホイールの回転量取得
int Input::GetWheelDelta() {
	return wheelDelta_;
}
#pragma endregion 
#pragma endregion