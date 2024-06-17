// Copyright (C) 2024 Peter Carsten Collins


#include "PieceData.h"

FPieceData::FPieceData() :
	Body{},
	RotationOrigin{ 0.f,0.f }
{}

FPieceData::FPieceData(const TArray<FIntPoint>& InBody, const FVector2D& InRotationOrigin) :
	Body{InBody},
	RotationOrigin{InRotationOrigin}
{}
