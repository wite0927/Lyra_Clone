# Lyra Clone Study: Unreal Engine 5 Project

## 프로젝트 소개 (Introduction)

이 프로젝트는 Epic Games의 **Lyra Starter Game**을 분석하고 주요 시스템을 직접 구현하며,  
**Unreal Engine의 구조와 설계 방식을 학습하기 위해 진행한 클론 코딩 프로젝트**입니다.

단순히 기능을 따라 만드는 것보다, **AssetManager, Experience System, GameFeature, GAS, UI**와 같은  
핵심 시스템이 어떤 방식으로 연결되고 동작하는지 이해하는 데 집중했습니다.

---

## 데모

- Lyra Clone Study 프로젝트 정리 및 구현 내용

---

## 엔진 및 주요 시스템 (Engine & Core Systems)

- **Engine Version**: Unreal Engine 5
- **Language**: C++
- **Core Systems**
  - `AssetManager`
  - `Experience System`
  - `GameFeature Plugin`
  - `Gameplay Ability System (GAS)`
  - `Inventory / Equipment`
  - `HUD / UI`

---

## 핵심 구현 내용 (Core Features)

- **AssetManager / Primary Asset Loading**  
  Primary Asset 기반 로딩 구조를 이해하고, 게임 시작 시 필요한 데이터가 어떻게 관리되는지 학습했습니다.

- **Experience System**  
  Experience 중심으로 게임 시작 흐름과 데이터 로딩 구조를 구현했습니다.

- **GameFeature / Modular Gameplay**  
  기능을 플러그인 단위로 분리하고, 필요 시 동적으로 확장할 수 있는 구조를 적용했습니다.

- **Inventory / Equipment System**  
  인벤토리, 장비, 퀵바 흐름을 구성하며 데이터 중심 구조를 학습했습니다.

- **Gameplay Ability System (GAS)**  
  Ability 부여와 무기 발사 로직을 구현하며, GAS 기반 캐릭터 능력 처리 방식을 익혔습니다.

- **HUD / UI**  
  HealthBar, AmmoCounter 등 주요 HUD 요소를 구현하며 UI 구조를 정리했습니다.

```text
## 소스 코드 구조 (Source Code Structure)

```text
Source/DI/
├── AbilitySystem/   # Gameplay Ability System 관련 클래스
├── Animation/       # 애니메이션 인스턴스 및 애니메이션 처리
├── Camera/          # 카메라 시스템
├── Character/       # 캐릭터 및 Pawn 관련 클래스
├── Cosmetics/       # 캐릭터 외형 및 코스메틱 처리
├── Equipment/       # 장비 시스템
├── GameFeatures/    # GameFeature 관련 클래스
├── GameModes/       # 게임 모드 및 게임 흐름 관리
├── Input/           # 입력 처리 관련 클래스
├── Inventory/       # 인벤토리 시스템
├── Physics/         # 물리 및 충돌 관련 처리
├── Player/          # PlayerController, PlayerState 등 플레이어 관련 클래스
├── System/          # 공용 시스템 및 매니저 클래스
└── UI/              # HUD 및 UI 관련 클래스

Plugins/
├── CommonGame/
├── CommonUser/
├── GameFeatures/ShooterCore/
├── ModularGameplayActors/
└── UIExtension/
