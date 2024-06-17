// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "PieceData.generated.h"

/**
 * Data table row for Tetris pieces.
 */
USTRUCT(BlueprintType)
struct FPieceData : public FTableRowBase
{
	GENERATED_BODY()
	
	FPieceData();
	FPieceData(const TArray<FIntPoint>& InBody, const FVector2D& InRotationOrigin);

	/* The coordinates that make the body of the piece.*/
	UPROPERTY(EditAnywhere, Category = "Piece Data")
	TArray<FIntPoint> Body;

	/* The origin of rotation. Origins between cells are given by a number between the cell coordinates.*/
	UPROPERTY(EditAnywhere, Category = "Piece Data")
	FVector2D RotationOrigin;

	/* The piece color.*/
	UPROPERTY(EditAnywhere, Category = "Piece Data")
	FLinearColor Color;
};
