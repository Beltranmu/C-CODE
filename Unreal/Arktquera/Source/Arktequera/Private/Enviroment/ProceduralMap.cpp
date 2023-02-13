// Fill out your copyright notice in the Description page of Project Settings.

#include <Enviroment/ProceduralMap.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AProceduralMap::AProceduralMap()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  //PrimaryActorTick.bCanEverTick = true;

  myScene = CreateDefaultSubobject<USceneComponent>("My Root");
  myMesh = CreateDefaultSubobject<UProceduralMeshComponent>("My Mesh");
  waterPlane_ = CreateDefaultSubobject<UStaticMeshComponent>("My Water Plane");

  RootComponent = myScene;
  myMesh->SetupAttachment(RootComponent);
  waterPlane_->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProceduralMap::BeginPlay()
{
  Super::BeginPlay();

}

void AProceduralMap::PostActorCreated()
{

  Super::PostActorCreated();
  /*initVertices();
  generateMap(0, 0, width - 1, height - 1);
  GenerateMesh();*/
  /*initVertices();
  generateMap(0, 0, size - 1, size - 1);*/
  createBoard();
  SelectCasilla();
  GenerateMesh();

}

void AProceduralMap::PostLoad()
{

  Super::PostLoad();
  /*initVertices();
  generateMap(0, 0, width - 1, height - 1);
  GenerateMesh();*/
  /*initVertices();
  generateMap(0, 0, size - 1, size - 1);*/
  createBoard();
  SelectCasilla();
  GenerateMesh();

}

void AProceduralMap::GenerateMesh()
{

  Vertices_.Reset();
  Triangles_.Reset();
  Normals_.Reset();
  Tangents_.Reset();
  UVs_.Reset();
  Colors_.Reset();

  int32 TriangleBuffer = 0;
  FVector triangleShape[3];
  FProcMeshTangent TangentSetup;

  triangleShape[0] = FVector(TriangleSize.X, TriangleSize.Y, TriangleSize.Z);
  triangleShape[1] = FVector(TriangleSize.X, TriangleSize.Y, -TriangleSize.Z);
  triangleShape[2] = FVector(TriangleSize.X, -TriangleSize.Y, -TriangleSize.Z);

  TangentSetup = FProcMeshTangent(0.f, 1.f, 0.f);
  //AddTriangleMesh(triangleShape[0], triangleShape[1], triangleShape[2], TriangleBuffer, TangentSetup);
  for (int i = 0; i < size - 1; ++i) {
    for (int j = 0; j < size - 1; ++j) {

      /*AddTriangleMesh(map[i][j] * 100.f, map[i+1][j] * 100.f, map[i+1][j+1] * 100.f, TriangleBuffer, TangentSetup);
      AddTriangleMesh(map[i][j] * 100.f, map[i+1][j+1] * 100.f, map[i][j+1] * 100.f, TriangleBuffer, TangentSetup);*/

      AddTriangleMesh(board[i][j].vertex_ * 100.f, board[i + 1][j].vertex_ * 100.f, board[i + 1][j + 1].vertex_ * 100.f, TriangleBuffer, TangentSetup);
      AddTriangleMesh(board[i][j].vertex_ * 100.f, board[i + 1][j + 1].vertex_ * 100.f, board[i][j + 1].vertex_ * 100.f, TriangleBuffer, TangentSetup);

      /*AddTriangleMesh(board[i][j].vertex_ * 100.f, board[i + 1][j].vertex_ * 100.f, board[i + 1][j + 1].vertex_ * 100.f, TriangleBuffer, TangentSetup);
      AddTriangleMesh(board[i+1][j+1].vertex_ * 100.f, board[i][j + 1].vertex_ * 100.f, board[i][j].vertex_ * 100.f, TriangleBuffer, TangentSetup);*/

    }
  }

  myMesh->CreateMeshSection_LinearColor(0, Vertices_, Triangles_, Normals_, UVs_, Colors_, Tangents_, true);

}

