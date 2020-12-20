//  Project03
//  Created by Shopnil Shahriar


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Ships:
//Carrier  5
//Battleship - 4
//Cruiser - 3
//Submarine - 3
//Destroyer - 2

struct node{
    char letterCoordinate;
    int numCoordinate;
    char shipType[15];
    struct node* next;
};

const int SIZE = 10;
int row = 0;
int col = 0;
int hitCounter = 0;

void updateStateWorld(char number, int letter, char board[SIZE][SIZE], struct node *head, struct node *temporaryNode);
int displayStateWorld(char board[SIZE][SIZE], struct node *head, int hitCounter, struct node *temporaryNode);
struct node *createNode(char lCoordinate, int nCoordinate, int sType);
void printlist(struct node *head);


void initialization(char board[SIZE][SIZE]){
    int i = 0;
    int j = 0;
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            board[i][j] = (char *)malloc(sizeof(char));
    //Dynamically allocates a byte for gameboard
    
    for(i = 0; i < SIZE; i++)
        for(j = 0; j < SIZE; j++)
            board[i][j] = '0'; //Populates the game with '0' characters
    
    
    srand(time(NULL)); //for randomization
    int numShips = 1, sType = 4, sFlag = 0, direction = -1;
    int r = -1, c = -1;
    //char ships[] = {'C','B','F','S','D'};
    //int count = 0;
    
    while(numShips <= 5){
        char ship = numShips + '0';
        direction = rand() % 2; //Determine a direction: either up or down or left/right
        while(!(board[r][c] == '0')){
            r = rand() % 10; //Randomizing rows and column for starting position
            c = rand() % 10;
        }
        if(direction == 1){ //In the verticle direction to check if there is space upwards
            if(board[r][c + sType] == '0'){
                for(i = 0; i <= sType; i++) //To fill up space on the board
                    board[r][c+i] = ship;
            }
            else if(board[r][c-sType] == '0'){ //To check if there is space downwards
                for(i = 0; i <= sType; i++)
                    board[r][c-i] = ship;
            }
        }
        else{ //if verticle is not possibe, then horizontal
            if(board[r + sType][c] == '0'){ //to fill in the space in the right
                for(i = 0; i <= sType; i++){
                    board[r+i][c] = ship;
                }
            }
            else if(board[r-sType][c] == '0'){ //if there is no space to the right, then left
                for(i = 0; i < sType; i++){
                    board[r-i][c] = ship;
                }
            }
            else{
                printf("Can't find a place to put %d\n", sType);
            }
        }
        if(sType == 2 && sFlag == 0){
            sType = 2;
            sFlag = 1;
        }
        else{
            sType--;
        }
        numShips++;
    }
    //displayStateWorld(board); //to display the board which contains random ships
}

void teardown(char board[SIZE][SIZE], struct node *head){
    printf("Game over! Clearing the battleship board!\n");
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            board[i][j] = NULL; //Clearing the board
        }
    }
    FILE *fPtr;
    fPtr = fopen("battleship.txt", "w"); //Open for writing
    if(fPtr == NULL){
        printf("Unable to create a file!");//If file is not created, this is printed
    }
    struct node *tempNode = head;
    printf("%d", tempNode -> letterCoordinate);
    printf("%c", tempNode -> numCoordinate);
    printf("%s", tempNode -> shipType);
    while(tempNode != NULL){
        fprintf(fPtr, "Fired at %d ", tempNode->letterCoordinate); //First letter coordinate fired at
        fprintf(fPtr, "%c", tempNode->numCoordinate); //Second number coordinate fired at
        fprintf(fPtr, "- %s.\n", tempNode -> shipType); //Type of ship fired at
        tempNode = tempNode->next; //Pointer to the next node
    }
    while(head != NULL){
        tempNode = head;
        head = head->next;
        //free(tempNode);
    }
    fclose(fPtr);
}

