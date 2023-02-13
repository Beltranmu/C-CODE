#include "astar.h"
#include "board.h"
// 1. Put the starting square in the open list.
// 2. Repeat until the open list is empty :
// 
//    1. Get the square on the open list which has the lowest score.Let’s call this
//    square S.
//    2. Remove S from the open list and add S to the closed list.
//    3. For each square T in S’s walkable adjacent tiles :
// 
//      1. If T is in the closed list : Ignore it.
//      2. If T is not in the open list : Add it and compute its score.
//      3. If T is already in the open list : Check if the F score is lower when
//         we use the current generated path to get there.If it is, update its
//         score and update its parent as well.


// F = G + H

Astar::Astar(){

  manhattanD = true;
  euclideanD = false;
  chebyshovD = false;
  //allowDiagonals = false;

  pathColors[0] = sf::Color(255,0,0,85);
  pathColors[1] = sf::Color(0,255,0,85);
  pathColors[2] = sf::Color(0,0,255,85);




}

void Astar::calculatePath(Board* board, int initPostition, int endPosition){
  if (initPostition == endPosition)
  {
    printf("Choose another position\n");
  }
  TypeDistanceUsedToCalculatePath currenttype = MANHATTAN_TYPE;
  uint32_t activatedD = 0;
  if (manhattanD) { activatedD++; }  
  if (euclideanD) { activatedD++; currenttype = EUCLIDEAN_TYPE; }
  if (chebyshovD) { activatedD++; currenttype = CHEBYSHOV_TYPE; }
  

  if (activatedD == 0 ) {
    printf("Choose a type to calculate the distance\n");
    return;
  }
  if(activatedD>1){
    printf("More than one distance types are chosen, choose only one, pls :D\n");
    return;
  }
  for(int i = 0; i<(int) currentPaths.size(); ++i){
    bool samePath = currentPaths[i].destination == endPosition;
    samePath = samePath && currentPaths[i].origin == initPostition;
    samePath = samePath && currentPaths[i].type == currenttype;
    //samePath = samePath && currentPaths[i].diagonal == allowDiagonals;
    if(samePath){
      printf("Path already calculated\n");
      return;
    }
  }

  printf("Calculate Path From %d -> %d\n", initPostition, endPosition);

  ACell initCell;
  openList.clear();
  closeList.clear();

  initCell.cellID = initPostition;
  initCell.parentCellID = initPostition;
  initCell.g = 0;
  
  if(manhattanD){ initCell.score = initCell.g + board->manhantanDistance(initPostition, endPosition); }
  if(euclideanD){ initCell.score = initCell.g + board->euclidianDistance(initPostition, endPosition); }
  if(chebyshovD){ initCell.score = initCell.g + board->chebyshovDistance(initPostition, endPosition); }
  uint32_t parentID = initPostition;
  bool pathFound = initPostition == endPosition;
  
  openList.push_back(initCell);
 
  ACell lowestScoreCell;
  uint32_t lowestScoreCellIndex = 0;
  while(!openList.empty() && !pathFound){
    //Get the one with lowest F in the openList
    uint32_t index = 0;
    std::list<ACell>::iterator it = openList.begin();
    float lowestScore = 9999.0f;
    for(it; it != openList.end(); ++it ){
      float score = it->score;
      /*CALCULAR SCORE*/
      if(score < lowestScore){
        lowestScore = score;
        lowestScoreCell = *it;
        lowestScoreCellIndex = index;
        parentID = it->cellID;
      }
      index++;
    }


    //Remove from the open list
    it = openList.begin();
    advance(it, lowestScoreCellIndex);
    openList.erase(it);
    //Add it in the close list
    closeList.push_front(lowestScoreCell);

    //Check Square
    const int numberPoints = 8;
    int numberNeighbourd = 4;
    uint32_t neighbourdCells[numberPoints];
    neighbourdCells[0] = board->north(lowestScoreCell.cellID); //North
    neighbourdCells[1] = board->south(lowestScoreCell.cellID); //South
    neighbourdCells[2] = board->west(lowestScoreCell.cellID);  //West
    neighbourdCells[3] = board->east(lowestScoreCell.cellID);  //East
    
    //if (allowDiagonals) { 
    //  numberNeighbourd = numberPoints; 
    //  neighbourdCells[4] = board->east(neighbourdCells[0]); //North -> East
    //  neighbourdCells[5] = board->west(neighbourdCells[0]); //North -> West
    //  neighbourdCells[6] = board->east(neighbourdCells[1]);  //South -> East
    //  neighbourdCells[7] = board->west(neighbourdCells[1]);  //South -> West
    //}

    for(int j = 0; j< numberNeighbourd && !pathFound; j++){
      if(board->checkUnitMovement(neighbourdCells[j])){
        bool isInCloseList = false;
        bool isInOpenList = false;
        //Search in the closeList
        it = closeList.begin();
        for(it; it != closeList.end() && !isInCloseList; ++it){
          if(it->cellID == neighbourdCells[j]){
            isInCloseList = true;
          }
        }

        if(!isInCloseList){

          for (int q = 0; ((q < (int)openList.size()) && !isInOpenList); ++q) {
            it = openList.begin();
            advance(it, q);
            if (it->cellID == neighbourdCells[j]) {
              isInOpenList = true;
            }
          }
          

         float GScore = lowestScoreCell.g + 1; 
         float HScore = 0;
         
          if (manhattanD) { HScore = initCell.g + board->manhantanDistance(neighbourdCells[j], endPosition); }
          if (euclideanD) { HScore = initCell.g + board->euclidianDistance(neighbourdCells[j], endPosition); }
          if (chebyshovD) { HScore = initCell.g + board->chebyshovDistance(neighbourdCells[j], endPosition); }
          float FScore = GScore + HScore;
          if (!isInOpenList) {
            ACell newCell;
            newCell.cellID = neighbourdCells[j];
           newCell.parentCellID = parentID;
            newCell.g = GScore;
            newCell.score = FScore;
            openList.push_front(newCell);
          }
          //Already in the openList
          else {
            if(FScore < it->score){
              it->score = FScore;
              it->g = GScore;
              it->parentCellID = parentID;
            }
          
          }
          if(HScore == 0){
            pathFound = true;
            ACell newCell;
            newCell.cellID = neighbourdCells[j];
            newCell.parentCellID = parentID;
            newCell.g = GScore;
            newCell.score = FScore;
            closeList.push_front(newCell);
          }
        }
      }
    }
  }

  // Get the path
  if(pathFound){
    std::list<ACell>::iterator it = closeList.begin();
    std::list<ACell> path;
    path.clear();
    //Camino esta en la closeList
    bool endPath = false;
    bool endAdded = false;
    uint32_t endCellID = endPosition;
    //path.push_front(closeList.begin()->cellID);
    for(it; it != closeList.end() && !endPath; ){

      if(it->cellID == it->parentCellID){
        endPath = true;
        path.push_front(*it);
      }

      if(it->cellID == endCellID && !endPath){
       // endAdded = true;
        endCellID = it->parentCellID;
        path.push_front(*it);
        it = closeList.begin();
      }else{
        ++it;
      }

      
    }
    it = path.begin();
    int sizePath = path.size();
    TPath newPath;
    newPath.origin = initPostition;
    newPath.destination = endPosition;
    newPath.type = currenttype;
    //newPath.path = (uint32_t*)malloc(sizeof(uint32_t) * sizePath);
    //Eliminar la lista para el path
    int i = 0;
    for (it; it != path.end(); ++it) {
      newPath.path.push_back(*it) ;
      i++;
    }
    currentPaths.push_back(newPath);

  }
}
