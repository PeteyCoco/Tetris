// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDCounter.generated.h"

/**
 * Widget that counts the number of cleared lines.
 */
UCLASS(Blueprintable)
class TETRIS_API UHUDCounter : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Update this widget using the board state.*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Line Counter")
	void Update(class ATetrisBoard* Board);
};
