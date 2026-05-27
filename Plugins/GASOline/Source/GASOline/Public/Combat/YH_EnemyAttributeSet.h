// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASO_AttributeSet.h"
#include "YH_EnemyAttributeSet.generated.h"

/**
 * Attribute set for all Wrath enemies (Light Wrath, Dark Wrath).
 * Inherits Health and MaxHealth from UGASO_AttributeSet.
 * Adds AttackPower for enemy damage output.
 */
UCLASS()
class GASOLINE_API UYH_EnemyAttributeSet : public UGASO_AttributeSet
{
	GENERATED_BODY()
	
public:

	UYH_EnemyAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//AttackPower - scales damage applied to Maurice on enemy attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Combat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UYH_EnemyAttributeSet, AttackPower)

protected:

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

};
