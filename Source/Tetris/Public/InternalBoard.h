// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Core/TetrisDelegates.h"
#include "InternalBoard.generated.h"

/* The result of an attempted piece placement.*/
UENUM()
enum EPlaceResult
{
	OK	UMETA(DisplayName = "Ok"),
	BAD	UMETA(DisplayName = "Bad"),
};

/**
 * The internal representation of the Tetris board.
 */
UCLASS()
class TETRIS_API UInternalBoard : public UObject
{
	GENERATED_BODY()

public: 
	/* Create a new board.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	static UInternalBoard* NewInternalBoard(int BoardWidth, int BoardHeight);

	/* Get the width of the grid.*/
	int32 GetWidth() const;

	/* Get the height of the grid.*/
	int32 GetHeight() const;

	/* Get all of the coordinates of the grid.*/
	TArray<FIntPoint> GetCoordinates() const;

	/* Get all of the coordinates in a row.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	TArray<FIntPoint> GetRowCoordinates(int32 Row) const;

	/* Return true if the given cell is occupied.*/
	bool IsOccupied(const FIntPoint& Coordinate) const;

	/* Query a placement of the given piece at the location.*/
	EPlaceResult Place(const class UPiece* Piece, const FIntPoint& Coordinate);

	/* Fill in any cleared rows.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	void Collapse();

	/* Clear filled rows and return their IDs.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	bool ClearRows(TArray<int32>& ClearedRows);

	/* Return true if the given row is full.*/
	bool IsRowFull(int32 Row) const;

	/* Empty the given row.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	void EmptyRow(int32 Row);
	
	/* Initialize an empty grid.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	void Initialize(int BoardWidth, int BoardHeight);

	/* Undo the grid to its previous state.*/
	void Undo();

	/* Commit the current board state as the backup.*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	void Commit();

	/* Get the height of the stack (i.e. the pieces in the well)*/
	UFUNCTION(BlueprintCallable, Category = "Board")
	int32 GetStackHeight() const;

	/* Delegate broadcast when rows are filled after committing a board.*/
	FOnRowsFilledSignature OnRowsFilled;

private:
	/* The occupancy state of the board*/
	TArray<TArray<bool>> Grid;

	/* The previous state of the board.*/
	TArray<TArray<bool>> PreviousGrid;

	/* Get the stack height of the column.*/
	int32 GetStackHeight(int32 Column) const;
};
