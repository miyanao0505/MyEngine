#pragma once

/// <summary>
/// パス管理用クラス
/// </summary>
class AssetPath {
public:
	// model ルートパス
	static constexpr const char* kModelRootPath = "resources/model/";
	// model 内の debug ルートパス
	static constexpr const char* kDebugModelRoot = "debug/";

	// texture ルートパス
	static constexpr const char* kTextureRootPath = "resources/texture/";

	// audio ルートパス
	static constexpr const char* kAudioRootPath = "resources/audio/";

	// json ルートパス
	static constexpr const char* kJsonRootPath = "resources/jsons/";
};