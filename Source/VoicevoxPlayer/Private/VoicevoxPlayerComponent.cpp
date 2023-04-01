/*
https://voicevox.hiroshiba.jp/
https://github.com/VOICEVOX/voicevox_core/tree/main/example/cpp/windows
*/

#include "VoicevoxPlayerComponent.h"
#include "VoicevoxManager.h"
#include "VoicevoxStringConverter.h"
#include "BuildInfomation.h"
#include "PluginInfomation.h"
#include <AudioDevice.h>
#include <Kismet/GameplayStatics.h>

#include <fstream>

#if WITH_EDITOR
#include <Misc/Paths.h>
#endif

namespace
{
	static const FString OpenJTalkDictionaryName = TEXT("open_jtalk_dic_utf_8-1.11");
}

UVoicevoxPlayerComponent::UVoicevoxPlayerComponent(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	mManager = voicevox::Manager::Instance();

	RuntimeAudioImporterLibrary = objectInitializer.CreateDefaultSubobject<URuntimeAudioImporterLibrary>(this, TEXT("RuntimeAudioImporter"));
}

UVoicevoxPlayerComponent::~UVoicevoxPlayerComponent()
{
	mManager.reset();
}

void UVoicevoxPlayerComponent::BeginPlay()
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
	openJTalkDictionaryPath = FPaths::Combine(openJTalkDictionaryPath, OpenJTalkDictionaryName);

	// VoicevoxManagerを初期化（初期化済みなら何もしない）
	const VoicevoxResultCode result = mManager->Initialize(
		voicevox::windows::ToUTF8(*openJTalkDictionaryPath).Get(),
		static_cast<int32_t>(SpeakerId)
	);

	// 音声変換完了イベントの登録
	RuntimeAudioImportResult = RuntimeAudioImporterLibrary->OnResultNative.AddUObject(this, &UVoicevoxPlayerComponent::OnAudioImporterResultNative);
}

void UVoicevoxPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 音声変換完了イベントの解除
	if (IsValid(RuntimeAudioImporterLibrary))
	{
		RuntimeAudioImporterLibrary->OnResultNative.Remove(RuntimeAudioImportResult);
	}

	// 親クラスを呼び出し
	Super::EndPlay(EndPlayReason);
}

void UVoicevoxPlayerComponent::Play(const FString& message)
{
	std::string speechText(voicevox::windows::ToUTF8(*message).Get());
	VoicevoxTtsOptions ttsOptions = voicevox_make_default_tts_options();
	
	VOICEVOX_PLAYER_COMMUNICATOR_LOG(TEXT("Play %s"), *message);
	mProcessing = true;
	mManager->Request(speechText, static_cast<int32_t>(SpeakerId), ttsOptions, [this](const voicevox::Result& voicevoxResult)
		{
			TArray<uint8> audioData;
			audioData.Append(voicevoxResult.GetWaveData().get(), voicevoxResult.GetWaveSize());
			RuntimeAudioImporterLibrary->ImportAudioFromBuffer(audioData, ERuntimeAudioFormat::Wav);
			audioData.Reset();

			VOICEVOX_PLAYER_COMMUNICATOR_LOG(TEXT("Play"));
		}
	);
}

void UVoicevoxPlayerComponent::OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status)
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
				AudioComponent->SetWorldTransform(GetComponentTransform());
				AudioComponent->SetVolumeMultiplier(5);
				if (PitchMultiplier != 1.f)
					AudioComponent->SetPitchMultiplier(PitchMultiplier);
				AudioComponent->Play();
			}
		}

		mProcessing = false;

		VOICEVOX_PLAYER_COMMUNICATOR_LOG(TEXT("OnAudioImporterResultNative"));
	}
}

bool UVoicevoxPlayerComponent::IsPlaying() const
{
	if (IsValid(AudioComponent) && AudioComponent->IsPlaying())
		return true;
	return mProcessing == true;
}
