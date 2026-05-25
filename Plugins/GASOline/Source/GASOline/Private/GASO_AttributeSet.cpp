// Fill out your copyright notice in the Description page of Project Settings.


#include "GASO_AttributeSet.h"
#include "Net/UnrealNetwork.h"

UGASO_AttributeSet::UGASO_AttributeSet() :
	Health(),
	MaxHealth(100.f)
{

}



void UGASO_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGASO_AttributeSet, Health);
	DOREPLIFETIME(UGASO_AttributeSet, MaxHealth);

}

void UGASO_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASO_AttributeSet, Health, OldValue);
}

void UGASO_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASO_AttributeSet, MaxHealth, OldValue);
}

