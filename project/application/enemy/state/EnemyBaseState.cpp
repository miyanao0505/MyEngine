#include "EnemyBaseState.h"
#ifdef _DEBUG
#include <Logger.h>

using namespace Logger;
#endif // _DEBUG

#ifdef _DEBUG
// デバックログ出力
void EnemyBaseState::DebugLog()
{
	Log(name_);
}
#endif // _DEBUG
