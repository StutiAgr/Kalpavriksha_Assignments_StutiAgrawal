#include<stdio.h>
#include<stdlib.h>
#include "Players_data.h"

#define MAX_NAME_LENGTH 51
#define MAX_PLAYER_ID 1500
#define NUMBER_OF_TEAMS 10

typedef struct PlayerNode{
    int playerID;
    char* name;
    char* teamName;
    char* role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* next;
} PlayerNode;

typedef struct{
    int teamID;
    char* name;
    int totalPlayers;
    int totalBatsmenAllRounders;
    float averageBattingStrikeRate;
    float avgStrikeRateBatterAllRounder;
    PlayerNode *batsmen;
    PlayerNode *bowlers;
    PlayerNode *allRounders;
} TeamNode;

//PlayerNode* playerList = NULL;
TeamNode teamList[NUMBER_OF_TEAMS];

int stringLength(const char* str){
    int length = 0;
    while(*str){
        length++;
        str++;
    }
    return length;
}

char* stringCopy(const char* source){
    char* destination = (char*)malloc(stringLength(source) + 1);
    char* ptr = destination;
    while(*source){
        *ptr = *source;
        source++;
        ptr++;
    }
    *ptr = '\0';
    return destination;
}

int stringCompare(const char* str1, const char* str2){
    while(*str1 && *str2){
        if(*str1 < *str2){
            return -1;
        }
        else if(*str1 > *str2){
            return 1;
        }
        str1++;
        str2++;
    }
    if(*str1) return 1;
    if(*str2) return -1;
    return 0;
}

void initializeTeams(){
    for(int i = 0; i < teamCount; i++){
        teamList[i].teamID = i + 1;
        teamList[i].name = stringCopy(teams[i]);
        teamList[i].totalPlayers = 0;
        teamList[i].totalBatsmenAllRounders = 0;
        teamList[i].averageBattingStrikeRate = 0;
        teamList[i].avgStrikeRateBatterAllRounder = 0;
        teamList[i].batsmen = NULL;
        teamList[i].bowlers = NULL;
        teamList[i].allRounders = NULL;
    }
}

void insertSorted(PlayerNode** head, PlayerNode* newPlayer){
    if(*head == NULL || newPlayer->performanceIndex > (*head)->performanceIndex){
        newPlayer->next = *head;
        *head = newPlayer;
        return;
    }
    PlayerNode* current = *head;
    while(current->next != NULL && current->next->performanceIndex > newPlayer->performanceIndex){
        current = current->next;
    }
    newPlayer->next = current->next;
    current->next = newPlayer;
}

