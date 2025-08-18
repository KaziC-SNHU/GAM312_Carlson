// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerChara.h"

// Sets default values
ACPP_PlayerChara::ACPP_PlayerChara()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Camera Libraries in gaming
	* Camera libraries are essential in gaming as they define
	* the player's perspective and how they are able to interact with the game world.
	* Without a camera, players would not have access to visual feedback in the game,
	* which could lead to confusion and a lack of engagement.
	* A lack of camera could be useful for a few different gameplay scenarios, but in general relies on other senses,
	* or a different form of camera.
	*/
	// Setup the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), "head"); // Attach Cam to head
	CameraComponent->bUsePawnControlRotation = true; // Rotate Camera with Chara

	//  Setting up an "inventory"
	ResourcesArray.SetNum(3); // Set size of ResourcesArray
	BuildingSuppliesArray.SetNum(3); // Set size of BuildingSuppliesArray
	ResourcesNames.Add(TEXT("Wood"));
	ResourcesNames.Add(TEXT("Stone"));
	ResourcesNames.Add(TEXT("Strawberry"));
}

// Called when the game starts or when spawned
void ACPP_PlayerChara::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &ACPP_PlayerChara::DecreaseStats, 1.0f, true);

	// Update objective UI
	if (objectiveUI) 
	{
		objectiveUI->UpdateMatOBJ(0.0f);
		objectiveUI->UpdateBuildOBJ(0.0f);
	}

}

void ACPP_PlayerChara::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	playerUI->UpdateBars(Health, Hunger, Stamina);

	if (isBuilding)
	{
		if (spawnedPart)
		{
			FVector StartLocation = CameraComponent->GetComponentLocation();
			FVector Direction = CameraComponent->GetForwardVector();
			FVector EndLocation = StartLocation + (Direction * 500.0f);
			spawnedPart->SetActorLocation(EndLocation);
		}
		
	}
}

// Adjust Health
void ACPP_PlayerChara::SetHealth(float amount)
{
	if (Health + amount < 100 && Health + amount > -1)
	{
		Health += amount;
	}

	else if (Health + amount >= 100)
	{
		Health = 100.0f; // Cap health at 100
	}

	else if (Health + amount <= 0)
	{
		Health = 0.0f; // Min health at 0
	}
}

// Adjust Hunger
void ACPP_PlayerChara::SetHunger(float amount)
{
	if (Hunger + amount < 100 && Hunger + amount > -1)
	{
		Hunger += amount;
	}

	else if (Hunger + amount >= 100)
	{
		Hunger = 100.0f; // Cap hunger at 100
	}

	else if (Hunger + amount <= 0)
	{
		Hunger = 0.0f; // Min hunger at 0
	}

}

// Adjust Stamina
void ACPP_PlayerChara::SetStamina(float amount)
{
	if (Stamina + amount < 100 && Stamina + amount > -1)
	{
		Stamina += amount;
	}

	else if (Stamina + amount >= 100)
	{
		Stamina = 100.0f; // Cap health at 100
	}

	else if (Stamina + amount <= 0)
	{
		Stamina = 0.0f; // Min health at 0
	}
}

// Drain Hunger / Health and regend stamina
void ACPP_PlayerChara::DecreaseStats()
{
	if (Hunger > 0.0f)
	{
		SetHunger(-1.0f);
	}

	SetStamina(10.0f);

	if (Hunger <= 0.0f)
	{
		SetHealth(-3.0f);
	}
}

// Add Resources
void ACPP_PlayerChara::GiveResource(float amount, FString resource)
{
	if (resource == "Wood")
	{
		ResourcesArray[0] += amount;
	}

	else if (resource == "Stone")
	{
		ResourcesArray[1] += amount;
	}
	else if (resource == "Strawberry")
	{
		ResourcesArray[2] += amount;
	}
	else // Break glass in case of emergency
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown resource type: %s"), *resource);
		return;
	}
}

void ACPP_PlayerChara::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0] && stoneAmount <= ResourcesArray[1])
	{
		ResourcesArray[0] -= woodAmount;
		ResourcesArray[1] -= stoneAmount;

		if (buildingObject == "Wall")
		{
			BuildingSuppliesArray[0] += 1;
		}

		if (buildingObject == "Floor")
		{
			BuildingSuppliesArray[1] += 1;
		}

		if (buildingObject == "Ceiling")
		{
			BuildingSuppliesArray[2] += 1;
		}
	}
}

void ACPP_PlayerChara::SpawnBuilding(int buildingID, bool& isSuccessful)
{
	if (!isBuilding) // If not already building
	{
		if (BuildingSuppliesArray[buildingID] > 0) // Check for building supplies
		{
			isBuilding = true;
			FActorSpawnParameters spawnParams;
			FVector StartLocation = CameraComponent->GetComponentLocation();
			FVector Direction = CameraComponent->GetForwardVector();
			FVector EndLocation = StartLocation + (Direction * 500.0f);
			FRotator myRot(0, 0, 0);

			BuildingSuppliesArray[buildingID] -= 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, spawnParams);

			isSuccessful = true;
		}

		else // Not enough supplies
		{
			isSuccessful = false;
		}
	}
}

void ACPP_PlayerChara::RotateBuilding()
{
	spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
}

