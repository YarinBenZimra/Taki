// Yarin Benzimra, MTA. 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

#define MAX_NAME_LEN 20
#define MAX_CARDS 50
#define CARDS_RANK 14
#define MAX_Players 10
#define COLOR_NUM 4
#define CARD_KIND 5
#define Length_Kind 6
#define SIZE_OF_START_ARRAY 4
#define Int_To_Char 48
#define STOP 10
#define PLUS 11
#define DIRECTION 12
#define TAKI 13
#define COLOR 14

struct Cards
{
	int rank;
	char cardcolor;
	char kindofcard[Length_Kind];
};
typedef struct Cards cards;

struct Player
{
	char name[MAX_NAME_LEN];
	cards* Pcards;
	int numberofcards;
	int phyNumberofcards;
	int Choice;
};
typedef struct Player player;

struct statistics
{
	int card_rank;
	int frequency;
};
typedef struct statistics Statistics;

void Welcome(int* pnumofplayers);
void ScanPlayers(player* p, int numofplayers);
void print_a_card(cards* c);
void First_Upper_Card (cards* c);
void First_Four_Cards(player* p, int numofplayers, Statistics* stat);
void Creat_Card(cards* c);
cards* CardRealloc(cards* c, int size, int newsize);
bool IsWin(player* p, int numofplayers);
bool turn(player* players, int numOfPlayers, int* playerTurn, cards* upperCard, bool* Is_Last_Choice, bool* Taki_Case, Statistics* stat);
bool Check_Valid_Input(player* p, int* choice, cards* uppercards, bool* Taki_Case);
void Print_Player_Cards(player* p);
void Take_Card_From_Deck(player* p, Statistics* stat);
void Put_Card_In_Middle(player* p, cards* uppercard, int choice);
void Which_Player_Plays(int* PlayerTurn, bool* Turn_Direction, int numofplayers);
void Swap(cards* card1, cards* card2);
void Deal_With_Special_Cards(cards* card, bool* Turn_Direction, int* PlayerTurn, int numofplayers, bool* Taki_Case);
void Color_Case(cards* UpperCard);
bool Is_Plus_Last_Card(player* p);
bool Is_Stop_Last_Card(player* p, int numOfPlayers);
void Initial_Statistics(Statistics stat[], int size);
void Statistics_Of_Game(Statistics* stat);
void MergeSort(Statistics stat[], int size);
void merge(Statistics stat1[], int size1, Statistics stat2[], int size2, Statistics res[]);
void copyArr(Statistics stat1[], Statistics stat2[], int size);
void Gameplay(player* Players, int numofplayers, int* PlayerTurn, cards* UpperCard, bool* Taki_Case, Statistics* Game_Statistics, bool* isWinner, bool* Turn_Direction);



