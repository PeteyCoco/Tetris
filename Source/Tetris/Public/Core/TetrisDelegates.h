// Copyright (C) 2024 Peter Carsten Collins

#pragma once

#include "CoreMinimal.h"

/* Delegate called when rows are filled on a board.*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRowsFilledSignature, TArray<int32> /* RowIDs */);