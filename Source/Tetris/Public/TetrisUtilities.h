// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TetrisUtilities.generated.h"

/**
 * Various utilities used in the Tetris project.
 */
UCLASS()
class TETRIS_API UTetrisUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/* Convert the time in seconds to a timestamp of the form MM:SS.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Utilities")
	static FString SecondsToTimeString(int32 InSeconds);
};
