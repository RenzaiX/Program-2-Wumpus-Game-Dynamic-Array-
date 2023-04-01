/*
	Program 2: Wumpus(Dynamic array)
	CS 211, UIC, Fall 2022
    System: Windows using Visual Studio Code
    Author: Mykola Turchak

	Hunt the Wumpus is a classic text-based adventure game by Gregory Yob from 1972.
	The Wumpus lives in a completely dark cave of 20 rooms.  Each room has 3 tunnels leading to other rooms.
	This program implements this game, without superbats or arrows, using a fixed size array.
*/

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
	int moveNumber;  // Counts up from 1, incrementing for each move
	int personRoom;  // Room 1..20 the person currently is in
	int wumpusRoom;  // Room 1..20 the Wumpus is in
	int pitRoom1;    // Room 1..20 the first pit is in
	int pitRoom2;    // Room 1..20 the second pit is in
    int batRoom1;    // Room 1..20 the first bat is in
    int batRoom2;    // Room 1..20 the second bat is in
    int arrow;       // Room 1..20 the arrow is in
};

// Function prototype needed to allow calls in any order between
//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *personIsAlive, int (*Rooms)[][3]);

//--------------------------------------------------------------------------------
void displayCave()
{
    printf( "\n"
			"       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}


//--------------------------------------------------------------------------------
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
        	"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
        	"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
        	"   fly you to some random room, which could be troublesome.  Then those bats go \n"
        	"   to a new room different from where they came from and from the other bats.   \n"
    		"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
        	"   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
        	"    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
        	"    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
        	"    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
        	"    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
        	"    room, he snaps your neck and you die!                                       \n"
        	"                                                                                \n"
        	"Moves:                                                                          \n"
        	"On each move you can do the following, where input can be upper or lower-case:  \n"
        	"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
        	"   then a room number.                                                          \n"
        	"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
        	"   you specify.  Your arrow ends up in the final room.                          \n"
        	"   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
        	"   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
        	"   by spaces. If an arrow can't go a direction because there is no connecting   \n"
        	"   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
        	"   continues doing this until it has traveled the designated number of rooms.   \n"
        	"   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
        	"   automatically pick up the arrow if you go through a room with the arrow in   \n"
        	"   it.                                                                          \n"
        	"3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
        	"4. Enter 'C' to cheat and display current board positions.                      \n"
        	"5. Enter 'D' to display this set of instructions.                               \n"
        	"6. Enter 'P' to print the maze room layout.                                     \n"
        	"7. Enter 'X' to exit the game.                                                  \n"
        	"                                                                                \n"
        	"Good luck!                                                                      \n"
        	" \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,      // New number we're checking
                 int randomNumbers[],  // Set of numbers previously found
                 int limit)            // How many numbers previously found
{
    int returnValue = false;
    
    // compare random value against all previously found values
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;   // It is already there
            break;
        }
    }
    
    return returnValue;
} 


//--------------------------------------------------------------------------------
// Fill this array with unique random integers 1..20
void setUniqueValues(int randomNumbers[],   // Array of random numbers
                     int size,              // Size of random numbers array
                     int startFrom)         //element in array where we want to start creating unique elements
{
    int randomValue = -1;
    
    for( int i = startFrom; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   // random number 1..20
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
} 


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)   // All game settings variables
{
    // Array of 6 unique values 1..20, to be used in initializing cave hazards locations
    int randomNumbers[7];
    // Select some unique random values 1..20 to be used for 2 bats rooms, 2 pits rooms, Wumpus room, and initial player room
    setUniqueValues(randomNumbers, 7, 0);
    // Use the unique random numbers to set initial locations of hazards, which should be non-overlapping.
    gameInfo->personRoom = randomNumbers[0];
    gameInfo->wumpusRoom = randomNumbers[1];
    gameInfo->pitRoom1 = randomNumbers[2];   
    gameInfo->pitRoom2 = randomNumbers[3];
    gameInfo->batRoom1 = randomNumbers[4];
    gameInfo->batRoom2 = randomNumbers[5];
    gameInfo->arrow = randomNumbers[6];

    gameInfo->moveNumber = 1;
}// end initializeBoard(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[ 3],     // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
    return( tunnels[0] == nextRoom ||
            tunnels[1] == nextRoom ||
            tunnels[2] == nextRoom
          );
}


