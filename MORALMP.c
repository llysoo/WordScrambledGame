/***********************************************************************
This is to certify that this project is my own work, based on my personal
efforts in studying and applying the concepts learned. I have constructed 
the functions and their respective algorithms and corresponding code by 
myself. The program was run, tested, and debugged by my own efforts. I 
further certify that I have not copied in part or whole or otherwise 
plagiarized the work of other students and/or persons.
Gwyneth Allyson Moral - 12278254 - S15
***************************************************************************/


/*
 Description: This is a Word Scrambled game that will give random 15 words and allows you to guess the correct word
 Programmed by: Gwyneth Allyson Moral S15
 Last modified: 11/28/2023
 Version: 1.0
 [Acknowledgements: “Structures, Keeping High Scores of Users.” Cboard.cprogramming.com, cboard
 						.cprogramming.com/cplusplus-programming/157498-structures-keeping-high-
						 scores-users.html. Accessed 27 Nov. 2023.
					“Scoreboard Saved in .Txt File?” Cboard.cprogramming.com, cboard.cprogramming.com
						/c-programming/135750-scoreboard-saved-txt-file.html. Accessed 27 Nov. 2023.
]*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


typedef char String[21];
typedef char Date[11];

struct Date {
    int month;
    int day;
    int year;
};

struct PlayerInfo {
    String name;
    struct Date dateToday;
    int score;
};

struct HighScore {
    String name;
    int score;
    struct Date dateAchieved;
};

/* function to clear the terminal*/
void clear(){
    system("cls");
} 

/*This function presents the menu and allows user to 
choose which part of the menu they want to see or do
Precondition: players select integers 1, 2, 3, or 4.*/
void displayMenu(){
    printf("\n1- Show first letter of the word\n"
           "2- Show last letter of the word\n"
           "3- Type Answer\n"
           "4- Surrender\n");
}

/*This function allows user to input details and verify if details inputed are correct.
Precondition: correct input for player information
@param validate determines if loop should continue or not based on user input
@param player is the structure for the player informations like name, score, and date
@param player.name is the current player name 
@param player.score is the current player score 
@param player.dateToday.month is the month the current player is playing
@param player.dateToday.day is the day the current player is playing
@param player.dateToday.year is the year the current player is playing
*/

void inputPlayerDetails(struct PlayerInfo *player) {
	int validate=0;
	
    printf("Enter Name of Player: ");
    scanf("%s", player->name);

    while (!validate) {
        printf("Enter Date Today (mm dd yyyy): ");
        scanf("%d %d %d", &player->dateToday.month, &player->dateToday.day, &player->dateToday.year);

        if (player->dateToday.day >= 1 && player->dateToday.day <= 31 &&
            player->dateToday.month >= 1 && player->dateToday.month <= 12 &&
            player->dateToday.year >= 2000 && player->dateToday.year <= 3000) {
            validate=1;
        } else {
            printf("Invalid date. Please enter a valid date.\n");
            validate=0;
        }
    }
}

/*This function shuffles the words that will be given to the user 
Precondition: words are from the text file
@param words[][] is a random word from the text file
@param clues[][] is the accompanying clue for the random word
@param i used for the outer loop
@param j used fo the inner loop
@param count  is the maximum number of words
@param tempW[] used for the swapping of the words
@param tempC[] used for the swapping of the accompanying clues
*/
void Shuffle_Words(char words[50][100], char clues[50][100], int count) {
    srand(time(NULL));
    int i,j;
    for (i = count - 1; i > 0; i--) {
        j = rand() % (i + 1);

       char tempW[100];
        strcpy(tempW, words[i]);
        strcpy(words[i], words[j]);
        strcpy(words[j], tempW);
		
        char tempC[100];
        strcpy(tempC, clues[i]);
        strcpy(clues[i], clues[j]);
        strcpy(clues[j], tempC);
    }
}

/*This function loads all the contents from the text file
Precondition: text file is not empty
@param dictionaryFile is where the contents from the dictionary file will be stored
@param i for the loop in inputting the words
*/
void Load_Words(char words[50][100], char clues[50][100]) {
    int i;
    FILE *dictionaryFile = fopen("dictionary.txt", "r");
    if (dictionaryFile == NULL) {
        perror("Error opening dictionary file");
        exit(1);
    }

    for (i = 0; i < 50; i++) {
        if (fscanf(dictionaryFile, "%s %s", words[i], clues[i]) != 2) {
            fprintf(stderr, "Error reading from dictionary file.\n");
            exit(1);
        }
    }

    fclose(dictionaryFile);

    Shuffle_Words(words, clues, 50); // Shuffle the words and clues
}

/*This function counts the letters in a word
Precondition: Word is 7-10 letters
@param len stores the length
@return length of word
*/
int countWordLength(char word[]){
	int len;
	return len = strlen(word);
}

/*This function scrambles the letters in the given word
Precondition: words are 7-10 long and can be scrambled
@param word is the word to be scrambled
@param len determines the length of the word
@param temp responsible for holding the string and swapping
*/
void scrambleWord(char *word) {
    int len = countWordLength(word);
    int i, j;
    char temp;
	
    if(len > 2) {
        for (i = 1; i < len - 1; i++) {
        	j = rand() % (len - 2) + 1;
            temp = word[i];
            word[i] = word[j];
            word[j] = temp;
        }
    }
}

