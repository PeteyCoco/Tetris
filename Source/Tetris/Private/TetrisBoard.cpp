// Copyright (C) 2024 Peter Carsten Collins


#include "TetrisBoard.h"
#include "Components/InstancedStaticMeshComponent.h" 
#include "InternalBoard.h"
#include "PieceFactory.h"
#include "PieceQueue.h"
#include "DrawDebugHelpers.h"
#include "BoardHUD.h"
#include "Components/WidgetComponent.h" 

ATetrisBoard::ATetrisBoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BlockMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Block Mesh"));
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Background Mesh"));
	PieceQueue = CreateDefaultSubobject<UPieceQueue>(TEXT("Piece Queue"));
	LineCounter = CreateDefaultSubobject<UWidgetComponent>(TEXT("Line Counter"));

	/* Attach components.*/
	BlockMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BackgroundMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LineCounter->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	/* Scale background according to board dimensions.*/
	BackgroundMesh->SetWorldTransform(GetBackgroundTransform());

	/* Set this actor to tick every frame.*/
	PrimaryActorTick.bCanEverTick = true;
}

void ATetrisBoard::Reset()
{
	/* Clear the board.*/
	InternalBoard = UInternalBoard::NewInternalBoard(BoardWidth, BoardHeight + BoardTopSpace);
	Draw();

	/* Reset metrics*/
	Score = 0;
	LinesCleared = 0;
	ElapsedTime = 0;

	/* Update the HUD.*/
	if (UBoardHUD* BoardHUD = Cast<UBoardHUD>(LineCounter->GetWidget()))
	{
		BoardHUD->Update(this);
	}
}

void ATetrisBoard::StartGame()
{
	/* Ensure the board is reset.*/
	Reset();

	/* Start the game timer.*/
	GetWorldTimerManager().SetTimer(GameTimer, this, &ATetrisBoard::HandleGameTimerTick, 1.f, true);

	/* Add a piece to the board.*/
	AddPiece();
}

void ATetrisBoard::Update(EAction Action)
{
	if (!InternalBoard){ return; }

	/* Do nothing if there isn't a piece in play.*/
	if (!CurrentPiece) { return; }

	/* Set the new coordinate according to the action.*/
	FIntPoint NewCoordinate;
	ComputeNewCoordinate(NewCoordinate, Action);

	/* Reset the board to the state before the piece was placed.*/
	InternalBoard->Undo();

	/* If the action is a rotation, change the current piece.*/
	if (Action == EAction::ROTATE_L)
	{
		CurrentPiece = CurrentPiece->Prev;
	}
	else if (Action == EAction::ROTATE_R)
	{
		CurrentPiece = CurrentPiece->Next;
	}

	/* Attempt to place the piece at the new location.*/
	EPlaceResult Result = InternalBoard->Place(CurrentPiece, NewCoordinate);

	if (Result == EPlaceResult::BAD && Action == EAction::DOWN)
	{
		/* If the piece could not be placed and the verb was DOWN, start the lock procedure.*/
		HandleOnPieceLocked();
	}
	else if (Result == EPlaceResult::BAD)
	{
		/* Undo rotation.*/
		if (Action == EAction::ROTATE_L)
		{
			CurrentPiece = CurrentPiece->Next;
		}
		else if (Action == EAction::ROTATE_R)
		{
			CurrentPiece = CurrentPiece->Prev;
		}
		/* If the piece could not be placed, do nothing.*/
		InternalBoard->Place(CurrentPiece, CurrentCoordinate);
	}
	else if (Result == EPlaceResult::OK)
	{
		/* If the placement is good, update the current piece coordinate.*/
		CurrentCoordinate = NewCoordinate;
	}
	/* Redraw the board.*/
	Draw();
}

void ATetrisBoard::Draw()
{
	/* Do nothing if the mesh isn't set.*/
	if (!BlockMesh) { return; }

	/* Update every block for now.*/
	BlockMesh->ClearInstances();
	for (int i = 0; i < InternalBoard->GetWidth(); ++i)
	{
		for (int j = 0; j < InternalBoard->GetHeight(); ++j)
		{
			FIntPoint Coordinate = { i,j };
			if (!InternalBoard->IsOccupied(Coordinate)) { continue; }
			BlockMesh->AddInstance(GetBlockTransform(Coordinate));
		}
	}
}

void ATetrisBoard::StopPlay()
{
	/* Stop the timer.*/
	GetWorldTimerManager().PauseTimer(UpdateTimer);
	GetWorldTimerManager().PauseTimer(GameTimer);
}

void ATetrisBoard::ResumePlay()
{
	/* Start the timer.*/
	GetWorldTimerManager().SetTimer(UpdateTimer, this, &ATetrisBoard::HandleTick, GetTickDelta(), true);
	GetWorldTimerManager().UnPauseTimer(GameTimer);
}

FTransform ATetrisBoard::GetBackgroundTransform() const
{
	FTransform Transform = GetActorTransform();
	Transform.SetLocation(- FVector(1.f, 1.f, 0.f) * BlockWidth / 2.f);
	Transform.SetScale3D(FVector(BoardHeight, BoardWidth, 1.f) * BlockWidth);
	return Transform;
}

