// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dl.h"
#include "Modules/ModuleManager.h"
#include "DlLogChannels.h"


class FDlModule : public FDefaultGameModuleImpl
{

public:
	virtual void StartupModule() override
	{
		// Module startup logic here
		FDefaultGameModuleImpl::StartupModule();
		UE_LOG(LogDl, Warning, TEXT("Dl module has started!"));
	}
	virtual void ShutdownModule() override
	{
		// Module shutdown logic here
		FDefaultGameModuleImpl::ShutdownModule();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FDlModule, Dl, "Dl");