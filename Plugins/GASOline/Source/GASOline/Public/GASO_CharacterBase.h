// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GASO_ASI.h"
#include "AbilitySystemInterface.h"
#include "GASO_ASC.h"
#include "GASO_AttributeSet.h"
#include "GASO_AbilitySet.h"
#include "GASO_CharacterBase.generated.h"


UCLASS()
class GASOLINE_API AGASO_CharacterBase : public ACharacter, public IGASO_ASI, public IAbilitySystemInterface //May need to take out IGASO_ASI if it doesn't do anything different from IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASO_CharacterBase();

	//IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//IGASO_ASI may not need to implement this if it doesn't do anything different from IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASO_ASC> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS") // This is the attribute set that will be used by this character. It should be a subclass of UGASO_AttributeSet.
	TObjectPtr<UGASO_AttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes") // This is the class of the attribute set that will be used by this character. It should be a subclass of UGASO_AttributeSet.
	TSubclassOf<UGASO_AttributeSet> AttributeSetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
	TObjectPtr<UGASO_AbilitySet> CombatAbilitySet;

	UPROPERTY()
	FGASO_AbilitySet_GrantedHandles GrantedHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS|Abilities")
	EGameplayEffectReplicationMode GASO_ReplicationMode = EGameplayEffectReplicationMode::Mixed;


private:

	void InitializeAbilitySystem();

public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GAS")
	void OnGASDamageReceived(float DamageAmount);

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	float DefaultMaxHealth = 100.f;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
