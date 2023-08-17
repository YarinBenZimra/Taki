#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>




#define TOTAL_CARD_DECK 52
typedef int Deck[TOTAL_CARD_DECK];


void setup_deck(Deck);
void shuffle(Deck);
void printcard(int);
void print_deck(Deck);
void player_one_cards(Deck);
void shuffle_deck(Deck);
void deal_deck(Deck);
void player_two_cards(Deck);




int main()
{

    char answer[10];

    printf("\nWould you like to play The Greatest Game of All Time? (y/n). press 'i' for info\n");

    scanf("%c", answer);

    while (strcmp(answer, "y") || strcmp(answer, "n")|| strcmp(answer, "i") || strcmp(answer, "Y"))
    {
        printf("Invalid Entry. 'y', 'n' or 'i' only : ");
        scanf("%c", answer);

    }
    if (!strcmp(answer, "y") || !strcmp(answer, "Y"))
    {

        Deck d;

        srand(time(NULL));
        setup_deck(d);
        print_deck(d);

        shuffle_deck(d);
        deal_deck(d);




    }
    else if (!strcmp(answer, "n"))
    {
        printf("Have a Nice day!");
    }
    else if (!strcmp(answer, "i"))
    {

        printf("Each suite is refered to by the first letter of their name. i.e. Hearts is H.\n");
        printf("X is for 10, J is for Jack, Q is for Queen, and K is for King");

    }


    return 0;
}




void setup_deck(Deck deck)
{
    int i;
    for (i = 0; i < TOTAL_CARD_DECK; i++)
    {
        deck[i] = i;
    }
}


void printcard(int card)
{
    static char suit[4] = { 'S','D','H','C' };
    static char face[13] = { '2','3','4','5','6','7','8','9','X','J','Q','K','A' };
    printf("%c%c", face[card % 13], suit[card / 13]);

}




void print_deck(Deck deck)
{
    int i;
    printf("\n\nDeck: \n **************** \n");

    for (i = 0; i < TOTAL_CARD_DECK; i++)
    {
        printcard(deck[i]);
        printf(" ");
    }
    printf("\n");
}


void shuffle_deck(Deck deck)
{

    int i;
    printf("\n\nRandom Deck: \n **************** \n");


    for (i = 0; i < TOTAL_CARD_DECK; i++)
    {


        int x = rand() % 52;



        int temp = deck[x];
        deck[x] = deck[i];
        deck[i] = temp;


    }
    for (i = 0; i < TOTAL_CARD_DECK; i++)
    {
        printcard(deck[i]);
        printf(" ");
    }
}


void deal_deck(Deck deck)
{



    printf("\n\nYour Cards: \n **************** \n");
    int card_number_dealt;
    int hand_player_one[26];
    int hand_player_two[26];
    char card[2];
    for (card_number_dealt = 0; card_number_dealt < 26; card_number_dealt++)
    {


        hand_player_one[card_number_dealt] = deck[card_number_dealt];

        printcard(hand_player_one[card_number_dealt]);
        printf(" ");
    }
    printf("\n\nComputer Cards: \n **************** \n");
    int x;
    for (card_number_dealt = 26, x = 0; card_number_dealt < 52, x < 26; card_number_dealt++, x++)
    {
        hand_player_two[x] = deck[card_number_dealt];
        printcard(hand_player_two[x]);
        printf(" ");
    }

    int card_number;
    int player_one_total = 0;
    int player_two_total = 0;

    printf("\n");
    for (card_number = 0; card_number < 26; card_number++)
    {
        if (hand_player_one[card_number] > hand_player_two[card_number])
        {

            printf("You Win!\n");
            player_one_total = player_one_total + 1;
        }

        else if (hand_player_one[card_number] < hand_player_two[card_number])
        {
            printf("You lose!\n");
            player_two_total = player_two_total + 1;
        }
        else if (hand_player_one[card_number] == hand_player_one[card_number])
        {

            if (hand_player_one[card_number] > hand_player_two[card_number])
            {
                printf("You Win!\n");
                player_one_total = player_one_total + 1;
                break;
            }

            else if (hand_player_one[card_number] < hand_player_one[card_number])
            {
                printf("You lose!");
                player_two_total = player_two_total + 1;
                break;
            }
        }


    }

    if (player_one_total == 26)
        printf("YOU WON THIS ROUND!!");

    else if (player_two_total == 26)
        printf("YOU LOST THIS ROUND");




    printf("\n");
}