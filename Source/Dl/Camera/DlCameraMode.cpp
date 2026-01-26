// Fill out your copyright notice in the Description page of Project Settings.


#include "DlCameraMode.h"
#include "DlPlayerCameraManager.h"
#include "DlCameraComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DlCameraMode)

FDlCameraModeView::FDlCameraModeView()
    : Location(ForceInit) 
    , Rotation(ForceInit)
    , ControlRotation(ForceInit)
    , FieldOfView(DL_CAMERA_DEFAULT_FOV)
{
}

void FDlCameraModeView::Blend(const FDlCameraModeView& Other, float OtherWeight)
{
    if (OtherWeight <= 0.0f)
    {
        return;
    }
    else if (OtherWeight >= 1.0f)
    {
        // Weight가 1.0이면 Other를 덮어쓰면 된다
        *this = Other;
        return;
    }

    // Location + OtherWeight * (Other.Location - Location);
    Location = FMath::Lerp(Location, Other.Location, OtherWeight);
    
    const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
    Rotation = Rotation + (OtherWeight * DeltaRotation);

    const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
    ControlRotation = ControlRotation + (OtherWeight * DeltaRotation);

    FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UDlCameraMode::UDlCameraMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    FieldOfView = DL_CAMERA_DEFAULT_FOV;
    ViewPitchMin = DL_CAMERA_DEFAULT_PITCH_MIN;
    ViewPitchMax = DL_CAMERA_DEFAULT_PITCH_MAX;

    BlendTime = 0.0f;
    BlendAlpha = 1.0f;
    BlendWeight = 1.0f;

    BlendFunction = EDlCameraModeBlendFunction::EaseOut;
    BlendExponent = 4.0f;
}

void UDlCameraMode::UpdateCameraMode(float DeltaTime)
{
    // Actor를 활용하여, Pivot[Location|Rotation]을 계산하여, View를 업데이트
    UpdateView(DeltaTime);

    // BlendWeight를 DeltaTime을 활용하여, BlendAlpha 계산 후, BlendFunction에 맞게 재-매핑하여 최종 계산
    UpdateBlending(DeltaTime);
}

void UDlCameraMode::UpdateView(float DeltaTiem)
{
    // CameraMode를 가지고 있는 CameraComponent의 Owner인 Character(Pawn)을 활용하여, PivotLocation/Rotation을 반환함
    FVector PivotLocation = GetPivotLocation();
    FRotator PivotRotation = GetPivotRotation();

    // Pitch 값에 대해 Min/Max를 Clamp시킴
    PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

    // FDlCameraModeView에 PivotLocation/Rotation 설정
    View.Location = PivotLocation;
    View.Rotation = PivotRotation;

    // PivotRotation을 똑같이 ControlRotation으로 활용
    View.ControlRotation = View.Rotation;
    View.FieldOfView = FieldOfView;

    // 정리하면, Character의 Location과 ControlRotation을 활용하여, View를 업데이트함
}

FVector UDlCameraMode::GetPivotLocation() const
{
    const AActor* TargetActor = GetTargetActor();
    check(TargetActor);

    if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
    {
        // BaseEyeHeight를 고려하여, ViewLocation을 반환함
        return TargetPawn->GetPawnViewLocation();
    }

    return TargetActor->GetActorLocation();
}

FRotator UDlCameraMode::GetPivotRotation() const
{
    const AActor* TargetActor = GetTargetActor();
    check(TargetActor);

    if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
    {
        // GetViewRoation() 확인, 보통 Pawn의 ControlRotation을 반환
        return TargetPawn->GetViewRotation();
    }
    return TargetActor->GetActorRotation();;
}

void UDlCameraMode::UpdateBlending(float DeltaTime)
{
    // BlendAlpha를 DeltaTime을 통해 계산
    if (BlendTime > 0.f)
    {
        BlendAlpha += (DeltaTime / BlendTime);
    }
    else
    {
        BlendAlpha = 1.f;
    }

    const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
    switch (BlendFunction)
    {
    case EDlCameraModeBlendFunction::Linear:
        BlendWeight = BlendAlpha;
        break;
    case EDlCameraModeBlendFunction::EaseIn:
        BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
        break;
    case EDlCameraModeBlendFunction::EaseOut:
        BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
        break;
    case EDlCameraModeBlendFunction::EaseInOut:
        BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
        break;
    default:
        checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
        break;
    }
}

UDlCameraComponent* UDlCameraMode::GetDlCameraComponent() const
{
    // 우리가 앞서 UDlCameraMode를 생성하는 곳은 UDlCameraModeStack이었다:
    // - 해당 코드를 보면, GetOuter()를 DlCameraMode로 DlCameraComponent로 설정하였다
    // - UDlCameraModeStack::GetCameraModeInstance() 확인
    return CastChecked<UDlCameraComponent>(GetOuter());
}

AActor* UDlCameraMode::GetTargetActor() const
{
    const UDlCameraComponent* CameraComponent = GetDlCameraComponent();
    return CameraComponent->GetTargetActor();
}

