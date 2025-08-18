// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameModeMain.generated.h"

/* AI Explanation
* The navigation mesh is a bounding box which scans and enables the AI
* to pathfind and navigate the world. It essentially tells the AI where it can and cannot go,
* allowing it to make intelligent decisions about movement and pathfinding, despite the simplicity of the AI itself.
 */
UCLASS()
class GAM312_CARLSON_API ACPP_GameModeMain : public AGameModeBase
{
	GENERATED_BODY()
	
};
