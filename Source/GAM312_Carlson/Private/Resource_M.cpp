// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource_M.h"

// Sets default values
AResource_M::AResource_M()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up components
	ResourceNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ResourceNameText"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Make mesh root
	RootComponent = Mesh;

	// Attach the text render component to the mesh
	ResourceNameText->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AResource_M::BeginPlay()
{
	Super::BeginPlay();
	
	// Get name of resource and set text to it
	tempText = tempText.FromString(resourceName);
	ResourceNameText->SetText(tempText);
}

// Called every frame
void AResource_M::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

