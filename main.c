// PIECE CIPHER
// 
// Forsynth-Edwards Notation pieces:
// 	black
// 	WHITE
// 	(p)awn
// 	(b)ishop
// 	(r)ook
// 	k(n)ight
// 	(k)ing
// 	(q)ueen
//
// PIECE   =VALUE=SPECIAL ACTION
//  NONE   =  0  = END OF LETTER
//  PAWN   =  1  = NONE
//  BISHOP =  2  = NONE
//  ROOK   =  3  = NONE
//  KNIGHT =  4  = NONE
//  QUEEN  =  5  = NONE
//  KING   =  6  = END OF MESSAGE
// 
// COLOR  =ACTION
//  BLACK = ADD PIECE VALUE TO ACCUMULATOR
//  WHITE = MULTIPLY ACCUMULATOR BY PIECE VALUE
//
// ACCUMULATOR VALUE=CHARACTER:
//   1=A,  2=B,  3=C,  4=D,  5=E,  6=F,  7=G,  8=H,  9=I, 10=J, 11=K, 12=L, 13=M, 14=N, 15=O,
//  16=P, 17=Q, 18=R, 19=S, 20=T, 21=U, 22=V, 23=W, 24=X, 25=Y, 26=Z, 27=!, 28=?, 29=., 30=,
//
// how to decode piece ciphers:
// 	to decode a piece cipher, read row by row. Each piece has a value (check piece value table above). 
// 	Each piece has an action according to its color (check color action table above). do the action to
// 	the accumulator. once you have a list of just numbers, add 3 to all of them to account for the shift
// 	cipher and convert them into characters according to the character map above. you should now have a 
// 	decoded message.

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 24
#define DEBUG_MODE 0

// enum for piece values. Pawn = 1, Bishop = 2, Rook = 3, Knight = 4, Queen = 5, King = 6.
typedef enum Pieces {
	Pawn = 1,
	Bishop,
	Rook,
	Knight,
	Queen,
	King,
	None
} Pieces;

// struct for pieces. stores if it is white, and the piece type .
typedef struct Piece {
	bool is_white;
	Pieces piece;
} Piece;

// checks if a character is a valid piece
bool is_valid_piece(char piece) {
	piece = toupper(piece);
	return (strchr("PBRNQK", piece)!=NULL);
}

// get a piece, returns the piece struct. if the piece isnt a valid piece returns the following struct: Piece {None, false};
Piece get_piece(char piece) {
	struct Piece output;
	if (is_valid_piece(piece)) { // continue if it is a valid piece
		// check if it is a white piece
		if (isupper(piece))
			output.is_white = true;
		else 
			output.is_white = false;
		// match character with piece enum
		switch (toupper(piece)) {
			case 'P': output.piece =   Pawn; break;
			case 'B': output.piece = Bishop; break;
			case 'R': output.piece =   Rook; break;
			case 'N': output.piece = Knight; break;
			case 'Q': output.piece =  Queen; break;
			case 'K': output.piece =   King; break;
			default : output.piece =   None; break;
		}
	} else {
		output.piece = None;
		output.is_white = false;
	}
	return output;
}

// decode a board. writes to the inputted buffer
void decode(char board[], char buf[MAX_MESSAGE_SIZE]) {
	char character_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!?.,ABC"; // character string to map accumulator to
	int c,i; // c for character index of board, i for index of the position to write to in the buffer
	int accumulator = 1; // accumulator
	bool last_char_piece = false;
	
	for (c=0;board[c]!='\0';c++) { // loop through all the characters in the board string
		Piece piece = get_piece(board[c]); // get the piece for the current character
		if (piece.piece != None) { // check to see if the piece is a valid piece
			if (DEBUG_MODE)
				printf("%c is a %s piece, accumulator is %d, ", board[c], (piece.is_white) ? "white" : "black" , accumulator);
			if (piece.piece == King) { // checks to see if the piece is a king
				buf[i++] = character_map[accumulator-4]; // writes a character according to the character map, with a shift cipher
				if (DEBUG_MODE)
					printf("resetting accumulator and adding character %c to the output\n", character_map[accumulator-4]);
				accumulator = 1; // resets the accumulator
				buf[i] = '\n'; // adding null ending to the output
				break; // stops the decoding proccess, because the message was fully decoded	
			}
			if (piece.is_white) { // checks to see if the piece is white
				accumulator *= piece.piece; // multiplies the accumulator by the piece value
				if (DEBUG_MODE)
					printf("multiplying accumulator by %d\n", piece.piece);
			} else { // checks to see if the piece is black
				accumulator += piece.piece; // adds the piece value to the accumulator
				if (DEBUG_MODE)
					printf("adding %d to the accumulator\n", piece.piece);
			}
			last_char_piece = true; // sets last_char_piece to true
		} else if((isdigit(board[c])) && last_char_piece) { // checks if the current character is an empty square and the last character was a piece
			buf[i++] = character_map[accumulator-4]; // writes a character according to the character map, with a right shift cipher
			if (DEBUG_MODE)
				printf("resetting accumulator (%d) and adding character %c to the output\n", accumulator, character_map[accumulator-4]);
			accumulator = 1; // resets the accumulator
			last_char_piece = false; // sets last_char_piece to false
		}
	}
}

bool test_is_valid_piece();
bool test_isnt_valid_piece();
bool test_get_piece();
bool test_get_invalid_piece();

int main(int argc, char *argv[]) {
	// 0        1         2         3
	// 123456789012345678901234567890
	// ?.,ABCDEFGHIJKLMNOPQRSTUVWXYZ!
	//
	//DIGHERE! = 7 12 10 11 8 21 8 30
	//p = 1, b = 2, r = 3, n = 4, q = 5
	//CAPITAL   = *
	//lowercase = +
	char board[] = "rr6/RN6/qn6/NBr5/qb6/rrR5/qb6/qQ5k"; //DIGHERE!
	char output[MAX_MESSAGE_SIZE]; 

	// tests
	if (DEBUG_MODE) { 
		assert(test_is_valid_piece());
		assert(test_isnt_valid_piece());
		assert(test_get_piece());
		assert(test_get_invalid_piece());
	}

	if(argc > 1) 
  	decode(argv[1], output);
	else 
		decode(board, output);

	printf("decoded message: %s\n", output);
	return 0;
}

// C unit test definitions
bool test_is_valid_piece() {
	return is_valid_piece('p') && is_valid_piece('P');
}

bool test_isnt_valid_piece() {
	return !(is_valid_piece('/') || is_valid_piece('5'));
}

bool test_get_piece() {
	struct Piece piece = get_piece('P');
	return (piece.piece==Pawn && piece.is_white==true);
}

bool test_get_invalid_piece() {
	struct Piece piece = get_piece('/');
	return (piece.piece==None && piece.is_white==false);
}
