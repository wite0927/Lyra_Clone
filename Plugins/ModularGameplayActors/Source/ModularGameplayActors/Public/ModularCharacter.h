// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularCharacter.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	/*
	Character's interface
	*/
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
