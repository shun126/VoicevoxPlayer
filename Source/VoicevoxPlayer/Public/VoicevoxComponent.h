#pragma once
#include <Components/ActorComponent.h>
#include "../ThirdParty/voicevox_core/voicevox_core.h"
#include "VoiceVoxComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQueryEventSignature, const TArray<FString>&, answers);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UVoiceVoxComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	explicit UVoiceVoxComponent(const FObjectInitializer& objectInitializer);
	virtual ~UVoiceVoxComponent();

	// UActorComponent overrides
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Message;

private:
    void Initialzie();
    void Finalize();

    void Play();
};
