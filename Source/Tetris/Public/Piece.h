// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Piece.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API UPiece : public UObject
{
	GENERATED_BODY()

public:
	/* The piece obtained from a clockwise rotation of this piece.*/
	UPROPERTY()
	UPiece* Next;
	
	/* The piece obtained from a counter-clockwise rotation of this piece.*/
	UPROPERTY()
	UPiece* Prev;

	/* The piece body.*/
	TArray<FIntPoint> Body;

	/* The piece skirt.*/
	TArray<FIntPoint> Skirt;
};
