// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisGameMode.generated.h"

/**
 * The game mode for Tetris
 */
UCLASS()
class TETRIS_API ATetrisGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	/* Reference to the game board.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris")
	class ATetrisBoard* Board;

	/* AActor overrides.*/
	virtual void BeginPlay() override;
};
