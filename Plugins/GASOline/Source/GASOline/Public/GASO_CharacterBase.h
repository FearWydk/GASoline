// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GASO_ASI.h"
#include "AbilitySystemInterface.h"
#include "GASO_ASC.h"
#include "GASO_AttributeSet.h"
#include "GASO_CharacterBase.generated.h"

UCLASS()
class GASOLINE_API AGASO_CharacterBase : public ACharacter, public IGASO_ASI, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASO_CharacterBase();

	//IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//IGASO_ASI
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() const override;

	/**
	 * Called by UYH_EnemyAttributeSet::PostGameplayEffectExecute
	 * when Health changes via a GAS GameplayEffect.
	 * Override in Blueprint to bridge to AC_Health.
	 *
	 * @param NewHealth   Current clamped health value
	 * @param MaxHealth   Maximum health value
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|Attributes|Health")
	void OnGASHealthChanged(float NewHealth, float MaxHealth);
	virtual void OnGASHealthChanged_Implementation(float NewHealth, float MaxHealth);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASO_ASC> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASO_AttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes")
	TSubclassOf<UGASO_AttributeSet> AttributeSetClass;



private:

	void InitializeAbilitySystem();

public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
