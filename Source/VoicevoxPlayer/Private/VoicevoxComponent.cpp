/*
https://voicevox.hiroshiba.jp/
https://github.com/VOICEVOX/voicevox_core/tree/main/example/cpp/windows
*/

#include "VoiceVoxComponent.h"
#include "VoicevoxManager.h"
#include "VoicevoxStringConverter.h"
#include <AudioDevice.h>
#include <Kismet/GameplayStatics.h>

#define OPENJTALK_DICT_NAME TEXT("open_jtalk_dic_utf_8-1.11")

#include <fstream>

#if WITH_EDITOR
#include <Misc/Paths.h>
#endif

UVoiceVoxComponent::UVoiceVoxComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	mManager = voicevox::Manager::Instance();

	RuntimeAudioImporterLibrary2 = objectInitializer.CreateDefaultSubobject<URuntimeAudioImporterLibrary>(this, TEXT("RuntimeAudioImporter"));
}

UVoiceVoxComponent::~UVoiceVoxComponent()
{
	mManager.reset();
}

void UVoiceVoxComponent::BeginPlay()
{
	// 親クラスを呼び出し
	Super::BeginPlay();

	// 辞書のパスを作成
	FString openJTalkDictionaryPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("VoicevoxPlayer"), TEXT("Binaries")));
#ifdef _WIN64
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, TEXT("Win64"));
#elif _WIN32
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, TEXT("Win32"));
#endif
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, OPENJTALK_DICT_NAME);

	// VoicevoxManagerを初期化（初期化済みなら何もしない）
	const VoicevoxResultCode result = mManager->Initialize(
		voicevox::windows::ToUTF8(*openJTalkDictionaryPath).Get(),
		static_cast<int32_t>(SpeakerId)
	);

	FDelegateHandle h3 = RuntimeAudioImporterLibrary2->OnResultNative.AddUObject(this, &UVoiceVoxComponent::OnAudioImporterResultNative);
}

void UVoiceVoxComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// RuntimeAudioImporterLibrary2.Remove(h4);

	// 親クラスを呼び出し
	Super::EndPlay(EndPlayReason);
}

void UVoiceVoxComponent::Play(const FString& message)
{
	std::string speechText(voicevox::windows::ToUTF8(*message).Get());
	VoicevoxTtsOptions ttsOptions = voicevox_make_default_tts_options();
	
	UE_LOG(LogTemp, Log, TEXT("UVoiceVoxComponent::Play %s"), *message);
	mProcessing = true;
	mManager->Request(speechText, ttsOptions, [this](const voicevox::Result& voicevoxResult)
		{
			TArray<uint8> audioData;
			audioData.Append(voicevoxResult.GetWaveData().get(), voicevoxResult.GetWaveSize());
			RuntimeAudioImporterLibrary2->ImportAudioFromBuffer(audioData, ERuntimeAudioFormat::Wav);
			audioData.Reset();

			UE_LOG(LogTemp, Log, TEXT("UVoiceVoxComponent::Play"));
		}
	);
}

void UVoiceVoxComponent::OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status)
{
	if (importedSoundWave != nullptr && ERuntimeImportStatus::SuccessfulImport == status)
	{
		if (AudioComponent)
		{
			AudioComponent->Stop();
			AudioComponent->MarkPendingKill();
			AudioComponent = nullptr;
		}

		if (FAudioDeviceHandle audioDevice = GetWorld()->GetAudioDevice())
		{
			FAudioDevice::FCreateComponentParams createParameter(GetOwner());
			createParameter.bAutoDestroy = true;
			createParameter.bPlay = false;

			AudioComponent = FAudioDevice::CreateComponent(importedSoundWave, createParameter);
			if (AudioComponent)
			{
				AudioComponent->SetVolumeMultiplier(5);
				//AudioComponent->SetPitchMultiplier(2);
				AudioComponent->Play();
			}
		}

		mProcessing = false;

		UE_LOG(LogTemp, Log, TEXT("UVoiceVoxComponent::OnAudioImporterResultNative"));
	}
}

bool UVoiceVoxComponent::IsPlaying() const
{
	return mProcessing == false && IsValid(AudioComponent) && AudioComponent->IsPlaying();
}
