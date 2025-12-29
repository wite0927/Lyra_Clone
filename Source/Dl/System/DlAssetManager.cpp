// Fill out your copyright notice in the Description page of Project Settings.


#include "DlAssetManager.h"
#include "Dl/DlLogChannels.h"

UDlAssetManager::UDlAssetManager()
{
}

UDlAssetManager& UDlAssetManager::Get()
{
	check(GEngine);

	if (UDlAssetManager* Singleton = Cast<UDlAssetManager>(GEngine->AssetManager))
	{
		// 프로젝트 세팅에서 설정된 에셋매니저
		return *Singleton;
	}

	// 에셋매니저 클래스가 잘못됐다. 중단한다.
	UE_LOG(LogDl, Fatal, TEXT("Invalid AssetManager class '%s'! It must be a '%s'"), *GEngine->AssetManager->GetClass()->GetName(), *UDlAssetManager::StaticClass()->GetName());

	return *NewObject<UDlAssetManager>();
}

PRAGMA_DISABLE_OPTIMIZATION
void UDlAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
PRAGMA_ENABLE_OPTIMIZATION

bool UDlAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();

	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
	//... -LogAssetLoads 붙이면 에셋 로드 타이밍 로그를 남기고
	//안 붙이면 로그를 안 남깁니다.
}

UObject* UDlAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("SynchronousLoadAsset: %s"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		return AssetPath.TryLoad();
	}
	return nullptr;

	// 프로젝트가 “반드시 UDlAssetManager를 AssetManager로 사용한다”는 전제 아래, 
	// SoftObjectPath를 안전하게 동기 로드하고 필요 시 로드 시간을 로깅하려는 코드입니다.
}

void UDlAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
