//Josh Brooks Copyright 2026
// Elemental Action RPG — The Resonance
// Maurice Sword Attack Ability
// GASoline Plugin | Combat Module


#include "Combat/Ability/YH_GA_SwordAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagsManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Combat/YH_PlayerAttributeSet.h"



UYH_GA_SwordAttack::UYH_GA_SwordAttack()
{
		// This ability is instanced per execution.
		InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;

		// Ability tag - used to activate via TryActiveAbilitesByTag
		FGameplayTagContainer NewTags;
		NewTags.AddTag(
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.SwordAttack")));
		SetAssetTags(NewTags);

		// Block other attacks while this is active
		ActivationBlockedTags.AddTag(
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Ability.GunAttack")));
}

void UYH_GA_SwordAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	

	// Wait for AnimNotify HitCheck event from Blueprint
	// Bluepring sends: Send Gamplay Event → YH.Combat.Event.HitCheck

	WaitHitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		FGameplayTag::RequestGameplayTag(FName("YH.Combat.Notify.HitCheck")),
		nullptr,
		true);

	WaitHitCheckTask->EventReceived.AddDynamic(this, &UYH_GA_SwordAttack::OnHitCheckReceived);
	WaitHitCheckTask->ReadyForActivation();

	// Note: Montage is played by Blueprint before calling TryActivateAbility.
	// GAS handle damage only - animation stays in Blueprint for easier combo system implementation.

}

void UYH_GA_SwordAttack::OnHitCheckReceived(FGameplayEventData Payload)
{

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, true);
		return;
	}

	PerformHitTrace(ActorInfo);

	// End this ability instance - Blueprint combo counter handles next step
	EndAbility(GetCurrentAbilitySpecHandle(), ActorInfo, GetCurrentActivationInfo(), true, false);
}

void UYH_GA_SwordAttack::PerformHitTrace(const FGameplayAbilityActorInfo* ActorInfo)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (!AvatarActor) return;

	UWorld* World = AvatarActor->GetWorld();
	if (!World) return;

	// Trace from Maurice forward by HitTraceDistance
	FVector TraceStart = AvatarActor->GetActorLocation();
	FVector TraceEnd = TraceStart + (AvatarActor->GetActorForwardVector() * HitTraceDistance);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	// Sphere sweep for enemies using HitTraceRadius around Maurice's forward position in the shape of a sphere. Hits are sto
	World->SweepMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(HitTraceRadius),
		QueryParams
	);



	



	UAbilitySystemComponent* SourceASC =
		ActorInfo->AbilitySystemComponent.Get();
	if (!SourceASC) return;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;

		// Get the enemy's ASC
		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC) {
			
			continue;
		}
	
	

		// Apply Sword damage effect to enemy
		if (SwordDamageEffectClass)
		{
			FGameplayEffectContextHandle EffectContext =
				SourceASC->MakeEffectContext();
			EffectContext.AddSourceObject(AvatarActor);

			FGameplayEffectSpecHandle SpecHandle =
				SourceASC->MakeOutgoingSpec(
					SwordDamageEffectClass, 1.0f, EffectContext);

			if (SpecHandle.IsValid())
			{
				SourceASC->ApplyGameplayEffectSpecToTarget(
					*SpecHandle.Data.Get(), TargetASC);
			}

		}
		
		//Send Hit.Confirmed event back to Blueprint
		//Blueprint uses this to progree combo counter
		FGameplayEventData HitConfirmedPayload;
		HitConfirmedPayload.Target = HitActor;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			AvatarActor,
			FGameplayTag::RequestGameplayTag(FName("YH.Combat.Hit.Confirmed")),
			HitConfirmedPayload
		);

		// Only damage the first valid enemy hit per swing
		break;
	}
}

void UYH_GA_SwordAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicatedEndAbility,
	bool bWasCancelled)
{
	if (WaitHitCheckTask)
	{
		WaitHitCheckTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}