void acceptInput(char board[SIZE][SIZE], struct node *head, struct node *temporaryNode){
  int number;
  char letter;
  int x = 1; //Variable used as a flag for the loops
  while(x = 1){
      printf("Enter a letter between A-J:\n");
      scanf("%c", &letter); //Takes a character input from user
      letter = tolower(letter); //Prompt checks if the character is valid or not
      if(!(letter >= 'a' && letter <= 'j')){
          printf("Invalid Letter!\n");
          fflush(stdin);
          //Since 'enter' is also taken as an input, it clears it out
      }
      else{
          x = 2;
          break;
      }
  }
  while(x == 2){
      printf("Enter a number between 0-9:\n");
      scanf("%d", &number); //Take a number input from the user
      if(number < 0 || number > 9){ //Prompt checks if the number is valid
          printf("Invalid Number!\n");
      }
      else{
          x = 3;
          break;
      }
  }
    updateStateWorld(letter, number, board, &head, &temporaryNode); //Sends the input to the function
}


void updateStateWorld(char letter, int number, char board[SIZE][SIZE], struct node *head, struct node *temporaryNode){
    int type;
    char tempLetter = letter;
    printf("You have entered %c%d\n", letter, number);//Prints the user input
    int a = 97; //This is the ascii value for 'a'
    int counter = 0; //Array reference
    int b = (int)letter; //Stores the letter in ascii value
    for(int i = 0; i < SIZE; i++){
        if(b != a){
            a++; //Code to figure out what the ascii value is
            counter++;
        }
    }
    col = counter;
    row = number;
    
    if(board[col][row-1] == '0'){ //Detecting if it is a miss or a hit
        printf("It is a miss!\n");
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                board[col][row-1] = 'M';
            }
        }
    }
    else{
        printf("It is hit!\n");
        hitCounter++;
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j < SIZE; j++){
                board[col][row-1] = 'H';
            }
        }
        if(board[col][row-1] == '1'){
            printf("You hit a Carrier!\n");
            type = 1;
        }
        else if(board[col][row-1] == '2'){
            printf("You hit a Battleship!\n");
            type = 2;
        }
        else if(board[col][row-1] == '3'){
            printf("you hit a Cruiser!\n");
            type = 3;
        }
        else if(board[col][row-1] == '4'){
            printf("You hit a Submarine!\n");
            type = 4;
        }
        else{
            printf("You hit a Destroyer!\n");
            type = 5;
        }
    }
    temporaryNode = createNode(tempLetter, number, type); //Puts the data into the node
    temporaryNode->next=head;
    head = temporaryNode;
}
    
int displayStateWorld(char board[SIZE][SIZE], struct node *head, int hitCounter, struct node *temp){
    for(int i = 0; i < SIZE; i++){
        for(int  j = 0; j < SIZE; j++){
            printf("%c ", board[i][j]); //Uses a nested for loop to print out the board
        }
        printf("\n"); //Prints a new line as the rows finish
    }
    if(hitCounter == 17){
        return 0;
    }
    else{
        return 1;
    }
}

struct node *createNode(char lCoordinate, int nCoordinate, int sType){ //Function to create nodes
    struct node *returnNode = malloc(sizeof(struct node));
    returnNode->letterCoordinate = lCoordinate;
    returnNode->numCoordinate = nCoordinate;
    switch(sType){
        case 0: strcpy(returnNode->shipType, "Miss!");
            break;
        case 1: strcpy(returnNode->shipType, "Carrier!");
            break;
        case 2: strcpy(returnNode->shipType, "Battleship!");
            break;
        case 3: strcpy(returnNode->shipType, "Cruiser!");
            break;
        case 4: strcpy(returnNode->shipType, "Submarine!");
            break;
        case 5: strcpy(returnNode->shipType, "Destroyer!");
            break;
    }
    return returnNode;
}


int main() {
    int setFlag = 1;
    struct node *head = NULL;
    struct node *temporaryNode;
    char *board[SIZE][SIZE];//Initialized a pointer of arrays which will be the gameboard
    initialization(board);//Sent the gameboard to the function to set it up
    while(setFlag == 1){
        acceptInput(board, &head, &temporaryNode);
        fflush(stdin);
        int returnValue = displayStateWorld(board, &head, hitCounter, temporaryNode);
        if(returnValue = 0){
            setFlag = 0; //End the loop and
        }
    }
    teardown(board, &head);
}
