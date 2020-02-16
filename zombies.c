/*
 *NAME: David Piatt
 *DATE: started: 01/29/17, gave up: 2/5/17 at 9:30 pm
 *CLASS: CSE 2421, M/W 16:15
 *
 * 
 *The purpose of this lab is to work with 
 * C arrays, file I/O
 * scanf, printf
 * #define
 * and random numbers
 * We will do this with a 2 dimmensional C array
 * 				Remember C arrays don't track their lenght, you have to remember it.
 
 The lab will calculate a simulation of zombies and puppies
 *
 * 
*/


//#include <unistd.h>
#include <stdio.h>


#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define NO_TURN 4

//This is to find the distance between 2 points
//it finds the absoltue value between 2 numbers.

int abs_dist(int one, int two){

	int distance = one - two; 
	if(distance < 0){
		distance = distance * -1;
	}
	return distance;

}

//implementation of pseudo random function

int rand(void){

	static unsigned long int next = 0;

	next = next * 1103515245 + 12345;

	return (unsigned int) (next/65536) % 32768;

}
//returns a random direction
int get_random_direction(void){
	int random = rand()%4;
	return random;

}


//save file under name results
void save_room(FILE * fp, int height, int width, char room[height][width]){
	int i, j;
	for(i =0; i < height; i++){
	
		for(j = 0; j < width; j++){
			char c = room[i][j];
			fprintf(fp, "%c", c);
			
		}
	
	}
	
	fprintf(fp, "\n");

}

//returns the room width from the input file
int get_room_width(FILE * file){
	int width = -1;
		
	fscanf(file, "DIMENSIONS:%*d%d", &width);
	  
	return width;
}

//returns the room height from the input file
int get_room_height(FILE * file){
	int height = -1;	
	
  //fseek resets our scanner since the width function
  //already consumed the first line	
	fseek(file, 0, SEEK_SET);
	fscanf(file, "DIMENSIONS:%d%*d", &height);
	return height;

}


//returns the simulation iterations from the input file
int get_iterations(FILE * file){
	int iterations = -1;
	
	fscanf(file, " ITERATIONS:%d", &iterations);

	return iterations;

	}

//receives and initializes the room matrix using the input file
void load_room(int height, int width, FILE * file, char room[height][width]){
	
	int col_counter; //may only get to width size
	
	int row_counter; //may only get to height size
	
	for(row_counter = 0; row_counter < height; row_counter ++){
	 	
		for(col_counter = 0; col_counter < width; col_counter ++){
			
			char c = fgetc(file);	
			
			room[row_counter][col_counter] = c;	
			

		}
	}
}




//returns the proper direction for a given zombie to move
int get_zombie_direction(int height, int width, char room[height][width], int zomb_x, int zomb_y){

int i=0, j=0, closest_pup_count=0, x_dist=100, y_dist=100;
  int direction = 4, pup_x, pup_y;

	
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
	
			if(room[i][j] == 'P' || room [i][j] == 'p'){
				pup_x = j; 
				pup_y = i;
			
				//changed from && to ||			
				if((abs_dist(pup_x,zomb_x) < x_dist )|| abs_dist(pup_y,zomb_y) < y_dist){
					closest_pup_count = 1;
					x_dist = abs_dist(pup_x,zomb_x);
					y_dist = abs_dist(pup_y,zomb_y);	
				
				}
				else if((abs_dist(pup_x,zomb_x) == x_dist )&& abs_dist(pup_y,zomb_y) == y_dist){
				closest_pup_count ++;
				}
			
			}
	
		}
	}
	
		
		//The zombie doesen't have more than 1 closest puppy
		
		if(closest_pup_count < 2){
		//it's x distance to zombie is less than y distance
		if(x_dist > y_dist){
			//figure out if it's going east or west
			if(abs_dist(pup_x, zomb_x+1) < abs_dist(pup_x,zomb_x)){
				direction = EAST;
			}else{
				direction = WEST;
			}
		}
		//x distance is greater than the y distance
		else if(x_dist < y_dist){
			//figure out if it's going north or south
			if(abs_dist(pup_y,zomb_y+1) < abs_dist(pup_y,zomb_y)){
				direction = SOUTH;
			}else{
				direction = NORTH;
			}
			
		}
		else if(x_dist == y_dist){
				direction = NO_TURN;
			}
		}
	 
	 else{
	 	direction = NO_TURN;
	 }
	 

	
		
	if(rand()%5){
			direction = get_random_direction();
			}
	
	
	return direction;

}



