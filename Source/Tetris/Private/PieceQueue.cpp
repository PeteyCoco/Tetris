// Copyright (C) 2024 Peter Carsten Collins


#include "PieceQueue.h"
#include "PieceFactory.h"
#include "Algo/RandomShuffle.h"

UPieceQueue::UPieceQueue()
{}

void UPieceQueue::AddBatch()
{
	/* Add a random permutation of all pieces to the queue.*/
	TArray<UPiece*> ShuffledPieces = Pieces;
	Algo::RandomShuffle(ShuffledPieces);

	for (const UPiece* Piece : ShuffledPieces)
	{
		Queue.Enqueue(Piece);
		++QueueSize;
	}
}

const UPiece* UPieceQueue::Pop()
{
	const UPiece* Result;
	Queue.Dequeue(Result);
	--QueueSize;
	/* Keep the queue filled with at least two batch at all times.*/
	if (QueueSize < 2*Pieces.Num())
	{
		AddBatch();
	}
	return Result;
}

const UPiece* UPieceQueue::Top() const
{
	const UPiece* Result;
	Queue.Peek(Result);
	return Result;
}


void UPieceQueue::BeginPlay()
{
	Super::BeginPlay();

	/* Construct the pieces from the data table.*/
	if (!PieceDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Error in %s: PieceDataTable not set. Aborting..."), __FUNCTION__)
		return;
	}

	/* Construct a piece for each row in the data table.*/
	UPieceFactory* PieceFactory = NewObject<UPieceFactory>();
	TArray<FName> RowNames = PieceDataTable->GetRowNames();
	for (const FName RowName : RowNames)
	{
		FString ContextString;
		FPieceData* Row = PieceDataTable->FindRow<FPieceData>(RowName, ContextString);
		if (Row)
		{
			Pieces.Add(PieceFactory->Build(*Row));
		}
	}

	/* Add a batch of pieces to the queue.*/
	AddBatch();
}
