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
struct elementData elementTurn = {65, 'A', 9, 'w'};

void readFile();
void updateFile();

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
	

	
	// update
/*	if((cfptr = fopen("credit.dat", "rb+")) == NULL){
		printf("nope");
	}else{
		int oldY = 0; //start value to be different from y;
		
		rewind(cfptr);

		while(! feof(cfptr)){
			fread(&element, sizeof(struct elementData), 1, cfptr);
			
			if(element.x == 'B' && element.y == 5){
				fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
				element.value = 'k';
				//printf("%c", element.value);
				fwrite(&element, sizeof(struct elementData), 1, cfptr);
				//printf("%d", element.id);
				break;
			}
		}
		
		fclose(cfptr);
	}*/
	
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
	int y,b;
	
	printf("\n\nPlease enter location then destionation(ex. A1B2)\n");
	scanf("%c%d%c%d", &x, &y, &a, &b);
	
	int indexOfX = getIndexOfChar(x);
	int indexOfA = getIndexOfChar(a);
	
	
	
	if(y < 0 || b < 0 || abs(y-b) > 1 || abs(indexOfX - indexOfA) > 1){
		printf("\nPlease enter valid values according to rule of games.\n");
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
			
			rewind(cfptr);
			
			while(! feof(cfptr)){
				fread(&element, sizeof(struct elementData), 1, cfptr);
				
				// find id of element that at coordinate ab 
				if(element.x == a && element.y == b && element.value == '-'){
					element.value = turn;
				
					rewind(cfptr);
					while(! feof(cfptr)){
						fread(&element2, sizeof(struct elementData), 1, cfptr);
						
						// find id of element that at coordinate xy 
						if(element2.x == x && element2.y == y ){
							
							if(element2.value == turn){
								// write new value of destination to file
								fseek(cfptr, (element2.id - 1) * sizeof(struct elementData), SEEK_SET);
								element2.value = '-';
								fwrite(&element2, sizeof(struct elementData), 1, cfptr);
								
								// write new location value to file
								fseek(cfptr, (element.id - 1) * sizeof(struct elementData), SEEK_SET);
								fwrite(&element, sizeof(struct elementData), 1, cfptr);	
								
								// set turn and write new turn value to file
								turn = turn == 'w' ? 'b' : 'w';
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
			
	
			
			fclose(cfptr);
		}
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
			
			counter++;
			
			if(counter == 65){
				break;
			}
		}
		
		printf("  \nA B C D E F G H\n");
		
		fclose(cfptr);
	}
}


