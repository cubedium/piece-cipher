# Piece Cipher

### USEFUL INFORMATION:

The piece cipher is a way to encode messages onto a chess board. The max size of the messages can be 24 characters long. The supported characters are: A-Z and basic punctuation (?!.,). The program supports FEN boards

### ENCODING: 

Step 1 - Character To Number Conversion: The first step to encoding a message is converting the characters. The order in which the characters are held are A-Z then the punctuation (?!.,). To get the numerical values for the characters, convert them into numbers based on the number of characters that come before it + itself. After that you shift all of them to the right by 3 by adding 3 to the value you got earlier and substracting 30 from it if it is higher than 30. This would result in "!" equalling 1, "." = 2, "," = 3, "A" = 4, "B" = 5, etc.

Step 2 - Number To Piece Conversion: The second step to encoding a message is converting the numbers into piece combinations. Each piece (aside from the king) has a value (Pawn = 1, Bishop = 2, Rook = 3, Knight = 4, Queen = 5). and each color of pieces has an action that it does with that value (Black = add value to counter, White = multiply counter by value). The counter has a base value of 1. Each piece modifies the counter in some way until an empty square shows up. This lets the decoder know that the current character has finished being calculated.

Step 3 - Ending a Message: To finish off a message, you have to put a king somewhere after the encoded characters. This lets the decoder know that it should stop decoding the piece combinations.

### DECODING:

Do Steps 1-3 from the ENCODING section but in reverse

### HOW TO USE:
either install the binary from the release, or compile main.c

