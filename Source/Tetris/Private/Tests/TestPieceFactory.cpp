#include "CoreMinimal.h"
#include "PieceFactory.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPieceFactoryTests, "Tetris.Piece Factory", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

// Custom comparator for FIntPoint
bool FIntPointComparator(const FIntPoint& A, const FIntPoint& B)
{
    if (A.X != B.X)
    {
        return A.X < B.X;
    }
    return A.Y < B.Y;
}

// Function to check equality of two TArrays of FIntPoint irrespective of the order
bool AreArraysEqual(const TArray<FIntPoint>& Array1, const TArray<FIntPoint>& Array2)
{
    // Check if the sizes are different
    if (Array1.Num() != Array2.Num())
    {
        return false;
    }

    // Create copies of the arrays to sort them
    TArray<FIntPoint> SortedArray1 = Array1;
    TArray<FIntPoint> SortedArray2 = Array2;

    // Sort both arrays using the custom comparator
    SortedArray1.Sort(FIntPointComparator);
    SortedArray2.Sort(FIntPointComparator);

    // Compare sorted arrays element by element
    for (int32 i = 0; i < SortedArray1.Num(); ++i)
    {
        if (SortedArray1[i] != SortedArray2[i])
        {
            return false;
        }
    }

    // If all elements are equal, the arrays are equal
    return true;
}

bool FPieceFactoryTests::RunTest(const FString& Parameters)
{
	/* The piece factory to use in all tests.*/
	UPieceFactory* PieceFactory = NewObject<UPieceFactory>();

	/* Tests for 'I' piece*/
    {
        FPieceData PieceDataI{ {{0,2},{1,2},{2,2},{3,2}}, {1.5f, 1.5f} };
        UPiece* IPiece0 = PieceFactory->Build(PieceDataI);

        /* 0-rotation tests.*/
        TestTrue("IPiece 0-rotation Body is correct.", AreArraysEqual(IPiece0->Body, { {{0,2},{1,2},{2,2},{3,2}} }));
        TestTrue("IPiece 0-rotation skirt is correct.", AreArraysEqual(IPiece0->Skirt, { {{0,2},{1,2},{2,2},{3,2}} }));

        /* R-rotation tests.*/
        UPiece* IPieceR = IPiece0->Next;
        TestTrue("IPiece R-rotation Body is correct.", AreArraysEqual(IPieceR->Body, { {{2,3},{2,2},{2,1},{2,0}} }));
        TestTrue("IPiece R-rotation skirt is correct.", AreArraysEqual(IPieceR->Skirt, { {2,0} }));

        /* 2-rotation tests.*/
        UPiece* IPiece2 = IPieceR->Next;
        TestTrue("IPiece 2-rotation Body is correct.", AreArraysEqual(IPiece2->Body, { {{0,1},{1,1},{2,1},{3,1}} }));
        TestTrue("IPiece 2-rotation skirt is correct.", AreArraysEqual(IPiece2->Skirt, { {{0,1},{1,1},{2,1},{3,1}} }));

        /* L-rotation tests.*/
        UPiece* IPieceL = IPiece0->Prev;
        TestTrue("IPiece L-rotation Body is correct.", AreArraysEqual(IPieceL->Body, { {{1,3},{1,2},{1,1},{1,0}} }));
        TestTrue("IPiece L-rotation skirt is correct.", AreArraysEqual(IPieceL->Skirt, { {1,0} }));
    }

    /* Tests for 'O' piece*/
    {
        FPieceData PieceDataO{ {{1, 1}, { 1,2 }, { 2,1 }, { 2, 2 }}, {1.5f, 1.5f} };
        UPiece* OPiece0 = PieceFactory->Build(PieceDataO);

        /* 0-rotation tests.*/
        TestTrue("OPiece 0-rotation Body is correct.", AreArraysEqual(OPiece0->Body, {{1, 1}, { 1,2 }, { 2,1 }, { 2, 2 }}));
        TestTrue("OPiece 0-rotation skirt is correct.", AreArraysEqual(OPiece0->Skirt, { {1, 1}, { 2,1 } }));

        /* R-rotation tests.*/
        UPiece* OPieceR = OPiece0->Next;
        TestTrue("OPiece R-rotation Body is correct.", AreArraysEqual(OPieceR->Body, {{1, 1}, { 1,2 }, { 2,1 }, { 2, 2 }}));
        TestTrue("OPiece R-rotation skirt is correct.", AreArraysEqual(OPieceR->Skirt, { {1, 1}, { 2,1 } }));

        /* 2-rotation tests.*/
        UPiece* OPiece2 = OPieceR->Next;
        TestTrue("OPiece 2-rotation Body is correct.", AreArraysEqual(OPiece2->Body, {{1, 1}, { 1,2 }, { 2,1 }, { 2, 2 }}));
        TestTrue("OPiece 2-rotation skirt is correct.", AreArraysEqual(OPiece2->Skirt, { {1, 1}, { 2,1 } }));

        /* L-rotation tests.*/
        UPiece* OPieceL = OPiece0->Prev;
        TestTrue("OPiece L-rotation Body is correct.", AreArraysEqual(OPieceL->Body, {{1, 1}, { 1,2 }, { 2,1 }, { 2, 2 }}));
        TestTrue("OPiece L-rotation skirt is correct.", AreArraysEqual(OPieceL->Skirt, { {1, 1}, { 2,1 } }));
    }

    /* Tests for 'L' piece*/
    {
        FPieceData PieceDataL{ {{0, 2}, { 0,1 }, { 1,1 }, { 2, 1 }}, {1.f, 1.f} };
        UPiece* LPiece0 = PieceFactory->Build(PieceDataL);

        /* 0-rotation tests.*/
        TestTrue("LPiece 0-rotation Body is correct.", AreArraysEqual(LPiece0->Body, {{0,2}, {0,1}, {1,1}, {2, 1 }}));
        TestTrue("LPiece 0-rotation skirt is correct.", AreArraysEqual(LPiece0->Skirt, { {0,1}, {1,1}, {2,1} }));

        /* R-rotation tests.*/
        UPiece* LPieceR = LPiece0->Next;
        TestTrue("LPiece R-rotation Body is correct.", AreArraysEqual(LPieceR->Body, {{1,0}, {1,1}, {1,2}, {2,2}}));
        TestTrue("LPiece R-rotation skirt is correct.", AreArraysEqual(LPieceR->Skirt, { {1,0},{2,2} }));

        /* 2-rotation tests.*/
        UPiece* LPiece2 = LPieceR->Next;
        TestTrue("LPiece 2-rotation Body is correct.", AreArraysEqual(LPiece2->Body, {{0,1}, {1,1}, {2,1}, {2,0}}));
        TestTrue("LPiece 2-rotation skirt is correct.", AreArraysEqual(LPiece2->Skirt, { {0,1}, {1,1}, {2,0} }));

        /* L-rotation tests.*/
        UPiece* LPieceL = LPiece0->Prev;
        TestTrue("LPiece L-rotation Body is correct.", AreArraysEqual(LPieceL->Body, { {0,0},{1,0},{1,1},{1,2} }));
        TestTrue("LPiece L-rotation skirt is correct.", AreArraysEqual(LPieceL->Skirt, { {0,0}, {1,0} }));
    }
	return true;
}

