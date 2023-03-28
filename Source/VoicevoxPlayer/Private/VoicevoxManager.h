
#pragma once
#include "../ThirdParty/voicevox_core/voicevox_core.h"
#include <future>
#include <memory>
#include <queue>
#include <string>

namespace voicevox
{
	// 前方宣言
	struct Request;

	/*
	VOICEVOX変換結果
	*/
	class Result final
	{
	public:
		Result(const uint8_t* wav, const size_t size, const VoicevoxResultCode resultCode);
		Result(const Result& other);
		Result(Result&& other) noexcept;
		Result& operator=(const Result& other);
		Result& operator=(Result&& other) noexcept;
		~Result() = default;

		std::shared_ptr<const uint8_t> GetWaveData() const noexcept;
		size_t GetWaveSize() const noexcept;
		VoicevoxResultCode GetVoicevoxResultCode() const noexcept;

	private:
		std::shared_ptr<uint8_t> mWav;
		size_t mSize;
		VoicevoxResultCode mVoicevoxResultCode;
	};

	/*
	VOICEVOX変換マネージャークラス
	スレッド状で要求を受けたテキストのボイス変換を随時処理します。
	処理結果はstd::future<Result>から取得することができます。
	*/
	class Manager final
	{
		Manager();
		Manager(const Manager&) = delete;
		Manager& operator=(const Manager &) = delete;

	public:
		/*
		デストラクタ
		*/
		~Manager();

		/*
		マネージャーの処理を開始します
		参照が無くなるとマネージャーの処理を停止します
		*/
		static std::shared_ptr<Manager> Instance();

		/*
		Voicevoxシステムを初期化します
		\param[in]	openJTalkDictionaryPath		OpenJTalk辞書のパス
		\param[in]	speakerId					初期化する話者番号（-1なら全て読み込む）
		\return		VoicevoxResultCode
		*/
		VoicevoxResultCode Initialize(const std::string& openJTalkDictionaryPath, const int32_t speakerId);

		/*
		Voicevoxシステムが初期化済みか調べます
		\return		trueならば初期化済み
		*/
		bool IsInitialized() const noexcept;

		/*
		テキストの変換を要求します
		処理結果はstd::future<Result>から取得することができます。
		*/
		std::future<Result> Request(const std::string& message, const VoicevoxTtsOptions& ttsOptions, std::function<void(const Result&)> callback = nullptr);

		/*
		テキストの変換を要求します
		処理結果はstd::future<Result>から取得することができます。
		*/
		std::future<Result> Request(const std::string& message, const int32_t speakerId, const VoicevoxTtsOptions& ttsOptions, std::function<void(const Result&)> callback = nullptr);

	private:
		void Thread();

	private:
		static std::weak_ptr<Manager> mInstance;

		std::thread mThread;
		std::condition_variable mConditionVariable;
		std::mutex mConditionVariableMutex;
		std::queue<std::shared_ptr<voicevox::Request>> mRequestQueue;
		std::mutex mRequestQueueMutex;
		std::atomic_bool mTerminate = {false};

		uint32_t mSpeakerId = 0;

		enum InitializePhase : uint8_t
		{
			SystemFinalized,
			SystemInitialized,
		};
		InitializePhase mInitializePhase = InitializePhase::SystemFinalized;
	};
}
