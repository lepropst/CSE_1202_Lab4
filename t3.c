// Tic-tac-toe (T3) Network Game
// Jason Losh

//-----------------------------------------------------------------------------
// Compile notes for C99 code
//-----------------------------------------------------------------------------

// gcc -o t3 t3.c udp.c

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdlib.h>  // EXIT_ codes
#include <stdbool.h> // bool
#include <stdio.h>   // printf, scanf
#include <string.h>  // strlen, strcmp
#include <ctype.h>   // toupper
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
    for (int i = 0; i < 3; i++)
    {
        printf("\t");
        for (int j = 0; j < 3; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

bool threeInARow(char row[3], char xo)
{
    int checks = 0;
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(&row[i], &xo) == 0)
        {
            checks++;
        }
    }
    if (checks == 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// TODO: Add code to determine if a winner (xo) has occurred
bool isWinner(char board[3][3], char xo)
{
    bool win;
    char col1[3], col2[3], col3[3];
    for (int i = 0; i < 3; i++)
    {
        if (threeInARow(board[i], xo))
        {
            win = true;
            return win;
        }
        for (int j = 0; j < 3; j++)
        {
            if (j == 0)
                col1[j] = board[i][j];
            if (j == 1)
                col2[j] = board[i][j];
            if (j == 2)
                col3[j] = board[i][j];
        }
    }

    if (threeInARow(col1, xo))
        return true;
    if (threeInARow(col2, xo))
        return true;
    if (threeInARow(col3, xo))
        return true;

    if (strcmp(&board[1][1], &xo) == 0)
    {
        if (strcmp(&board[0][0], &xo) == 0)
        {
            if (strcmp(&board[2][2], &xo) == 0)
            {
                return true;
            }
        }
        if (strcmp(&board[0][2], &xo) == 0)
        {
            if (strcmp(&board[2][0], &xo) == 0)
            {
                return true;
            }
        }
    }

    return win;
}

// TODO: Add code to test if an x or o (xo) is a valid move
//       and then record in the game board

char getOpponentLetter(char *myLetter)
{
    if (*myLetter == 'x')
    {
        return 'y';
    }
    return 'x';
}
void setCoords(int *x, int *y, char move[2])
{
    *x = 3;
    while (*x >= 0)
    {
        // if (char.ToUpper(move[0]).Equals(char.ToUpper(ROWS[*x])))
        // if (move[0] == ROWS[*x])
        if (toupper(move[0]) == toupper(ROWS[*x]))
        {
            break;
        }
        *x -= 1;
    }
    *y = move[1] - '0';
    *y -= 1;
    printf("%d, %d\n", *x, *y);
}
bool addMove(char board[3][3], char move[2], char xo)
{
    bool ok = strlen(move) == 2;
    if (!ok)
    {
        printf("wrong move\n");
        return ok;
    }
    char opp = getOpponentLetter(&xo);
    int x = 3;
    int y;
    setCoords(&x, &y, move);
    printf("%d, %d\n", x, y);
    if (board[x][y] == xo)
    {
        printf("Move already made\n");
        ok = false;
    }
    else if (board[x][y] == opp)
    {
        printf("Opponent has made that move\n");
        ok = false;
    }
    else if (board[x][y] == '.')
    {
        printf("Move made\n");
        board[x][y] = xo;
        ok = true;
    }
    else
    {
        printf("edge case not taken into account");
        ok = false;
    }
    return ok;
}

void checkArguments(char **remoteIp, char **role, int *remotePort, bool *goodArguments, char *args[3])
{
    *remoteIp = args[1];
    *role = args[2];
    printf("|%s|\t|%s|\n\n", *remoteIp, *role);
    // client role
    if (strcmp("invite", *role) == 0)
    {
        *role = "client";
        *remotePort = SERVER_PORT;
        *goodArguments = true;
    }
    // server rol
    else if (strcmp("accept", *role) == 0)
    {
        *role = "server";
        *remotePort = CLIENT_PORT;
        *goodArguments = true;
    }
    else
    {
        printf("invalid role for application:\t%s\nQUITTING NOW\n", args[2]);
        *goodArguments = false;
    }
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    char *remoteIp;
    char rstring[6];
    char *role = rstring;

    char move[2];
    char board[3][3];
    bool validMove;
    bool winner = false;
    int moveCount = 0;

    bool myTurn;
    char myLetter, opponentLetter;
    int remotePort;

    // TODO: Verify arguments are valie
    bool goodArguments;
    if (argc == 3)
    {

        checkArguments(&remoteIp, &role, &remotePort, &goodArguments, argv);
        printf("ip: %s\nport %d\nrole: %s\n", remoteIp, remotePort, role);
    }
    else
    {
        printf("not correct amount of params");
        goodArguments = false;
    }
    if (!goodArguments)
    {
        printf("usage: t3 IPV4_ADDRESS ROLE\n\twhere:\n\tIPV4_ADDRESS is address of the remote machine\n\tROLE is either invite or accept\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Determine if client or server
    // A server will wait to accept an invitation to play
    // A client will send an invitation to play
    // DETERMINED ABOVE IN GOOD ARGUMENTS. ROLE AS CLIENT/SERVER IS STORED IN {ROLE}
    // TODO: Open listener port number dependent on client/server role
    // TODO: Determine remote port that you will send data to
    //       If you are server, send to client port, and vice versa
    bool openListener = openListenerPort(remoteIp, remotePort);
    if (openListener)
    {
        if (strcmp(role, "client") == 0)
        {
            remotePort = SERVER_PORT;
            //client, initiate with sendData
            printf("client\n");
            sendData(remoteIp, remotePort, "invite");
            myTurn = false;
            myLetter = 'y';
            opponentLetter = getOpponentLetter(&myLetter);
            printf("Sending an invite\n");
        }
        else if (strcmp(role, "server") == 0)
        {
            remotePort = CLIENT_PORT;
            //server use RecieveData
            printf("Waiting for an invitation\n");
            char tmp[10];
            receiveData(tmp, 10);
            printf("data recieved %s\n", tmp);
            myTurn = true;
            myLetter = 'x';
            opponentLetter = getOpponentLetter(&myLetter);
        }
    }
    else
    {
        printf("unable to open port");
        exit(1);
    }

    // Setup game
    clearBoard(board);
    showBoard(board);
    // TODO: Determine whether it is your turn or not
    // myTurn = ______;

    // TODO: Determine your letter (x or o) and your opponent's letter

    // TODO: Add code to send an invite or wait to accept an invite

    // Start game loop, alternating turns
    while (!winner && moveCount != 9)
    {
        // get my move
        if (myTurn)
        {
            printf("Enter your move(%c): ", myLetter);
            move[0] = getchar();
            move[1] = getchar();
            if (addMove(board, move, myLetter))
            {
                validMove = true;
                isWinner(board, myLetter);
                sendData(remoteIp, remotePort, move);
                printf("valid move\n");
            }
            else
            {
                validMove = false;
                printf("invalid move, please try again: ");
            }
            // TODO: add code your move here to get the move, validate move,
            //       show board, send move to remote port, and check for a winner
        }
        // get opponent's move
        else
        {
            printf("not my move");
            receiveData(move, 2);
            printf("Your opponent (%c) move to %s\n", opponentLetter, move);
            showBoard(board);
            int x = 3;
            int y;
            //strcpy(move, str);
            setCoords(&x, &y, move);
            addMove(board, move, myLetter);
            isWinner(board, opponentLetter);

            // TODO: add code to receive your opponent's move, validate move,
            //       show board, and check for a winner
        }

        // Increment move count, alternate turns
        moveCount++;
        myTurn = !myTurn;
    }
    if (!winner)
    {

        printf("The game was a draw\n");
    }
    // TO DO: Close listener port

    closeListenerPort();
    return EXIT_SUCCESS;
}