//--------------------------------------------------------------------------------
// Display where everything is on the board.
void displayCheatInfo(struct GameInfo gameInfo)
{
    printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %6d %4d %4d %5d %5d %5d \n\n", 
			gameInfo.personRoom,
			gameInfo.wumpusRoom,
			gameInfo.pitRoom1,
			gameInfo.pitRoom2,
            gameInfo.batRoom1,
            gameInfo.batRoom2,
            gameInfo.arrow
		);
}// end displayCheatInfo(...)


//--------------------------------------------------------------------------------
// Display room number and hazards detected
void displayRoomInfo( struct GameInfo gameInfo, int (*Rooms)[][3])
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.personRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = (*Rooms)[currentRoom][0];
    int room2 = (*Rooms)[currentRoom][1];
    int room3 = (*Rooms)[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) 
    {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room || room2 == pit1Room || room2 == pit2Room || room3 == pit1Room || room3 == pit2Room) 
    {
        printf("You feel a draft. ");
    }

    // Display hazard detection message if a bat is in an adjacent room
    int bat1Room = gameInfo.batRoom1;
    int bat2Room = gameInfo.batRoom2;
    if( room1 == bat1Room || room1 == bat2Room || room2 == bat1Room || room2 == bat2Room || room3 == bat1Room || room3 == bat2Room) 
    {
        printf("You hear rustling of bat wings. ");
    }
    
    printf("\n\n");
}//end displayRoomInfo(...)


//--------------------------------------------------------------------------------
// If the player just moved into a room with a pit, the person dies.
// If the person just moved into the Wumpus room, then if the room number is odd
// the Wumpus moves to a random adjacent room.
// If the person jist moved into a room with a bat, the person flies to random room
void checkForHazards(
         struct GameInfo *gameInfo, // Hazards location and game info
         bool *personIsAlive,       // Person is alive, but could die depending on the hazards
         int (*Rooms)[][3])        // all of the rooms
{
    // retrieve the room the person is in
    int personRoom = gameInfo->personRoom;
    int randomNumbers[8] = {gameInfo->batRoom1, gameInfo->batRoom2, gameInfo->pitRoom1, gameInfo->pitRoom2, gameInfo->personRoom, gameInfo->arrow, gameInfo->wumpusRoom, 21};
    
    // Check whether there is a pit
    if( personRoom == gameInfo->pitRoom1 || personRoom == gameInfo->pitRoom2) {
        // Person falls into pit
        printf("Aaaaaaaaahhhhhh....   \n");
        printf("    You fall into a pit and die. \n");
        *personIsAlive = false;
        return;
    }
    
    // Check for the Wumpus
    if( personRoom == gameInfo->wumpusRoom) {
        // Wumpus moves if it is currently in an odd-numbered room, and it moves into the lowest-numbered adjacent room. 
        if( gameInfo->wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            printf( "You hear a slithering sound, as the Wumpus slips away. \n"
					"Whew, that was close! \n");
            gameInfo->wumpusRoom = (*Rooms)[ personRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					"It is over.\n");
            *personIsAlive = false;
            return;
        }
    }

    // Check for the bats 
    if(personRoom == gameInfo->batRoom1 || personRoom == gameInfo->batRoom2)
    {
        //person is moved to a different room
        setUniqueValues(randomNumbers, 8, 7);
        gameInfo->personRoom = randomNumbers[7];
        randomNumbers[4] = randomNumbers[7];                    // change the personRoom to new random number in the array

        // check which bat is in the player's room to randomize the room
        if(personRoom == gameInfo->batRoom1)
        {
            setUniqueValues(randomNumbers, 8, 7);
            gameInfo->batRoom1 = randomNumbers[7];
            randomNumbers[0] = randomNumbers[7];                // change the batRoom1 to new random number in the array
        }
        else if (personRoom = gameInfo->batRoom2)
        {
            setUniqueValues(randomNumbers, 8, 7);
            gameInfo->batRoom2 = randomNumbers[7];
            randomNumbers[1] = randomNumbers[7];                // change the batRoom2 to new random number in the array
        }
        printf("Woah... you're flying! \n");
        printf("You've just been transported by bats to room %d.\n", gameInfo->personRoom);
        
    }

    // Check for arrow
    if(personRoom == gameInfo->arrow) 
    {
        printf("Congratulations, you found the arrow and can once again shoot.\n");
        gameInfo->arrow = -1;
    }

}//end checkForHazards(...)