void AProceduralMap::AddTriangleMesh(FVector topLeft, FVector bottomLeft, FVector bottomRight, int32 & TriIndex, FProcMeshTangent Tangent)
{

  int32 point1 = TriIndex++;
  int32 point2 = TriIndex++;
  int32 point3 = TriIndex++;

  Vertices_.Add(topLeft);
  Vertices_.Add(bottomLeft);
  Vertices_.Add(bottomRight);

  Triangles_.Add(point1);
  Triangles_.Add(point2);
  Triangles_.Add(point3);

  //FVector normal = FVector::CrossProduct(topLeft, bottomRight).GetSafeNormal();
  //FVector normal = FVector::CrossProduct(bottomRight, topLeft).GetSafeNormal();

  FVector v1 = topLeft - bottomLeft;
  FVector v2 = bottomRight - bottomLeft;
  FVector normal = FVector::CrossProduct(v1, v2).GetSafeNormal();

  for (int i = 0; i < 3; ++i) {

    Normals_.Add(normal);
    Tangents_.Add(Tangent);
    Colors_.Add(FLinearColor::Red);

  }

  /*UVs_.Add(FVector2D(0.f, 1.f));
  UVs_.Add(FVector2D(0.f, 0.f));
  UVs_.Add(FVector2D(1.f, 0.f));*/

}

void AProceduralMap::initVertices()
{

  /*for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j){
      map[i][j].X = (float)j / width;
      map[i][j].Y = (float)i / height;
    }
  }*/

  map[0][0].Z = 0.05f;
  map[0][width - 1].Z = 0.05f;
  map[height - 1][0].Z = 0.05f;
  map[height - 1][width - 1].Z = 0.05f;

}

float AProceduralMap::generateNoise()
{
  float tmpRand = rand() % 5;

  return tmpRand / 100.f;
}

float AProceduralMap::getCenter(float a, float b, float c, float d)
{
  return (a + b + c + d) / 4.f;
}

float AProceduralMap::getMiddle(float a, float b)
{
  return (a + b) / 2.f;
}

void AProceduralMap::generateMap(int minX, int minY, int maxX, int maxY)
{

  int tmpWidth;
  int tmpHeight;

  int middleX;
  int middleY;

  tmpWidth = maxX - minX;
  tmpHeight = maxY - minY;

  middleX = tmpWidth / 2 + minX;
  middleY = tmpHeight / 2 + minY;

  if (tmpWidth / 2 >= 1 && tmpHeight / 2 >= 1) {

    board[middleY][middleX].vertex_.Z = generateNoise() * getCenter(board[minY][minX].vertex_.Z, board[minY][maxX].vertex_.Z,
      board[maxY][minX].vertex_.Z, board[maxY][maxX].vertex_.Z);

    board[minY][middleX].vertex_.Z = 0.2 * generateNoise() * getMiddle(board[minY][minX].vertex_.Z, board[minY][maxX].vertex_.Z);
    board[maxY][middleX].vertex_.Z = 0.2 * generateNoise() * getMiddle(board[maxY][minX].vertex_.Z, board[maxY][maxX].vertex_.Z);

    board[middleY][minX].vertex_.Z = 0.2 * generateNoise() * getMiddle(board[minY][minX].vertex_.Z, board[maxY][minX].vertex_.Z);
    board[middleY][maxX].vertex_.Z = 0.2 * generateNoise() * getMiddle(board[minY][minX].vertex_.Z, board[maxY][maxX].vertex_.Z);

    generateMap(middleX, minY, maxX, middleY);
    generateMap(middleX, middleY, maxX, maxY);
    generateMap(minX, minY, middleX, middleY);
    generateMap(minX, middleY, middleX, maxY);

  }

}

void AProceduralMap::createBoard()
{

  for (int r = 0; r < kNumRows; ++r) {

    for (int c = 0; c < kNumCols; ++c) {

      board[r][c].vertex_.Z = 0.f;
      board[r][c].type_ = 0;

      if (r == 0 || r == 1 || c == 0 || c == 1 || r == size - 1 || r == size - 2 || c == size - 1 || c == size - 2) {
        board[r][c].state_ = 0;
      }
      else {

        if (rand() % 100 <= concentration * 100) {

          board[r][c].state_ = (rand() % 3) + 1;
          board[r][c].vertex_.Z = board[r][c].state_ * 0.1;

        }
        else {

          board[r][c].state_ = 0;

        }
      }

    }
  }

}

