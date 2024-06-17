// Copyright (C) 2024 Peter Carsten Collins


#include "PieceFactory.h"

UPiece* UPieceFactory::Build(const FPieceData& PieceData)
{
	// Array to hold the pieces
	UPiece* Pieces[4];

	// Create the initial piece (0-rotation)
	Pieces[0] = NewObject<UPiece>();
	Pieces[0]->Body = PieceData.Body;
	Pieces[0]->Skirt = CalculateSkirt(PieceData.Body);

	// Create the rotated pieces
	for (int i = 1; i < 4; ++i)
	{
		Pieces[i] = NewObject<UPiece>();
		Pieces[i]->Body = CalculateRotation(Pieces[i - 1]->Body, PieceData.RotationOrigin);
		Pieces[i]->Skirt = CalculateSkirt(Pieces[i]->Body);
	}

	// Connect the pieces
	for (int i = 0; i < 4; ++i)
	{
		Pieces[i]->Next = Pieces[(i + 1) % 4];
		Pieces[i]->Prev = Pieces[(i + 3) % 4];
	}

	return Pieces[0];
}

TArray<FIntPoint> UPieceFactory::CalculateRotation(const TArray<FIntPoint>& Points, const FVector2D& RotationOrigin) const
{
	TArray<FIntPoint> Rotated;
	Rotated.Reserve(Points.Num());

	for (const FIntPoint& Point : Points)
	{
		// Convert to float and center about the origin
		FVector2D RotatedFloatPoint{ Point.X - RotationOrigin.X, Point.Y - RotationOrigin.Y };

		// Perform R-rotation (clockwise 90 degrees)
		RotatedFloatPoint = { RotatedFloatPoint.Y, -RotatedFloatPoint.X };

		// Un-center the points
		RotatedFloatPoint = { RotatedFloatPoint.X + RotationOrigin.X, RotatedFloatPoint.Y + RotationOrigin.Y };

		// Convert back to FIntPoint
		FIntPoint RotatedIntPoint{ FMath::RoundToInt32(RotatedFloatPoint.X), FMath::RoundToInt32(RotatedFloatPoint.Y) };
		Rotated.Add(RotatedIntPoint);
	}
	return Rotated;
}

TArray<FIntPoint> UPieceFactory::CalculateSkirt(const TArray<FIntPoint>& Points) const
{
	/* Mapping of column index to skirt points.*/
	TMap<int, FIntPoint> SkirtMap;

	/* Track the points with the smallest Y-value in each column.*/
	for (const FIntPoint& Point : Points)
	{
		if (!SkirtMap.Contains(Point.X))
		{
			SkirtMap.Add(Point.X, Point);
		}
		else if (SkirtMap[Point.X].Y > Point.Y)
		{
			SkirtMap[Point.X] = Point;
		}
	}

	/* Extract the values from the skirt map into an array.*/
	TArray<FIntPoint> Skirt;
	SkirtMap.GenerateValueArray(Skirt);
	return Skirt;
}

