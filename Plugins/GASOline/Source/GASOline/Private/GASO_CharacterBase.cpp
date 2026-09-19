// Fill out your copyright notice in the Description page of Project Settings.


#include "GASO_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GASO_AttributeSet.h"


// Sets default values
AGASO_CharacterBase::AGASO_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGASO_ASC>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(GASO_ReplicationMode);

	//Set size of capsule component to match the character's size. This is important for collision and movement.
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Set default attribute set class to UGASO_AttributeSet
	AttributeSetClass = UGASO_AttributeSet::StaticClass();

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
	UE_LOG(LogTemp, Warning, TEXT("%s BeginPlay ASC valid: %d"),
		*GetName(), AbilitySystemComponent != nullptr);
	if (CombatAbilitySet && AbilitySystemComponent)
	{
		CombatAbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHandles);
	}
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

		// Create AttributeSet from class
		if (AttributeSetClass)
		{
			AttributeSet = NewObject<UGASO_AttributeSet>(this, AttributeSetClass);
			AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());
			AttributeSet->SetMaxHealth(DefaultMaxHealth);
			AttributeSet->SetHealth(DefaultMaxHealth);
		}
	}
}

// Called to bind functionality to input
void AGASO_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGASO_CharacterBase::OnGASDamageReceived_Implementation(float DamageAmount)
{
	//Override in BP
}
