// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoardHUD.generated.h"

/**
 * The HUD for the Tetris board.
 */
UCLASS()
class TETRIS_API UBoardHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/* Update this widget using the board state.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Line Counter")
	void Update(class ATetrisBoard* Board);
};