unsigned char AProceduralMap::CheckSingleNeighbour(unsigned char pos, const signed char desp)
{

  unsigned char resul = pos + desp;

  resul %= kNumCols;

  return resul;

}

unsigned char AProceduralMap::MatrixNxM(const FSMap tmpBoard[size][size], const unsigned char row, const unsigned char col, const unsigned char n, const unsigned char m, const unsigned char tmpState)
{

  unsigned char neighbours = 0;

  int start_row = -1 * (n / 2);
  int end_row = n / 2 - 1 + n % 2;

  int start_col = -1 * (m / 2);
  int end_col = m / 2 - 1 + m % 2;

  for (int i = start_row; i <= end_row; ++i) {

    for (int j = start_col; j <= end_col; ++j) {

      if (i != 0 || j != 0) {

        if (tmpBoard[CheckSingleNeighbour(row, i)][CheckSingleNeighbour(col, j)].state_ == tmpState) { ++neighbours; }

      }

    }

  }

  return neighbours;

}

unsigned char AProceduralMap::CheckNeighbours(const FSMap tmpBoard[size][size], const unsigned char row, const unsigned char col, const unsigned char n, const unsigned char m, const unsigned char tmpState)
{

  return MatrixNxM(tmpBoard, row, col, n, m, tmpState);

}

void AProceduralMap::SwapCells(unsigned char row1, unsigned char col1, unsigned char state1, unsigned char row2, unsigned char col2, unsigned char state2)
{

  board[row1][col1].state_ = state2;
  board[row2][col2].state_ = state1;

}

signed char AProceduralMap::TemporalSwap(unsigned char row1, unsigned char col1, unsigned char state1, unsigned char row2, unsigned char col2, unsigned char state2)
{

  signed char tmp_gain;

  SwapCells(row1, col1, state1, row2, col2, state2);

  tmp_gain = CheckNeighbours(board, row2, col2, 3, 3, board[row2][col2].state_);

  SwapCells(row1, col1, state2, row2, col2, state1);

  return tmp_gain;

}

signed char AProceduralMap::CalculateGain(unsigned char row1, unsigned char col1, unsigned char row2, unsigned char col2)
{

  signed char orig_gain;
  signed char new_gain;

  orig_gain = CheckNeighbours(board, row2, col2, 3, 3, board[row2][col2].state_);

  new_gain = TemporalSwap(row2, col2, board[row2][col2].state_,
    row1, col1, board[row1][col1].state_);

  return new_gain - orig_gain;

}

void AProceduralMap::EraseTile(FSMap tmpBoard[size][size], unsigned char row, unsigned char col, unsigned char tmpState)
{

  if (CheckNeighbours(tmpBoard, row, col, 3, 3, tmpState) <= 3 || CheckNeighbours(tmpBoard, row, col, 3, 3, tmpState) == 0) {

    tmpBoard[row][col].state_ = 0;
    tmpBoard[row][col].type_ = 0;

  }

}

void AProceduralMap::PickCell(unsigned char original_row, unsigned char original_col, unsigned char orig_gain)
{

  signed char horizontal_movement;
  signed char vertical_movement;

  unsigned char next_row;
  unsigned char next_col;

  signed char new_gain;
  signed char new_gain2;

  do {

    horizontal_movement = rand() % (search_range + 1);

    if (search_range - horizontal_movement == 0) {

      vertical_movement = 0;

    }
    else {

      vertical_movement = rand() % (search_range - horizontal_movement);

    }

    if (rand() % 2 == 0) {

      horizontal_movement *= -1;

    }

    if (rand() % 2 == 0) {

      vertical_movement *= -1;

    }

    next_row = CheckSingleNeighbour(original_row, vertical_movement);
    next_col = CheckSingleNeighbour(original_col, horizontal_movement);

  } while (horizontal_movement == 0 && vertical_movement == 0 ||
    (next_row == 0 || next_col == 0 || next_row == size - 1 || next_col == size - 1 ||
      next_row == 1 || next_col == 1 || next_row == size - 2 || next_col == size - 2));

  if (8 != CheckNeighbours(board, next_row, next_col, 3, 3, board[next_row][next_col].state_) &&
    board[next_row][next_col].state_ != board[original_row][original_col].state_) {

    // Gain in the new cell position
    new_gain = TemporalSwap(original_row, original_col, board[original_row][original_col].state_,
      next_row, next_col, board[next_row][next_col].state_);

    new_gain -= orig_gain;

    if (0 == board[next_row][next_col].state_) {

      if (new_gain >= 0 || (new_gain == -1 && rand() % chance_to_move == 21)) {

        SwapCells(original_row, original_col, board[original_row][original_col].state_,
          next_row, next_col,
          board[next_row][next_col].state_);

      }

    }
    else {

      new_gain2 = CalculateGain(original_row, original_col, next_row, next_col);

      new_gain += new_gain2;

      if (new_gain >= 0 || ((new_gain == -1 || new_gain == -2) && rand() % chance_to_move == 21)) {

        SwapCells(original_row, original_col, board[original_row][original_col].state_,
          next_row, next_col,
          board[next_row][next_col].state_);

      }

    }

  }

}

