#include "TimeManager.h"

using namespace std;

/// static member 定義
unique_ptr<TimeManager> TimeManager::sInstance_ = nullptr;

/// Singleton Instance を取得
TimeManager* TimeManager::GetInstance() {
	if (sInstance_ == nullptr) {
		sInstance_ = make_unique<TimeManager>(TimeManager::ConstructorKey{});
	}
	return sInstance_.get();
}

/// 初期化
void TimeManager::Initialize() {

}

/// 終了
void TimeManager::Finalize() {
	sInstance_.reset();
}