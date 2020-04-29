#include <stdio.h>
#include <stdlib.h>

#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct elementData{
	int id;
	char x;
	int y;
	char value;
};

char turn = 'w';
char anti_turn = 'b';
char turnChecker = 'x';
char anti_turnChecker = 'y';
struct elementData elementTurn = {65, 'A', 9, 'w'};


void readFile();
void updateFile();
int getIndexOfChar(char);
int main(int argc, char *argv[]) {
	
	FILE *cfptr; 
	
	struct elementData element = {0, 'A', 1, 's'};
	struct elementData element2 = {1, 'H', 1, 'b'};

	
	char horizontal[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};


	// write to file default values
	if((cfptr = fopen("credit.dat", "wb")) == NULL){
		printf("nope");
	}else{
		int count = 1;
		for(int i=0; i<8; i++){	
		
			if(i==1 || i==2){
				element.value = 'w';
			}else if(i==5 || i==6){
				element.value = 'b';
			}else{
				element.value = '-';
			}
			
			// set y-aksis
			element.y = i;
			for(int j=0; j<8; j++){
				fseek(cfptr, (count - 1) * sizeof(struct elementData), SEEK_SET);
				
				element.id = count;
				
				// set x aksis
				element.x = horizontal[j];
				fwrite(&element, sizeof(struct elementData), 1, cfptr);	
				
				count++;
			}
		
		}
		
		// add turn operator to switch between players
		fwrite(&elementTurn, sizeof(struct elementData), 1 ,cfptr);
		
		fclose(cfptr);
	}
	
	
	readFile();
	
	while(1 == 1){
		updateFile();
		printf("\n\n");
		readFile();
	}
	

	return 0;
}

