#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#define SEQUENCELENGTH 4
#define MAXTENTATIVES 9
#define COLORS "RBVG"

// Lista Prototipi delle funzioni implementate
void printInfo();
void generateCode(char encoderCode[]);
void playGame(char encoderCode[], char playerCode[]);
void insertCode(char playerCode[], char encoderCode[]);
void clearScreen();

int main(){

    // Chiamata metodo per inizializzare il generatore pseudocasuale di numeri interi random
    srand(time(NULL));

    // Array di caratteri per sequenza da indovinare e sequenza inserita dal giocatore
    char encoderCode[SEQUENCELENGTH+1] = {0};
    char playerCode[SEQUENCELENGTH+1] = {0};

    // Chiamata metodo di stampa delle regole e informazioni di gioco
    printInfo();

    // Chiamata metodo di generazione della sequenza da indovinare
    generateCode(encoderCode);

    // Chiamata metodo di avvio del gioco
    playGame(encoderCode, playerCode);

    return EXIT_SUCCESS;

}

void printInfo(){

    FILE * file;
    file = fopen("info.txt", "r");

    if(file != NULL){

        char ch;
        while((ch = fgetc(file)) != EOF){
            printf("%c", ch);
        }

        fclose(file);

    } else {
        printf("Info sul gioco non disponibili\n");
        printf("Colori disponibili:\n", COLORS);
        printf("R --> ROSSO \n");
        printf("B --> BLU \n");
        printf("V --> VERDE \n");
        printf("G --> GIALLO \n");
        printf("Lunghezza sequenza: %d\n", SEQUENCELENGTH);
        printf("Numero massimo tentativi: %d\n\n", MAXTENTATIVES);
    }

}

void generateCode(char encoderCode[]){

    int i;
    char colors[5] = COLORS;

    for(i = 0; i < SEQUENCELENGTH; i++){
        encoderCode[i] = colors[rand() % 4];
    }

}

void playGame(char encoderCode[], char playerCode[]){

    // Array per segnare i colori neri
    int checkedDigits[SEQUENCELENGTH];

    // Contatore tentativi giocatore
    int playerTentativesCounter = 0;

    // Contatori per i neri e i bianchi
    int blacks;
    int whites;

    // Contatori colori presenti nella sequenza generata dal computer
    int encoderRedCounter;
    int encoderBlueCounter;
    int encoderGreenCounter;
    int encoderYellowCounter;

    // Contatori colori presenti nella sequenza inserita dal giocatore
    int playerRedCounter;
    int playerBlueCounter;
    int playerGreenCounter;
    int playerYellowCounter;

    // Contatori generici per il computer e il giocatore
    int encoderCounter;
    int playerCounter;

    // Contatore per i cicli for
    int i;

    // Booleano per stabilire se il gioco è terminato (per vittoria o per sconfitta)
    bool isGameFinished = false;

    do {

        // Inizializzazione contatori
        blacks = 0;
        whites = 0;
        encoderRedCounter = 0;
        encoderBlueCounter = 0;
        encoderGreenCounter = 0;
        encoderYellowCounter = 0;
        playerRedCounter = 0;
        playerBlueCounter = 0;
        playerGreenCounter = 0;
        playerYellowCounter = 0;
        encoderCounter = 0;
        playerCounter = 0;

        // Inizializzazione array checkedDigits
        for(i = 0; i < SEQUENCELENGTH; i++){
            checkedDigits[i] = 0;
        }

        // Inserimento sequenza da parte del giocatore
        insertCode(playerCode, encoderCode);

        // Calcolo neri
        for(i = 0; i < SEQUENCELENGTH; i++){

            if(playerCode[i] == encoderCode[i]){

                blacks++;
                checkedDigits[i] = 1;

            } else {

                switch(encoderCode[i]){
                    case 'R':
                        encoderRedCounter++;
                        break;
                    case 'B':
                        encoderBlueCounter++;
                        break;
                    case 'V':
                        encoderGreenCounter++;
                        break;
                    case 'G':
                        encoderYellowCounter++;
                        break;
                }

            }
        }

        // Verifica se il giocatore ha indovinato la sequenza
        if(blacks == SEQUENCELENGTH){

            printf("\nHai vinto!\n");
            isGameFinished = true;

        } else {

            // Calcolo bianchi
            for(i = 0; i < SEQUENCELENGTH; i++){

                if(checkedDigits[i] == 0){

                    switch(playerCode[i]){
                        case 'R':
                            playerRedCounter++;
                            encoderCounter = encoderRedCounter;
                            playerCounter = playerRedCounter;
                            break;
                        case 'B':
                            playerBlueCounter++;
                            encoderCounter = encoderBlueCounter;
                            playerCounter = playerBlueCounter;
                            break;
                        case 'V':
                            playerGreenCounter++;
                            encoderCounter = encoderGreenCounter;
                            playerCounter = playerGreenCounter;
                            break;
                        case 'G':
                            playerYellowCounter++;
                            encoderCounter = encoderYellowCounter;
                            playerCounter = playerYellowCounter;
                            break;
                    }

                    if(encoderCounter >= 1 && playerCounter <= encoderCounter){
                        whites++;
                    }

                }

            }

            // Pulizia schermo del terminale
            clearScreen();

            // Riepilogo tentativo
            printf("Sequenza giocatore inserita: %s \n", playerCode);
            printf("Numero colori giusti posto giusto: %d \n", blacks);
            printf("Numero colori giusti posto sbagliato: %d \n", whites);
            printf("Contatore tentativi: %d \n\n", ++playerTentativesCounter);

            // Verifica se il giocatore ha terminato i tentativi
            if(playerTentativesCounter == MAXTENTATIVES){
                printf("Hai perso!\n");
                isGameFinished = true;
            }

        }

    } while(!isGameFinished);

}

void insertCode(char playerCode[], char encoderCode[]){

    // Booleano per stabilire se la sequenza inserita dal giocatore è valida
    bool isSequenceOk = false;

    // Buffer per inserimento sequenza
    char buffer[SEQUENCELENGTH+3] = {0};

    // Contatore per ciclo for
    int i;

    do {

        // Stampa informazioni per inserimento e debug
        printf("[DEBUG] - Codice generato dal computer: %s \n", encoderCode);
        printf("Colori consentiti: R -> rosso, B --> blue, V --> verde, G --> giallo \n");
        printf("Inserisci sequenza: ");

        // Lettura sequenza dallo standard input
        fgets(buffer, SEQUENCELENGTH+3, stdin);

        // Controllo validità sequenza
        if(strlen(buffer) == SEQUENCELENGTH+1 && strspn(buffer,COLORS) == SEQUENCELENGTH){

            // Imposta valore booleano per affermare che la sequenza è valida
            isSequenceOk = true;

            // Inserisci colori nell'array della sequenza del giocatore
            for(i = 0; i < SEQUENCELENGTH; i++){
                playerCode[i] = buffer[i];
            }

        } else {

            // Pulisci schermo del terminale
            clearScreen();

            // Pulisci array buffer
            memset(buffer, 0, SEQUENCELENGTH+2);

            printf("SEQUENZA NON VALIDA!\n\n");

        }

        // Pulizia standard input
        fflush(stdin);

    } while(!isSequenceOk);

}

void clearScreen(){

#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
#endif

}
