/*
 * LUDO GAME - Dev C++ Console Version
 * Supports 2-4 players with coloured tokens, captures, safe squares,
 * home columns and win detection.
 *
 * Compile: g++ -o ludo ludo.cpp  (or press F11 in Dev-C++)
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>   // SetConsoleTextAttribute, system("cls")

using namespace std;

// ──────────────────────────────────────────────
//  Console colour helpers
// ──────────────────────────────────────────────
enum Colour {
    BLACK=0, BLUE=1, GREEN=2, CYAN=3, RED=4, MAGENTA=5,
    BROWN=6, LGRAY=7, DGRAY=8, LBLUE=9, LGREEN=10,
    LCYAN=11, LRED=12, LMAGENTA=13, YELLOW=14, WHITE=15
};

HANDLE hConsole;

void setColour(int fg, int bg = BLACK) {
    SetConsoleTextAttribute(hConsole, (bg << 4) | fg);
}
void resetColour() { setColour(WHITE, BLACK); }

// ──────────────────────────────────────────────
//  Constants
// ──────────────────────────────────────────────
const int NUM_TOKENS   = 4;       // tokens per player
const int TRACK_LENGTH = 52;      // shared outer track
const int HOME_LENGTH  = 5;       // home-column length
const int FINISH_POS   = TRACK_LENGTH + HOME_LENGTH; // 57 = finished

// Starting positions on the outer track (0-indexed)
const int START_POS[4] = { 0, 13, 26, 39 };

// Safe squares (0-indexed on the 52-step track)
const int SAFE_SQUARES[] = { 0, 8, 13, 21, 26, 34, 39, 47 };
const int NUM_SAFE = 8;

// Player display colours
const int PLAYER_FG[4] = { LRED, LGREEN, YELLOW, LBLUE };
const string PLAYER_NAME[4] = { "Red", "Green", "Yellow", "Blue" };
const char TOKEN_CHAR[4] = { 'R', 'G', 'Y', 'B' };

// ──────────────────────────────────────────────
//  Outer track: board cell (col, row) for each step 0-51
// ──────────────────────────────────────────────
const int TRACK_COL[52] = {
     1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 7, 8, 8,
     8, 8, 8, 8, 9,10,11,12,13,14,14,14,14,14,14,13,
    12,11,10, 9, 8, 8, 8, 8, 8, 8, 7, 6, 6, 6, 6, 6,
     6, 5, 4, 3, 2, 1
};
const int TRACK_ROW[52] = {
     6, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0, 0, 1,
     2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 7, 8, 9,10,11,12,
    13,14,14,13,12,11,10, 9, 8, 7, 8, 8, 9,10,11,12,
    13,14,14,13,12,11
};

// ──────────────────────────────────────────────
//  Game state
// ──────────────────────────────────────────────
int  tokenPos[4][NUM_TOKENS];   // -1=yard, 0..56=track+home, 57=finished
bool tokenHome[4][NUM_TOKENS];  // true when token has finished
int  numPlayers;
int  finishedCount[4];

// ──────────────────────────────────────────────
//  Helpers
// ──────────────────────────────────────────────
int rollDice() { return rand() % 6 + 1; }

bool isSafe(int trackPos) {
    for (int i = 0; i < NUM_SAFE; i++)
        if (SAFE_SQUARES[i] == trackPos) return true;
    return false;
}

// Convert (player, step) -> board (col, row)
void getTokenCell(int player, int step, int &col, int &row) {
    if (step < 0 || step >= FINISH_POS) { col = -1; row = -1; return; }

    if (step < TRACK_LENGTH) {
        int a = (step + START_POS[player]) % TRACK_LENGTH;
        col = TRACK_COL[a];
        row = TRACK_ROW[a];
        return;
    }

    int hs = step - TRACK_LENGTH; // 0..4
    switch (player) {
        case 0: col = 7;        row = 6 - (hs+1); break; // Red:    col7 rows 5->1
        case 1: col = 7;        row = 8 + (hs+1); break; // Green:  col7 rows 9->13
        case 2: col = 7+(hs+1); row = 7;           break; // Yellow: row7 cols 8->12
        case 3: col = 6-(hs+1); row = 7;           break; // Blue:   row7 cols 5->1
    }
}

// ──────────────────────────────────────────────
//  Board cell classifiers
// ──────────────────────────────────────────────
bool inYard(int col, int row, int player) {
    if (player==0) return col>=0&&col<=5  && row>=0&&row<=5;
    if (player==1) return col>=9&&col<=14 && row>=0&&row<=5;
    if (player==2) return col>=0&&col<=5  && row>=9&&row<=14;
    if (player==3) return col>=9&&col<=14 && row>=9&&row<=14;
    return false;
}
int yardPlayer(int col, int row) {
    for (int p=0; p<4; p++) if (inYard(col,row,p)) return p;
    return -1;
}
int homeStripPlayer(int col, int row) {
    if (col==7 && row>=1 && row<=5)  return 0; // Red
    if (col==7 && row>=9 && row<=13) return 1; // Green
    if (row==7 && col>=8 && col<=12) return 2; // Yellow
    if (row==7 && col>=2 && col<=6)  return 3; // Blue
    return -1;
}
bool isCentre(int col, int row) { return col>=6&&col<=8 && row>=6&&row<=8; }
bool isPath(int col, int row) {
    if (col>=6&&col<=8 && row>=0&&row<=5)  return true;
    if (col>=6&&col<=8 && row>=9&&row<=14) return true;
    if (col>=0&&col<=5 && row>=6&&row<=8)  return true;
    if (col>=9&&col<=14&& row>=6&&row<=8)  return true;
    return false;
}

// ──────────────────────────────────────────────
//  Board Drawing
// ──────────────────────────────────────────────
void drawBoard() {
    char dispChar[15][15];
    int  dispPlayer[15][15];
    for (int r=0;r<15;r++) for (int c=0;c<15;c++) { dispChar[r][c]=' '; dispPlayer[r][c]=-1; }

    for (int p=0; p<numPlayers; p++)
        for (int t=0; t<NUM_TOKENS; t++) {
            int step = tokenPos[p][t];
            if (step<0 || tokenHome[p][t]) continue;
            int col,row; getTokenCell(p,step,col,row);
            if (col<0||col>14||row<0||row>14) continue;
            dispChar[row][col]=TOKEN_CHAR[p]; dispPlayer[row][col]=p;
        }

    setColour(YELLOW); cout << "\n    === LUDO ===\n\n"; resetColour();
    cout << "   ";
    for (int c=0;c<15;c++) { setColour(DGRAY); printf("%2d ",c); }
    resetColour(); cout << "\n   ";
    for (int c=0;c<15;c++) cout << "---";
    cout << "\n";

    for (int r=0;r<15;r++) {
        setColour(DGRAY); printf("%2d|",r); resetColour();
        for (int c=0;c<15;c++) {
            int yp=yardPlayer(c,r), hp=homeStripPlayer(c,r);
            bool centre=isCentre(c,r), path=isPath(c,r), safe=false;
            for (int s=0;s<NUM_SAFE&&!safe;s++) {
                int a=SAFE_SQUARES[s];
                if (TRACK_COL[a]==c&&TRACK_ROW[a]==r) safe=true;
            }
            int dp=dispPlayer[r][c];
            if (dp>=0) {
                setColour(PLAYER_FG[dp]); printf(" %c ",dispChar[r][c]);
            } else if (centre) {
                setColour(YELLOW,BROWN); cout<<" * ";
            } else if (hp>=0) {
                int bg=(hp==0?RED:(hp==1?GREEN:(hp==2?BROWN:BLUE)));
                setColour(PLAYER_FG[hp],bg); cout<<"   ";
            } else if (yp>=0) {
                int bg=(yp==0?RED:(yp==1?GREEN:(yp==2?BROWN:BLUE)));
                setColour(WHITE,bg); cout<<"   ";
            } else if (safe&&path) {
                setColour(YELLOW); cout<<" * ";
            } else if (path) {
                setColour(LGRAY); cout<<" . ";
            } else {
                setColour(BLACK); cout<<"   ";
            }
            resetColour();
        }
        setColour(DGRAY); printf("|%2d",r); resetColour(); cout<<"\n";
    }
    cout<<"   ";
    for (int c=0;c<15;c++) cout<<"---";
    cout<<"\n\n";

    for (int p=0;p<numPlayers;p++) {
        setColour(PLAYER_FG[p]); printf("  %-7s: ",PLAYER_NAME[p].c_str());
        for (int t=0;t<NUM_TOKENS;t++) {
            if (tokenHome[p][t])        { setColour(YELLOW); cout<<"[HOME] "; }
            else if (tokenPos[p][t]<0) { setColour(DGRAY);  cout<<"[YARD] "; }
            else                       { setColour(PLAYER_FG[p]); printf("[%3d]  ",tokenPos[p][t]); }
        }
        resetColour(); cout<<"\n";
    }
    cout<<"\n";
}

// ──────────────────────────────────────────────
//  Capture
// ──────────────────────────────────────────────
void checkCapture(int attacker, int movedToken) {
    int step=tokenPos[attacker][movedToken];
    if (step<0||step>=TRACK_LENGTH) return;
    int absPos=(step+START_POS[attacker])%TRACK_LENGTH;
    if (isSafe(absPos)) return;
    for (int victim=0;victim<numPlayers;victim++) {
        if (victim==attacker) continue;
        for (int t=0;t<NUM_TOKENS;t++) {
            if (tokenPos[victim][t]<0||tokenPos[victim][t]>=TRACK_LENGTH) continue;
            if (tokenHome[victim][t]) continue;
            int vAbs=(tokenPos[victim][t]+START_POS[victim])%TRACK_LENGTH;
            if (vAbs==absPos) {
                setColour(LRED);
                printf("  >> %s token %d captured %s token %d! Sent to yard.\n",
                    PLAYER_NAME[attacker].c_str(),movedToken+1,
                    PLAYER_NAME[victim].c_str(),t+1);
                resetColour();
                tokenPos[victim][t]=-1;
            }
        }
    }
}

// ──────────────────────────────────────────────
//  Move helpers
// ──────────────────────────────────────────────
bool getMovable(int player, int dice, bool canMove[]) {
    bool any=false;
    for (int t=0;t<NUM_TOKENS;t++) {
        canMove[t]=false;
        if (tokenHome[player][t]) continue;
        int pos=tokenPos[player][t];
        if (pos==-1) { if (dice==6){canMove[t]=true;any=true;} continue; }
        if (pos+dice<=FINISH_POS){canMove[t]=true;any=true;}
    }
    return any;
}

void moveToken(int player, int token, int dice) {
    int pos=tokenPos[player][token];
    if (pos==-1) {
        tokenPos[player][token]=0;
        setColour(LGREEN);
        printf("  %s token %d enters the board!\n",PLAYER_NAME[player].c_str(),token+1);
        resetColour();
        checkCapture(player,token);
        return;
    }
    int np=pos+dice;
    if (np>FINISH_POS) { setColour(DGRAY); printf("  Cannot move (overshoot).\n"); resetColour(); return; }
    tokenPos[player][token]=np;
    if (np==FINISH_POS) {
        tokenHome[player][token]=true;
        finishedCount[player]++;
        setColour(YELLOW);
        printf("  %s token %d reached HOME! (%d/4)\n",PLAYER_NAME[player].c_str(),token+1,finishedCount[player]);
        resetColour();
    } else if (np<TRACK_LENGTH) {
        checkCapture(player,token);
    }
}

// ──────────────────────────────────────────────
//  Main
// ──────────────────────────────────────────────
int main() {
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned)time(0));
    system("cls");

    setColour(YELLOW);
    cout<<"\n";
    cout<<"  ██╗     ██╗   ██╗██████╗  ██████╗ \n";
    cout<<"  ██║     ██║   ██║██╔══██╗██╔═══██╗\n";
    cout<<"  ██║     ██║   ██║██║  ██║██║   ██║\n";
    cout<<"  ██║     ██║   ██║██║  ██║██║   ██║\n";
    cout<<"  ███████╗╚██████╔╝██████╔╝╚██████╔╝\n";
    cout<<"  ╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ \n";
    resetColour();
    cout<<"\n  Welcome to LUDO!\n\n";

    do {
        cout<<"  How many players? (2, 3 or 4): ";
        cin>>numPlayers; cin.ignore();
    } while (numPlayers<2||numPlayers>4);

    for (int p=0;p<4;p++) {
        finishedCount[p]=0;
        for (int t=0;t<NUM_TOKENS;t++) { tokenPos[p][t]=-1; tokenHome[p][t]=false; }
    }

    cout<<"\n  Legend: ";
    for (int p=0;p<numPlayers;p++) { setColour(PLAYER_FG[p]); printf(" [%c]=%s ",TOKEN_CHAR[p],PLAYER_NAME[p].c_str()); }
    resetColour();
    cout<<"  [*]=safe/home  [.]=path\n\n  Press Enter to start..."; cin.get();

    int cp=0; bool gameOver=false; int winner=-1;

    while (!gameOver) {
        system("cls"); drawBoard();

        bool bonusTurn=true;
        while (bonusTurn&&!gameOver) {
            bonusTurn=false;

            setColour(PLAYER_FG[cp]);
            printf("\n  === %s's turn ===\n",PLAYER_NAME[cp].c_str());
            resetColour();
            cout<<"  Press Enter to roll...";
            cin.get();

            int dice=rollDice();
            setColour(YELLOW); printf("  Rolled: %d\n\n",dice); resetColour();

            bool canMove[NUM_TOKENS];
            bool any=getMovable(cp,dice,canMove);

            if (!any) {
                setColour(DGRAY); cout<<"  No moves available. Turn skipped.\n"; resetColour();
            } else {
                int movable=0,lastMovable=-1;
                for (int t=0;t<NUM_TOKENS;t++) if (canMove[t]){movable++;lastMovable=t;}

                int chosen=-1;
                if (movable==1) {
                    chosen=lastMovable;
                    printf("  Auto-moving token %d.\n",chosen+1);
                } else {
                    cout<<"  Choose a token to move:\n";
                    for (int t=0;t<NUM_TOKENS;t++) {
                        if (!canMove[t]) continue;
                        printf("    [%d] Token %d",t+1,t+1);
                        if (tokenPos[cp][t]<0) cout<<" (yard -> board)";
                        else printf(" (step %d -> %d)",tokenPos[cp][t],tokenPos[cp][t]+dice);
                        cout<<"\n";
                    }
                    do {
                        cout<<"  Your choice (1-4): ";
                        int inp; cin>>inp; cin.ignore();
                        chosen=inp-1;
                    } while (chosen<0||chosen>=NUM_TOKENS||!canMove[chosen]);
                }
                moveToken(cp,chosen,dice);
                if (finishedCount[cp]==NUM_TOKENS){gameOver=true;winner=cp;}
            }

            if (dice==6&&!gameOver) {
                setColour(LCYAN); cout<<"\n  Rolled a 6 – bonus turn!\n"; resetColour();
                bonusTurn=true;
                system("cls"); drawBoard();
            }
        }
        if (!gameOver) cp=(cp+1)%numPlayers;
    }

    system("cls"); drawBoard();
    setColour(YELLOW); cout<<"\n  *** ";
    setColour(PLAYER_FG[winner]); cout<<PLAYER_NAME[winner];
    setColour(YELLOW); cout<<" WINS! Congratulations! ***\n\n";
    resetColour();
    cout<<"  Press Enter to exit..."; cin.get();
    return 0;
}
