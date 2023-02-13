#include "bricks.h"

const int kRowsBricks = 6;
const int kNumbBricks = 10;

void CreateBrick(BrickStruct **brick, int num_brick, int row_order, RowStruct **row){	// Creates a brick in a row
	// Num brick = Position of the brick in its row
	// Row order = Order of this row
	BrickStruct *aux_brick = NULL;
	
	aux_brick = (BrickStruct*)malloc(sizeof(BrickStruct));
	
	aux_brick->id = num_brick;
	aux_brick->hp = 1;
	aux_brick->body_info = {70*num_brick + 3, (40*row_order)+100, 61, 20};
	aux_brick->color = (*row)->row_color;
	aux_brick->next_brick = *brick;
	aux_brick->previous_brick = NULL;
	
	if(*brick != NULL){
		
		(*brick)->previous_brick = aux_brick;
		
	}
	
	*brick = aux_brick;
	
}

void CreateRow(RowStruct **main_row, int order){	// Creates ONE Row of bricks
	// Order is the number of the row
	RowStruct *aux_row = NULL;
	
	aux_row = (RowStruct*)malloc(sizeof(RowStruct));
	
	aux_row->brick_list = NULL;
	aux_row->previous_row = NULL;
	
	// Color depending on the row order
	switch (order){
		
		case 0:	// Grey
					
			aux_row->row_color = {159, 159, 159, 255};
			
		break;
		
		case 1:	// Red
		
			aux_row->row_color = {255, 0, 0, 255};
			
		break;
		
		case 2: // Yellow
		
			aux_row->row_color = {255, 255, 0, 255};
			
		break;
		
		case 3:	// Blue
		
			aux_row->row_color = {0, 0, 255, 255};
			
		break;
		
		case 4:	// Purple
		
			aux_row->row_color = {255, 0, 255, 255};
			
		break;
		
		case 5:	// Green
		
			aux_row->row_color = {0, 255, 0, 255};
			
		break;
		
	}
	
	for(int i = 0; i < kNumbBricks; ++i){
		
		CreateBrick(&aux_row->brick_list, i, order, &aux_row);
		
		
	}
	aux_row->next_row = *main_row;
	
	if(*main_row != NULL){
		
		(*main_row)->previous_row = aux_row;
		
	}
	
	*main_row = aux_row;
	
}

void CreateAllRows(RowStruct **row_list){	// Creates the 6 rows of bricks
	
	for(int i = 0; i < kRowsBricks; ++i){
		
		CreateRow(row_list, i);
				
	}
}

void DrawBrick(SDL_Renderer* render, BrickStruct *brick){	// Draw Block
	
	SDL_SetRenderDrawColor(render, brick->color.r, brick->color.g,
																brick->color.b, brick->color.a);
	SDL_RenderFillRect(render, (&brick->body_info));
	
}

void CheckList(RowStruct *main_row, SDL_Renderer* render){
	
	RowStruct *aux_row = NULL;
	BrickStruct *aux_brick = NULL;
	
	for(aux_row = main_row; aux_row != NULL; aux_row = aux_row->next_row){
		
		for(aux_brick = aux_row->brick_list; aux_brick != NULL; aux_brick = aux_brick->next_brick){
			
			DrawBrick(render, aux_brick);
			
		}
		
	}
	
}

void BallToBar(BrickStruct *ball, BrickStruct player_bar){
  
  SDL_Rect aux_rect;
  bool collide = false;
  
  collide = SDL_IntersectRect(&ball->body_info, &player_bar.body_info, &aux_rect);
  
  if(collide && ball->body_info){
    
    // Right Movement
    if(ball->vec_dir.x >= 0){
      
      // Player Bar moving to the right
      if(player_bar.last_vec_dir == 1){
        
        ball->vec_dir.x *= -1;
        ball->vec_dir.y *= -1;
        
      }
      
      // Player Bar moving to the right
      if(player_bar.last_vec_dir == -1){
        
        ball->vec_dir.y *= -1;
        
      }
      
    }else{
      
      // Player Bar moving to the left
      if(player_bar.last_vec_dir == 1){
        
        ball->vec_dir.x *= -1;
        ball->vec_dir.y *= -1;
        
      }
      
      // Player Bar moving to the right
      if(player_bar.last_vec_dir == -1){
        
        ball->vec_dir.y *= -1;
        
      }
      
    }
    
  }
  
}

void BallToWall(BrickStruct *ball, int width, int height){

  // Collide with bottom wall
  if(ball->body_info.y < 0){
    
    ball->vec_dir.y *= -1;
    
  }

  if(ball->body_info.x < 0 || ball->body_info.x + ball->body_info.w > width){
    
    ball->vec_dir.x *= -1;
    
  }
  
  if(ball->body_info.y + ball->body_info.h > height){
    
    ball->vec_dir.y *= -1;
    
  }
  
}

  