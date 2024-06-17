// Copyright (C) 2024 Peter Carsten Collins


#include "Core/TetrisGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TetrisBoard.h"

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();

	/* Get reference to the game board.*/
	Board = Cast<ATetrisBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), ATetrisBoard::StaticClass()));
	if (!Board)
	{
		UE_LOG(LogTemp, Error, TEXT("Error in ATetrisGameMode: ATetrisBoard not found in world. Aborting."));
		return;
	}
}