#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node{
	char move;
	struct Node* next;	
};

//functions that make the text pretty
void green(){
	printf("\033[0;32m");
}

void red(){
	printf("\033[1;31m");
}

void blue(){
        printf("\033[0;34m");
}

void white(){
	printf("\033[0;37m");
}

void yellow(){
	printf("\033[0;33m");
}

//reverses the user input so it gives the illusion
//that you move the number tile not the empty tile 
char Reverse(char dir){
	if(dir=='a') return 'd'; 
	if(dir=='d') return 'a';
	if(dir=='w') return 's';
	if(dir=='s') return 'w';
}


//switches the empty cell with another one given the instruction
void SwitchTile(int n,int (*arr)[n],int *x, int *y,char dir){
	int aux,i,j;
	if(dir=='a'){
		if(*y>0){
			aux=arr[*x][*y];  
			arr[*x][*y]=arr[*x][*y-1]; 
			arr[*x][*y-1]=aux;
			(*y)--;
		}
		else return;
	}
	else if(dir=='d'){
		if(*y<n-1){
			aux=arr[*x][*y];  
			arr[*x][*y]=arr[*x][*y+1]; 
			arr[*x][*y+1]=aux;
			(*y)++;
		}
		else return;
	}
	else if(dir=='w'){
		if(*x>0){
			aux=arr[*x][*y];  
			arr[*x][*y]=arr[*x-1][*y]; 
			arr[*x-1][*y]=aux;
			(*x)--;
		}
		else return;
	}
	else if(dir=='s'){
		if(*x<n-1){
			aux=arr[*x][*y];  
			arr[*x][*y]=arr[*x+1][*y]; 
			arr[*x+1][*y]=aux;
			(*x)++;
		}
		else return;
	}

	
}

//displays the board and the instructions
//in the array there is the n^2 cell, which is represented as the empty tile
void DisplayBoard(int n,int arr[n][n]){
	int i,j,k;
	system("clear");
	white();
	printf("left - A     right - D     up - W     down - S     undo - U    restart - R     save - F     quit to main menu - Q\n\n\n");
	for(i=0;i<n;i++){
		blue();
		for (k=0;k<n*8;k++) printf("-");
		printf("\n");
		for(j=0;j<n;j++)printf("|       ");
		printf("|\n");
       	for(j=0;j<n;j++){
			blue();
			if(arr[i][j]==n*n) printf("|       ");
            else if(arr[i][j]<10) printf("|   %d   ",arr[i][j]);
			else printf("|  %d %d  ",arr[i][j]/10,arr[i][j]%10);
		}
		blue();
		printf("|\n");
		for(j=0;j<n;j++)printf("|       ");
		printf("|\n");
	}
	for (k=0;k<n*8;k++) printf("-");
	printf("\n");
}


//checks every iteration if the game has been finished
//by verifying that the difference between each consecutive cell is 1
//if that is the case, it displays the board in a winningly green colour
int GameWon(int n,int arr[n][n]){
	int i,j,aux=0,k,comp;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++){
			if(arr[i][j]-aux!=1) return 0;
			aux=arr[i][j];
		}

	system("clear");
	white();
	green();
	for(i=0;i<n;i++){
		for (k=0;k<n*8;k++) printf("-");
		printf("\n");
		for(j=0;j<n;j++)printf("|       ");
		printf("|\n");
       	for(j=0;j<n;j++){
			if(arr[i][j]==n*n) printf("|       ");
            else if(arr[i][j]<10) printf("|   %d   ",arr[i][j]);
			else printf("|  %d %d  ",arr[i][j]/10,arr[i][j]%10);
		}
		printf("|\n");
		for(j=0;j<n;j++)printf("|       ");
		printf("|\n");
	}
	for (k=0;k<n*8;k++) printf("-");
	printf("\n\n YOU WON!\n\n\n");

	return 1;
}


//list is used for the undo feature, stores user moves
void InsertNode(struct Node** head_ref, char dir){
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	struct Node* last = *head_ref;

	new_node->move = dir;
	new_node->next = NULL;

	if (*head_ref == NULL){
		*head_ref = new_node;
		return;
	}

	while (last->next != NULL) last = last -> next;
	last->next = new_node;
	return;
}

