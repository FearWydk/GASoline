// Elemental Action RPG — The Severance
// Base Wrath Enemy Attribute Set
// GASoline Plugin | Combat Module



#include "Combat/YH_EnemyAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GASO_CharacterBase.h"

UYH_EnemyAttributeSet::UYH_EnemyAttributeSet() : AttackPower(15.f)
{
	//Health = 0, Max Health = 100 set by UGASO_AttibuteSet base constructor
}

void UYH_EnemyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UYH_EnemyAttributeSet, AttackPower);
}

void UYH_EnemyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Only handle Health changes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp Health between 0 and MaxHealth
		float CurrentHealth = GetHealth();
		float ClampedHealth = FMath::Clamp(CurrentHealth, 0.f, GetMaxHealth());
		SetHealth(ClampedHealth);

		// Calculate actual damage dealt
		float DamageDealt = GetMaxHealth() - ClampedHealth;

		// Notify the owning character so Blueprint can forward to AC_Health
		AActor* OwnerActor = GetOwningActor();
		if (OwnerActor)
		{
			AGASO_CharacterBase* OwnerCharacter =
				Cast<AGASO_CharacterBase>(OwnerActor);
			if (OwnerCharacter)
			{
				// Call Blueprint event on the owner character
				OwnerCharacter->OnGASHealthChanged(ClampedHealth, GetMaxHealth());
			}
		}
	}
}


void UYH_EnemyAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UYH_EnemyAttributeSet, AttackPower, OldValue);
}

