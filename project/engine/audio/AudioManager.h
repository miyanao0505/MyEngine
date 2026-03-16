#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <unordered_map>

#pragma comment(lib, "xaudio2.lib")

/// <summary>
/// XAudio2を使用した音声管理クラス
/// ゲーム全体で使用する音声(SE・BGMなど)を一元管理します。
/// </summary>
class AudioManager
{
private:	// オーディオ関係の構造体
	// チャンクヘッダ
	struct ChunkHeader {
		char id[4];		// チャンク毎のID
		int32_t size;	// チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk;	// "RIFF"
		char type[4];		// "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk;	// "fmt"
		WAVEFORMATEX fmt;	// 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		WAVEFORMATEX wfex;			// 波形フォーマット
		BYTE* buffer;				// バッファの先頭アドレス
		unsigned int bufferSize;	// バッファのサイズ
	};

public:	// メンバ関数
	/// <summary>
	/// Singleton Instance を取得
	/// </summary>
	/// <returns>AudioManager</returns>
	static AudioManager* GetInstance();

	/// ------ Passkey Idion ------
	/// コントラクタを渡すための鍵
	class ConstructorKey {
	private:
		ConstructorKey() = default;
		friend class AudioManager;
	};

	/// PassKeyを受け取るコンストラクタ
	explicit AudioManager(ConstructorKey) {}

	/// コピー禁止
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// 音声データの読み込み
	/// <summary>
	/// 音声データ(Wave)の読み込み
	/// </summary>
	/// <param name="filename">ファイル名 パスは "resources/audio/" に続く</param>
	void LoadAudioWave(const std::string& filename);


	/// 音声再生
	/// <summary>
	/// 音声再生(Wave)
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <param name="volume">音量</param>
	/// <param name="loop">ループの有無</param>
	void PlayWave(const std::string& filename, const float& volume = kDefaultVolume, const bool& loop = false);


	/// 音声停止
	/// <summary>
	/// 音声停止(Wave)
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void StopWave(const std::string& filename);


	/// <summary>
	/// 読み込んだ音声データを解放
	/// </summary>
	/// <param name="filename">削除する音声ファイル名</param>
	void UnloadAudio(const std::string& filename);

public:	// getter


public:	// setter


public:	// Singleton Instance
	static std::unique_ptr<AudioManager> sInstance_;

private:	// メンバ変数
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	// 音声データ
	std::unordered_map<std::string, SoundData> soundDataMap_;
	std::unordered_map<std::string, IXAudio2SourceVoice*> playingVoices_;

#pragma region 定数
	static constexpr char kRiffId[] = "RIFF";
	static constexpr char kWaveId[] = "WAVE";
	static constexpr char kFmtId[] = "fmt ";
	static constexpr char kDataId[] = "data";
	static constexpr char kJunkId[] = "JUNK";

	static constexpr size_t kChunkIdSize = 4;
	static constexpr float kDefaultVolume = 1.0f;
#pragma endregion
};