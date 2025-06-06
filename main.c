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
//
// example board with the encoded message "DIGHERE!": 
//
//     A   B   C   D   E   F   G   H
//   + - + - + - + - + - + - + - + - +
// 1 | N |   | r | r |   |   | B | N | DIG
//   + - + - + - + - + - + - + - + - +
// 2 |   | b | n |   |   |   | n |   | HE
//   + - + - + - + - + - + - + - + - +
// 3 |   |   |   |   |   |   |   |   | 
//   + - + - + - + - + - + - + - + - +
// 4 |   |   |   |   |   | q | R |   | R
//   + - + - + - + - + - + - + - + - +
// 5 |   |   |   |   |   |   |   |   | E
//   + - + - + - + - + - + - + - + - +
// 6 |   |   |   |   |   |   |   |   |  
//   + - + - + - + - + - + - + - + - +
// 7 |   |   | q | Q | P | K |   |   | !
//   + - + - + - + - + - + - + - + - +
// 8 | k |   |   |   |   |   |   |   |
//   + - + - + - + - + - + - + - + - +
//
// 	in FEN:
// 		N1rr2BN/1bn3n1/8/5qR1/8/8/2qQPK2/k7

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_ENCRYPTION_SIZE 32
#define DEBUG_MODE 0

bool is_valid_piece(char piece) {
	piece = toupper(piece);
	bool is_valid_piece = false;
	char valid_pieces[] = "PNBRQK";
	for (int c=0;valid_pieces[c]!='\n';c++) {
		if (piece==valid_pieces[c])
			is_valid_piece = true;
	}
	return is_valid_piece;
}

bool is_white_piece(char piece) {
	return piece == toupper(piece);
}

int get_piece_value(char piece) {
	piece = toupper(piece);
	char pieces[] = "PBRNQK";

	int i;

	for(i=0;pieces[i]!='\0';i++) {
		if (piece==pieces[i])
			return i+1;
	}

	return -1;
}

int main(int argc, char *argv[]) {
	char board[] = "N1rr2BN/1bn3n1/8/5qR1/8/8/2qQPK2/k7";
	char character_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!?.,";

	int c,i = 0;
	int accumulator = 1;

	char output[32];

	for(c=0;board[c]!='\0';c++) { // loop through all the characters
		printf("%c", board[c]);
		if (is_valid_piece(board[c])) { // if it is a piece
			printf(" is a piece");
			if (get_piece_value(board[c])==6) { // if it is a king
			 printf(", is a king\n");
				output[i++] = character_map[accumulator];
  			accumulator = 1;
				output[i] = '\0';
				break;
			}
			if (is_white_piece(board[c])) { // if it is a white piece
				printf(", is white\n");
				accumulator *= get_piece_value(board[c]);
			} else { // if it is a black piece
				printf(", is black\n");
				accumulator += get_piece_value(board[c]);
			}
		} else if ('0' <= board[c] && board[c] <= '9') { // if it isnt a piece at all
			printf(" isnt a piece\n");
			output[i++] = character_map[accumulator];
  		accumulator = 1;
		}	
	}

	if (DEBUG_MODE) {
		printf("get_piece_value test: %s with %d as piece value\n",
				(get_piece_value('p') == 6) ? "Passed" : "Failed", get_piece_value('k'));
	}

	printf("decoded message:%s\n", output);
	return 0;
}