//--------------------------------------------------------------------------------
// Check if the player has an arrow
// Make sure user doesn't enter more that 3 rooms
// Check if the room the arrow shoots at is adjacent, if not shoot to the lowest adjacent Room to the one chosen by user
// Check if anyone was killed

void shootArrow(struct GameInfo *gameInfo, // Hazards location and game info
                bool *personIsAlive,       // Person is alive, but could die depending on the arrow
                bool *wumpusIsAlive,       // Wumpus is alive, but could die depending on the arrow    
                int (*Rooms)[][3])         // all of the rooms
{
    int *arrowDirection;
    int arrowDistance;

    // Attempt to shoot arrow you don't have
    if(gameInfo->arrow != -1)
    {
        printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
        return;
    }

    // Prompt user to enter input
    printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
    scanf(" %d", &arrowDistance);

    // User enters more than 3 rooms to shoot arrow through
    if(arrowDistance > 3)
    {
	    printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
        arrowDistance = 3;
    }

    // allocating memory for all the rooms user wants to shoot at
    arrowDirection = (int*)malloc(arrowDistance * sizeof(int));

    //take the input
    for(int i = 0; i< arrowDistance; i++)
    {
        scanf(" %d", &arrowDirection[i]);
    }

    gameInfo->arrow = gameInfo->personRoom;
    //go through all of the rooms entered by user
    for (size_t i = 0; i < arrowDistance; i++)
    {
        //check if the room is adjacent
        if(roomIsAdjacent((*Rooms)[ gameInfo->arrow], arrowDirection[i]))
        {
            gameInfo->arrow = arrowDirection[i];
            //Check for person in the room
            if(gameInfo->arrow == gameInfo->personRoom)
            {
                // Arrow killed player
                printf( "You just shot yourself.  \n"
                        "Maybe Darwin was right.  You're dead.\n");
                *personIsAlive = false;
                return;
            }
            //Check for wumpus in the room
            else if(gameInfo->arrow == gameInfo->wumpusRoom)
            {
                // Arrow killed Wumpus
                printf( "Wumpus has just been pierced by your deadly arrow! \n"
                        "Congratulations on your victory, you awesome hunter you.\n");
                *wumpusIsAlive = false;
                return;
            }
        }
        // Attempt to shoot arrow through room that is not adjacent
        else
        { 
            printf("Room %d is not adjacent.  Arrow ricochets...\n", arrowDirection[i]); 
            gameInfo->arrow = (*Rooms)[ gameInfo->arrow][ 0];                                //move to lowest adjacent room
            //Check for person in the room
            if(gameInfo->arrow == gameInfo->personRoom)
            {
                // Arrow ricochets, and kills person
                printf( "You just shot yourself, and are dying.\n"
                        "It didn't take long, you're dead.\n");
                *personIsAlive = false;
                return;
            }
            //Check for wumpus in the room
            else if(gameInfo->arrow == gameInfo->wumpusRoom)
            {
                // Arrow ricochets, and kills Wumpus
                printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                        "Accidental victory, but still you win!\n");
                *wumpusIsAlive = false;
                return;
            }
        }
    } 
    gameInfo->wumpusRoom = (*Rooms)[gameInfo->wumpusRoom][0];
}




//--------------------------------------------------------------------------------
// Prompt for and reset the positions of the game hazards and the person's location,
// useful for testing.  No error checking is done on these values.
void resetPositions(struct GameInfo *theGameInfo)
{
    printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");
	scanf(" %d %d %d %d %d %d %d", &theGameInfo->personRoom, &theGameInfo->wumpusRoom, 
                          &theGameInfo->pitRoom1, &theGameInfo->pitRoom2, 
                          &theGameInfo->batRoom1, &theGameInfo->batRoom2, 
                          &theGameInfo->arrow);
    printf("\n");
}


