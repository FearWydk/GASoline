// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// GASoline Plugin | Combat Module



#include "Combat/YH_EnemyAttributeSet.h"
#include "Net/UnrealNetwork.h"

UYH_EnemyAttributeSet::UYH_EnemyAttributeSet() : AttackPower(15.f)
{
	//Health = 0, Max Health = 100 set by UGASO_AttibuteSet base constructor
}

void UYH_EnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYH_EnemyAttributeSet, AttackPower);
}

void UYH_EnemyAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_EnemyAttributeSet, AttackPower, OldValue);
}
