#include "TimeManager.h"

TimeManager* TimeManager::sInstance = nullptr;

// シングルトンインスタンスの取得
TimeManager* TimeManager::GetInstance()
{
	if (sInstance == nullptr) {
		sInstance = new TimeManager;
	}
	return sInstance;
}

void TimeManager::Initialize() {

}

void TimeManager::Finalize() {

}