int binarySearchTeam(const char* teamName){
    int left = 0, right = teamCount - 1;
    while(left <= right){
        int mid = left + (right - left) / 2;
        int comparision = stringCompare(teamList[mid].name, teamName);
        if(comparision == 0) return mid;
        else if(comparision < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

void updateTeam(PlayerNode* newPlayer){
    int teamIndex = binarySearchTeam(newPlayer->teamName);
    if(teamIndex == -1){
        printf("No team found.\n");
        return;
    }
    if(stringCompare(newPlayer->role, "Batsman") == 0){
        insertSorted(&teamList[teamIndex].batsmen, newPlayer);
        teamList[teamIndex].avgStrikeRateBatterAllRounder = ((teamList[teamIndex].avgStrikeRateBatterAllRounder * teamList[teamIndex].totalBatsmenAllRounders) + newPlayer->strikeRate) / (teamList[teamIndex].totalBatsmenAllRounders + 1);
        teamList[teamIndex].totalBatsmenAllRounders++;
    }
    else if(stringCompare(newPlayer->role, "All-rounder") == 0){
        insertSorted(&teamList[teamIndex].allRounders, newPlayer);
        teamList[teamIndex].avgStrikeRateBatterAllRounder = ((teamList[teamIndex].avgStrikeRateBatterAllRounder * teamList[teamIndex].totalBatsmenAllRounders) + newPlayer->strikeRate) / (teamList[teamIndex].totalBatsmenAllRounders + 1);
        teamList[teamIndex].totalBatsmenAllRounders++;
    }
    else{
        insertSorted(&teamList[teamIndex].bowlers, newPlayer);
    }
    teamList[teamIndex].averageBattingStrikeRate = ((teamList[teamIndex].averageBattingStrikeRate * teamList[teamIndex].totalPlayers) + newPlayer->strikeRate) / (teamList[teamIndex].totalPlayers + 1);
    teamList[teamIndex].totalPlayers++;
}

float calculatePerformanceIndex(PlayerNode* newPlayer){
    float performanceIndex;
    if(stringCompare(newPlayer->role, "Batsman") == 0){
        performanceIndex = (newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f;
    }
    else if(stringCompare(newPlayer->role, "Bowler") == 0){
        performanceIndex = (newPlayer->wickets * 2.0f) + (100.0f - newPlayer->economyRate);
    }
    else{
        performanceIndex = ((newPlayer->battingAverage * newPlayer->strikeRate) / 100.0f) + (newPlayer->wickets * 2.0f);
    }
    return performanceIndex;
}

void initializePlayers(){
    for(int i = 0; i < playerCount; i++){
        PlayerNode *newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
        newPlayer->playerID = players[i].id;
        newPlayer->name = stringCopy(players[i].name);
        newPlayer->teamName = stringCopy(players[i].team);
        newPlayer->role = stringCopy(players[i].role);
        newPlayer->totalRuns = players[i].totalRuns;
        newPlayer->battingAverage = players[i].battingAverage;
        newPlayer->strikeRate = players[i].strikeRate;
        newPlayer->wickets = players[i].wickets;
        newPlayer->economyRate = players[i].economyRate;
        newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
        newPlayer->next = NULL;
        updateTeam(newPlayer);
    }
}

int getValidRole(){
    int role;
    while(1){
        if(scanf("%d", &role) != 1){
            printf("Invalid Role. Enter again: ");
            while(getchar() != '\n');
        }
        else{
            if(getchar() != '\n' || role < 1 || role > 3){
                printf("Invalid Role. Enter again: ");
                while(getchar() != '\n');
            }
            else{
                return role;
            }
        }
    }
}

int getValidInteger(){
    int number;
    while(1){
        if(scanf("%d", &number) != 1){
            printf("Invalid input. Please enter again: ");
            while(getchar() != '\n');
        }
        else{
            if(getchar() != '\n'){
                printf("Invalid input. Please enter again: ");
                while(getchar() != '\n');
            }
            else{
                return number;
            }
        }
    }
}

float getValidFloat(){
    float number;
    while(1){
        if(scanf("%f", &number) != 1){
            printf("Invalid input. Please enter again: ");
            while(getchar() != '\n');
        }
        else{
            if(getchar() != '\n'){
                printf("Invalid input. Please enter again: ");
                while(getchar() != '\n');
            }
            else{
                return number;
            }
        }
    }
}

int getValidTeamId(){
    while(1){
        int id = getValidInteger();
        if(id < 1 || id > teamCount){
            printf("Enter ID in range 1-10: ");
        }
        else{
            return id;
        }
    }
}

int playerIdExists(int id){
    for(int i = 0; i < teamCount; i++){
        PlayerNode* temp = teamList[i].batsmen;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
        temp = teamList[i].allRounders;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
        temp = teamList[i].bowlers;
        while(temp != NULL){
            if(temp->playerID == id) return 1;
            temp = temp->next;
        }
    }
    return 0;
}

int getValidPlayerId(){
    while(1){
        int id = getValidInteger();
        if(id < 1 || id > MAX_PLAYER_ID){
            printf("Enter ID in range 1-1000: ");
        }
        else if(playerIdExists(id)){
            printf("ID already exists! Enter again: ");
        }
        else{
            return id;
        }
    }
}

void addNewPlayer(){
    PlayerNode* newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
    if(!newPlayer){
        printf("Failed to allocate memory!\n");
        return;
    }

    printf("Enter Team ID to add player: ");
    int teamID = getValidTeamId();
    newPlayer->teamName = stringCopy(teamList[teamID - 1].name);
    printf("Enter Player Details: \n");

    printf("Player ID: ");
    newPlayer->playerID = getValidPlayerId();

    printf("Name: ");
    char* name = (char*)malloc(MAX_NAME_LENGTH);
    fgets(name, MAX_NAME_LENGTH, stdin);
    if(*(name + stringLength(name) - 1) == '\n'){
        *(name + stringLength(name) - 1) = '\0';
    }
    newPlayer->name = stringCopy(name);
    free(name);

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    if(role == 1) newPlayer->role = stringCopy("Batsman");
    else if(role == 2) newPlayer->role = stringCopy("Bowler");
    else newPlayer->role = stringCopy("All-rounder");

    printf("Total Runs: ");
    newPlayer->totalRuns = getValidInteger();

    printf("Batting Average: ");
    newPlayer->battingAverage = getValidFloat();

    printf("Strike Rate: ");
    newPlayer->strikeRate = getValidFloat();

    printf("Wickets: ");
    newPlayer->wickets = getValidInteger();

    printf("Economy Rate: ");
    newPlayer->economyRate = getValidFloat();

    newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
    newPlayer->next = NULL;
    updateTeam(newPlayer);

    printf("Player added successfully to Team %s!\n", newPlayer->teamName);
}

void playerDetails(PlayerNode* newPlayer){
        printf("%-5d %-20s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n", newPlayer->playerID, newPlayer->name, newPlayer->role, newPlayer->totalRuns, newPlayer->battingAverage, newPlayer->strikeRate, newPlayer->wickets, newPlayer->economyRate, newPlayer->performanceIndex);
}

void displayPlayersOfTeam(){
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    printf("Players of Team %s:\n", teamList[teamID - 1].name);
    printf("\n====================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================\n");
    PlayerNode* temp = teamList[teamID - 1].batsmen;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    temp = teamList[teamID - 1].allRounders;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    temp = teamList[teamID - 1].bowlers;
    while(temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    printf("\n====================================================================================================\n");
    printf("\nTotal Players: %d\n", teamList[teamID - 1].totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n",teamList[teamID - 1].averageBattingStrikeRate);
}

void displayTeamsBySR(){
    int teamIndices[teamCount];
    for(int i = 0; i < teamCount; i++){
        teamIndices[i] = i;
    }
    for(int i = 0; i < teamCount; i++){
        for(int j = 0; j < teamCount - i - 1; j++){
            if(teamList[teamIndices[j]].avgStrikeRateBatterAllRounder < teamList[teamIndices[j + 1]].avgStrikeRateBatterAllRounder){
                int temp = teamIndices[j];
                teamIndices[j] = teamIndices[j + 1];
                teamIndices[j + 1] = temp;
            }
        }
    }
    printf("Teams Sorted by Average Batting Strike Rate");
    printf("\n=========================================================\n");
    printf("%-3s %-15s %-10s %-14s", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("\n=========================================================\n");
    for(int i = 0; i < teamCount; i++){
        printf("%-3d %-15s %-10.2f %-2d\n", teamList[teamIndices[i]].teamID, teamList[teamIndices[i]].name, teamList[teamIndices[i]].avgStrikeRateBatterAllRounder, teamList[teamIndices[i]].totalPlayers);
    }
    printf("\n=========================================================\n");

}

void displayTopKPlayers(){
    printf("Enter Team ID: ");
    int teamID = getValidTeamId();
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    printf("Enter number of players: ");
    int k = getValidInteger();

    printf("Top %d %s of Team %s: ", k, (role == 1 ? "Batsmen" : (role == 2 ? "Bowlers" : "All-rounders")), teamList[teamID - 1].name);
    printf("\n====================================================================================================\n");
    printf("%-5s %-20s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================\n");
    PlayerNode* temp;
    if(role == 1) temp = teamList[teamID - 1].batsmen;
    else if(role == 2) temp = teamList[teamID - 1].bowlers;
    else temp = teamList[teamID - 1].allRounders;
    while(k-- && temp != NULL){
        playerDetails(temp);
        temp = temp->next;
    }
    printf("\n====================================================================================================\n");

}

void displayAllPlayersByRole(){
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    int role = getValidRole();
    PlayerNode* teamPlayers[teamCount];
    for(int i = 0; i < teamCount; i++){
        if(role == 1){
            teamPlayers[i] = teamList[i].batsmen;
        }
        else if(role == 2){
            teamPlayers[i] = teamList[i].bowlers;
        }
        else{
            teamPlayers[i] = teamList[i].allRounders;
        }
    }
    printf("\n====================================================================================================================\n");
    printf("%-5s %-20s %-15s %-15s %-8s %-8s %-8s %-8s %-8s %-12s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("\n====================================================================================================================\n");

    while(1){
        int bestIndex = -1;
        float bestPI = -1;

        for(int i = 0; i < teamCount; i++){
            if(teamPlayers[i] != NULL){
                if(teamPlayers[i]->performanceIndex > bestPI){
                    bestPI = teamPlayers[i]->performanceIndex;
                    bestIndex = i;
                }
            }
        }
        if(bestIndex == -1) break;
        PlayerNode* temp = teamPlayers[bestIndex];
        printf("%-5d %-20s %-15s %-15s %-8d %-8.2f %-8.2f %-8d %-8.2f %-12.2f\n", temp->playerID, temp->name, temp->teamName, temp->role, temp->totalRuns, temp->battingAverage, temp->strikeRate, temp->wickets, temp->economyRate, temp->performanceIndex);
        teamPlayers[bestIndex] = teamPlayers[bestIndex]->next;
    }
    printf("\n====================================================================================================================\n");
}

void freeMemory(){
    for(int i = 0; i < teamCount; i++){
        PlayerNode* lists[3] = {teamList[i].batsmen, teamList[i].bowlers, teamList[i].allRounders};
        for(int role = 0; role < 3; role++){
            PlayerNode* current = lists[role];
            while(current != NULL){
                PlayerNode* temp = current;
                current = current->next;
                free(temp->name);
                free(temp->teamName);
                free(temp->role);
                free(temp);
            }
        }
        free(teamList[i].name);
    }
}

void displayMenu(){
    printf("\n==============================================================================\n");
    printf("ICC ODI Player Performance Analyzer");
    printf("\n==============================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams by performance index\n");
    printf("6. Exit");
    printf("\n==============================================================================\n");
    printf("Enter your choice: ");
}

void initiateAnalyzer(){
    int choice = 0;
    while(choice != 6){
        displayMenu();
        choice = getValidInteger();
        switch(choice){
            case 1:
                addNewPlayer();
                break;
            case 2:
                displayPlayersOfTeam();
                break;
            case 3:
                displayTeamsBySR();
                break;
            case 4:
                displayTopKPlayers();
                break;
            case 5:
                displayAllPlayersByRole();
                break;
            case 6:
                freeMemory();
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main(){
    initializeTeams();
    initializePlayers();
    initiateAnalyzer();
}