//returns the proper direction for a given puppy to move
int get_puppy_direction(int height, int width, char room[height][width], int pup_x, int pup_y){

	int i=0, j=0, closest_zombie_count=0, x_dist=100, y_dist=100;
  int direction = 4, zomb_x, zomb_y;

	
	for(i = 0; i < height; i++){
		for(j = 0; j < width; j++){
	
			if(room[i][j] == 'Z' || room [i][j] == 'z'){
				zomb_x = j; 
				zomb_y = i;
				
				//ATTENTION: HERE IS AN ISSUE
				//the || accounts for it being a neighbor
				//because the z or y value won't be smaller than
				//the x and y values of two neighboring, equidistant zombies
							
				if((abs_dist(pup_x,zomb_x) < x_dist )|| abs_dist(pup_y,zomb_y) < y_dist){
					
					closest_zombie_count = 1;
					x_dist = abs_dist(pup_x,zomb_x);
					y_dist = abs_dist(pup_y,zomb_y);	
				
				}
				else if((abs_dist(pup_x,zomb_x) == x_dist )&& abs_dist(pup_y,zomb_y) == y_dist){
				
				closest_zombie_count ++;
				}
			
			}
	
		}
	}
	
			
		//If the puppy doesn't have more than one zombie around it	
		if(closest_zombie_count < 2){
		//x distance shorter than y distance
		if(x_dist < y_dist){
			//does puppy move east or west
			if(abs_dist(pup_x+1, zomb_x) > abs_dist(pup_x,zomb_x)){
				direction = EAST;
				//was x_dist now zomb_x
			}else if(abs_dist(pup_x-1,zomb_x) > abs_dist(pup_x,zomb_x)){
				direction = WEST;
			}
		}
		//x distance bigger than y distance
		
		
		else if(x_dist > y_dist){
			//does puppy move south or north
			
			if(abs_dist(pup_y+1,zomb_y) > abs_dist(pup_y,zomb_y)){
				
				direction = SOUTH;
			
			}else if(abs_dist(pup_y-1,zomb_y) > abs_dist(pup_y,zomb_y)){
				direction = NORTH;
			}
			
		}
		//if the zombie is equal parts x and y away don't move puppy.
		else if(x_dist == y_dist){
				direction = NO_TURN;
			}
		}
			
						
		else{
			direction = NO_TURN;
		}

	
		
	if(rand()%5 == 0){
			direction = get_random_direction();
			}

	return direction;
}