/*This function prints the scrambled word and clue
Precondition: words are scrambled and clue is presented
@param word is the word to be scrambled
@param len determines the length of the word
*/
void displayWordAndClue(char *word, char *clue) {
    printf("Scrambled Word: %s\n", word);
    printf("Clue: %s\n", clue);
}

/*This function looks at the input answer and determines if its wrong or right
Precondition: Input is capital letters
@param correctWord is the non scrambled word
@param answer is the guess of player
@param points is the current score of player 
@paramusedClues is the amount of clues used
@param result determines is guess and correct answer are the same
@return integer to determine if correct or incorrect and points
*/
int guessAnswer(char *correctWord, char *answer, int *points, int usedClues) {
	int result;
	
    if (strcmp(correctWord, answer) == 0) {
        printf("\nCORRECT!\n");
        if(usedClues==1){
            *points += (countWordLength(correctWord)*10) - 10;
		}else if (usedClues==2){
			*points += countWordLength(correctWord)*10 - 20;
		}
        else 
			*points += 200;
        result = 1; // Correct guess
    } else {
        printf("\nINCORRECT. TRY AGAIN.\n");
        *points -= 300;
        result =0 ; // Incorrect guess
    }
    return result;
}

/*This function is the processes the current turns of the player 
Precondition: player chooses from the following given in menu
@param loop determines if loop continues or not
@param answer is the guess of player
@param usedClues determines if clues are all used
@param result determines is guess and correct answer are the same
@param nAction is the action choice of the player
@return the continuous turn of the player
*/
int processPlayerTurn(char *correctWord, int *points) {
    String answer;
    int usedClues = 0; 
	int nAction;
	int result;
	int loop=0;
    
    while (!loop) {
        do{
        	displayMenu();
        	printf("Press the number for the action you want to do: ");
       		scanf("%d", &nAction);
       		
			if(usedClues==2 && (nAction == 1 || nAction == 2)){
				printf("You already used all your clues\n");
			}
		}while(usedClues==2 && (nAction == 1 || nAction == 2));

        switch (nAction) {
            case 1:{
            	printf("First letter of the word is %c\n", correctWord[0]);
                usedClues++;
                break;
			}

            case 2:{
                printf("Last letter of the word is %c\n", correctWord[countWordLength(correctWord) - 1]);
                usedClues++;
                break;
            }

            case 3:{
				printf("Please input answer: ");
                scanf("%s", answer);
        
				if(guessAnswer(correctWord, answer, points, usedClues)) {
                    result= 1;
					loop= 1; 
                }
                break;
			}
                
            case 4:{
            	printf("Surrendering...\n");
                result=2; 
				loop=1;
				break;
			}

            default:{
            	printf("Invalid input. Please try again.\n");
				break;
			}
    	}
    	if(*points <= 0){
            result= 2;
			loop= 1;
		}
    }
    return result; 
}

/*This function loads the highscore information from the text file 
Precondition: text file is not empty
@param i is for the loop
@param highscores the text file of the high scores
@param highScores[].name is the high score player name 
@param highScores[].score is the high score player score 
@param highScores[].dateAchieved.month is the month the player with the high score played 
@param highScores[].dateAchieved.day is the day the player with the high score played 
@param highScores[].dateAchieved.year is the year the player with the high score played 
*/
void loadHighScores(struct HighScore highScores[5]) {
	FILE *highscores = fopen("Highscore_Ranking.txt", "r");
	int i;
    for (i = 0; i < 5; ++i) {
        if (fscanf(highscores, "%s %d %d/%d/%d",
                   highScores[i].name, &highScores[i].score,
                   &highScores[i].dateAchieved.month,
                   &highScores[i].dateAchieved.day,
                   &highScores[i].dateAchieved.year) != 5) {
            break;  
        }
    }

    fclose(highscores);
}

/*This function saves the highscores information in the text file 
Precondition: information are correct
@param i is used for the loop
@param highscores the text file of the high scores 
@param highScores[].name is the name of the high score player  
@param highScores[].score is the score of the high score player  
@param highScores[].dateAchieved.month is the month the player with the high score played 
@param highScores[].dateAchieved.day is the day the player with the high score played 
@param highScores[].dateAchieved.year is the year the player with the high score played 
*/
void saveHighScores(struct HighScore highScores[5]) {
    FILE *highscores = fopen("Highscore_Ranking.txt", "w");
    if (highscores == NULL) {
        perror("Error opening high scores file");
        exit(1);
    }
    int i;
    for (i = 0; i < 5; i++) {
        fprintf(highscores, "%s %d %d/%d/%d\n",
                highScores[i].name, highScores[i].score,
                highScores[i].dateAchieved.month,
                highScores[i].dateAchieved.day,
                highScores[i].dateAchieved.year);
    }

    fclose(highscores);
}