//--------------------------------------------------------------------------------
int main(void) 
{
    struct GameInfo gameInfo;   // Used to more easily pass game info variables around
    bool personIsAlive = true;  // Used in checking for end of game
    bool wumpusIsAlive = true;  // Used in checking for end of game
    char typeOfMove;            // Used to handle user input letter
    int nextRoom;               // User input of destination room number, used on a 'M' type move
    int row = 21, col = 3;      // setting the size for the row and column in 2d array

    // Room connection values are set as global constants, and don't need to be passed.
    // The rooms array is indexed from 0..22, but we don't use the 0th entry and 
    // only use 1..20.  This approach helps prevent off-by-one indexing errors.

    int roomNums[ 21][ 3] = {
      // Room 0 is unused, to help avoid off-by-one indexing errors.
                            {0,0,0},
      //                    Room 1     Room 2      Room 3      Room 4      Room 5
      /* Rooms 1-5   */     {2,5,8},  {1,3,10},   {2,4,12},   {3,5,14},    {1,4,6},
      //                    Room 6     Room 7      Room 8      Room 9     Room 10
      /* Rooms 6-10  */    {5,7,15},  {6,8,17},    {1,7,9},  {8,10,18},   {2,9,11},
      //                   Room 11    Room 12     Room 13     Room 14     Room 15
      /* Rooms 11-15 */  {10,12,19}, {3,11,13}, {12,14,20},  {4,13,15},  {6,14,16},
      //                   Room 16    Room 17     Room 18     Room 19     Room 20
      /* Rooms 16-20 */  {15,17,20}, {7,16,18},  {9,17,19}, {11,18,20}, {13,16,19},
    };

    // allocating memory for the 2d array
    int (*Rooms)[row][col] = malloc(sizeof *Rooms);
    
    // taking values from the 2d array and putting them into allocated array
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            (*Rooms)[i][j] = roomNums[i][j];

    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);
    
    // Set random initial values for person, Wumpus, bats and pits
    initializeGame( &gameInfo);

	// Main playing loop.  Break when player dies, or player kills Wumpus
    while (personIsAlive && wumpusIsAlive) 
    {  
		// Display current room information: Room number, hazards detected
		displayRoomInfo( gameInfo, &(*Rooms));
		
		// Prompt for and handle move
		printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
		scanf(" %c", &typeOfMove);						
		typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
		
		// Check all types of user input and handle them.  This uses if-else-if code
		//   rather than switch-case, so that we can take advantage of break and continue.
		if( typeOfMove == 'D') 
        {
			displayCave();
			displayInstructions();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'P') 
        {
			// display the cave layout
			displayCave();
			continue;       // Loop back up to reprompt for the same move
		}
		else if( typeOfMove == 'M') 
        {
			// Move to an adjacent room,
			scanf(" %d", &nextRoom);
			
			if( roomIsAdjacent( (*Rooms)[ gameInfo.personRoom], nextRoom)) 
            {
				gameInfo.personRoom = nextRoom;        // move to a new room
				// Check if pit or wumpus is present in this new room
				checkForHazards(&gameInfo, &personIsAlive, &(*Rooms));
			}
			else 
            {
				printf("Invalid move.  Please retry. \n");
				continue;        // Doesn't count as a move, so retry same move.
			}
		}
        else if( typeOfMove == 'S')
        {
            shootArrow(&gameInfo, &personIsAlive, &wumpusIsAlive, &(*Rooms));
            
        }
		else if( typeOfMove == 'C') 
        {
			// Display Cheat information
			displayCheatInfo(gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'R') 
        {
			// Specify resetting the hazards and person positions, useful for testing
			resetPositions( &gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
        else if( typeOfMove == 'G') 
        {
			// Prompt for room number guess and see if user has found the Wumpus
            int wumpusRoomGuess = 0;
			printf("Enter room (1..20) you think Wumpus is in: ");
            scanf(" %d", &wumpusRoomGuess);
            // See if userguess was correct, for a win or loss.
            if( gameInfo.wumpusRoom == wumpusRoomGuess) 
            {
                printf("You won!\n");
            }
            else
            {
                printf("You lost.\n");
            }
			break;
		}
		else if( typeOfMove == 'X') 
        {
			// Exit program
			personIsAlive = false;   // Indicate person is dead as a way to exit playing loop
			break;
		}

		// Increment the move number
		gameInfo.moveNumber = gameInfo.moveNumber + 1;
        
    }//end while(personIsAlive && wumpusIsAlive)
    
    printf("\nExiting Program ...\n");
	free(Rooms);
	return 0;
}