void AProceduralMap::SetHeights(int biome, float & heightIn)
{

  float ricardo_noise = ((rand() % 101) - 50) / 10000.0f;

  switch (biome)
  {

  case 0:

    heightIn = ricardo_noise;

    break;

  case 1:

    heightIn = -0.1f + ricardo_noise *10;

    break;

  case 2:

    heightIn = 0.1f + ricardo_noise * 10;

    break;

  case 3:

    heightIn = 0.2f + ricardo_noise * 10;

    break;

  }

}

void AProceduralMap::EditHeights(float & heightIn, int row, int col)
{

  float tmpHeight = 0.f;
  int neighbours = CheckNeighbours(board, row, col, 3, 3, board[row][col].state_);

  if (8 == neighbours && board[row][col].state_ != 0) {

    tmpHeight += board[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, -1)].vertex_.Z;
    tmpHeight += board[CheckSingleNeighbour(row, -1)][col].vertex_.Z;
    tmpHeight += board[CheckSingleNeighbour(row, -1)][CheckSingleNeighbour(col, 1)].vertex_.Z;
    tmpHeight += board[row][CheckSingleNeighbour(col, -1)].vertex_.Z;
    tmpHeight += board[row][CheckSingleNeighbour(col, 1)].vertex_.Z;
    tmpHeight += board[CheckSingleNeighbour(row, 1)][CheckSingleNeighbour(col, -1)].vertex_.Z;
    tmpHeight += board[CheckSingleNeighbour(row, 1)][col].vertex_.Z;
    tmpHeight += board[CheckSingleNeighbour(row, 1)][CheckSingleNeighbour(col, 1)].vertex_.Z;

    tmpHeight += heightIn;
    tmpHeight /= 7.f;

    heightIn = tmpHeight;

  }

}

void AProceduralMap::SelectCasilla()
{

  unsigned int max_repeats = size * size;
  unsigned char rand_row;
  unsigned char rand_col;
  unsigned char gain;
  unsigned char max_pieces = 7;
  char pieces = 7;

  while (repeats < max_repeats * 1024) {

    rand_row = 2 + rand() % (kNumRows - 2);
    rand_col = 2 + rand() % (kNumCols - 2);

    gain = CheckNeighbours(board, rand_row, rand_col, 3, 3, board[rand_row][rand_col].state_);

    if (gain != 8 && board[rand_row][rand_col].state_ != 0) {

      PickCell(rand_row, rand_col, gain);

    }

    ++repeats;

  }

  for (int n = 0; n < 2; ++n) {

    for (int i = 0; i < size; ++i) {

      for (int j = 0; j < size; ++j) {

        EraseTile(board, i, j, board[i][j].state_);

      }

    }

  }

  for (int i = 0; i < kNumCols; ++i) {

    for (int j = 0; j < kNumRows; ++j) {

      SetHeights(board[i][j].state_, board[i][j].vertex_.Z);

    }

  }

  for (int n = 0; n < 2; ++n) {

    for (int i = 0; i < kNumCols; ++i) {

      for (int j = 0; j < kNumRows; ++j) {

        EditHeights(board[i][j].vertex_.Z, i, j);

      }

    }

  }

  for (int i = 0; i < kNumCols; ++i) {

    for (int j = 0; j < kNumRows; ++j) {

      board[i][j].vertex_.X = (float)j / size;
      board[i][j].vertex_.Y = (float)i / size;

    }

  }


}