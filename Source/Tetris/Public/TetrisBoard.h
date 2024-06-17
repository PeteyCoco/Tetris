// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"
#include "Core/TetrisDelegates.h"
#include "GameFramework/Actor.h"
#include "TetrisBoard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRowsClearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLinesClearedSignature); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLockCompleteSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverSignature);

/* Board actions.*/
UENUM(BlueprintType)
enum EAction
{
	LEFT,
	RIGHT,
	DOWN,
	ROTATE_R,
	ROTATE_L,
};

/*
* The in-game representation of the Tetris board.
* 
* Responsible for rendering and modifying internal board state.
*/
UCLASS(Blueprintable)
class TETRIS_API ATetrisBoard : public AActor
{
	GENERATED_BODY()
protected:
	/* Static meshes for the blocks. Assumed to have unit extent.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board | Blocks")
	UInstancedStaticMeshComponent* BlockMesh;

	/* Static mesh for the background. Assumed to have unit extent.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board | Background")
	UStaticMeshComponent* BackgroundMesh;

	/* The queue of tetris pieces.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board | Blocks")
	class UPieceQueue* PieceQueue;
	
	/* The board HUD.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board | HUD")

	class UWidgetComponent* LineCounter;

public:	
	ATetrisBoard();

	/* Reset the board to an empty state.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void Reset();

	/* Start the game.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void StartGame();

	/* Update the board according to the given action.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void Update(EAction Action);

	/* Draw the board to reflect the internal board data.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void Draw();

	/* Stop board updates.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board | Timer")
	void StopPlay();

	/* Resume board updates.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board | Timer")
	void ResumePlay();

	/* Handler for updating the board on tick.*/
	UFUNCTION()
	void HandleTick();

	/* The elapsed time for this game.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Tetris Board | Timer")
	int32 ElapsedTime;

	/* Handler for game timer tick logic.*/
	UFUNCTION()
	void HandleGameTimerTick();

	/* Add a piece to the top of the board.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void AddPiece();

	/* Logic executed when a piece has been locked.*/
	UFUNCTION()
	void HandleOnPieceLocked();

	/* Logic executed for clearing rows.*/
	UFUNCTION()
	void ClearRows();

	/* Collapse empty rows in the board.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void Collapse();

	/* Check if the game is over.*/
	bool IsGameOver() const;

	/* Destroy blocks in the given coordinates.*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DestroyBlocks(const TArray<int32>& Rows);

	/* The number of lines cleared.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tetris Board")
	int32 LinesCleared{ 0 };

	/* Get the score.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	int32 GetScore() const;

	/* Get the current level.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	int32 GetBoardLevel() const;

	/* Get the time between ticks.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	float GetTickDelta() const;

	/* Get the number of line clears needed for the next level.*/
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	int32 GetLinesNextLevel() const;

	/* Delegates.*/
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tetris Board")
	FOnLinesClearedSignature OnLinesCleared;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tetris Board")
	FOnLockCompleteSignature OnLockComplete;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Tetris Board")
	FOnGameOverSignature OnGameOver;

	/* Event handler to run after piece locking is complete.*/
	UFUNCTION()
	void HandleOnLockComplete();

protected:
	/* The internal board data.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tetris Board")
	class UInternalBoard* InternalBoard;

	/* Flag to enable debug visuals during gameplay.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	bool DebugMode{ false };

	/* The time in seconds between board updates.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board | Timer")
	float TickSpeed{ 1.f };

	/* The timer for board updates.*/
	FTimerHandle UpdateTimer;

	/* The timer for game time.*/
	UPROPERTY(VisibleInstanceOnly, Category = "Tetris Board")
	FTimerHandle GameTimer;

	/* The width of the board in blocks.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	int32 BoardWidth{ 10 };

	/* The height of the board playspace in blocks.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	int32 BoardHeight{ 20 };

	/* The extra height for spawning pieces.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	int32 BoardTopSpace{ 4 };

	/* The width of the blocks in the world.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	float BlockWidth{ 50.f };

	/* The width of the block mesh in unreal units. Used for normalizing the dimensions.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	float BlockMeshWidth;

	/* Compute the transform of the background mesh.*/
	FTransform GetBackgroundTransform() const;

	/* Compute the transform of the block at the given coordinate.*/
	FTransform GetBlockTransform(const FIntPoint& InCoordinate) const;

	/* The currently active piece.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Current Piece")
	const class UPiece* CurrentPiece;
	
	/* The position of the origin of the current piece's local frame in board space (can be negative).*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Current Piece")
	FIntPoint CurrentCoordinate;

	/* The score.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Tetris Board")
	int32 Score {0};

	/* Compute the new coordinate according to the given action.*/
	void ComputeNewCoordinate(FIntPoint& NewCoordinate, EAction Action) const;

	/* Update the score given the IDs of the cleared lines.*/
	void UpdateScore(int32 NumLines);

	/*** AActor overrides ***/
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void Tick(float DeltaSeconds) override;
};
