#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "State/Enumerations/AlsGait.h"
#include "State/Enumerations/AlsMovementDirection.h"
#include "State/Enumerations/AlsRotationMode.h"
#include "State/Enumerations/AlsStance.h"
#include "State/Enumerations/AlsViewMode.h"
#include "State/Structures/AlsFeetState.h"
#include "State/Structures/AlsInAirState.h"
#include "State/Structures/AlsLayeringState.h"
#include "State/Structures/AlsLeanState.h"
#include "State/Structures/AlsLocomotionAnimationState.h"
#include "State/Structures/AlsMovementState.h"
#include "State/Structures/AlsPoseState.h"
#include "State/Structures/AlsRagdollingAnimationState.h"
#include "State/Structures/AlsRotateInPlaceState.h"
#include "State/Structures/AlsTurnInPlaceState.h"
#include "State/Structures/AlsViewAnimationState.h"
#include "AlsAnimationInstance.generated.h"

class UAlsAnimationInstanceSettings;
class AAlsCharacter;

UCLASS()
class ALS_API UAlsAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	UAlsAnimationInstanceSettings* Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter;

	// Used to indicate that the animation instance has not been updated for a long time
	// and its current state may be incorrect (such as foot location used in foot locking).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bPendingUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bAllowDynamicTransitions{true};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsGait Gait;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRotationMode RotationMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsViewMode ViewMode{EAlsViewMode::ThirdPerson};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag OverlayMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GroundedEntryMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLocomotionAnimationState LocomotionState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLayeringState LayeringState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsPoseState PoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsViewAnimationState ViewState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsFeetState FeetState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsLeanState LeanState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsMovementDirection MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRotateInPlaceState RotateInPlaceState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsTurnInPlaceState TurnInPlaceState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsInAirState InAirState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsRagdollingAnimationState RagdollingState;

	FTimerHandle DynamicTransitionsAllowanceTimer;

	FTimerHandle PivotResetTimer;

	FTimerHandle JumpResetTimer;

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// Core

public:
	void SetPendingUpdate(bool bNewPendingUpdate);

	EAlsRotationMode GetRotationMode() const;

	const FGameplayTag& GetLocomotionMode() const;

	const FAlsFeetState& GetFeetState() const;

private:
	void RefreshLocomotion(float DeltaTime);

	void RefreshLayering();

	void RefreshPose();

	void RefreshView(float DeltaTime);

protected:
	virtual bool IsSpineRotationAllowed();

	// Feet

private:
	void RefreshFeet(float DeltaTime);

	void HandleFootLockChangedBase(FAlsFootState& FootState, const FName& FootBoneName, const FVector& BaseLocation,
	                               const FQuat& BaseRotation, const FTransform& RelativeTransform) const;

	void RefreshFootLock(FAlsFootState& FootState, const FName& FootBoneName,
	                     const FName& FootLockCurveName, const FTransform& RelativeTransform,
	                     float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation) const;

	void RefreshFootOffset(FAlsFootState& FootState, float DeltaTime, FVector& TargetLocationOffset,
	                       FVector& FinalLocation, FQuat& FinalRotation) const;

	static void ResetFootOffset(FAlsFootState& FootState, float DeltaTime, FVector& FinalLocation, FQuat& FinalRotation);

	static void RefreshFinalFootState(FAlsFootState& FootState, const FTransform& RelativeTransform,
	                                  const FVector& FinalLocation, const FQuat& FinalRotation);

	void RefreshPelvisOffset(float DeltaTime, float TargetFootLeftLocationOffsetZ, float TargetFootRightLocationOffsetZ);

	// Grounded Movement

public:
	void SetGroundedEntryMode(const FGameplayTag& NewModeTag);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void ResetGroundedEntryMode();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void SetHipsDirection(EAlsHipsDirection NewDirection);

private:
	void RefreshMovement(float DeltaTime);

	EAlsMovementDirection CalculateMovementDirection() const;

	void RefreshVelocityBlend(const float DeltaTime);

	float CalculateStrideBlendAmount() const;

	float CalculateWalkRunBlendAmount() const;

	float CalculateStandingPlayRate() const;

	float CalculateCrouchingPlayRate() const;

	// Pivot

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void ActivatePivot();

	// Transitions

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	UAnimSequenceBase* SelectDynamicTransitionForLeftFoot() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	UAnimSequenceBase* SelectDynamicTransitionForRightFoot() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransition(UAnimSequenceBase* Animation, float BlendInTime = 0.2f, float BlendOutTime = 0.2f,
	                    float PlayRate = 1.0f, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void PlayTransitionFromStandingIdleOnly(UAnimSequenceBase* Animation, float BlendInTime = 0.2f,
	                                        float BlendOutTime = 0.2f, float PlayRate = 1.0f, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void StopTransitionAndTurnInPlaceAnimations(float BlendOutTime = 0.2f);

private:
	void RefreshDynamicTransitions();

	void PlayDynamicTransition(UAnimSequenceBase* Animation, float BlendInTime = 0.2f, float BlendOutTime = 0.2f,
	                           float PlayRate = 1.0f, float StartTime = 0.0f, float AllowanceDelayTime = 0.0f);

	// Rotate In Place

protected:
	virtual bool IsRotateInPlaceAllowed();

private:
	void RefreshRotateInPlace(float DeltaTime);

	// Turn In Place

protected:
	virtual bool IsTurnInPlaceAllowed();

private:
	void RefreshTurnInPlace(float DeltaTime);

	void StartTurnInPlace(float TargetYawAngle, float PlayRateScale = 1.0f, float StartTime = 0.0f, bool bAllowRestartIfPlaying = false);

	// Jump

public:
	void Jump();

	// In Air

private:
	void RefreshInAir(float DeltaTime);

	float CalculateGroundPredictionAmount() const;

	FAlsLeanState CalculateInAirLeanAmount() const;

	// Ragdolling

private:
	void RefreshRagdolling();

public:
	void StopRagdolling();

private:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Animation Instance")
	void FinalizeRagdolling();

	// Utility

public:
	float GetCurveValueClamped01(const FName& CurveName) const;
};

inline void UAlsAnimationInstance::SetPendingUpdate(const bool bNewPendingUpdate)
{
	bPendingUpdate = bNewPendingUpdate;
}

inline EAlsRotationMode UAlsAnimationInstance::GetRotationMode() const
{
	return RotationMode;
}

inline const FGameplayTag& UAlsAnimationInstance::GetLocomotionMode() const
{
	return LocomotionMode;
}

inline const FAlsFeetState& UAlsAnimationInstance::GetFeetState() const
{
	return FeetState;
}

inline void UAlsAnimationInstance::SetGroundedEntryMode(const FGameplayTag& NewModeTag)
{
	GroundedEntryMode = NewModeTag;
}

inline void UAlsAnimationInstance::ResetGroundedEntryMode()
{
	GroundedEntryMode = FGameplayTag::EmptyTag;
}

inline void UAlsAnimationInstance::SetHipsDirection(const EAlsHipsDirection NewDirection)
{
	MovementState.HipsDirection = NewDirection;
}
