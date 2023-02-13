#include "player_bar.h"

void InitPlayer(BrickStruct *player){	// Initializes Player_bar
	
	*player = {0, 3, 5, {0, 0}, {340, 800}, {340, 800, 100, 20}, {0, 168, 243, 255}, NULL, NULL, 1};
	
}

void DrawBlock(SDL_Renderer* render, BrickStruct *brick){	// Draw Block
	
	SDL_SetRenderDrawColor(render, brick->color.r, brick->color.g,
																brick->color.b, brick->color.a);
	SDL_RenderFillRect(render, &brick->body_info);
	
}

void ControlPlayer(SDL_Event event, const int width, BrickStruct *player){
	
	if(event.type == SDL_KEYDOWN){
		
		switch(event.key.keysym.sym){
			
			case SDLK_LEFT:
			
				if(player->body_info.x > 0){
					
					player->vec_dir.x = -1;
          player->last_vec_dir = -1;
					
				}
				
			break;
				
			case SDLK_RIGHT:
			
				if(player->body_info.x + player->body_info.w < width){
					
					player->vec_dir.x = 1;
          player->last_vec_dir = 1;
					
				}
				
			break;
			
		}
		
	}
	
	if(event.type == SDL_KEYUP){
							
		player->vec_dir.x = 0;
				
	}			
	
}

void MovePlayer(BrickStruct *player, int width){
    
  player->position.x += player->vec_dir.x * player->speed;
  player->position.y += player->vec_dir.y * player->speed;
  
  if(player->position.x < 0){
    
    player->position.x = 0;
    
  }
  
  if(player->position.x + player->body_info.w > width){
    
    player->position.x = width - player->body_info.w;
    
  }
	
	player->body_info.x = player->position.x;
	player->body_info.y = player->position.y;
	
}