Lyra Clone Study

Unreal Engine 5 기반의 Lyra Starter Game 클론 코딩 프로젝트입니다.
단순 기능 복제가 아니라, Lyra의 구조를 해석하고 저의 프로젝트 구조로 재구성하는 과정에 집중했습니다.

📌 Overview

이 프로젝트는 Epic Games의 Lyra Starter Game을 분석하며,
언리얼 엔진의 샘플 프로젝트를 “왜 이렇게 설계되었는가” 관점에서 따라 구현한 클론 코딩 프로젝트입니다.

초기에는 UE5 소스 환경 구성과 프로젝트 세팅, DefaultEditorOverview 맵 구성부터 시작했고,
이후 AssetManager / Experience / GameFeature / Modular Gameplay / Inventory / Equipment / GAS / GameplayCue / UI/HUD / Health / Ammo UI까지 점진적으로 확장했습니다.
매 주차별 구현 내용을 정리하며, 단순 구현이 아니라 구조 이해와 디버깅 과정까지 기록하는 방식으로 학습했습니다.

🎯 Goal

이 프로젝트의 목표는 단순히 Lyra를 따라 만드는 것이 아니라, 다음을 훈련하는 것이었습니다.

대규모 Unreal 샘플 프로젝트를 읽고 구조를 파악하는 능력
기능이 아니라 시스템 단위로 이해하는 습관
확장성과 유지보수성을 고려한 C++ 기반 프로젝트 구성
학습 내용을 문서화하고 설명 가능한 형태로 정리하는 습관
🧩 What I Focused On
1. 구조를 이해하며 구현하기

Lyra는 단순 예제 프로젝트가 아니라, AssetManager, Experience, GameFeature, GAS, UI 등
언리얼의 여러 시스템이 유기적으로 연결된 샘플입니다.

이 프로젝트에서는 코드를 그대로 옮기는 것보다,
각 시스템이 어떤 책임을 가지는지 파악하고 내 프로젝트 구조에 맞게 정리하는 것에 집중했습니다.

2. 기능보다 흐름을 이해하기

특히 아래 흐름을 중요하게 봤습니다.

게임 시작 시 어떤 데이터가 먼저 로딩되는가
Experience가 어떤 방식으로 게임 흐름을 바꾸는가
GameFeature가 어떤 방식으로 기능을 주입하는가
플레이어 초기화가 어떤 시점에 어떤 순서로 일어나는가
UI/HUD가 LocalPlayer 기준으로 어떻게 생성되는가
3. 디버깅 가능한 상태로 학습하기

단순히 “작동했다”에서 끝내지 않고,

로딩 실패 원인
초기화 순서 문제
Asset 등록 누락
Plugin / Module 설정 문제
ASC 초기화 시점 문제
HUD 로딩 및 InputConfig 문제

같은 지점을 직접 추적하며 구조적으로 이해하려고 했습니다.

