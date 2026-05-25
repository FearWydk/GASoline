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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASO_ASC> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UGASO_AttributeSet> AttributeSet;

private:

	void InitializeAbilitySystem();

public:	


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