//receives the room matrix, performs one iteration of the simulation
void iterate_simulation(int iterations, int height, int width, char room[height][width]){

	int dir;
	int col_counter = 0;
	int row_counter = 0;
	int pup_counter;
	
	while(iterations > 0){
	pup_counter = 0;
	dir = 4;
	
	
		//first 'p' -> 'P'
		
		for(row_counter = 0; row_counter < height; row_counter++){
		for(col_counter = 0; col_counter < width; col_counter++){
			//'p' becomes 'P'
			if(room[row_counter][col_counter] == 'p'){
				room[row_counter][col_counter] = 'P';
			  }
		  }
		}

		
		//second get_puppy dir
		//  if it's open move
		
			for(row_counter = 0; row_counter < height; row_counter++){
		for(col_counter = 0; col_counter < width; col_counter++){
				if(room[row_counter][col_counter] == 'P'){
					dir = get_puppy_direction(height, width, room, col_counter, row_counter);
					
					if(dir == EAST){
						
						if(room[row_counter][col_counter+1] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter][col_counter + 1] = 'p'; 
						}
					}else if(dir == NORTH){
						if(room[row_counter-1][col_counter] == ' '){
							room[row_counter][col_counter] = ' '; 
							room[row_counter-1][col_counter] = 'p';
						}
					}else if(dir == WEST){
						
						if(room[row_counter][col_counter-1] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter][col_counter-1] = 'p';
						}
					}else if(dir == SOUTH){
						if(room[row_counter+1][col_counter] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter+1][col_counter] = 'p';
					}
				}
				/*
				else if(dir == NO_TURN){
					room[row_counter][col_counter] = 'p';
				}
				*/
			}		
		}
		
		}

		
		
		//zombies by 2 or more puppies are killed
		//	puppy state doesn't change
		
				for(row_counter = 0; row_counter < height; row_counter++){
			for(col_counter = 0; col_counter < width; col_counter++){
				if(room[row_counter][col_counter] == 'z' || room[row_counter][col_counter] =='Z'){

					
					
					if(room[row_counter][col_counter+1] == 'p' || room[row_counter][col_counter+1] == 'P'){pup_counter ++;}
						
					
					if(room[row_counter][col_counter-1] == 'p' || room[row_counter][col_counter-1] =='P'){pup_counter ++;}
						
						
					if(room[row_counter+1][col_counter] == 'p' || room[row_counter+1][col_counter] =='P'){pup_counter ++;}
					
					if(room[row_counter-1][col_counter] == 'p' || room[row_counter-1][col_counter] =='P'){pup_counter ++;}
					
					if(pup_counter >= 2){
					room[row_counter][col_counter] = ' ';
					//set it back to sero so everything doesn't get destroyed.
					pup_counter = 0;
					}else{
						pup_counter = 0;
					}
				
				}							
				
				
			 }
			  
			}

		
		//'z' -> 'Z'
		
		for(row_counter = 0; row_counter < height; row_counter++){
				for(col_counter = 0; col_counter < width; col_counter++){
					if(room[row_counter][col_counter] == 'z'){ 
					room[row_counter][col_counter] = 'Z';
					}
				}
			}
		
		//change puppy to 'z' if adjacent, 'Z' -> 'z'
		
					for(row_counter = 0; row_counter < height; row_counter++){
				for(col_counter = 0; col_counter < width; col_counter++){
			
					if(room[row_counter][col_counter] == 'Z'){
						if(room[row_counter][col_counter+1] == 'p' || room[row_counter][col_counter+1] == 'P'){
							room[row_counter][col_counter+1] = 'z';
							room[row_counter][col_counter] = 'z';
						} else if(room[row_counter][col_counter-1] == 'p' || room[row_counter][col_counter-1] == 'P'){
							room[row_counter][col_counter-1] = 'z';
							room[row_counter][col_counter] = 'z';
						} else if(room[row_counter+1][col_counter] == 'p' || room[row_counter+1][col_counter] == 'P'){
							room[row_counter+1][col_counter] = 'z';
							room[row_counter][col_counter] = 'z';
						} else if(room[row_counter-1][col_counter] == 'p' || room[row_counter-1][col_counter] == 'P'){
							room[row_counter-1][col_counter] = 'z';
							room[row_counter][col_counter] = 'z';
						}
					}
				}
				
			}	
			
		
		//get_zombie_direction
		//	move if not occupied
		
		
		for(row_counter = 0; row_counter < height; row_counter++){
		for(col_counter = 0; col_counter < width; col_counter++){
				if(room[row_counter][col_counter] == 'Z'){
					
					dir = get_zombie_direction(height, width, room, col_counter, row_counter);

					
					if(dir == EAST){
						if(room[row_counter][col_counter+1] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter][col_counter + 1] = 'z'; 
						}
					}else if(dir == NORTH){
						if(room[row_counter-1][col_counter] == ' '){
							room[row_counter][col_counter] = ' '; 
							room[row_counter-1][col_counter] = 'z';
						}
					}else if(dir == WEST){
						if(room[row_counter][col_counter-1] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter][col_counter-1] = 'z';;
						}
					}else if(dir == SOUTH){
						if(room[row_counter+1][col_counter] == ' '){
							room[row_counter][col_counter] = ' ';
							room[row_counter+1][col_counter] = 'z';
					}
				}
				
			}
			
			}
		
		}			

		
			iterations --;

}
}


int main(void){

	
	int width = 0, height = 0, iterations = -1;

  
  FILE *fp = fopen("room","r");
	
	//the case room doesn't exist
	if(fp == NULL){
		return(1);
	}
	
	
	width = get_room_width(fp) +1 ;
  
	height = get_room_height(fp) ;
	
	iterations = get_iterations(fp);


	char room[height][width];
	
	load_room(height,width, fp, room);
	
  
  iterate_simulation(iterations, height, width, room);

  
  fclose(fp);

	//now create the file to print to:
	FILE *result = fopen("results","ab+");
	save_room(result, height, width, room);
	fclose(result);



	return 0;

}

