#pragma once
#include <Components/ActorComponent.h>
#include <RuntimeAudioImporterLibrary.h>
#include "../ThirdParty/voicevox_core/voicevox_core.h"
#include "VoiceVoxComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoiceVoxQueryEventSignature, const TArray<FString>&, answers);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class VOICEVOXPLAYER_API UVoiceVoxComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UVoiceVoxComponent(const FObjectInitializer& objectInitializer);
	virtual ~UVoiceVoxComponent();

	// UActorComponent overrides
	virtual void BeginPlay() override;

	void OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status);

	void Play(const FString& message);
	bool IsPlaying() const;

private:
    void Initialzie();
    void Finalize();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Message;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<URuntimeAudioImporterLibrary> RuntimeAudioImporterLibrary2;

	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UAudioComponent> AudioComponent;
};
