// Copyright (C) 2024 Peter Carsten Collins


#include "InternalBoard.h"
#include "Piece.h"

int32 UInternalBoard::GetWidth() const
{
	return Grid.Num();
}

int32 UInternalBoard::GetHeight() const
{
	return Grid.IsEmpty() ? 0 : Grid[0].Num();
}

TArray<FIntPoint> UInternalBoard::GetCoordinates() const
{
	TArray<FIntPoint> Coordinates;
	for (int32 Row = 0; Row < GetHeight(); ++Row)
	{
		for (int32 Col = 0; Col < GetWidth(); ++Col)
		{
			Coordinates.Add({ Col, Row });
		}
	}
	return Coordinates;
}

TArray<FIntPoint> UInternalBoard::GetRowCoordinates(int32 Row) const
{
	TArray<FIntPoint> Coordinates;
	for (int32 Col = 0; Col < GetWidth(); ++Col)
	{
		Coordinates.Add({ Col, Row });
	}
	return Coordinates;
}

bool UInternalBoard::IsOccupied(const FIntPoint& Coordinate) const
{
	return Grid[Coordinate.X][Coordinate.Y];
}

EPlaceResult UInternalBoard::Place(const UPiece* Piece, const FIntPoint& Coordinate)
{
	/* Check that the skirt hasn't passed the bottom of the playfield or collided with an occupied cell.*/
	for (const FIntPoint& SkirtPointInPieceSpace : Piece->Skirt)
	{
		FIntPoint SkirtPointInBoardSpace = SkirtPointInPieceSpace + Coordinate;

		if (SkirtPointInBoardSpace.X < 0 || SkirtPointInBoardSpace.X >= GetWidth() || SkirtPointInBoardSpace.Y < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Piece body has collided with the board boundary. SkirtPointInBoardSpace: (%d,%d) Early exit..."), SkirtPointInBoardSpace.X, SkirtPointInBoardSpace.Y);
			return EPlaceResult::BAD;
		}
		else if (IsOccupied(SkirtPointInBoardSpace))
		{
			UE_LOG(LogTemp, Warning, TEXT("Piece has collided with another. Early exit..."))
			return EPlaceResult::BAD;
		}
	}

	/* Place the points that comprise the piece's body onto the board.*/
	const TArray<FIntPoint> Body = Piece->Body;
	for (const FIntPoint& BodyPointInPieceSpace : Body)
	{
		/* Update the point*/
		FIntPoint BodyPointInBoardSpace = BodyPointInPieceSpace + Coordinate;
		Grid[BodyPointInBoardSpace.X][BodyPointInBoardSpace.Y] = true;
	}
	return EPlaceResult::OK;
}

void UInternalBoard::Collapse()
{
	/* Track the number of cleared rows.*/
	int32 NumEmpty = 0;

	/* Iterate from the bottom row to the top row.*/
	for (int32 WorkingRow = 0; WorkingRow < GetHeight(); ++WorkingRow)
	{
		/* Count the number of occupied cells in the current row.*/
		int32 NumOccupied = 0;
		for (int32 Col = 0; Col < GetWidth(); ++Col)
		{
			if (IsOccupied({ Col, WorkingRow }))
			{
				++NumOccupied;
			}
		}

		/* If the current row is empty, increase the counter of empty rows.*/
		if (NumOccupied == 0)
		{
			++NumEmpty;
		}
		else if (NumEmpty > 0)
		{
			/* If there are empty rows below, move the current row down. */
			for (int32 Col = 0; Col < GetWidth(); ++Col)
			{
				Grid[Col][WorkingRow - NumEmpty] = Grid[Col][WorkingRow];
				Grid[Col][WorkingRow] = false;
			}
		}
	}

	/* Clear the top NumEmpty rows which are now effectively empty.*/
	for (int32 ClearRow = GetHeight() - NumEmpty; ClearRow < GetHeight(); ++ClearRow)
	{
		for (int32 Col = 0; Col < GetWidth(); ++Col)
		{
			Grid[Col][ClearRow] = false;
		}
	}
}

bool UInternalBoard::ClearRows(TArray<int32>& ClearedRows)
{
	for (int32 Row = 0; Row < GetHeight(); ++Row)
	{
		if (IsRowFull(Row))
		{
			EmptyRow(Row);
			ClearedRows.Add(Row);
		}
	}
	return !ClearedRows.IsEmpty();
}

bool UInternalBoard::IsRowFull(int32 Row) const
{
	for (int32 Col = 0; Col < GetWidth(); ++Col)
	{
		if (!IsOccupied({Col, Row}))
		{
			return false;
		}
	}
	return true;
}

void UInternalBoard::EmptyRow(int32 Row)
{
	for (int32 Col = 0; Col < GetWidth(); ++Col)
	{
		Grid[Col][Row] = false;
	}
}

UInternalBoard* UInternalBoard::NewInternalBoard(int BoardWidth, int BoardHeight)
{
	UInternalBoard* Board = NewObject<UInternalBoard>();
	Board->Initialize(BoardWidth, BoardHeight);
	Board->PreviousGrid = Board->Grid;
	return Board;
}

void UInternalBoard::Initialize(int BoardWidth, int BoardHeight)
{
	if (!this){return;} /* Necessary to avoid crash in UE from static NewInternalBoard*/

	/* Empty the grid and fill it with empty columns.*/
	Grid.Empty();
	for (int i = 0; i < BoardWidth; ++i)
	{
		TArray<bool> Column;
		Column.Init(false, BoardHeight);
		Grid.Add(Column);
	}
}

void UInternalBoard::Undo()
{
	Grid = PreviousGrid;
}

void UInternalBoard::Commit()
{
	PreviousGrid = Grid;
}

int32 UInternalBoard::GetStackHeight() const
{
	int32 MaxStackHeight = 0;
	int32 Width = GetWidth();

	for (int32 i = 0; i < Width; ++i)
	{
		int32 ColStackHeight = GetStackHeight(i);
		if (ColStackHeight > MaxStackHeight)
		{
			MaxStackHeight = ColStackHeight;
		}
	}
	return MaxStackHeight;
}

int32 UInternalBoard::GetStackHeight(int32 Column) const
{
	int32 Height = GetHeight();

	/* Start from the highest possible row */
	for (int32 i = Height - 1; i >= 0; --i)
	{
		if (IsOccupied(FIntPoint(Column, i)))
		{
			return i + 1; 
		}
	}
	return 0;
}
