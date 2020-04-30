#include <stdio.h>
#include <stdlib.h>

#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct elementData{
	int id;
	char x;
	int y;
	char value[2];
};

char turn[3] = "w ";
char anti_turn[4] = "b ";
char turnChecker[5] = "ww";
char anti_turnChecker[6] = "bb";
struct elementData elementTurn = {65, 'A', 9, "w "};
FILE *logs;


void readFile();
void updateFile(char x, char a, int y, int b);
int getIndexOfChar(char);
void getTurn();
int main(int argc, char *argv[]) {	
	
	FILE *cfptr; 
		
	struct elementData element = {0, 'A', 1, "b "};
	struct elementData element2 = {1, 'H', 1, "w "};
	
	
	char horizontal[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	
	while(1==1){
		printf("\n1-)New Game\n2-)Continue Game\n3-)End Game\n");
		
		int option;
		scanf("%d", &option);
		
		if(option == 3){
			printf("\nClosing the game..\n");
			break;
		}else if(option == 2){
			
			getTurn();
			
			if((logs = fopen("logs.txt", "a")) == NULL){
				printf("\nLogs could not opened.Please check 'logs' file in the directory\n");
			}else{
				
				fprintf(logs, "Continue to old game\n");
				
				readFile();
				while(1 == 1){
					
					char x,a;
					char enterKeeper;
					int y = 0,b = 0;
					
					printf("\n\nPlease enter location then destionation(ex. A1B2)\nTo exit enter 'Q1Q1'\n");
					scanf("%c%d%c%d%c", &x, &y, &a, &b, &enterKeeper);
		
					if(x == 'Q' && y == 1 && a == 'Q' && b == 1){
						printf("\n****Closing the game...****\n");
						fprintf(logs, "End Games\n");
						break;
					}
					
					updateFile(x,a,y,b);
					printf("\n\n");
					readFile();
				}
			}
		
		}else if(option == 1){
			

			 strcpy(turn, "w ");
			 strcpy(anti_turn,  "b ");
			 strcpy(turnChecker, "ww");
			 strcpy(anti_turnChecker, "bb");
			 strcpy(elementTurn.value, "w ");
		
			
			if((logs = fopen("logs.txt", "a")) == NULL){
				printf("\nLogs could not opened.Please check 'logs' file in the directory\n");
			}else{
				fprintf(logs, "New Game Start\n");
				
				// write to file default values
				if((cfptr = fopen("element.dat", "wb")) == NULL){
					printf("\nFile Could not open\n");
				}else{
					int count = 1;
					for(int i=0; i<8; i++){	
					
						if(i==1 || i==2){
							strcpy(element.value, "w ");
						}else if(i==5 || i==6){
							strcpy(element.value, "b ");
						}else{
							strcpy(element.value, "- ");
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
					
					char x,a;
					char enterKeeper;
					int y = 0,b = 0;
					
					printf("\n\nPlease enter location then destionation(ex. A1B2)\nTo exit enter 'Q1Q1'\n");
					scanf("%c%d%c%d%c", &x, &y, &a, &b, &enterKeeper);
		
					if(x == 'Q' && y == 1 && a == 'Q' && b == 1){
						printf("\n****Closing the game...****\n");
						fprintf(logs, "End Games\n");
						break;
					}
					
					updateFile(x,a,y,b);
					printf("\n\n");
					readFile();
				}
			}

			
		}else{
			printf("\nPlease enter a valid option!\n");
		}
		
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

void updateFile(char x, char a, int y, int b){
	
	int isLegal = 0;
	int goChecker = 0;
	
	//char x,a;
	char enterKeeper;
	//int y = 0,b = 0;
	int midY;
	int midX;
	
	
		
	int indexOfX = getIndexOfChar(x);
	int indexOfA = getIndexOfChar(a);
	

	if(y < 0 || b < 0 || abs(y-b) > 1 || abs(indexOfX - indexOfA) > 1){
		
		// CHECKER PLAYING WITH LONG UNIT MOVING
		if(abs(y-b) > 2 || abs(indexOfX - indexOfA) > 2){
			
	
			if(strcmp(turn , "w ") == 0){
				strcpy(turnChecker, "ww");
				strcpy(anti_turnChecker, "bb");
			}else{
				strcpy(turnChecker, "bb");
				strcpy(anti_turnChecker, "ww");
			}
			
			FILE *cfptr;
			
			struct elementData elementL = {0, 'A', 1, "b "};
			struct elementData elementM = {0, 'A', 1, "b "};
			struct elementData elementD = {0, 'A', 1, "b "};
			
			if((cfptr = fopen("element.dat", "rb+")) == NULL){
				printf("\nFile could no open\n");
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
				char *elementValues[6];
				int index = 0; // index for elementIds
				
				// User moved in vertical
				if(indexOfA == indexOfX){

					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						
						// find id of element that at coordinate ab (destination)
						if(elementD.x == x && elementD.y == b && strcmp(elementD.value,  "- ") == 0){
							strcpy(elementD.value, turnChecker);
							//printf("destination id = %d \n", elementD.id);
								
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);

								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(strcmp(elementL.value, turnChecker) == 0){
									    	
												
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
												}
											}else{
												if(midY == y){
													break;
												}
											}
											
											if(elementM.x == x  && elementM.y == midY ){
												rewind(cfptr);
												
												if(index == 1){
													goChecker = 1;
												}else{
													goChecker = 0;
												}
												
												if(strcmp(elementM.value , anti_turn) == 0  || strcmp(elementM.value , anti_turnChecker) == 0){
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
										printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
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
						if(elementD.x == a && elementD.y == y && strcmp(elementD.value , "- ") == 0){
							strcpy(elementD.value, turnChecker);
							//printf("destination id = %d \n", elementD.id);
								
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);

								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(strcmp(elementL.value , turnChecker) == 0 ){
									    	
												
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
												
												if(index == 1){
													goChecker = 1;
												}else{
													goChecker = 0;
												}
												
												
												if(strcmp(elementM.value , anti_turn) == 0  || strcmp(elementM.value , anti_turnChecker) == 0){
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
										printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}
				
				}
				
				    
				    
				    if(goChecker == 1){
				    	int success = 0;
				    	int counter = 0;
						// UPDATE VALUES
						for(int i=0; i < 6; i++){
							rewind(cfptr);
							
							// check if it is not default value
							if(elementIds[i] != 10){ 
								counter++;
							}
						}
						
						if(counter == 1){
							success = 1;
							fseek(cfptr, (elementIds[0] - 1) * sizeof(struct elementData), SEEK_SET);
							//	printf(" %d %c %d %c \n", elementIds[i], elementXs[i], elementYs[i], elementValues[i]);
							struct elementData newM = {elementIds[0], elementXs[0], elementYs[0], "- "};
							
							fwrite(&newM, sizeof(struct elementData), 1, cfptr);
							
							isLegal = 1;
							rewind(cfptr);
							// write new value of old location
							fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
							strcpy(elementL.value, "- ");
							//struct elementData oldL = {elementL.id,  elementL.x, elementL.y, "-"};
							fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
													
							rewind(cfptr);
							// write new value of new location (it was destination before)
							fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
							//struct elementData newL = {elementD.id,  elementD.x, elementD.y, "q"};
							fwrite(&elementD, sizeof(struct elementData), 1, cfptr);
							
							rewind(cfptr);
							// set turn and write new turn value to file also set anti-turn to old value of turn
							strcpy(anti_turn, turn);
							strcpy(anti_turnChecker, turnChecker);
									
							strcpy(turn, turn == "w " ? "b " : "w ");
							strcpy(turnChecker, strcmp(turnChecker , "ww") == 0 ? "bb" : "ww");
							fseek(cfptr, (elementTurn.id - 1) * sizeof(struct elementData), SEEK_SET);
							strcpy(elementTurn.value, turn);
								
							fwrite(&elementTurn, sizeof(struct elementData), 1, cfptr);
							
						}
											
					}
		
			}
			fclose(cfptr);
		// STANDART PLAYING WITH 2 UNIT AWAY MOVING
		}else{
			// if user select a destination that 2 unit away 
			// decrease values to avoid bugs of array
			y--;
			b--;
			
			// set midY for the value between location and destination
			if(y - b < 0){
				midY = y;
				midY++;
			}else{
				midY = y;
				midY--;
			}	
			
			
			// set midX for the value between location and destination
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
				
				struct elementData elementL = {0, 'A', 2, "b "}; // location
				struct elementData elementM = {0, 'A', 2, "b "}; // mid
				struct elementData elementD = {0, 'A', 2, "b "}; // dest
			
				
				if((cfptr = fopen("element.dat", "rb+")) == NULL){
					printf("\nFile could no open\n");
				}else{
					
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						// find id of element that at coordinate ab (destination)
						if(elementD.x == a && elementD.y == b && strcmp(elementD.value , "- ") == 0){
							strcpy(elementD.value, turn);
			
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);
								
								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(strcmp(elementL.value , turn) == 0 || strcmp(elementL.value , turnChecker) == 0){
										
										if((b <= y && strcmp(elementL.value, "w ") == 0) || (y <= b && strcmp(elementL.value,  "b ") == 0)){
											break;
										}
										
										// change value of destiation if player using checker
										strcpy(elementD.value, strcmp(elementL.value , turnChecker) == 0 ? turnChecker : turn);
										
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
									
											// find id of element at mid
											if(elementM.x == x && elementM.y == midY && (strcmp(elementM.value, anti_turn) == 0  || strcmp(elementM.value , anti_turnChecker) == 0)){
												
												// write the new value of mid
												fseek(cfptr, (elementM.id  - 1) * sizeof(struct elementData), SEEK_SET);
												struct elementData newMid = {elementM.id,  elementM.x, elementM.y, "- "};
												fwrite(&newMid, sizeof(struct elementData), 1, cfptr);
												
										
												// write new value of old location
												fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
												strcpy(elementL.value, "- ");
	
												fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
												
												// write new value of new location (it was destination before)
												fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
												//elementD.value = turn;
											
												// check if we made our element "checker"
												if(y == 5 && strcmp(elementD.value , "w ") == 0){
													strcpy(elementD.value, "ww");
													
												}else if(y == 2 && strcmp(elementD.value , "b ") == 0){
													strcpy(elementD.value, "bb");
												
												}
												
												fwrite(&elementD, sizeof(struct elementData), 1, cfptr);
										
												isLegal = 1;
												break;
											}
										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
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
				
				struct elementData elementL = {0, 'A', 2, "b "}; // location
				struct elementData elementM = {0, 'A', 2, "b "}; // mid
				struct elementData elementD = {0, 'A', 2, "b "}; // dest
				
				if((cfptr = fopen("element.dat", "rb+")) == NULL){
					printf("\nFile could no open\n");
				}else{
					
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&elementD, sizeof(struct elementData), 1, cfptr);
						// find id of element that at coordinate ab (destination)
						if(elementD.x == a && elementD.y == b && strcmp(elementD.value , "- ") == 0){
							strcpy(elementD.value, turn);
						
		
							rewind(cfptr);
							while(! feof(cfptr)){
								fread(&elementL, sizeof(struct elementData), 1, cfptr);
								
								// find id of element that at coordinate xy  (location)
								if(elementL.x == x && elementL.y == y){
									
									if(strcmp(elementL.value , turn) == 0 || strcmp(elementL.value , turnChecker) == 0){
										// change value of destiation if player using checker
										strcpy(elementD.value, strcmp(elementL.value , turnChecker) == 0 ? turnChecker : turn);
									
										
										rewind(cfptr);
										// find element at mid
										while(! feof(cfptr)){
											fread(&elementM, sizeof(struct elementData), 1, cfptr);
										
											// find id of element at mid
											if(elementM.x == midX && elementM.y == y && (strcmp(elementM.value, anti_turn) == 0  || strcmp(elementM.value , anti_turnChecker) == 0)){
												
												// write the new value of mid
												fseek(cfptr, (elementM.id  - 1) * sizeof(struct elementData), SEEK_SET);
												struct elementData newMid = {elementM.id,  elementM.x, elementM.y, "- "};
												fwrite(&newMid, sizeof(struct elementData), 1, cfptr);
												
										
												// write new value of old location
												fseek(cfptr, (elementL.id - 1) * sizeof(struct elementData), SEEK_SET);
												//printf("\n %d %c %d %c \n", elementL.id, elementL.x, elementL.y, elementL.value);
												strcpy(elementL.value, "- ");
												
												fwrite(&elementL, sizeof(struct elementData), 1, cfptr);
												
												// write new value of new location (it was destination before)
												fseek(cfptr, (elementD.id - 1) * sizeof(struct elementData), SEEK_SET);
											//	elementD.value = turn;
												
												
												fwrite(&elementD, sizeof(struct elementData), 1, cfptr);
										
												isLegal = 1;
												break;
											}
										}
										
									}else{
										printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
									}
									break;
								}
							}
						     break;
						}
					}
				}
				fclose(cfptr);
			}
		}
	// STANDART PLAYING WITH ONE UNIT AWAY MOVING
	}else{
	// to avoid bugs I decrement y and b 1 less;
		y--;
		b--;
	
	
		FILE *cfptr;
		
		struct elementData element = {0, 'A', 2, "b "};
		struct elementData element2 = {0, 'A', 2, "b "};
	
		if((cfptr = fopen("element.dat", "rb+")) == NULL){
			printf("\nFile could no open\n");
		}else{
			
			if(abs(y-b) == 1 && indexOfX - indexOfA == 0){
				
				rewind(cfptr);
				while(! feof(cfptr)){
					fread(&element, sizeof(struct elementData), 1, cfptr);
					
					// find id of element that at coordinate ab (destination)
					if(element.x == x && element.y == b && strcmp(element.value , "- ") == 0){
						strcpy(element.value, turn);
					
					
						rewind(cfptr);
						while(! feof(cfptr)){
							fread(&element2, sizeof(struct elementData), 1, cfptr);
							
							// find id of element that at coordinate xy  (location)
							if(element2.x == x && element2.y == y ){
								
								if(strcmp(element2.value , turn) == 0 || strcmp(element2.value , turnChecker) == 0){
									
									// DO NOT ALLOW GO BACK 
									if((b <= y && strcmp(element2.value, "w ") == 0) || (y <= b && strcmp(element2.value,  "b ") == 0)){
										break;
									}
									
									// change value of destiation if player using checker
									strcpy(element.value , strcmp(element2.value , turnChecker) == 0 ? turnChecker : turn);
								
									
									
									// write new value of old location to file
									fseek(cfptr, (element2.id - 1) * sizeof(struct elementData), SEEK_SET);
									strcpy(element2.value, "- ");
								
									fwrite(&element2, sizeof(struct elementData), 1, cfptr);
									
									// write new location value to file
									fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
									if(y == 6 && strcmp(element.value , "w ") == 0){
										strcpy(element.value, "ww");
													
									}else if(y == 1 && strcmp(element.value , "b ") == 0){
										strcpy(element.value, "bb");
												
									}
									fwrite(&element, sizeof(struct elementData), 1, cfptr);	
									
									// set turn and write new turn value to file also set anti-turn to old value of turn
									strcpy(anti_turn, turn);
									strcpy(anti_turnChecker, turnChecker);
											
									strcpy(turn, strcmp(turn , "w ") == 0  ? "b " : "w ");
									strcpy(turnChecker, strcmp(turnChecker , "ww") == 0 ? "bb" : "ww");
									fseek(cfptr, (elementTurn.id - 1) * sizeof(struct elementData), SEEK_SET);
									strcpy(elementTurn.value, turn);
								
									fwrite(&elementTurn, sizeof(struct elementData), 1, cfptr);
											
									isLegal = 1;
									
									break;
								}else{
									printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
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
					if(element.x == a && element.y == y && strcmp(element.value , "- ") == 0){
						strcpy(element.value, turn);
					
					
						rewind(cfptr);
						while(! feof(cfptr)){
							fread(&element2, sizeof(struct elementData), 1, cfptr);
							
							// find id of element that at coordinate xy  (location)
							if(element2.x == x && element2.y == y){
								
								if(strcmp(element2.value , turn) == 0 || strcmp(element2.value , turnChecker) == 0){
									
									// change value of destiation if player using checker
									strcpy(element.value , strcmp(element2.value , turnChecker) == 0 ? turnChecker : turn);
							
									
									// write new value of old location to file
									fseek(cfptr, (element2.id - 1) * sizeof(struct elementData), SEEK_SET);
									strcpy(element2.value , "- ");
									
									fwrite(&element2, sizeof(struct elementData), 1, cfptr);
									
									// write new location value to file
									fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
									fwrite(&element, sizeof(struct elementData), 1, cfptr);	
									
									// set turn and write new turn value to file also set anti-turn to old value of turn
									strcpy(anti_turn, turn);
									strcpy(anti_turnChecker, turnChecker);
											
									strcpy(turn, strcmp(turn , "w ") == 0  ? "b " : "w ");
									strcpy(turnChecker, strcmp(turnChecker , "ww") == 0 ? "bb" : "ww");
									fseek(cfptr, (elementTurn.id - 1) * sizeof(struct elementData), SEEK_SET);
									strcpy(elementTurn.value, turn);
									
	
									
									fwrite(&elementTurn, sizeof(struct elementData), 1, cfptr);
									
									isLegal = 1;
									break;
								}else{
									printf("\nWrong player tried to play, please wait till -> %s <- player make its move\n", turn);	
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
	
	if(isLegal == 0){
		printf("\nForbidden Movement Try Again\n");
	}else{
		y++;
		b++;
		fprintf(logs, "%s:%c%d%c%d\n", turn, x, y, a, b);
		
		printf("\nTeam %s please enter your command\n", turn);
	}
	
}

void getTurn(){
	
	FILE *cfptr;
	struct elementData element = {0, 'A', 1, "w "}; // default values
	
	if((cfptr = fopen("element.dat", "rb")) == NULL){
		printf("\nFile could not open\n");
	}else{
		
		while(! feof(cfptr)){
			fread(&element, sizeof(struct elementData), 1, cfptr);
			
			if(element.id == 65){
				strcpy(turn, element.value);
				
			
				if(turn == "w "){
					strcpy(turnChecker , "ww");
					strcpy(anti_turnChecker , "bb");
					strcpy(anti_turn, "b ");
				}else{
					strcpy(turnChecker , "bb");
					strcpy(anti_turnChecker , "ww");
					strcpy(anti_turn, "w ");
				}
				//printf("TURN = %c\n", turn);
				break;
			}
		}
	}
}


void readFile(){
	
	FILE *cfptr;
	
	struct elementData element = {0, 'A', 2, "w "};
	
	if((cfptr = fopen("element.dat", "rb")) == NULL){
		printf("\nFile could no open\n");
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
			
			printf("%s ", element.value);
			//printf("%d  %c %d %c \n", element.id, element.x, element.y, element.value);
			
			counter++;
			
			if(counter == 65){
				break;
			}
		}
		
		printf("  \nA  B  C  D  E  F  G  H\n");
		
		fclose(cfptr);
	}
}



