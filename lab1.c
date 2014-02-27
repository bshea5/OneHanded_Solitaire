/* 	Brandon Shea
	CS 367 Lab 01 
	
	Print(deck, 0) for short print of deck
	Print(deck, int != 0) for long print of deck
*/
	
//SEE TODO at bottom
	

#include <stdio.h>
#include <stdlib.h>

typedef struct Card Card;
typedef struct Deck Deck;

struct Card {
	char face;			// Ace, two through ten, Jack, Queen, King
	char suit;			// Hearts(H), Spades(S), Diamonds(D), Clubs(C)
	Card* next;			// next card in hand or deck
	Card* prev;			// prev card in hand or deck
};

struct Deck {
	Card* top;			// Top of Deck
	int length;			// Cards in Deck, must be 52 for a game
};


/* add your functions here */
Deck* makeDeck(FILE* input);
Card* makeCard(char face, char suit);
void removeCard(Card* card);
void Print(Deck* deck, int d);
void printHand(Card* c);
int playGame(Deck* deck);
void deleteDeck(Deck* deck);
void peekAtCard(Deck* deck, int atNumber);

/* global variables */
//static Card* head; //to initialize linked list //static to not be seen outside offile

//using code from assignment01 to start main to read file
int main(int argc, char **argv) {
	printf("Welcome l(^_^l)\n\n");

	FILE *input_file;
	char filename[32];
	if (argc != 2) {printf("Usage: %s <filename>\n",argv[0]); exit(2);}
	if ((input_file = fopen(argv[1],"r")) == NULL) { 
		printf("Unknown file: %s\n",argv[1]); exit(3);
	}
		
	//build deck(s) with file input
	//play game with 52 cards
	Deck* deck;
	int games = 0;
	while( (deck = makeDeck(input_file)) != NULL ){
		games++;
		if (playGame(deck) == 1)
			printf("You Win Game %d\n", games);
		else {
			printf("You Lose Game %d\n", games);
		}
		printf("remaining deck:\n");
		Print(deck, 0);
		printf("\n");
		deleteDeck(deck);
	}
	fclose(input_file);
	return 0; //END
}

//make a deck from file input
//Construct each card as a node of a linked list
//this will be a doubly linked list with prev/next pointers
Deck* makeDeck(FILE* input) {
	Deck* deck = (Deck*) malloc(sizeof(Deck));
	deck->top = NULL;
	deck->length = 0;
	char face, suit;
	while( !(deck->length >= 52) &&
			((fscanf(input, " %c", &face)!=EOF) && (fscanf(input, " %c", &suit)!=EOF)) ){
		Card* newCard = makeCard(face, suit);
		//printf("%c%c\n", newCard->face, newCard->suit);
		if(deck->top == NULL){
			deck->top = newCard;
		}
		else {	
			deck->top->prev = newCard;
			newCard->next = deck->top;
			deck->top = newCard;
		}
		deck->length++;
	}
	//do a if length < 52, abandon deck
	if (deck->length != 52){
		printf("Not enough cards for a deck\n");
		deleteDeck(deck);
		return NULL;
	}
	return deck;
}

//creat a card with initial next/prev pointers to NULL
Card* makeCard(char face, char suit) {
	Card *newCard = (Card*) malloc(sizeof(Card));
	newCard->face = face;
	newCard->suit = suit;
	newCard->next = NULL;
	newCard->prev = NULL;
	return newCard;
}

//free(card) and change the pointers of its prev/next if they exist
void removeCard(Card* card){
	Card *delCard = card;
	
	//if card is not first node
	if(delCard->prev != NULL){
		delCard->prev->next = delCard->next;
	}
	//if card is not last node
	if(delCard->next != NULL){
		delCard->next->prev = delCard->prev;
	}
	free(delCard);
	delCard = NULL;
}