//if the user undos a move, it removes given move from the storage
void DeleteNode(struct Node** head_ref){
	if(*head_ref != NULL) {
    
    	if((*head_ref)->next == NULL) {
    		*head_ref = NULL;
    	} 
		else {
      		struct Node* temp = *head_ref;
      		while(temp->next->next != NULL)
        	temp = temp->next;
     
      		struct Node* last = temp->next;
      		temp->next = NULL;
      		free(last); 
    	}
  	}
}

//retrieves the final move made by the user for undo
char FinalMove(struct Node** head_ref){
	struct Node* temp = *head_ref;
	while(temp->next!=NULL) temp = temp->next;
	return temp->move;
}

//starts the game whether loaded from a file, or a brand new one
void StartGame(int tok,int n){
	int i,j,x,y;
	int arr[n][n];
	struct Node* head = NULL;

	//if the user chooses to load the game, it reads the board data from savefile.txt
	if(tok){
		FILE* f;
		if((f = fopen("savefile.txt", "r"))!=NULL){
			fscanf(f, "%d",&n);
			for(i=0;i<n;i++)
				for(j=0;j<n;j++){
					fscanf(f, "%d",&arr[i][j]);
					if(arr[i][j]==n*n){x=i;y=j;}
				}
		}
		fclose(f);
	}


	//otherwise, it creates the board by randomizing it
	else{
		system("clear");
	

		//initializing the solved board
		int k=1,a=0,b=0;
		for(i=0;i<n;i++)
			for(j=0;j<n;j++){
				arr[i][j]=k;
				
				k++;
			}
		
		//randomizing the board by randomly moving the empty space
		//this way a solution is always ensured
		//no impossible casses are admitted
		int rnd;
		x=n-1,y=n-1;
		
		
		for(i=0;i<100;i++){
			rnd = clock()%4;
			if(rnd==0) SwitchTile(n,arr,&x,&y,'a');
			else if(rnd==1) SwitchTile(n,arr,&x,&y,'w');
			else if(rnd==2) SwitchTile(n,arr,&x,&y,'s');
			else if(rnd==3) SwitchTile(n,arr,&x,&y,'d');
			system("clear");
		}

	}

	DisplayBoard(n,arr);
	char v;

	//keeps the game itself going as long as the user has not finished it
	//the screen is cleared and the updated board is printed each time
	while(!GameWon(n,arr)){
		
		input_eval2:
		white();
		printf("User Input: ");
		scanf(" %c",&v);
		if(v=='q'){system("clear");return;}
		if(v=='r'){StartGame(0,n);return;}
		//saves game in file hopefully
		if(v=='f') {
			FILE* f;
			if((f = fopen("savefile.txt", "w"))==NULL) printf("Error");
			fprintf(f, "%d\n",n);
			for(i=0;i<n;i++)
				for(j=0;j<n;j++)
					fprintf(f, "%d ",arr[i][j]);

			fclose(f);
			printf("\nGAME SAVED\n\n");
			continue;
		}
		if(v=='u'){
			if(head!=NULL){
				system("clear");
				SwitchTile(n,arr,&x,&y,FinalMove(&head));
				DeleteNode(&head);
				DisplayBoard(n,arr);
				continue;
			}
			
		}

		
		InsertNode(&head,v);
		v = Reverse(v);
		if(v!='a' && v!='w' && v!='s' && v!='d' && v!='f') {
			red(); 
			printf("please input a valid token\n"); 
			goto input_eval2;
		}
		SwitchTile(n,arr,&x,&y,v);
		DisplayBoard(n,arr);


	}

}

//displays the main menu and all its options
void MainMenu(){
	green();
	printf("NEW GAME (n)\n");
	printf("LOAD GAME (l)\n");
	red();
	printf("EXIT (any other key)\n\n\n\n");\
	
	char v;

	white();
	int n;
	printf("User Input: ");
    scanf(" %c",&v);
	if(v=='n'){
		system("clear");
		blue();
		printf("Input puzzle size (3-10)\n");

		input_eval:
		white();
		printf("User Input: ");
		scanf("%d",&n);
		if(n<3 || n>50) {
			red(); 
			printf("please input a valid number\n"); 
			goto input_eval;
		}	
		StartGame(0,n);
	}
	else if (v=='l'){
		FILE* f;
		if((f = fopen("savefile.txt", "r"))==NULL)
			printf("\nThere is no saved game\n");
		else{
        	fscanf(f, "%d",&n);
			StartGame(1,n);
		}
		fclose(f);
	}
	else {system("clear");return; }
	
	MainMenu();
}

int main(){
	system("clear");
	MainMenu();
	return 0;
}
