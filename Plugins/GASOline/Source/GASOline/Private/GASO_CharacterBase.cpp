// Fill out your copyright notice in the Description page of Project Settings.


#include "GASO_CharacterBase.h"

// Sets default values
AGASO_CharacterBase::AGASO_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGASO_ASC>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGASO_AttributeSet>(TEXT("AttributeSet"));

}

UAbilitySystemComponent* AGASO_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAbilitySystemComponent* AGASO_CharacterBase::GetAbilitySystemComponent_Implementation() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGASO_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASO_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void AGASO_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();	
}



void AGASO_CharacterBase::InitializeAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

// Called to bind functionality to input
void AGASO_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

