#pragma once
#include "../ThirdParty/voicevox_core/voicevox_core.h"
#include <Components/SceneComponent.h>
#include <Delegates/IDelegateInstance.h>
#include <RuntimeAudioImporterLibrary.h>
#include <thread>
#include <mutex>
#include "VoicevoxPlayerComponent.generated.h"

/*
前方宣言
*/
namespace voicevox
{
	class Manager;
}

/*
Voicevoxの話者
TODO:話者のIDが正しくない可能性があります。正確な情報を探して定義してください
*/
UENUM(BlueprintType)
enum class VocevoxSpeakerId : uint8
{
	Zundamon_Normal = 3			UMETA(DisplayName = "ずんだもん：ノーマル"),
	Zundamon_Sweetheart = 1		UMETA(DisplayName = "ずんだもん：あまあま"),
	Zundamon_4 = 5				UMETA(DisplayName = "ずんだもん：セクシー"),
	Zundamon_6 = 7				UMETA(DisplayName = "ずんだもん：ツンツン"),
	ShikokuMetan_Normal = 2		UMETA(DisplayName = "四国めたん：ノーマル"),
	ShikokuMetan_Sweetheart = 0	UMETA(DisplayName = "四国めたん：あまあま"),
	ShikokuMetan_3 = 4			UMETA(DisplayName = "四国めたん：セクシー"),
	ShikokuMetan_5 = 6			UMETA(DisplayName = "四国めたん：ツンツン"),
	KasukabeTsumugi_ = 8		UMETA(DisplayName = "春日部つむぎ：ノーマル"),
	Ritsu_ = 9					UMETA(DisplayName = "波音リツ：ノーマル"),
	Ama_ = 10					UMETA(DisplayName = "雨晴はう：ノーマル"),
	Kuro_ = 11					UMETA(DisplayName = "玄野武宏：ノーマル"),
	Shiro_ = 12					UMETA(DisplayName = "白上虎太郎：ノーマル"),
	Ao_ = 13					UMETA(DisplayName = "青山龍星：ノーマル"),
	Mei_ = 14					UMETA(DisplayName = "冥鳴ひまり：ノーマル"),
	Kyusyuu_2 = 16				UMETA(DisplayName = "九州そら：ノーマル"),
	Kyusyuu_1 = 15				UMETA(DisplayName = "九州そら：あまあま"),
	Kyusyuu_3 = 17				UMETA(DisplayName = "九州そら：セクシー"),
	Kyusyuu_4 = 18				UMETA(DisplayName = "九州そら：ツンツン"),
	Kyusyuu_5 = 19				UMETA(DisplayName = "九州そら：ささやき"),
};

/*
Voicevoxによるテキスト読み上げクラス
*/
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class VOICEVOXPLAYER_API UVoicevoxPlayerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/*
	コンストラクタ
	*/
	explicit UVoicevoxPlayerComponent(const FObjectInitializer& objectInitializer);

	/*
	デストラクタ
	*/
	virtual ~UVoicevoxPlayerComponent();

	/*
	テキストを読み上げます
	テキストの音声化をバックグラウンド処理するので、
	関数実行から再生まで時間がかかることがあります。
	*/
	UFUNCTION(BlueprintCallable, Category = "Vocevox")
		void Play(const FString& message);

	/*
	Playのテキスト読み上げが再生中か取得します
	*/
	UFUNCTION(BlueprintPure, Category = "Vocevox")
		bool IsPlaying() const;

	/*
	テキスト読み上げ音声の変換
	テキストの音声化をバックグラウンド処理するので、
	関数実行から変換完了まで時間がかかることがあります。
	*/
	//TArray<uint8> Spawn(const FString& message);

protected:
	// UActorComponent overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnAudioImporterResultNative(URuntimeAudioImporterLibrary* importer, UImportedSoundWave* importedSoundWave, ERuntimeImportStatus status);

protected:
	/*
	話者番号
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VoicevoxPlayer")
		VocevoxSpeakerId SpeakerId = VocevoxSpeakerId::Zundamon_Normal;

	/*
	音量に乗算する値
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VoicevoxPlayer", meta = (ClampMin = "0"))
		float VolumeMultiplier = 4.f;

	/*
	ピッチに乗算する値
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VoicevoxPlayer", meta = (ClampMin = "0"))
		float PitchMultiplier = 1.f;

	/*
	音声再生
	*/
	UPROPERTY(BlueprintReadOnly, Category = "VoicevoxPlayer")
		UAudioComponent* AudioComponent;
	//TObjectPtr<UAudioComponent> AudioComponent;

	/*
	ランタイム音声データインポーター
	*/
	UPROPERTY()
		URuntimeAudioImporterLibrary* RuntimeAudioImporterLibrary;
	//TObjectPtr<URuntimeAudioImporterLibrary> RuntimeAudioImporterLibrary;

	/*
	ランタイム音声データインポートイベント
	*/
	FDelegateHandle RuntimeAudioImportResult;

private:
	std::shared_ptr<voicevox::Manager> mManager;
	std::atomic_bool mProcessing = { false };
};
