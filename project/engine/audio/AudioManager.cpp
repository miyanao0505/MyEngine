#include "AudioManager.h"
#include <cassert>
#include "AssetPath.h"

using namespace std;

/// static member 定義
unique_ptr<AudioManager> AudioManager::sInstance_ = nullptr;

/// Singleton Instance を取得
AudioManager* AudioManager::GetInstance() {
	if (!sInstance_) {
		sInstance_ = make_unique<AudioManager>(AudioManager::ConstructorKey{});
	}
	return sInstance_.get();
}

// 初期化
void AudioManager::Initialize() {
	HRESULT hr;

	// xAudioエンジンのインスタンスを生成
	hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	// マスターボイスを生成
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));
}

// 更新
void AudioManager::Update() {
	// 再生中の音声の状態をチェック
	for (auto it = activeVoices_.begin(); it != activeVoices_.end();) {
		// ループ再生中の音声は状態をチェックせずにスキップ
		if (it->isLoop) {
			++it;
			continue;
		}
		
		// ループ再生でない音声は状態をチェック
		XAUDIO2_VOICE_STATE state{};
		it->voice->GetState(&state);

		// 再生終了
		if(state.BuffersQueued == 0) {
			it->voice->DestroyVoice();
			it = activeVoices_.erase(it);
		}
		else {
			++it;
		}
	}
}

// 終了
void AudioManager::Finalize() {
	for (auto& voice : activeVoices_) {
		if (voice.voice) {
			voice.voice->DestroyVoice();
		}
	}

	activeVoices_.clear();
}

// 音声データ(Wave)の読み込み
void AudioManager::LoadAudioWave(const std::string& filename) {
	// 読み込み済みなら早期リターン
	if (soundDataMap_.contains(filename)) return;
	
	std::string filePath = AssetPath::kAudioRootPath + filename;

	/// 1. ファイルオープン
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(filePath, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	/// 2. .wavデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	ReadRiffHeader(file, riff);
	
	// Formatチャンクの読み込み
	FormatChunk format = {};
	ReadFormatChunk(file, format);

	// Dataチャンクの読み込み
	ChunkHeader data;
	ReadDataChunk(file, data);

	/// 3. 読み込んだ音声データの参照を取得する
	SoundData& soundData = soundDataMap_[filename];
	soundData.wfex = format.fmt;
	soundData.buffer.resize(data.size);

	// Dataチャンクのデータ部(波形データ)の読み込み
	file.read(reinterpret_cast<char*>(soundData.buffer.data()), data.size);
}

// 音声再生(Wave)
void AudioManager::PlayWave(const std::string& filename, float volume, bool loop) {
	HRESULT hr;
	SoundData& soundData = soundDataMap_.at(filename);

	// ループ再生する場合、すでに同じファイルがループ再生中かチェック
	if (loop) {
		auto it = std::ranges::find_if(
			activeVoices_,
			[&filename](const ActiveVoice& voice) {
				return voice.filename == filename &&
					voice.isLoop;
			});

		if(it != activeVoices_.end()) {
			// すでにループ再生中なら早期リターン
			return;
		}
	}

	// 波形フォーマットを元にSoundVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.buffer.data();
	buf.AudioBytes = static_cast<UINT32>(soundData.buffer.size());
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// ループの設定
	buf.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	// 波形データ送信と再生開始
	hr = pSourceVoice->SetVolume(volume);
	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));
	hr = pSourceVoice->Start();
	assert(SUCCEEDED(hr));

	// 再生中リストに追加
	activeVoices_.push_back({ filename, pSourceVoice, loop });
}

// 音声停止(Wave)
void AudioManager::StopWave(const std::string& filename) {
	// 再生中の音声の中から対象の音声ファイル名を探す
	for (auto it = activeVoices_.begin(); it != activeVoices_.end(); ++it) {
		// 対象の音声ファイル名でなければスキップ
		if (it->filename != filename) continue;

		// 対象の音声ファイル名が見つかったら停止してSourceVoiceを破棄
		it->voice->Stop();
		it->voice->DestroyVoice();

		// 再生中リストから削除
		activeVoices_.erase(it);
		return;
	}
}

// 音声データの解放
void AudioManager::UnloadAudio(const std::string& filename) {
	// 見つからなかったら早期リターン
	if (!soundDataMap_.contains(filename)) return;

	// 再生中なら停止
	StopWave(filename);

	// 音声データの解放
	soundDataMap_.erase(filename);
}

/// RIFFヘッダの読み込み
void AudioManager::ReadRiffHeader(std::ifstream& file, RiffHeader& riffHeader) {
	file.read(reinterpret_cast<char*>(&riffHeader), sizeof(riffHeader));
	// ファイルがRIFFかチェック
	if (strncmp(riffHeader.chunk.id, kRiffId, kChunkIdSize) != 0) assert(0);
	// タイプがWAVEかチェック
	if (strncmp(riffHeader.type, kWaveId, kChunkIdSize) != 0) assert(0);
}

/// FMTチャンクの読み込み
void AudioManager::ReadFormatChunk(std::ifstream& file, FormatChunk& formatChunk) {
	// チャンクヘッダーの確認
	file.read(reinterpret_cast<char*>(&formatChunk.chunk), sizeof(ChunkHeader));
	if (strncmp(formatChunk.chunk.id, kFmtId, kChunkIdSize) != 0) assert(0);
	// チャンク本体の読み込み
	assert(formatChunk.chunk.size <= sizeof(formatChunk.fmt));
	file.read(reinterpret_cast<char*>(& formatChunk.fmt), formatChunk.chunk.size);
}

/// Dataチャンクの読み込み
void AudioManager::ReadDataChunk(std::ifstream& file, ChunkHeader& dataChunk) {
	file.read(reinterpret_cast<char*>(&dataChunk), sizeof(dataChunk));
	// JUNKチャンクを検出した場合
	if (strncmp(dataChunk.id, kJunkId, kChunkIdSize) == 0) {
		// 読み込み一をJUNKチャンクの終わりまで進める
		file.seekg(dataChunk.size, std::ios_base::cur);
		// 再読み込み
		file.read(reinterpret_cast<char*>(&dataChunk), sizeof(dataChunk));
	}
	if (strncmp(dataChunk.id, kDataId, kChunkIdSize) != 0) assert(0);
}