FTransform ATetrisBoard::GetBlockTransform(const FIntPoint& InCoordinate) const
{
	FVector BlockPosition = FVector(InCoordinate.Y, InCoordinate.X, 0.f) * BlockWidth;
	FVector BlockScale = FVector(1.f, 1.f, 1.f) * BlockWidth;
	FTransform Transform(FRotator::ZeroRotator, BlockPosition, BlockScale);
	return {FRotator::ZeroRotator, BlockPosition, BlockScale};
}

void ATetrisBoard::ComputeNewCoordinate(FIntPoint& NewCoordinate, EAction Action) const
{
	NewCoordinate = CurrentCoordinate;
	switch (Action)
	{
	case EAction::DOWN:
		--NewCoordinate.Y;
		break;
	case EAction::LEFT:
		--NewCoordinate.X;
		break;
	case EAction::RIGHT:
		++NewCoordinate.X;
		break;
	}
}

void ATetrisBoard::UpdateScore(int32 NumLines)
{
	/* Hard code the scoring rules for simplicity.*/
	int32 BoardLevel = GetBoardLevel();
	switch (NumLines)
	{
	case 1: 
		Score += BoardLevel * 100;
		break;
	case 2:
		Score += BoardLevel * 300;
		break;
	case 3:
		Score += BoardLevel * 500;
		break;
	case 4:
		Score += BoardLevel * 800;
		break;
	}
}

void ATetrisBoard::HandleTick()
{
	Update(EAction::DOWN);
}

void ATetrisBoard::HandleGameTimerTick()
{
	if (UBoardHUD* BoardHUD = Cast<UBoardHUD>(LineCounter->GetWidget()))
	{
		++ElapsedTime;
		BoardHUD->Update(this);
	}
}

void ATetrisBoard::AddPiece()
{
	/* Grab the next piece.*/
	CurrentPiece = PieceQueue->Pop();

	/* Add the piece to the top of the internal board.*/
	CurrentCoordinate = { BoardWidth / 2 - 2 , BoardHeight };
	InternalBoard->Place(CurrentPiece, CurrentCoordinate);
	Draw();

	/* Restart the board update timer.*/
	ResumePlay();
}

void ATetrisBoard::HandleOnPieceLocked()
{
	/* Stop board updates.*/
	StopPlay();

	/* Place current piece at the current location.*/
	InternalBoard->Place(CurrentPiece, CurrentCoordinate);
	CurrentPiece = nullptr;

	/* Clear any filled rows.*/
	ClearRows();
}

void ATetrisBoard::ClearRows()
{
	TArray<int32> Lines;
	InternalBoard->ClearRows(Lines);
	int32 NumLines = Lines.Num();
	if (NumLines > 0)
	{
		LinesCleared += NumLines;
		UpdateScore(NumLines);
		Draw();
		OnLinesCleared.Broadcast();

		/* Collapse board after a delay.*/
		FTimerHandle CollapseTimer;
		GetWorldTimerManager().SetTimer(CollapseTimer, this, &ATetrisBoard::Collapse, 1.0f, false);
	}
	else
	{
		/* Lock is complete if there's nothing to clear.*/
		OnLockComplete.Broadcast();
	}
}

void ATetrisBoard::Collapse()
{
	/* Collapse the board and notify that lock procedure is complete.*/
	InternalBoard->Collapse();
	Draw();
	OnLockComplete.Broadcast();
}

bool ATetrisBoard::IsGameOver() const
{
	return InternalBoard->GetStackHeight() > BoardHeight;
}

void ATetrisBoard::BeginPlay()
{
	Super::BeginPlay();
	/* Register delegates*/
	OnLockComplete.AddUniqueDynamic(this, &ATetrisBoard::HandleOnLockComplete);
}

void ATetrisBoard::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	/* Reinitialize internal board.*/
	InternalBoard->Initialize(BoardWidth, BoardHeight);

	/* Scale background according to block size.*/
	BackgroundMesh->SetRelativeTransform(GetBackgroundTransform());
}

void ATetrisBoard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!DebugMode) { return; }

	/* Superimpose internal board state to board.*/
	for (int i = 0; i < InternalBoard->GetWidth(); ++i)
	{
		for (int j = 0; j < InternalBoard->GetHeight(); ++j)
		{
			FIntPoint Coordinate = { i,j };
			FVector BlockCenter = GetActorLocation() + GetActorRotation().RotateVector(GetBlockTransform(Coordinate).GetLocation());
			FColor BlockColor = InternalBoard->IsOccupied(Coordinate) ? FColor::Green : FColor::Black;
			DrawDebugPoint(GetWorld(), BlockCenter, 5, BlockColor, false);
		}
	}
}


int32 ATetrisBoard::GetScore() const
{
	return Score;
}

int32 ATetrisBoard::GetBoardLevel() const
{
	/* Using the Tetris guideline formula.*/
	return FMath::Min( (LinesCleared / 10) + 1, 20);
}

float ATetrisBoard::GetTickDelta() const
{
	/* Using the Tetris guideline formula.*/
	int32 Level = GetBoardLevel();
	return FMath::Pow((0.8 - ((Level - 1) * 0.007)), Level - 1);
}

int32 ATetrisBoard::GetLinesNextLevel() const
{
	/* Using the Tetris guideline formula.*/
	return GetBoardLevel() * 10;
}

void ATetrisBoard::HandleOnLockComplete()
{
	if (IsGameOver())
	{
		OnGameOver.Broadcast();
		return;
	}
	/* Commit changes and continue play.*/
	InternalBoard->Commit();
	AddPiece();
}
