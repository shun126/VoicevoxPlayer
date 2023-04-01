#include "VoicevoxManager.h"

DEFINE_LOG_CATEGORY(VoicevoxPlayer);

namespace voicevox
{
	std::weak_ptr<Manager> Manager::mInstance;

	struct Request final
	{
		std::function<void(const Result&)> mCallback;
		std::promise<Result> mPromise;
		VoicevoxTtsOptions mTtsOptions;
		std::string mMessage;
		int32_t mSpeakerId;
	};



	Result::Result(const uint8_t* wav, const size_t size, const VoicevoxResultCode resultCode)
		: mWav(const_cast<uint8_t*>(wav), voicevox_wav_free)
		, mSize(size)
		, mVoicevoxResultCode(resultCode)
	{
	}

	Result::Result(const Result& other)
		: mWav(other.mWav)
		, mSize(other.mSize)
		, mVoicevoxResultCode(other.mVoicevoxResultCode)
	{
	}
	
	Result::Result(Result&& other) noexcept
		: mWav(std::move(other.mWav))
		, mSize(std::move(other.mSize))
		, mVoicevoxResultCode(std::move(other.mVoicevoxResultCode))
	{
		other.mWav = nullptr;
	}

	Result& Result::operator=(const Result& other)
	{
		mWav = other.mWav;
		mSize = other.mSize;
		mVoicevoxResultCode = other.mVoicevoxResultCode;
		return *this;
	}

	Result& Result::operator=(Result&& other) noexcept
	{
		mWav = std::move(other.mWav);
		other.mWav = nullptr;
		mSize = std::move(other.mSize);
		mVoicevoxResultCode = std::move(other.mVoicevoxResultCode);
		return *this;
	}

	std::shared_ptr<const uint8_t> Result::GetWaveData() const noexcept
	{
		return mWav;
	}

	size_t Result::GetWaveSize() const noexcept
	{
		return mSize;
	}

	VoicevoxResultCode Result::GetVoicevoxResultCode() const noexcept
	{
		return mVoicevoxResultCode;
	}



	std::shared_ptr<Manager> Manager::Instance()
	{
		std::shared_ptr<Manager> instance = mInstance.lock();
		if (instance == nullptr)
		{
			instance = std::shared_ptr<Manager>(new Manager());
			mInstance = instance;
		}
		return instance;
	}

	Manager::Manager()
	{
		mThread = std::thread([this]() { Thread(); });
	}

	Manager::~Manager()
	{
		if (mThread.joinable())
		{
			mTerminate = true;
			mConditionVariable.notify_one();
			mThread.join();
		}
	}

	VoicevoxResultCode Manager::Initialize(const std::string& openJTalkDictionaryPath, const int32_t speakerId)
	{
		VoicevoxResultCode result = VoicevoxResultCode::VOICEVOX_RESULT_OK;

		if (mInitializePhase < InitializePhase::SystemInitialized)
		{
			VoicevoxInitializeOptions initializeOptions = voicevox_make_default_initialize_options();
			initializeOptions.open_jtalk_dict_dir = openJTalkDictionaryPath.c_str();
			initializeOptions.load_all_models = speakerId < 0;

			result = voicevox_initialize(initializeOptions);
			if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
			{
				voicevox_finalize();
				return result;
			}

			mInitializePhase = InitializePhase::SystemInitialized;
		}

		if (mInitializePhase >= InitializePhase::SystemInitialized)
		{
			result = voicevox_load_model(static_cast<uint32_t>(speakerId));
		}

		return result;
	}

	bool Manager::IsInitialized() const noexcept
	{
		return mInitializePhase >= InitializePhase::SystemInitialized;
	}

	std::future<Result> Manager::Request(const std::string& message, const int32_t speakerId, const VoicevoxTtsOptions& ttsOptions, std::function<void(const Result&)> callback)
	{
		if (!IsInitialized())
			return std::future<Result>();

		std::shared_ptr<voicevox::Request> request = std::make_shared<voicevox::Request>();

		auto future = request->mPromise.get_future();
		request->mTtsOptions = ttsOptions;
		request->mMessage = message;
		request->mSpeakerId = speakerId;
		request->mCallback = callback;

		std::lock_guard<std::mutex> lock(mRequestQueueMutex);
		mRequestQueue.push(request);
		mConditionVariable.notify_one();

		return future;
	}

	void Manager::Thread()
	{
		std::unique_lock<std::mutex> conditionVariableLock(mConditionVariableMutex);

		while (true)
		{
			mConditionVariable.wait(conditionVariableLock);
			if (mTerminate)
				break;

			std::shared_ptr<voicevox::Request> request;
			{
				std::lock_guard<std::mutex> lock(mRequestQueueMutex);
				request = mRequestQueue.front();
				mRequestQueue.pop();
			}

			uint8_t* outputWav = nullptr;
			uintptr_t outputBinarySize = 0;
			const VoicevoxResultCode resultCode = voicevox_tts(
				request->mMessage.c_str(),
				request->mSpeakerId,
				request->mTtsOptions,
				&outputBinarySize,
				&outputWav
			);

			Result result(outputWav, outputBinarySize, resultCode);
			if (request->mCallback)
			{
				if (result.GetVoicevoxResultCode() == VoicevoxResultCode::VOICEVOX_RESULT_OK)
					request->mCallback(result);
			}
			request->mPromise.set_value(std::move(result));
		}
	}
}
