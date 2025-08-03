// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_PlayerCont.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


ACPP_PlayerCont::ACPP_PlayerCont()
{
}

// On event start
void ACPP_PlayerCont::BeginPlay()
{
	Super::BeginPlay();

	// Set Chara ref
	PlayerChara = Cast<ACPP_PlayerChara>(GetCharacter());

	// Check Player cont valid
	if (ACPP_PlayerCont* PC = Cast<ACPP_PlayerCont>(this))
	{
		// Get subsystem and set to local player
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// Set Input mapping context
			if (PlayerInputContext)
			{
				Subsystem->AddMappingContext(PlayerInputContext, 0);
			}
		}
	}
}

void ACPP_PlayerCont::SetupInputComponent()
{
	// Set up actual inputs
	Super::SetupInputComponent();
	if (ACPP_PlayerCont* PC = Cast<ACPP_PlayerCont>(this))
	{
		if (UEnhancedInputComponent* Subsystem = Cast<UEnhancedInputComponent>(InputComponent))
		{
			if (MoveAction)
			{
				Subsystem->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCont::Move);
			}
			if (LookAction)
			{
				Subsystem->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCont::Look);
			}
			if (JumpAction)
			{
				Subsystem->BindAction(JumpAction, ETriggerEvent::Started, this, &ACPP_PlayerCont::Jump);
			}
			if (InteractAction)
			{
				Subsystem->BindAction(InteractAction, ETriggerEvent::Started, this, &ACPP_PlayerCont::Interact);
			}
			if (RotateBuildingAction)
			{
				Subsystem->BindAction(RotateBuildingAction, ETriggerEvent::Started, this, &ACPP_PlayerCont::Rotate);
			}
		}
	}
}

void ACPP_PlayerCont::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Validate Controlled
	if (APawn* ControlledPawn = GetPawn())
	{
		// Get Rotation of controller
		FRotator CameraRotation = GetControlRotation();
		FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f); // Get Yaw and ignore Pitch

		// Get Forward and Right Direction Vectors
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add Movement Input
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACPP_PlayerCont::Look(const FInputActionValue& Value)
{
	// Check if looking
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// Validate Controlled
	if (APawn* ControlledPawn = GetPawn())
	{
		// Add Yaw and Pitch Input to camera
		AddYawInput(LookAxisVector.X);
		AddPitchInput(LookAxisVector.Y);
	}
}

void ACPP_PlayerCont::Jump(const FInputActionValue& Value)
{
	if (PlayerChara)
	{
		PlayerChara->Jump();
	}
}

void ACPP_PlayerCont::Rotate(const FInputActionValue& Value)
{
	if (PlayerChara->isBuilding)
	{
		FRotator CurrentRotation = PlayerChara->spawnedPart->GetActorRotation();
		CurrentRotation.Yaw += 90.0f; // Rotate 90 degrees on Yaw
		PlayerChara->spawnedPart->SetActorRotation(CurrentRotation);
	}
}

void ACPP_PlayerCont::Interact(const FInputActionValue& Value)
{
	// Line Trace set to 1000 units
	FVector Start = PlayerChara->CameraComponent->GetComponentLocation();
	FVector End = Start + (PlayerChara->CameraComponent->GetForwardVector() * 1000.0f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerChara); // Ignore the player character
	CollisionParams.bTraceComplex = true; // Use complex collision for better accuracy
	CollisionParams.bReturnFaceIndex = true; // Return face index for more detailed hit information

	// Start the line trace
	if (!PlayerChara->isBuilding)
	{
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			if (HitResult.GetActor())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Interacting with: %s"), *HitResult.GetActor()->GetName()));

				// Cast the hit actor to Resource_M
				AResource_M* Resource = Cast<AResource_M>(HitResult.GetActor());

				// If the hit actor is a resource
				if (PlayerChara->Stamina >= 5.0f)
				{
					if (Resource)
					{
						// Get Resource Name and Amount
						FString hitName = Resource->resourceName;
						int resourceValue = Resource->resourceAmount;

						// Damage Resource
						Resource->totalResource -= resourceValue;

						// Give to player
						if (Resource->totalResource >= resourceValue)
						{
							PlayerChara->GiveResource(resourceValue, hitName);

							PlayerChara->matsCollected += resourceValue; // Update mats collected

							PlayerChara->objectiveUI->UpdateMatOBJ(PlayerChara->matsCollected); // Update objective UI

							check(GEngine != nullptr);
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Gave %d of %s to player"), resourceValue, *hitName)); // debug

							// Spawn hit marker
							UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 0.5f);

							PlayerChara->SetStamina(-5.0f); // Decrease stamina by 5
						}

						// If resource is depleted, destroy it
						else
						{
							Resource->Destroy();
							check(GEngine != nullptr);
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Destroyed %s"), *hitName)); // debug
						}
					}
				}
			}
		}

	}

	else
	{
		PlayerChara->isBuilding = false;
		PlayerChara->objectsBuilt += 1; // Update objects built

		PlayerChara->objectiveUI->UpdateBuildOBJ(PlayerChara->objectsBuilt); // Update objective UI
	}
}
