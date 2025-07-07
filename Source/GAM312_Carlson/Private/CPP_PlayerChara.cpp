// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerChara.h"

// Sets default values
ACPP_PlayerChara::ACPP_PlayerChara()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetMesh(), "head"); // Attach Cam to head
	CameraComponent->bUsePawnControlRotation = true; // Rotate Camera with Chara
}

// Called when the game starts or when spawned
void ACPP_PlayerChara::BeginPlay()
{
	Super::BeginPlay();
	
}

