/*
https://voicevox.hiroshiba.jp/
https://github.com/VOICEVOX/voicevox_core/tree/main/example/cpp/windows
*/

#include "VoiceVoxComponent.h"
#include <AudioDevice.h>
#include <Kismet/GameplayStatics.h>

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
}



UVoiceVoxComponent::UVoiceVoxComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	RuntimeAudioImporterLibrary2 = objectInitializer.CreateDefaultSubobject<URuntimeAudioImporterLibrary>(this, TEXT("RuntimeAudioImporter"));
}

UVoiceVoxComponent::~UVoiceVoxComponent()
{
	Finalize();
}

void UVoiceVoxComponent::BeginPlay()
{
	// 親クラスを呼び出し
	Super::BeginPlay();


	FDelegateHandle h3 = RuntimeAudioImporterLibrary2->OnResultNative.AddUObject(this, &UVoiceVoxComponent::OnAudioImporterResultNative);
	// SampleDelegate.Remove(h4);


	Initialzie();
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
	//initializeOptions.load_all_models = true;
	initializeOptions.load_all_models = false;

	VoicevoxResultCode result = VoicevoxResultCode::VOICEVOX_RESULT_OK;
	result = voicevox_initialize(initializeOptions);
	if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		//OutErrorMessage(result);
		//return 0;
	}

	result = voicevox_load_model(0);
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

void UVoiceVoxComponent::Play(const FString& message)
{
	int32_t speaker_id = 0;
	uintptr_t output_binary_size = 0;
	uint8_t* output_wav = nullptr;
	VoicevoxTtsOptions ttsOptions = voicevox_make_default_tts_options();

	//Message;
	//auto result = voicevox_tts(ToMultiByte(TEXT("ボイスボックスのアンリアルエンジンプラグインなのだ")), speaker_id, ttsOptions, &output_binary_size, &output_wav);
	auto result = voicevox_tts(ToMultiByte(*message), speaker_id, ttsOptions, &output_binary_size, &output_wav);
	if (result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		//OutErrorMessage(result);
		return;
	}

	TArray<uint8> audioData;
	audioData.Append(output_wav, output_binary_size);
	RuntimeAudioImporterLibrary2->ImportAudioFromBuffer(audioData, ERuntimeAudioFormat::Wav);

	voicevox_wav_free(output_wav);

	UE_LOG(LogTemp, Log, TEXT("UVoiceVoxComponent::Play"));
}

void UVoiceVoxComponent::OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status)
{
	if (importedSoundWave != nullptr && ERuntimeImportStatus::SuccessfulImport == status)
	{
		//UGameplayStatics::PlaySound2D(GetWorld(), importedSoundWave);
#if 0
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), importedSoundWave, GetOwner()->GetActorLocation());
#else
		if (FAudioDeviceHandle audioDevice = GetWorld()->GetAudioDevice())
		{
			FAudioDevice::FCreateComponentParams createParameter(GetOwner());
			createParameter.bAutoDestroy = false;
			createParameter.bPlay = false;

			AudioComponent = FAudioDevice::CreateComponent(importedSoundWave, createParameter);
			if (AudioComponent)
			{
				AudioComponent->SetVolumeMultiplier(5);
				//AudioComponent->SetPitchMultiplier(2);
				AudioComponent->Play();
			}
		}
#endif
		UE_LOG(LogTemp, Log, TEXT("UVoiceVoxComponent::OnAudioImporterResultNative"));
	}
}

bool UVoiceVoxComponent::IsPlaying() const
{
	return IsValid(AudioComponent) ? AudioComponent->IsPlaying() : false;
}
