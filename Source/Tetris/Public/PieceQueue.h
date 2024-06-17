// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PieceQueue.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TETRIS_API UPieceQueue : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPieceQueue();

	/* Add a random batch of pieces into the queue.*/
	UFUNCTION()
	void AddBatch();

	/* Pop the next piece from the queue.*/
	UFUNCTION()
	const UPiece* Pop();

	/* Return a reference to the top piece in the queue.*/
	UFUNCTION()
	const UPiece* Top() const;

protected:
	virtual void BeginPlay() override;

	/* The Tetris pieces.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Tetris Board")
	TArray<class UPiece*> Pieces;

	/* The datatable used to create pieces.*/
	UPROPERTY(EditAnywhere, Category = "Tetris Board")
	UDataTable* PieceDataTable;

	/* The queue of upcoming pieces.*/
	TQueue<const class UPiece*> Queue;

	/* The size of the queue.*/
	int32 QueueSize{ 0 };
};