🛠 Tech Stack
Language: C++
Engine: Unreal Engine 5
Core Topics
Lyra Starter Game
AssetManager / Primary Asset
Experience System
GameFeature Plugin
Modular Gameplay
Gameplay Ability System
GameplayCue
Inventory / Equipment / QuickBar
HUD / UI Extension / CommonGame
📚 Major Implementations
1) Project / Engine Setup
UE5 소스 환경에서 프로젝트를 구성하고, LyraStarterGame과 별도 프로젝트를 같은 환경에서 관리
Setup.bat, GenerateProjectFiles.bat, .uprojectdirs 동작 방식 확인
Git / Git LFS를 활용한 uasset, umap 관리 방식 정리
2) AssetManager / Primary Asset Loading
HakAssetManager를 직접 구성하며 AssetManager 기반 로딩 구조 확인
UserFacingExperienceDefinition, ExperienceDefinition, PawnData 연결
PrimaryAssetType, Specific Assets, AsyncLoadPrimaryAssetList 흐름 추적
에셋 단순 참조가 아니라, 게임 시작 시 어떤 데이터가 어떤 기준으로 로딩되는지 학습
3) Experience System
GameMode에서 Experience 로딩을 시작하고 완료 이후 플레이어 스폰으로 이어지는 흐름 구성
Experience 로딩 전후의 게임 시작 순서 제어
맵, PawnData, ActionSet, 추가 컴포넌트 활성화가 Experience 중심으로 이어지는 구조 구현
4) InitState / Modular Initialization
PawnExtensionComponent, HeroComponent 구현
GameFrameworkComponentManager 기반 Feature 등록/해제 구조 이해
단순 BeginPlay가 아닌 초기화 상태 머신 기반 흐름 학습
5) Camera System
CameraMode, CameraModeStack, ThirdPerson CameraMode 구현
카메라 값을 단순 갱신하는 방식이 아닌, 스택 기반 평가와 블렌딩 구조 정리
6) GameFeature Plugins
CommonUser, ShooterCore, ModularGameplayActors, CommonGame 구성
GameFeatureData를 통해 플러그인별 기능과 로딩 경로 관리
프로젝트 본체와 기능 단위를 분리하는 구조를 직접 적용
7) Modular Actor / Cosmetics
Character, PlayerController를 Modular Actor 계열로 교체
GameFeatureAction 기반 컴포넌트 주입 구조 적용
Cosmetic 컴포넌트와 애셋 흐름을 통해 데이터 중심 외형 구성 방식 학습
8) Inventory / Equipment / QuickBar
InventoryItemDefinition, InventoryItemInstance, InventoryManagerComponent
EquipmentDefinition, EquipmentManagerComponent, QuickBarComponent
초기 인벤토리 지급 → 장비 장착 → 퀵바 연동 흐름 구성
Fragment 기반 데이터 설계를 통해 아이템 구조 확장 방식 학습
9) Gameplay Ability System
AbilitySystemComponent, GameplayAbility, AbilitySet 구성
PlayerState 기반 ASC 소유 구조 적용
PawnData / EquipmentDefinition을 통한 AbilitySet 부여
GameplayAbility_RangedWeapon, GA_Weapon_Fire_Pistol로 이어지는 무기 Ability 구조 구현
10) GameplayCue / Weapon FX
발사 Ability의 TargetData 처리 이후 GameplayCue로 발사/피격 이펙트 연결
GameplayCueNotify, GameFeatureAction_AddGameplayCuePath, HakGameFeaturePolicy, HakGameplayCueManager 구성
GameplayTag ↔ GameplayCue 연결 구조 학습
권총 발사, 피격 이펙트, 데칼 흐름 구현
11) HUD / UI / Health / Ammo
CommonGame 기반 LocalPlayer 중심 UI 구조 구성
HUDLayout, ActivatableWidget, UIExtensionPointWidget 구현
W_ShooterHUDLayout, W_HealthBar, W_AmmoCounter_Pistol 제작
HealthSet, Item StatTag, Ability Cost와 HUD를 연결하며 게임플레이 데이터와 UI의 연결 구조 학습
🗂 Project Structure
Source/
├─ HakGame/
│  ├─ AbilitySystem/
│  ├─ Animation/
│  ├─ Camera/
│  ├─ Character/
│  ├─ Cosmetics/
│  ├─ Equipment/
│  ├─ GameFeatures/
│  ├─ GameModes/
│  ├─ Inventory/
│  ├─ Player/
│  ├─ System/
│  ├─ UI/
│  └─ Weapons/
│
Plugins/
├─ CommonUser/
├─ ShooterCore/
├─ ModularGameplayActors/
└─ CommonGame/
🔍 What I Learned

이 프로젝트를 통해 가장 크게 얻은 것은 언리얼 샘플 프로젝트를 읽는 방식이었습니다.

Lyra를 따라 구현하면서, 기능 하나를 만들더라도
단순히 어디에 코드를 넣을지가 아니라,

이 책임은 어느 클래스가 가져야 하는지
이 데이터는 에셋으로 관리해야 하는지
이 기능은 Experience에 묶여야 하는지
GameFeature로 분리하는 것이 맞는지
LocalPlayer 기준으로 관리해야 하는지

를 먼저 고민하는 습관을 가지게 되었습니다.

즉, 이 프로젝트는 단순한 클론 코딩이 아니라,
대규모 언리얼 프로젝트의 구조를 읽고 재구성하는 연습이었습니다.

🚀 Future Work
전투 Ability 고도화
Attribute / Effect 기반 전투 시스템 확장
멀티플레이 동기화 구조 강화
Lyra 구조를 응용한 나만의 캐릭터 액션 시스템 설계
✅ Summary

이 프로젝트는 Lyra의 기능을 따라 만든 결과물이라기보다,
언리얼 엔진의 구조를 분석하고 설명 가능한 형태로 정리한 학습 프로젝트입니다.

저는 이 프로젝트를 통해,

샘플 프로젝트를 읽고
구조를 파악하고
시스템 단위로 재구성하고
문제를 디버깅하며 해결하는 과정

자체를 훈련했습니다.