UDlCameraModeStack::UDlCameraModeStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UDlCameraMode* UDlCameraModeStack::GetCameraModeInstance(TSubclassOf<UDlCameraMode>& CameraModeClass)
{
    // 카메라를 매번 만들 필요 없이, CameraMode를 캐싱해서 가져올 수 있게 최적화하는 역할
    check(CameraModeClass);

    // CameraModeInstances 에서 먼저 생성되어 있는지 확인
    for (UDlCameraMode* CameraMode : CameraModeInstances)
    {
        if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
        {
            return CameraMode;
        }
    }

    // CameraModeClass에 맞는 CameraMode의 인스턴스가 없으면 생성
    UDlCameraMode* NewCameraMode = NewObject<UDlCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
    check(NewCameraMode);

    CameraModeInstances.Add(NewCameraMode);

    return NewCameraMode;
}

void UDlCameraModeStack::PushCameraMode(TSubclassOf<UDlCameraMode>& CameraModeClass)
{
    if (!CameraModeClass)
    {
        return;
    }

    UDlCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
    check(CameraMode);

    int32 StackSize = CameraModeStack.Num();
    if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
    {
        // CameraModeStack[0] 가장 최근에 이미 CameraMode가 Stacking 되었으므로 리턴
        return;
    }

    // ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
    // ExistingStackContribution은 위에서 아래로 최종 BlendWeight 값을 찾기 위해 초기값으로 1.0으로 설정
    int32 ExistingStackIndex = INDEX_NONE;
    float ExistingStackContribution = 1.0f;

    /*
     * 이해용 예시
     * BlendWeight    |    ExistingStackCOntribution    |    ExistingStackCOntribution (accumulated)
     * 0.1f           |    (1.0f) * 0.1f = 0.1f         |    (1.0f - 0.1f) = 0.9f
     * 0.3f           |    (0.9f) * 0.3f = 0.27f        |    (1.0f - 0.3f) * 0.9f = 0.63f
     * 0.6f           |    (0.63f) * 0.6f = 0.378f      |    (1.0f - 0.6f) * 0.63f = 0.252f
     * 1.0f           |    (0.252f) * 1.0f = 0.252f     |
     *                |    0.1f + 0.27f + 0.378f + 0.252f = 1.0f!
     */

    for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
    {
        if (CameraModeStack[StackIndex] == CameraMode)
        {
            ExistingStackIndex = StackIndex;
            // BlendWeight를 CameraMode에 추가해주자:
            // - 여기서 ExistingStackContribution 우리가 찾는 CameraMode를 찾았으니깐, 
            // 누적된 BlendWeight와 함께 BlendWeight을 곱하여, 루프를 빠져나온다
            ExistingStackContribution *= CameraMode->BlendWeight;
            break;
        }
        else
        {
            // 당연히 우리가 원하는 CamearMode가 아니니깐, InvBlendWeight = (1.0 - BlendWeight)를 곱해줘야 값이 누적
            ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
        }
    }

    // CameraMode를 Top으로 반영하기 위해, 당연히 중간에 있었다면, 제거하여 다시 Push
    if (ExistingStackIndex != INDEX_NONE)
    {
        CameraModeStack.RemoveAt(ExistingStackIndex);
        StackSize--;
    }
    else
    {
        ExistingStackContribution = 0.0f;
    }

    // BlendTime이 0보다 크다는 것은 Blend를 얼마 시간동안 진행함을 의미 따라서, ExistingStackContribution을 적용
    // - 따라서 Blend하지 않는다면, BlendWeight를 1.0을 넣어 새로 넣는 CameraMode만 적용할 것이다
    const bool bShouldBlend = ((CameraMode->BlendTime > 0.0f) && (StackSize > 0));
    const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
    CameraMode->BlendWeight = BlendWeight;

    CameraModeStack.Insert(CameraMode, 0);

    // 마지막은 항상 1
    CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UDlCameraModeStack::EvaluateStack(float DeltaTime, FDlCameraModeView& OutCameraModeView)
{
    UpdateStack(DeltaTime);

    BlendStack(OutCameraModeView);
}

void UDlCameraModeStack::UpdateStack(float DeltaTime)
{
    const int32 StackSize = CameraModeStack.Num();
    if (StackSize <= 0) return;

    int32 RemoveCount = 0;
    int32 RemoveIndex = INDEX_NONE;
    for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
    {
        UDlCameraMode* CameraMode = CameraModeStack[StackIndex];
        check(CameraMode);

        CameraMode->UpdateCameraMode(DeltaTime);

        if (CameraMode->BlendWeight >= 1.0f)
        {
            RemoveIndex = (StackIndex + 1);
            RemoveCount = (StackSize - RemoveIndex);
            break;
        }
    }

    if (RemoveCount > 0)
    {
        CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
    }
}

void UDlCameraModeStack::BlendStack(FDlCameraModeView& OutCameraModeView) const
{
    const int32 StackSize = CameraModeStack.Num();
    if (StackSize <= 0)
    {
        return;
    }

    // CameraModeStack은 Bottom -> Top 순서로 Blend를 진행
    const UDlCameraMode* CameraMode = CameraModeStack[StackSize - 1];
    check(CameraMode);

    OutCameraModeView = CameraMode->View;

    // 이미 Index = [StackSize - 1] 이미 OutCameraModeView로 지정했으므로, StackSize - 2부터 순회하면 된다
    for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
    {
        CameraMode = CameraModeStack[StackIndex];
        check(CameraMode);

        OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
    }
}
