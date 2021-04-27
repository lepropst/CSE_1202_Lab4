// Tic-tac-toe (T3) Network Game
// Jason Losh

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -o t3 t3.c udp.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp

#include "udp.h"

#define CLIENT_PORT 4096
#define SERVER_PORT 4097
const char ROWS[4] = {'A', 'B', 'C', '\0'};
void clearBoard(char board[3][3])
{
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            board[r][c] = '.';
}

// TODO: Add code to display the game board
void showBoard(char board[3][3])
{
	printf("Current Game Board:\n\n");
	for(int i = 0; i < 3; i++) {
		printf("\t");
		for(int j = 0; j < 3; j++) {
			printf("%c ", board[i][j]);
		}
	printf("\n");
	}
}

bool threeInARow(char row[3], char xo) {
	int checks = 0;
	for(int i = 0; i < 3; i++) {
		if(strcmp(&row[i], &xo) == 0) {
			checks++;
		}
	}
	if(checks == 3) {
		return true;
	} else {
		return false;
	}
}

// TODO: Add code to determine if a winner (xo) has occurred
bool isWinner(char board[3][3], char xo)
{
    bool win;
    char col1[3], col2[3], col3[3];
    for(int i = 0; i < 3; i++) {
	if(threeInARow(board[i], xo)) {
		win = true;
		return win;
	}
	for(int j = 0; j < 3; j++) {
		if(j == 0) col1[j] = board[i][j];
		if(j == 1) col2[j] = board[i][j];
		if(j == 2) col3[j] = board[i][j];
	}
    }

    if(threeInARow(col1, xo)) return true;
    if(threeInARow(col2, xo)) return true;
    if(threeInARow(col3, xo)) return true;

    if(strcmp(&board[1][1], &xo) ==0) {
    if(strcmp(&board[0][0], &xo) == 0) {
	    if(strcmp(&board[2][2], &xo) == 0) {
		    return true;
	    }
    }
    if(strcmp(&board[0][2], &xo) == 0) {
	    if(strcmp(&board[2][0], &xo) == 0) {
		    return true;
	    }
    }
}

    return win;
}

// TODO: Add code to test if an x or o (xo) is a valid move
//       and then record in the game board
bool addMove(char board[3][3], char move[], char xo)
{
    bool ok = strlen(move) == 2;
    return ok;
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    char *remoteIp;
    // tmp val to hold debugging value
    char str[100], str2[100];
    char board[3][3];
    bool validMove;
    bool winner = false;
    int moveCount = 0;

    bool myTurn;
    char myLetter, opponentLetter;
    int remotePort;

    // TODO: Verify arguments are valie
     bool goodArguments;
    if(argc == 3) {
	remoteIp = argv[1];	
	strcpy("accept", str);
	strcpy("invite", str2);   
	
	char *tmp = argv[2];
	if(strcmp(str, tmp) == 0) {
		strcpy("client", str);
		remotePort = CLIENT_PORT;
		goodArguments = true;
	} else if(strcmp(str, tmp) == 0) {
		strcpy("server", str2);
		remotePort = SERVER_PORT;
		goodArguments = true;
	} else {
		printf("invalid role for application:\t%s\nQUITTING NOW\n", argv[2]);
		goodArguments = false;
	}

    }
    if (!goodArguments)
    {
        printf("usage: t3 IPV4_ADDRESS ROLE\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        printf("  ROLE is either invite or accept\n");
        exit(EXIT_FAILURE);
    } else {
	    bool openListener = openListenerPort(remoteIp, remotePort);
    }

    // TODO: Determine if client or server
    // A server will wait to accept an invitation to play
    // A client will send an invitation to play
    // DETERMINED ABOVE IN GOOD ARGUMENTS. ROLE AS CLIENT/SERVER IS STORED IN {ROLE} 
    // TODO: Open listener port number dependent on client/server role
    // TODO: Determine remote port that you will send data to
    //       If you are server, send to client port, and vice versa
	if(remotePort == CLIENT_PORT) {
	//client, initiate with sendData
		printf("client");
		//sendData(remoteIp, remotePort, str);
	} else if(remotePort == SERVER_PORT) {
		//server use RecieveData
		printf("server");
	//	RecieveData(str2, 100);
	}
	
    // Setup game
    clearBoard(board);
    showBoard(board);
    // TODO: Determine whether it is your turn or not
    // myTurn = ______;


    // TODO: Determine your letter (x or o) and your opponent's letter

    // TODO: Add code to send an invite or wait to accept an invite

    // Start game loop, alternating turns
    while(!winner && moveCount != 9)
    {
        // get my move
        if (myTurn)
        {
            // TODO: add code your move here to get the move, validate move,
            //       show board, send move to remote port, and check for a winner
        }
        // get opponent's move
        else
        {
            // TODO: add code to receive your opponent's move, validate move,
            //       show board, and check for a winner
        }

        // Increment move count, alternate turns
        moveCount++;
        myTurn = !myTurn;
    }
    if (!winner)
        printf("The game was a draw\n");


    // TO DO: Close listener port
    closeListenerPort();
    return EXIT_SUCCESS;
}
