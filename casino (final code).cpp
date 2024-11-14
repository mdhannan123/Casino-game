#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_PLAYERS 10
char name[50];

typedef struct {
    char name[50];
    float deposit;
} Player;

Player players[MAX_PLAYERS];
int numPlayers = 0;

int isValidDeposit(float amount) {
    if (amount > 0 && amount < 100000.0) {
        return 1;
    }
    return 0;
}

void loadPlayerData() {
    FILE *file = fopen("player_data.txt", "r");
    if (file == NULL) {
        printf("No previous player data found.\n");
        return;
    }

    numPlayers = 0;
    while (numPlayers < MAX_PLAYERS && fscanf(file, "%s %f", players[numPlayers].name, &players[numPlayers].deposit) == 2) {
        numPlayers++;
    }

    fclose(file);
}

void savePlayerData() {
    FILE *file = fopen("player_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numPlayers; i++) {
        fprintf(file, "%s %f\n", players[i].name, players[i].deposit);
    }

    fclose(file);
}

int rules() {
    printf("WELCOME TO THE GAME \n\n\n");
    printf("RULES\n");
    printf("..................\n");
    printf("1. Choose any number between 1 to 100\n\n");
    printf("2. If you win you will get 10 times of money you bet\n\n");
    printf("3. If you bet on the wrong number you will lose your betting amount\n\n");
    printf("..................\n");
    return 0;
}

void spinAnimation() {
    int i;
    for (i = 0; i < 5; i++) {
        printf("-");
        fflush(stdout);
        usleep(200000);
    }
}

void gui();

float round1() {
    printf("\n\n");

    float bet1, new_los;

    printf("%s, Enter the money to bet: Rs.", name);
    scanf("%f", &bet1);

    printf("\n\n");

    int playerIndex = -1;
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, name) == 0) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex == -1) {
        printf("Error: Player data not found.\n");
        return 0;
    }

    if (bet1 > players[playerIndex].deposit) {
        printf("Money Exceeds.\n\n");
        printf("Enter Again: Rs.");
        scanf("%f", &bet1);
    }

    printf("\n\n");

    int computer1;
    srand(time(NULL));
    computer1 = rand() % 100 + 1;

    int guess1;
    printf("Guess a number from 1 to 100: ");
    scanf("%d", &guess1);

    printf("\n\n");

    if (guess1 > 100) {
        printf("Number Exceeds.\n\n");
        printf("Enter Again: ");
        scanf("%d", &guess1);
    }

    printf("\n\n");

    printf("Spinning the slot machine...");
    spinAnimation();
    printf("\n\n");

    if (guess1 == computer1) {
        printf("Congratulations! You have won Rs.%f\n\n", bet1);

        float bet2 = bet1 * 10;
        float new_won = bet1 == players[playerIndex].deposit ? bet2 : bet2 + players[playerIndex].deposit;

        printf("You have won Rs.%f in total\n\n", new_won);

        players[playerIndex].deposit = new_won;
    } else {
        printf("Sorry, you have lost Rs.%f\n\n", bet1);
        printf("The correct number is: %d\n\n", computer1);

        new_los = players[playerIndex].deposit - bet1;
        printf("Now you have Rs.%f as a balance\n\n", new_los);
        players[playerIndex].deposit = new_los;

        printf("\n\n");

        if (new_los == 0) {
            printf("Sir, you have Rs.0 as a balance\n\n");
            printf("You have no more money to play\n\n");
            printf("Have a nice day\n");
        } else {
            int decision;
            printf("Press 1 to play again\n\n");
            printf("Press 2 to exit\n\n");
            printf("Do you want to continue?\n ");
            scanf("%d", &decision);

            printf("\n\n");

            if (decision == 1) {
                system("cls");
                printf("Your current balance is Rs.%f", players[playerIndex].deposit);
                round1();
            } else if (decision == 2) {
                printf("Have a nice day sir\n\n");
            }
        }
    }

    return 0;
}

void gui() {
    rules();

    printf("\n\n");

    round1();

    return ;
}

int dice() {
    float bet;

    int playerIndex = -1;
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, name) == 0) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex == -1) {
        printf("Error: Player data not found.\n");
        return 0; // Exit the function since the player data is not found
    }

    printf("Welcome to the Dice Roll Casino Game!\n\n");
    printf("You have Rs.%f\n\n", players[playerIndex].deposit);

    while (players[playerIndex].deposit > 0) {
        printf("Place your bet (0 to quit): Rs.");
        scanf("%f", &bet);

        if (bet == 0) {
            printf("Thank you for playing! You leave the casino with Rs.%f\n\n", players[playerIndex].deposit);
            break;
        }

        if (bet > players[playerIndex].deposit) {
            printf("You don't have enough money to place that bet!\n\n");
            continue;
        }

        printf("Rolling the dice...\n");

        srand(time(NULL));
        int diceRoll = rand() % 6 + 1;

        printf("You rolled: %d\n", diceRoll);

        if (diceRoll == 1 || diceRoll == 3 || diceRoll == 5) {
            players[playerIndex].deposit -= bet;
            printf("Sorry, you lose Rs.%f\n\n", bet);
        } else {
            players[playerIndex].deposit += bet;
            printf("Congratulations! You win Rs.%f\n\n", bet);
        }

        printf("Your current balance is Rs.%f\n", players[playerIndex].deposit);
    }

    if (players[playerIndex].deposit <= 0) {
        printf("You have run out of money. Better luck next time!\n");
    }

    return 0;
}

int game() {
    int choice;
    printf("Choose any one: \n1>>>Play Guess Game\n2>>>Play Dice\n\n");
    printf("Enter your choice:");
    scanf("%d", &choice);
    if (choice == 1) {
        gui();
    } else {
        dice();
    }

    int decision;
    printf("Press 1 to start again\n\n");
    printf("Press any other key to exit\n\n");
    printf("Do you want to continue?\n\n ");
    scanf("%d", &decision);
    if (decision == 1) {
        printf("Your current balance is Rs.%f", players[numPlayers - 1].deposit);
        game();
    } else {
        printf("(10 percent of your remaining amount will be deducted.)\n\nYou will receive %f in your account\n\n", (players[numPlayers - 1].deposit - (players[numPlayers - 1].deposit / 10.0)));
        printf("Thank you for playing.");
    }
    return 0;
}

int main() {
    loadPlayerData();

    printf("Enter your name: ");
    scanf("%s", name);
    printf("\nWelcome %s\n\n", name);

    // Check if the player's name exists in the file
    int playerIndex = -1;
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, name) == 0) {
            playerIndex = i;
            break;
        }
    }

    if (playerIndex == -1) {
        // If the player is new, ask for the initial deposit
        float deposit_input;
        do {
            printf("Enter the amount of money to deposit for the game (limit: 5 digits): Rs.");
            scanf("%f", &deposit_input);
            if (!isValidDeposit(deposit_input)) {
                printf("Invalid deposit amount. Please enter a valid amount.\n");
            }
        } while (!isValidDeposit(deposit_input));

        strcpy(players[numPlayers].name, name);
        players[numPlayers].deposit = deposit_input;
        numPlayers++;
        playerIndex = numPlayers - 1; // Update playerIndex for the new player
    } else {
        // If the player already exists, load their deposit
        printf("\n\n");
        printf("Your current balance is Rs. %f", players[playerIndex].deposit);
        printf("\n\n");
    }

    game();

    savePlayerData();

    return 0;
}

