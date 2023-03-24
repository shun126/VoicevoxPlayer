/*
https://voicevox.hiroshiba.jp/
https://github.com/VOICEVOX/voicevox_core/tree/main/example/cpp/windows
*/

#include "VoiceVoxComponent.h"

#define OPENJTALK_DICT_NAME TEXT("open_jtalk_dic_utf_8-1.11")

#include <fstream>
#include <windows.h>
#include <malloc.h>

#if WITH_EDITOR
#include <Misc/Paths.h>
#endif

namespace
{
	class ToMultiByte final
	{
	private:
		ToMultiByte() = delete;
		ToMultiByte(const ToMultiByte&) = delete;
		ToMultiByte& operator = (const ToMultiByte&) = delete;

	public:
		ToMultiByte(LPCWSTR text, const UINT codePage = CP_UTF8)
		{
			if (text)
			{
				const int size = WideCharToMultiByte(codePage, 0, text, -1, NULL, 0, NULL, NULL);
				mText = static_cast<LPSTR>(std::malloc(size + 1));
				if (mText)
					WideCharToMultiByte(codePage, 0, text, -1, mText, size, NULL, NULL);
			}
		}

		~ToMultiByte()
		{
			std::free(mText);
		}

		LPCSTR Get() const
		{
			return mText ? mText : "";
		}

		operator LPSTR() const
		{
			return mText ? mText : "";
		}

		operator LPCSTR() const
		{
			return mText ? mText : "";
		}

	private:
		LPSTR mText = nullptr;
	};

	class ToWideChar final
	{
	private:
		ToWideChar() = delete;
		ToWideChar(const ToWideChar&) = delete;
		ToWideChar& operator = (const ToWideChar&) = delete;

	public:
		ToWideChar(LPCSTR text, const UINT codePage = CP_UTF8)
		{
			if (text)
			{
				const int size = MultiByteToWideChar(codePage, 0, text, -1, NULL, 0);
				mText = static_cast<LPWSTR>(std::malloc((size + 1) * sizeof(WCHAR)));
				if (mText)
					MultiByteToWideChar(codePage, 0, text, -1, mText, size);
			}
		}

		~ToWideChar()
		{
			std::free(mText);
		}

		LPCWSTR Get() const
		{
			return mText ? mText : L"";
		}

		operator LPWSTR() const
		{
			return mText ? mText : L"";
		}

		operator LPCWSTR() const
		{
			return mText ? mText : L"";
		}

	private:
		LPWSTR mText = nullptr;
	};
}



UVoiceVoxComponent::UVoiceVoxComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

UVoiceVoxComponent::~UVoiceVoxComponent()
{
	Finalize();
}

void UVoiceVoxComponent::BeginPlay()
{
	// 親クラスを呼び出し
	Super::BeginPlay();

	Initialzie();
	Play();
}


void UVoiceVoxComponent::Initialzie()
{
	//dllファイルのパスを取得
	FString openJTalkDictionaryPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("VoicevoxPlayer"), TEXT("Binaries")));
#ifdef _WIN64
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, TEXT("Win64"));
#elif _WIN32
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, TEXT("Win32"));
#endif
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, OPENJTALK_DICT_NAME);


	VoicevoxInitializeOptions initializeOptions = voicevox_make_default_initialize_options();
	initializeOptions.open_jtalk_dict_dir = ToMultiByte(*openJTalkDictionaryPath);
	initializeOptions.load_all_models = true;

	VoicevoxResultCode result = VoicevoxResultCode::VOICEVOX_RESULT_OK;
	result = voicevox_initialize(initializeOptions);
	if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		//OutErrorMessage(result);
		//return 0;
	}
}

void UVoiceVoxComponent::Finalize()
{
	voicevox_finalize();
}

void UVoiceVoxComponent::Play()
{
	int32_t speaker_id = 0;
	uintptr_t output_binary_size = 0;
	uint8_t* output_wav = nullptr;
	VoicevoxTtsOptions ttsOptions = voicevox_make_default_tts_options();

	//Message;
	auto result = voicevox_tts(ToMultiByte(TEXT("ボイスボックスのアンリアルエンジンプラグインなのだ")), speaker_id, ttsOptions, &output_binary_size, &output_wav);
	if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		//OutErrorMessage(result);
		return;
	}

	{
		//音声ファイルの保存
		const FString path = FPaths::ProjectSavedDir() + TEXT("/test.wav");
		std::ofstream out_stream(TCHAR_TO_UTF8(*path), std::ios::out | std::ios::binary);
		out_stream.write(reinterpret_cast<const char*>(output_wav), output_binary_size);
	}  //ここでファイルが閉じられる


	//PlaySound((LPCTSTR)output_wav, nullptr, SND_MEMORY);

	voicevox_wav_free(output_wav);
}