//print each card and its respective prev/next cards
//d=0 short print()             d=1 Detailed print()
void Print(Deck* deck, int d) {
	Card* c = deck->top;
	//printf("Printing Deck:\n");
	while( d != 0 && c != NULL ){
		printf("face: %c\tsuit: %c\t   ", c->face, c->suit);
		if ( c->next != NULL){
			printf("nextFace: %c\tnextSuit: %c\t", c->next->face, c->next->suit);
		}
		if ( c->prev != NULL ){
			printf("prevFace: %c\tprevSuit: %c\n\n", c->prev->face, c->prev->suit);
		}
		else{
			printf("\n\n");
		}
		c = c->next;
	}
	while( d == 0 && c != NULL ){
		printf("%c%c ", c->face,c->suit);
		c = c->next;
	}
	printf("\n");
	//printf("End Deck Print\n");
}

//print card and its previous 3
//4 cards in a hand, but this currently prints all cards drawn...
//for testing purposes
void printHand(Card* c){
	while(c != NULL){
		printf("%c%c ", c->face, c->suit);
		c = c->prev;
	}
	printf("\n");
}

//delete entire/remnants of deck
void deleteDeck(Deck* deck){
	//check if deck of cards exists
	if(deck->top == NULL){
		free(deck);
		return;
	}
	Card* c = deck->top;
	//go to bottom of the deck
	while(c->next != NULL ){
		c = c->next;
	}
	//delete from the bottom up
	while(c->prev != NULL ){
		c = c->prev;
		removeCard(c->next);
		deck->length--;
	}
	removeCard(c);
	deck->length--;
	free(deck);
	deck = NULL;
}

//take a peek at a card int numbers down
//not used in game, only for testing purposes
void peekAtCard(Deck* deck, int atNumber){
	int i = 0;
	if( (deck->length < atNumber) || (atNumber < 0) ){
		printf("invalid peek\n\n");
	}
	else{
		Card* current = deck->top;
		for(i=1; i<atNumber; i++){
			current = current->next;
		}
		printf("%d", atNumber);
		if(atNumber == 1)
			printf("st");
		else if(atNumber == 2)
			printf("nd");
		else if(atNumber == 3)
			printf("rd");
		else
			printf("th");
		printf(" card from the top is:\n Face: %c\t Suit: %c\n\n", 
									current->face, current->suit);
	}
	
}

//play One-handed solitaire
//return 1 for win	return 0 for lose
int playGame(Deck* deck){
	Card* c = deck->top;
	Card* cPrime;
	int cardsDrawn = 1;
	int numCards = 1;
	while( c != NULL ){
		//printHand(c);
		if(numCards >= 4){
			//printf("Have at least 4 cards! check suits and faces\n");
			cPrime = c->prev->prev->prev;
			if( c->suit == cPrime->suit ){
				//printf("remove 2 cards\n");
				
				removeCard(c->prev);
				removeCard(cPrime->next);
				
				numCards = numCards - 2;
			}
			else if ( c->face == cPrime->face ){
				//printf("remove 4 cards\n");
				
				removeCard(cPrime->next);
				removeCard(cPrime->next);
				removeCard(cPrime);
				
				//if not empty hand after
				if(c->prev != NULL){
					c = c->prev;
					removeCard(c->next);
					numCards = numCards - 4;
				}
				//hand empty after
				else{
					//if more cards left in deck
					if(c->next != NULL){
						c = c->next;
						removeCard(c->prev);
						numCards = numCards - 3;
						cardsDrawn++;
					}
					//deck is empty
					else {
						c = NULL;
						numCards = numCards - 4; 
					}
				}
			}
			else {
				//printf("no match\n");
				c = c->next;
				if (c != NULL){
					numCards++;
					cardsDrawn++;
				}
			}
		}
		else{
			//printf("you have less than 4 cards\n");
			c = c->next;
			numCards++;
			cardsDrawn++;
		}
	}
	deck->length = numCards;
	if (numCards == 0){
		deck->top = NULL;
		return 1; //win
	}
	return 0; //lose
}




