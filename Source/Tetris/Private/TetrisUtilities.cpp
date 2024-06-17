// Copyright (C) 2024 Peter Carsten Collins


#include "TetrisUtilities.h"

FString UTetrisUtilities::SecondsToTimeString(int32 InSeconds)
{
	int32 NumMinutes = FMath::Floor(InSeconds / 60);
	int32 NumSeconds = FMath::Floor(InSeconds - NumMinutes * 60);
	return FString::Printf(TEXT("%02d:%02d"), NumMinutes, NumSeconds);
}
