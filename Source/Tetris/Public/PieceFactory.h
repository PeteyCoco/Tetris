// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Piece.h"
#include "PieceData.h"
#include "PieceFactory.generated.h"

/**
 * Factory responsible for constructing Tetris pieces and their rotations from data.
 */
UCLASS()
class TETRIS_API UPieceFactory : public UObject
{
	GENERATED_BODY()
public:
	/* Build the tetris piece according to the piece data.*/
	UFUNCTION()
	UPiece* Build(const FPieceData& PieceData);

protected:
	/* Calculate the R-rotation of the given body points.*/
	TArray<FIntPoint> CalculateRotation(const TArray<FIntPoint>& Points, const FVector2D& RotationOrigin) const;

	/* Calculate the skirt of the given body points.*/
	TArray<FIntPoint> CalculateSkirt(const TArray<FIntPoint>& Points) const;

};