//////////////////////////////////////////////////////////////////////////////////////////////////////////- *Explanation of the functions in signatures 
void main()
{
	srand(time(NULL));
	player* Players;
	Statistics Game_Statistics[CARDS_RANK];
	int numofplayers;
	cards UpperCard;
	int PlayerTurn = 0;
	bool isWinner = false;
	bool Turn_Direction = true;
	bool Taki_Case = false;
	Welcome(&numofplayers);
	Players = (player*)malloc(numofplayers * sizeof(player));
	Gameplay(Players, numofplayers, &PlayerTurn, &UpperCard, &Taki_Case, Game_Statistics, &isWinner, &Turn_Direction);
}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool turn(player* players, int numOfPlayers, int* playerTurn, cards* upperCard, bool* Is_Last_Choice, bool* Taki_Case, Statistics* stat) // This function is responsible for splitting cases according to the choice of card 
{
	bool Valid = false;
	int choice;
	int turnP = *playerTurn;
	printf("Upper Card:\n");
	print_a_card(upperCard);
	printf("%s's turn:\n", players[*playerTurn].name);
	Print_Player_Cards((players+turnP));// print all player cards
	
	while (Valid == false)
	{
		if (*(Taki_Case) == false)
		{
			printf("Please enter 0 if you want to take a card from the deck\n");// print menu
		}
		else
		{
			printf("Please enter 0 if you want to finish your turn\n");

		}
		printf("or 1-%d if you want to put one of your cards in the middle : \n", players[*playerTurn].numberofcards);
		Valid = Check_Valid_Input((players + turnP), &choice, upperCard, &Taki_Case);// check valid input
	}
	(players + turnP)->Choice = choice;
	if (choice == 0)
	{
		if (*(Taki_Case) == false)
		{
			Take_Card_From_Deck((players + turnP), stat); // take from deck
			*Is_Last_Choice = false;
		}
		else
		{
			*(Taki_Case) = false;
		}
	}
	else if (Is_Plus_Last_Card(players + turnP) == true)
	{
		Put_Card_In_Middle((players + turnP), upperCard, choice); // put in the middle
		Take_Card_From_Deck((players + turnP), stat);// take from deck
		*(Is_Last_Choice) = false;
		*(Taki_Case) = false;
	}
	else if (Is_Stop_Last_Card((players + turnP), numOfPlayers))
	{
		Put_Card_In_Middle((players + turnP), upperCard, choice); // put in the middle
		Take_Card_From_Deck((players + turnP), stat);// take from deck
		*(Is_Last_Choice) = true;
		*(Taki_Case) = false;
	}
	else  
	{
		Put_Card_In_Middle((players + turnP), upperCard, choice); // put in the middle
	}
	
	if ((players[*playerTurn].numberofcards) == 0)
	{
	return true;
	}
    return false;
}
void Welcome(int* pnumofplayers)// This function writes the welcome message and scan the number of players
{
	int numofplayers;
	printf("************  Welcome to TAKI game !!! ***********\n");
	printf("Please enter the number of players:\n");
	scanf("%d", &numofplayers);
	*pnumofplayers = numofplayers;
	printf("\n");
}
void ScanPlayers(player* p, int numofplayers) // This function scan the name of players
{
	char name[MAX_NAME_LEN];
	int i;
	printf("\n");
	for (i = 1; i <= numofplayers; i++)
	{
		printf("Please enter the first name of player #%d:\n", i);
		scanf("%s", name);
		strcpy(p[i-1].name, name);
	}
}
void print_a_card(cards* c) // this function print one single card (for the first upper-card)
{
	if(strlen(c->kindofcard)<=2)
	{
		printf("*********\n");
		printf("*       *\n");
		printf("*  %2s   *\n", c->kindofcard);
		printf("*  %2c   *\n", c->cardcolor);
		printf("*       *\n");
		printf("*********\n");
	}
	else
	{
		printf("*********\n");
		printf("*       *\n");
		printf("* %-4s *\n", c->kindofcard);
		printf("*  %2c   *\n", c->cardcolor);
		printf("*       *\n");
		printf("*********\n");
	}
	printf("\n");
}
void First_Upper_Card(cards* c) // This function print the first card in the game knwon as "upper-card"
{
	char Numbers[2] = { 0 };
	char Letters[4] = { 'R','B','G','Y' };
	int color = rand() % 4;
	(*c).rank = 1 + rand() % 9;
	(*c).cardcolor = Letters[color];
	Numbers[0] = c->rank + Int_To_Char;
	Numbers[1] = 0;
	strcpy(c->kindofcard, Numbers);
}
void Creat_Card(cards* c, Statistics* stat) // This function creat the values of the card
{
	char ranktochar[2] = { 0 };
	int Random_Color;
	c->rank = 1 + rand() % CARDS_RANK;
	Random_Color = 1 + rand() % COLOR_NUM;
	switch (Random_Color)
	{
	case 1:
		c->cardcolor = 'R';
		break;
	case 2:
		c->cardcolor = 'B';
		break;
	case 3:
		c->cardcolor = 'G';
		break;
	case 4:
		c->cardcolor = 'Y';
		break;
	}
	switch (c->rank)
	{
	case STOP:
	{
		strcpy(c->kindofcard, " STOP");
		break;
		}
	case PLUS:
	{
		strcpy(c->kindofcard, "+");
		break;
	}
	case DIRECTION:
	{
		strcpy(c->kindofcard, " <-> ");
		break;
	}
	case TAKI:
	{
		strcpy(c->kindofcard, " TAKI");
		break;
	}
	case COLOR:
	{
		strcpy(c->kindofcard, "COLOR");
		c->cardcolor = ' ';
		break;
	}
	default:
	{
		ranktochar[0] = c->rank + Int_To_Char;
		ranktochar[1] = 0;
		strcpy(c->kindofcard, ranktochar);
	}
	}
	(stat[(c->rank)-1].frequency)++;
}
void First_Four_Cards(player* p, int numofplayers, Statistics* stat) // This function deals four cards to the players at the beginning of the game
{
	for (int i = 0; i < numofplayers; i++)
	{
		p[i].Pcards = (cards*)malloc(sizeof(cards) * SIZE_OF_START_ARRAY);
		if (p[i].Pcards == NULL)
		{
			printf("Memory allocation failed!!!\n");
			exit(1);
		}
		for (int j = 0; j < SIZE_OF_START_ARRAY; j++)
		{
			Creat_Card(&p[i].Pcards[j], stat);
		}
		p[i].numberofcards = SIZE_OF_START_ARRAY;
		p[i].phyNumberofcards = SIZE_OF_START_ARRAY;
	}
}
cards* CardRealloc(cards* c, int size, int newsize) //This function increases/decreases the players's array of cards as needed 
{
	cards* newArr = (cards*)malloc(newsize * sizeof(cards));
	int i;

	// check if memory allocation succeeded
	if (newArr)
	{
		//copy arr into newArr
		for (i = 0; i < size && i < newsize; i++)
		{
			newArr[i] = c[i];
		}
		free(c);
	}
	else
	{
		printf("Memory allocation failed!");
		exit(1);
	}
	return newArr;
}
bool IsWin(player* p, int numofplayers) //This function declares the winner 
{
	bool win = false;
	int i;
	for (i = 0; i < numofplayers; i++)
	{
		if (p[i].numberofcards == 0)
		{
			printf("The winner is... %s! Congratulations!", p[i].name);
			win = true;
		}
	}
}
bool Check_Valid_Input(player* p, int* choice, cards* uppercards, bool* Taki_Case) // This function check if input is valid
{
	bool Valid = false;
	while (Valid == false)
	{
		scanf("%d", choice);// scan the input
		if (*choice > p->numberofcards || *choice < 0)
		{
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");// print menu
			printf("or 1-%d if you want to put one of your cards in the middle : \n", p->numberofcards);

		}
		else if (*choice == 0)
		{
			Valid = true;
		}
		else if ((*p).Pcards[*choice - 1].rank == COLOR)
		{
			*(Taki_Case) = false;
				Valid = true;
        }
		else if ((*p).Pcards[*choice - 1].cardcolor != (*uppercards).cardcolor && (*p).Pcards[*choice - 1].rank != (*uppercards).rank)
		{
			printf("Invalid card! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");// print menu
			printf("or 1-%d if you want to put one of your cards in the middle : \n", p->numberofcards);
		}
		else if (*Taki_Case == true) 
		{
			if ((*p).Pcards[*choice - 1].cardcolor != (*uppercards).cardcolor)
			{
				printf("Invalid card! Try again.\n");
				printf("Please enter 0 if you want to take a card from the deck\n");// print menu
				printf("or 1-%d if you want to put one of your cards in the middle : \n", p->numberofcards);
			}
		}
		else
		{
			Valid = true;
		}
	}
	return Valid;
}
void Print_Player_Cards(player* p) // this function print all the cards the player has
{
	int i;
	for (i = 0; i < (*p).numberofcards; i++)
	{
		printf("Card #%d:\n", (i + 1));
		print_a_card(&p->Pcards[i]);
	}
}
void Take_Card_From_Deck(player* players, Statistics* stat) // This function adds one card to the cards-array of the player
{
	players->numberofcards= players->numberofcards+1;
	if (players->numberofcards >= (players->phyNumberofcards))// check if there is enough space in cards arr
	{
		players->Pcards =CardRealloc(players->Pcards, players->numberofcards, (players->phyNumberofcards) * 2);
		players->phyNumberofcards *= 2;//  phynumbersOfCards *= 2;

	}
	Creat_Card(&players->Pcards[players->numberofcards-1], stat);// CardRealloc(cards, numberofcards, phynumberofcards * 2);
}
void Put_Card_In_Middle(player* players, cards* uppercard, int choice) // this function removes one card from the player's array of cards and replaces the upper-card according to the card the player chooses to put in the middle
{
    *uppercard = players->Pcards[choice -1];
	Swap((&players->Pcards[choice - 1]), (&players->Pcards[players->numberofcards - 1]));
	(players->numberofcards)--;
	players->Pcards =CardRealloc(players->Pcards, players->numberofcards, players->numberofcards);
	if (players->numberofcards != 0)
	{
		players->phyNumberofcards = players->numberofcards;
	}
	else
	{
		players->phyNumberofcards = 1;
	}

}
void Which_Player_Plays(int* PlayerTurn, bool* Turn_Direction, int numofplayers) // this function determines who will play the next turn
{
	
	if (*Turn_Direction == true && (*PlayerTurn) != numofplayers - 1)
	{
		(*PlayerTurn)++;
	}
	else if (*Turn_Direction == true && (*PlayerTurn) >= numofplayers - 1)
	{
		(*PlayerTurn) = 0;
	}

	else if (*Turn_Direction == false && (*PlayerTurn) != 0)
	{
		(*PlayerTurn)--;
	}
	else if (*Turn_Direction == false && (*PlayerTurn) <= 0)
	{
			(*PlayerTurn) = numofplayers - 1;
		
	}
}
void Swap(cards* card1, cards* card2)// swap function
{
	cards temp= *card1;
	*card1 = *card2;
	*card2 = temp;
}
void Deal_With_Special_Cards(cards* UpperCard, bool* Turn_Direction, int* PlayerTurn, int numofplayers, bool* Taki_Case) // this function deals with special cards and determines how to act in each special card
{
	switch (UpperCard->rank)
	{
	case STOP: 
	{
		if (*(Taki_Case) == false)
		{
			Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
			Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
		}
		break;
	}
	case PLUS: 
	{
		break;
	}
	case DIRECTION: 
	{
		if (*(Taki_Case) == false)
		{
			*Turn_Direction = !*Turn_Direction;
			Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
		}
		break;
	}
	case TAKI:
	{
		*(Taki_Case) = true;
		break;
	}

	case COLOR: 
	{
			Color_Case(UpperCard);
			Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
		
		break;
	}
	default:
	{
		if (*(Taki_Case) == false)
		{
			Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
		}
		break;
	}
	}
}
void Color_Case(cards* UpperCard) // Deals with "COLOR" card
{
	int color_choice;
	bool valid_choice = false;
	printf("Please enter your color choice :\n");
	printf("1 - Yellow\n");
	printf("2 - Red\n");
	printf("3 - Blue\n");
	printf("4 - Green\n");
	scanf("%d", &color_choice);
	while (valid_choice == false)
	{
		switch (color_choice)
		{
		case 1:
		{
			(*UpperCard).cardcolor = ('Y');
			valid_choice = true;
			break;
		}
		case 2:
		{
			(*UpperCard).cardcolor = ('R');
			valid_choice = true;
			break;
		}
		case 3:
		{
			(*UpperCard).cardcolor = ('B');
			valid_choice = true;
			break;
		}
		case 4:
		{
			(*UpperCard).cardcolor = ('G');
			valid_choice = true;
			break;
		}
		default:
		{
			printf("Invalid color! Try again.\n");
			printf("1 - Yellow\n");
			printf("2 - Red\n");
			printf("3 - Blue\n");
			printf("4 - Green\n");
			scanf("%d", &color_choice);
			break;
		}
		}
	}
}
bool Is_Stop_Last_Card(player* p, int numOfPlayers) // Stop last card situation
{
	if (numOfPlayers == 2)
	{
		
		if (p->numberofcards == 1 && p->Pcards[p->Choice - 1].rank == STOP)
		{
			return true;
		}
	}
	return false;
}
bool Is_Plus_Last_Card(player* p) // Plus last card situation
{
	if (p->numberofcards == 1 && p->Pcards[p->Choice-1].rank == PLUS)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Initial_Statistics(Statistics stat[], int size)// Intial the statistics array
{
	int i;
	for (i = 0; i < CARDS_RANK; i++)
	{
		stat[i].card_rank= i;
		stat[i].frequency = 0;
	}
}
void MergeSort(Statistics stat[], int size)
{
	Statistics* TmpArr = NULL;
	if (size <= 1)
	{
		return;
	}
	else
	{
		MergeSort(stat, size / 2);
		MergeSort(stat + size / 2, size - size / 2);
		TmpArr = (Statistics*)malloc(size * sizeof(Statistics));
		if (TmpArr)
		{
			merge(stat, size / 2, stat + size / 2, size - size / 2, TmpArr);
			copyArr(stat, TmpArr, size);
			free(TmpArr);
		}
		else
		{
			printf("Memory allocation failure!!!\n");
			exit(1);
		}
		
	}
}
void merge(Statistics stat1[], int size1, Statistics stat2[], int size2, Statistics res[])
{
	int ind1 = 0, ind2 = 0, ResInd = 0;
	while ((ind1 < size1) && (ind2 < size2))
	{
		if (stat1[ind1].frequency <= stat2[ind2].frequency)
		{
			res[ResInd]= stat1[ind1];
			ind1++;
		}
		else
		{
			res[ResInd] = stat2[ind2];
			ind2++;
		}
		ResInd++;
	}
	while (ind1 < size1)
	{
		res[ResInd] = stat1[ind1];
		ind1++;
		ResInd++;
	}
	while (ind2 < size2)
	{
		res[ResInd] = stat2[ind2];
		ind2++;
		ResInd++;
	}
}
void copyArr(Statistics stat1[], Statistics stat2[], int size)// Copy stat2 to stat1
{
	int i;
	for (i = 0; i < size; i++)
	{
		stat1[i] = stat2[i];
		
	}
}
void Statistics_Of_Game(Statistics* stat) // Print the result of game statistics 
{
	int i;
	MergeSort(stat, CARDS_RANK);
	printf("************ Game Statistics ************\n");
    printf("Card # | Frequency\n");
    printf("__________________\n");
	for (i = CARDS_RANK-1; i >= 0; i--)
	{
		switch ((stat + i)->card_rank)
		{
		case 9: //STOP
		{
			printf(" STOP  |   %d  \n", (stat + i)->frequency);
			break;
		}
		case 10: //+
		{
			printf("   +   |   %d  \n", (stat + i)->frequency);
			break;
		}
		case 11: //<->
		{
			printf("  <->  |   %d  \n", (stat + i)->frequency);
			break;
		}
		case 12: // TAKI
		{
			printf(" TAKI  |   %d  \n", (stat + i)->frequency);
			break;
		}
		case 13: // COLOR
		{
			printf(" COLOR |   %d  \n", (stat + i)->frequency);
			break;
		}

		default:
			printf("   %d   |   %d  \n", ((stat + i)->card_rank)+1, (stat + i)->frequency);
			break;

		}
	}
	


}
void Gameplay(player* Players, int numofplayers, int* PlayerTurn, cards* UpperCard, bool* Taki_Case, Statistics* Game_Statistics, bool* isWinner, bool* Turn_Direction)// this function contains all function for gameplay
{
	bool Is_Last_Choice;
	Initial_Statistics(Game_Statistics, CARDS_RANK);
	ScanPlayers(Players, numofplayers);
	First_Upper_Card(UpperCard);
	First_Four_Cards(Players, numofplayers, Game_Statistics);
	while (*isWinner == false)
	{
		Is_Last_Choice = true;
		*isWinner = turn(Players, numofplayers, PlayerTurn, UpperCard, &Is_Last_Choice, Taki_Case, Game_Statistics);
		if (*isWinner == false)
		{
			if (Is_Last_Choice == true)
			{
				Deal_With_Special_Cards(UpperCard, Turn_Direction, PlayerTurn, numofplayers, Taki_Case);
			}
			else
			{
				Which_Player_Plays(PlayerTurn, Turn_Direction, numofplayers);
			}
		}
	}
	printf("The winner is... %s! Congratulations!\n\n", (Players+*PlayerTurn)->name);
	Statistics_Of_Game(Game_Statistics);
	free(Players);
}


