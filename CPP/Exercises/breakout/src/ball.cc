#include "ball.h"

void InitBall(BrickStruct *ball, float randA){
	
	float angle = (randA * 3.141592)/180.0;
	
	(*ball).vec_dir = {cos(angle), sin(angle)};
	(*ball).position = {340, 600};
	(*ball).body_info = {340, 600, 20, 10};
	(*ball).color = {255, 255, 255, 255};
	(*ball).speed = 5;
		
}

void DrawBall(SDL_Renderer* render, BrickStruct *ball){	// Draw Block
	
	SDL_SetRenderDrawColor(render, (*ball).color.r, (*ball).color.g,
																(*ball).color.b, (*ball).color.a);
	SDL_RenderFillRect(render, &(*ball).body_info);
	
}

void MoveBall(BrickStruct *ball){
	
	ball->position.x += ball->vec_dir.x * ball->speed;
	ball->position.y += ball->vec_dir.y * ball->speed;
	
	(*ball).body_info.x =  ball->position.x;
	(*ball).body_info.y =  ball->position.y;
  
  printf("%f %f\n", ball->vec_dir.x, ball->vec_dir.y);
	
}