/*This function checks if score qualifies for high score and updates 
Precondition: information is not empty
@param i is used for the outer loop
@param j is used for the outer loop
@param highscores the text file of the high scores 
@param highScores[].name is the name of the high score player  
@param highScores[].score is the score of the high score player  
@param highScores[].dateAchieved.month is the month the player with the high score played 
@param highScores[].dateAchieved.day is the day the player with the high score played 
@param highScores[].dateAchieved.year is the year the player with the high score played 
@param Replace is the index where the information will be replaced
*/
void updateAndSort(struct PlayerInfo player, struct HighScore highScores[5]) {
    int Replace = -1;
    int i,j;
    for (i = 0; i < 5; i++) {
        if (player.score > highScores[i].score || (player.score == highScores[i].score && 
            (player.dateToday.year > highScores[i].dateAchieved.year || 
            (player.dateToday.year == highScores[i].dateAchieved.year && 
            (player.dateToday.month > highScores[i].dateAchieved.month ||
            (player.dateToday.month == highScores[i].dateAchieved.month && 
            player.dateToday.day > highScores[i].dateAchieved.day)))))) {
            
            Replace = i;
            break;
        }
    }

    if (Replace != -1) {
        strncpy(highScores[Replace].name, player.name, sizeof(highScores[Replace].name) - 1);
        highScores[Replace].score = player.score;
        highScores[Replace].dateAchieved = player.dateToday;

        for (i = 0; i < 5 - 1; i++) {
            for (j = i + 1; j < 5; j++) {
                if (highScores[i].score < highScores[j].score) {
                    struct HighScore temp = highScores[i];
                    highScores[i] = highScores[j];
                    highScores[j] = temp;
                }
            }
        }
    }
}

/*this function prints the top 5 high scores 
Precondition: none
@param numScores is how many space is occupied
@param i for the loop of printing
@param totalGames is the total games player's have played
@param highScores.name is the name of the high scorers
@param highScores.score is the score of the high scorers
@param highScores.dateAchieved.month is the month played of the high scorers
@param highScores.dateAchieved.day is the day played of the high scorers
@param highScores.dateAchieved.year is the year played of the high scorers*/
void displayHighScores(struct HighScore highScores[5]) {
    printf("High Scores:\n");
    int i;
    for (i = 0; i < 5; i++) {
        printf("%d. Name: %s, Score: %d, Date: %d/%d/%d\n", i + 1, 
            highScores[i].name, highScores[i].score,
            highScores[i].dateAchieved.month, 
            highScores[i].dateAchieved.day, 
            highScores[i].dateAchieved.year);
    }
}

/*This function asks player if they want to play again or not
@param playAgain takes the user input
@return integer to determine if play again or not
*/
int askToPlayAgain(){
    int playAgain;
    printf("\nPlay again?\n1- Yes\n2- No\nEnter: ");
    scanf("%d", &playAgain);
    return playAgain;
}

int main() {
	int wholeGame=0;
	
    while(!wholeGame){
        struct PlayerInfo player;
        inputPlayerDetails(&player);
        int givenWords = 0;
    	int points = 2000;
		int loopGame=0;
    	char words[50][100];
    	char clues[50][100];
    	Load_Words(words, clues); 
	
    	while (!loopGame) {
    		int i;
			for(i=0; i<15; i++){
        		char correctWord[100];
        		char scrambledWord[100];
        		char Clue[100];
        	
        		strcpy(correctWord, words[i]);
        		strcpy(scrambledWord, correctWord);
        		scrambleWord(scrambledWord);
				strcpy(Clue, clues[i]);
			
        		printf("\nScrambled Word %d/%d:\n", givenWords + 1, 15);
        		displayWordAndClue(scrambledWord, Clue); 
    		
    			int playerTurn;
    			playerTurn=processPlayerTurn(correctWord, &points);
        	
        		if (playerTurn==1){
            		givenWords++;
            		loopGame=0;
        		}
    			if(playerTurn==2 || points<= 0 || givenWords==15 ){ // end game 
        			loopGame=1;
        			i=16;
				}
    		}
    		printf("\nGame Over! Final Score: %d\n", points);
    		
    		player.score = points;
    		struct HighScore highScores[5]={
        			{"", 0, {0, 0, 0}},
					{"", 0, {0, 0, 0}},
					{"", 0, {0, 0, 0}},
					{"", 0, {0, 0, 0}},
					{"", 0, {0, 0, 0}}};
					
    		if (points > 0){
        			loadHighScores(highScores);
            		updateAndSort(player, highScores);
            		saveHighScores(highScores);
            		displayHighScores(highScores);	
    		}  
    		
    		int again=0;
    		int playAgain;
    		
    		while(!again){
    			playAgain=askToPlayAgain();
    			if(playAgain==1){
    				wholeGame=0;
    				again=1;
    				clear();
				}else if(playAgain==2){
					wholeGame=1;
					again=1;
					clear();
					displayHighScores(highScores);
					printf("\nScramble Word End. Good Bye!");
				}else{
					printf("only enter 1 or 2!\n");
					again=0;
				}
			}
		}
    } 
	
    return 0;
}