int getIndexOfChar(char x){
	char horizontal[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	
	for(int i=0; i< 8; i++){
		if(horizontal[i] == x){
			return i;
		}
	}
}

void updateFile(){
	
	char x,a;
	int y = 0,b = 0;
	int midY;
	int midX;
	
	printf("\n\nPlease enter location then destionation(ex. A1B2)\n");
	scanf("%c%d%c%d", &x, &y, &a, &b);
	
	int indexOfX = getIndexOfChar(x);
	int indexOfA = getIndexOfChar(a);
	

	if(y < 0 || b < 0 || abs(y-b) > 1 || abs(indexOfX - indexOfA) > 1){
		
		// CHECKER PLAYING WITH LONG UNIT MOVING
		if(abs(y-b) > 2 || abs(indexOfX - indexOfA) > 2){
			
			
			if(turn == 'w'){
				turnChecker = 'x';
				anti_turnChecker = 'y'; 
			}else{
				turnChecker = 'y';
				anti_turnChecker = 'x'; 
			}
			
			FILE *cfptr;
			
			struct elementData elementL = {0, 'A', 1, 'b'};
			struct elementData elementM = {0, 'A', 1, 'b'};
			struct elementData elementD = {0, 'A', 1, 'b'};
			
			if((cfptr = fopen("credit.dat", "rb+")) == NULL){
				printf("nope");
			}else{
				y--;
				b--;
				
				int up = 0; // boolean
				if(y - b > 0){
					up = 1;
				}else{
					up = 0;
				}
				
				int elementIds[6] = {10,10,10,10,10,10};  // add default values
				char elementXs[6];
				int elementYs[6];
				char elementValues[6];
				int index = 0; // index for elementIds
				
				// User moved in vertical
				if(indexOfA == indexOfX){

					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						
						// find id of element that at coordinate ab (destination)
						if(elementD.x == x && elementD.y == b && elementD.value == '-'){
							elementD.value = turnChecker;
							//printf("destination id = %d \n", elementD.id);
								
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);

								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(elementL.value == turnChecker){
									    	
												
										if(y > b){
											midY = y-1;
										}else{
											midY = b-1;
										}
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
											
											// to stop the loop, nad insert new values of location and destination
											if(y > b){
												if(midY == b){
													break;
												};
											}else{
												if(midY == y){
													break;
												}
											}
											
											if(elementM.x == x  && elementM.y == midY ){
												rewind(cfptr);
												
												if(elementM.value == anti_turn || elementM.value == anti_turnChecker){
													//rewind(cfptr);
													elementIds[index] = elementM.id;
													elementXs[index] = elementM.x;
													elementYs[index] = elementM.y;
													elementValues[index] = elementM.value;
													index++;
												}
												
												midY--;
											}	

										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}	
				}else if(y == b){
					// user moved in horizontal
					
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						
						// find id of element that at coordinate ab (destination)
						if(elementD.x == a && elementD.y == y && elementD.value == '-'){
							elementD.value = turnChecker;
							//printf("destination id = %d \n", elementD.id);
								
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);

								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(elementL.value == turnChecker){
									    	
												
										rewind(cfptr);
										if(indexOfX > indexOfA){
											midX = indexOfX - 1;
										}else{
											midX = indexOfA - 1;	
										}
										
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
											//printf("elementM.X = %d", getIndexOfChar(elementM.x));
											
											// to stop the loop, nad insert new values of location and destination
											if(indexOfX > indexOfA){
												if(midX == indexOfA){;
													break;
												}
											}else{
												if(midX == indexOfX){
													break;
												}
											}
											
											if(elementM.y == y  && getIndexOfChar(elementM.x) == midX){
												rewind(cfptr);
												
												if(elementM.value == anti_turn || elementM.value == anti_turnChecker){
													//printf("midX = %d", midX);
													//rewind(cfptr);
													elementIds[index] = elementM.id;
													elementXs[index] = elementM.x;
													elementYs[index] = elementM.y;
													elementValues[index] = elementM.value;
													index++;	
												}
												
												midX--;	
											}											

										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}
				
				}
				
				    int success = 0;
				
					// UPDATE VALUES
					for(int i=0; i < 6; i++){
						rewind(cfptr);
						// check if it is not default value
						if(elementIds[i] != 10){ 
							fseek(cfptr, (elementIds[i] - 1) * sizeof(struct elementData), SEEK_SET);
						//	printf(" %d %c %d %c \n", elementIds[i], elementXs[i], elementYs[i], elementValues[i]);
							struct elementData newM = {elementIds[i], elementXs[i], elementYs[i], '-'};
							
							fwrite(&newM, sizeof(struct elementData), 1, cfptr);
							
							success = 1;
						}
					}
					
					if(success == 1){
						rewind(cfptr);
						// write new value of old location
						fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
						elementL.value = '-';
						//struct elementData oldL = {elementL.id,  elementL.x, elementL.y, '-'};
						fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
												
						rewind(cfptr);
						// write new value of new location (it was destination before)
						fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
						//struct elementData newL = {elementD.id,  elementD.x, elementD.y, 'q'};
						fwrite(&elementD, sizeof(struct elementData), 1, cfptr);	
					}
		
			}
			fclose(cfptr);
		// STANDART PLAYING WITH 2 UNIT AWAY MOVING
		}else{
			// if user select a destination that 2 unit saway 
			y--;
			b--;
			
			if(y - b < 0){
				midY = y;
				midY++;
			}else{
				midY = y;
				midY--;
			}	
			
			if(indexOfX - indexOfA < 0){
				midX = x;
				midX++;
			}else{
				midX = x;
				midX--;
			}
			
			//	printf("\n y = %d  midY = %d \n", y, midY);
			
			if(abs(y-b) == 2 && indexOfX - indexOfA == 0){
				
				FILE *cfptr;
				
				struct elementData elementL = {0, 'A', 2, 'b'}; // location
				struct elementData elementM = {0, 'A', 2, 'b'}; // mid
				struct elementData elementD = {0, 'A', 2, 'b'}; // dest
				
				if((cfptr = fopen("credit.dat", "rb+")) == NULL){
					printf("nope");
				}else{
					
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						// find id of element that at coordinate ab (destination)
						if(elementD.x == a && elementD.y == b && elementD.value == '-'){
							elementD.value = turn;
		
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);
								
								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(elementL.value == turn || elementL.value == turnChecker){
										// change value of destiation if player using checker
										elementD.value = elementL.value == turnChecker ? turnChecker : turn;
										
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
										
											// find id of element at mid
											if(elementM.x == x && elementM.y == midY && (elementM.value == anti_turn || elementM.value == anti_turnChecker)){
												

												// write the new value of mid
												fseek(cfptr, (elementM.id  - 1) * sizeof(struct elementData), SEEK_SET);
												struct elementData newMid = {elementM.id,  elementM.x, elementM.y, '-'};
												fwrite(&newMid, sizeof(struct elementData), 1, cfptr);
												
										
												// write new value of old location
												fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
												elementL.value = '-';
												fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
												
												
												
												// write new value of new location (it was destination before)
												fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
												//elementD.value = turn;
											
												// check if we made our element 'checker'
												if(y == 5 && elementD.value == 'w'){
													elementD.value = 'x';
												}else if(y == 2 && elementD.value == 'b'){
													elementD.value = 'y';
												}
												fwrite(&elementD, sizeof(struct elementData), 1, cfptr);
										
												break;
											}
										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}
				}
				fclose(cfptr);
			}else if(abs(indexOfX - indexOfA) == 2 && abs(y-b) == 0){
				FILE *cfptr;
				
				struct elementData elementL = {0, 'A', 2, 'b'}; // location
				struct elementData elementM = {0, 'A', 2, 'b'}; // mid
				struct elementData elementD = {0, 'A', 2, 'b'}; // dest
				
				if((cfptr = fopen("credit.dat", "rb+")) == NULL){
					printf("nope");
				}else{
					
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						// find id of element that at coordinate ab (destination)
						if(elementD.x == a && elementD.y == b && elementD.value == '-'){
							elementD.value = turn;
		
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);
								
								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(elementL.value == turn || elementL.value == turnChecker){
										// change value of destiation if player using checker
										elementD.value = elementL.value == turnChecker ? turnChecker : turn;
										
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
										
											// find id of element at mid
											if(elementM.x == midX && elementM.y == y && (elementM.value == anti_turn || elementM.value == anti_turnChecker)){
												
												// write the new value of mid
												fseek(cfptr, (elementM.id  - 1) * sizeof(struct elementData), SEEK_SET);
												struct elementData newMid = {elementM.id,  elementM.x, elementM.y, '-'};
												fwrite(&newMid, sizeof(struct elementData), 1, cfptr);
												
										
												// write new value of old location
												fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
												printf("\n %d %c %d %c \n", elementL.id, elementL.x, elementL.y, elementL.value);
												elementL.value = '-';
												fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
												
												// write new value of new location (it was destination before)
												fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
											//	elementD.value = turn;
												
												// check if we made our element 'checker'
												if(y == 6 && elementD.value == 'w'){
													elementD.value = 'x';
												}else if(y == 1 && elementD.value == 'b'){
													elementD.value = 'y';
												}
												
												fwrite(&elementD, sizeof(struct elementData), 1, cfptr);
										
												break;
											}
										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}
				}
				fclose(cfptr);
			}else{
				printf("laan");
			}
			
		}
	// STANDART PLAYING WITH ONE UNIT AWAY MOVING
	}else{
	// to avoid bugs I decrement y and b 1 less;
		y--;
		b--;
	
	
		FILE *cfptr;
		
		struct elementData element = {0, 'A', 2, 'b'};
		struct elementData element2 = {0, 'A', 2, 'b'};
	
		if((cfptr = fopen("credit.dat", "rb+")) == NULL){
			printf("nope");
		}else{
			
			if(abs(y-b) == 1 && indexOfX - indexOfA == 0){
				
				rewind(cfptr);
				while(! feof(cfptr)){
					fread(&element, sizeof(struct elementData), 1, cfptr);
					
					// find id of element that at coordinate ab (destination)
					if(element.x == x && element.y == b && element.value == '-'){
						element.value = turn;
					
						rewind(cfptr);
						while(! feof(cfptr)){
							fread(&element2, sizeof(struct elementData), 1, cfptr);
							
							// find id of element that at coordinate xy  (location)
							if(element2.x == x && element2.y == y){
								
								if(element2.value == turn || element2.value == turnChecker){
									// change value of destiation if player using checker
									element.value = element2.value == turnChecker ? turnChecker : turn;
									
									
									// write new value of old location to file
									fseek(cfptr, (element2.id - 1) * sizeof(struct elementData), SEEK_SET);
									element2.value = '-';
									fwrite(&element2, sizeof(struct elementData), 1, cfptr);
									
									// write new location value to file
									fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
									fwrite(&element, sizeof(struct elementData), 1, cfptr);	
									
									// set turn and write new turn value to file also set anti-turn to old value of turn
									anti_turn = turn;
									anti_turnChecker = turnChecker;
									
									turn = turn == 'w' ? 'b' : 'w'; 
									turnChecker = turnChecker == 'x' ? 'y' : 'x';
									fseek(cfptr, (elementTurn.id - 1) * sizeof(struct elementData), SEEK_SET);
									elementTurn.value = turn;
								
									fwrite(&elementTurn, sizeof(struct elementData), 1, cfptr);
									break;
								}else{
									printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
								}
							}
						}
						break;
					}
				}
			}else if(abs(y-b) == 0 && abs(indexOfX - indexOfA) == 1){
				
				rewind(cfptr);
				while(! feof(cfptr)){
					fread(&element, sizeof(struct elementData), 1, cfptr);
					
					// find id of element that at coordinate ab (destination)
					if(element.x == a && element.y == y && element.value == '-'){
						element.value = turn;
					
						rewind(cfptr);
						while(! feof(cfptr)){
							fread(&element2, sizeof(struct elementData), 1, cfptr);
							
							// find id of element that at coordinate xy  (location)
							if(element2.x == x && element2.y == y){
								
								if(element2.value == turn || element2.value == turnChecker){
									// change value of destiation if player using checker
									element.value = element2.value == turnChecker ? turnChecker : turn;
									
									// write new value of old location to file
									fseek(cfptr, (element2.id - 1) * sizeof(struct elementData), SEEK_SET);
									element2.value = '-';
									fwrite(&element2, sizeof(struct elementData), 1, cfptr);
									
									// write new location value to file
									fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
									fwrite(&element, sizeof(struct elementData), 1, cfptr);	
									
									// set turn and write new turn value to file also set anti-turn to old value of turn
									anti_turn = turn;
									anti_turnChecker = turnChecker;
									
									turn = turn == 'w' ? 'b' : 'w'; 
									turnChecker = turnChecker == 'x' ? 'y' : 'x';
									fseek(cfptr, (elementTurn.id - 1) * sizeof(struct elementData), SEEK_SET);
									elementTurn.value = turn;
									
	
									
									fwrite(&elementTurn, sizeof(struct elementData), 1, cfptr);
									break;
								}else{
									printf("\nWrong player tried to play, please wait till -> %c <- player make its move\n", turn);	
								}
							}
						}
						break;
					}
				}
			}

		}
		fclose(cfptr);
	}
	
}

void getTurn(){
	
	FILE *cfptr;
	struct elementData element = {0, 'A', 1, 'b'}; // default values
	
	if((cfptr = fopen("credit.dat", "rb")) == NULL){
		printf("\nWarning turn value could not read\n");
	}else{
		
		while(! feof(cfptr)){
			fread(&element, sizeof(struct elementData), 1, cfptr);
			
			if(element.id == 65){
				turn  = element.value;
				break;
			}
		}
	}
}


void readFile(){
	
	FILE *cfptr;
	
	struct elementData element = {0, 'A', 2, 'b'};
	
	if((cfptr = fopen("credit.dat", "rb")) == NULL){
		printf("nope");
	}else{
		int oldY = 0; //start value to be different from y;
		
		//reset the pointer of explorer
		rewind(cfptr);
		
		int row = 1;
		int counter = 0;
		while(! feof(cfptr)){
			fread(&element, sizeof(struct elementData), 1, cfptr);
			int y = element.y;
			
			// pass to bottom lane
			if(oldY != y){
				printf("  %d\n", row);
				row++;
				oldY = y;
			}
			
			printf("%c ", element.value);
			//printf("%d  %c %d %c \n", element.id, element.x, element.y, element.value);
			
			counter++;
			
			if(counter == 65){
				break;
			}
		}
		
		printf("  \nA B C D E F G H\n");
		
		fclose(cfptr);
	}
}



