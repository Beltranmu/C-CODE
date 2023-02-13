// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMap.generated.h"

USTRUCT()
struct FSMap {

  GENERATED_BODY()

  int state_;
  int type_;
  FVector vertex_;

};

class UStaticMeshComponent;

//class UProceduralMeshComponent;
UCLASS()
class ARKTEQUERA_API AProceduralMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMap();
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
    FVector TriangleSize = FVector(100.f, 100.f, 100.f);

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Water Plane")
    UStaticMeshComponent* waterPlane_;

  // First Algorithm
  static const int width = 257;
  static const int height = 257;

  // Second Algorithm
  static const unsigned char size = 64;
  static const unsigned char kNumRows = size;
  static const unsigned char kNumCols = size;
  static const unsigned char search_range = (size / 4) * 3; // Search Range
  const float concentration = 0.50f;
  static const int chance_to_move = 721;
  static const unsigned char view_size = 128;
  unsigned int repeats = 0;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Scene")
    USceneComponent* myScene;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Scene")
    UProceduralMeshComponent* myMesh;

  virtual void PostActorCreated() override;
  virtual void PostLoad() override;

  void GenerateMesh();

public:

  // Map
  FVector map[height][width];

  TArray<FVector> Vertices_;
  TArray<int32> Triangles_;
  TArray<FVector> Normals_;
  TArray<FProcMeshTangent> Tangents_;
  TArray<FVector2D> UVs_;
  TArray<FLinearColor> Colors_;

  // Second Algorithm
  FSMap board[kNumRows][kNumCols];

  // Functions
  void createBoard();
  unsigned char CheckSingleNeighbour(unsigned char pos, const signed char desp);
  unsigned char MatrixNxM(const FSMap tmpBoard[size][size], const unsigned char row, const unsigned char col, const unsigned char n, const unsigned char m, const unsigned char tmpState);
  unsigned char CheckNeighbours(const FSMap tmpBoard[size][size], const unsigned char row, const unsigned char col, const unsigned char n, const unsigned char m, const unsigned char tmpState);
  void SwapCells(unsigned char row1, unsigned char col1, unsigned char state1, unsigned char row2, unsigned char col2, unsigned char state2);
  signed char TemporalSwap(unsigned char row1, unsigned char col1, unsigned char state1, unsigned char row2, unsigned char col2, unsigned char state2);
  signed char CalculateGain(unsigned char row1, unsigned char col1, unsigned char row2, unsigned char col2);
  void EraseTile(FSMap tmpBoard[size][size], unsigned char row, unsigned char col, unsigned char tmpState);
  void PickCell(unsigned char original_row, unsigned char original_col, unsigned char orig_gain);
  void SetHeights(int biome, float& height);
  void EditHeights(float& height, int row, int col);
  void SelectCasilla();

  // First Algorithm
  void AddTriangleMesh(FVector topLeft, FVector bottomLeft, FVector bottomRight, int32& TriIndex, FProcMeshTangent Tangent);
  void initVertices();
  float generateNoise();
  float getCenter(float a, float b, float c, float d);
  float getMiddle(float a, float b);
  void generateMap(int minX, int minY, int maxX, int maxY);

};