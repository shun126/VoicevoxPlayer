#pragma once
#include "../ThirdParty/voicevox_core/voicevox_core.h"
#include <Components/ActorComponent.h>
#include <RuntimeAudioImporterLibrary.h>
#include <thread>
#include <mutex>
#include "VoiceVoxComponent.generated.h"

namespace voicevox
{
	class Manager;
}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoiceVoxQueryEventSignature, const TArray<FString>&, answers);

UENUM(BlueprintType)
enum class VoceVoxSpeakerId : uint8
{
	Zundamon,
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class VOICEVOXPLAYER_API UVoiceVoxComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UVoiceVoxComponent(const FObjectInitializer& objectInitializer);
	virtual ~UVoiceVoxComponent();

	/*
	テキストを読み上げます
	テキストの音声化をバックグラウンド処理するので、
	関数実行から再生まで時間がかかることがあります。
	*/
	void Play(const FString& message);

	bool IsPlaying() const;

	//TArray<uint8> Spawn(const FString& message);

protected:
	// UActorComponent overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		VoceVoxSpeakerId SpeakerId = VoceVoxSpeakerId::Zundamon;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<URuntimeAudioImporterLibrary> RuntimeAudioImporterLibrary2;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UAudioComponent> AudioComponent;

private:
	std::shared_ptr<voicevox::Manager> mManager;
	std::atomic_bool mProcessing = { false };
};
