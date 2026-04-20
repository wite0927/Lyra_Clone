// Fill out your copyright notice in the Description page of Project Settings.

#include "DlActivatableWidget.h"
#include "CommonInputModeTypes.h"

UDlActivatableWidget::UDlActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UDlActivatableWidget::GetDesiredInputConfig() const
{
	// 앞서 정의 했던 WidgetInputMode에 따라, InputConfig를 설정해준다
	// - ECommonInputMode에 따라 Input의 흐름을 Game/Menu 혹은 둘다로 정하는 것이 가능하다
	// - 예로 들어, 우리가 Menu로 들어갔을 경우, 더이상 Game에 Input을 보내고 싶지 않을 경우 매우 용의
	switch (InputConfig)
	{
	case EHakWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EHakWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EHakWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);
	case EHakWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
