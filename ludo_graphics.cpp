/*
 * LUDO GAME - graphics.h (WinBGIm) хувилбар
 * Dev-C++ дээр: Tools > Compiler Options > Linker:
 *   -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
 * Дараа нь F11 дарж compile & run хийнэ.
 */

#include <graphics.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

// ─────────────────────────────────────────────
//  Тогтмолууд
// ─────────────────────────────────────────────
const int WIN_W      = 700;
const int WIN_H      = 750;
const int BOARD_X    = 50;
const int BOARD_Y    = 50;
const int BOARD_SIZE = 600;
const int CELL       = 40;   // нэг нүдний хэмжээ (40px)

// Тоглогчийн өнгө (WinBGIm өнгөнүүд)
const int PLR_COLOR[4]    = { RED, GREEN, YELLOW, BLUE };
const int PLR_DARK[4]     = { DARKGRAY, DARKGRAY, BROWN, DARKGRAY };
const char PLR_NAME[4][8] = { "Red", "Green", "Yellow", "Blue" };

// Гадна замын нүдний байрлал (52 алхам)
const int TRACK_COL[52] = {
     1,2,3,4,5,6,6,6,6,6,6,7,8,8,
     8,8,8,8,9,10,11,12,13,14,14,14,14,14,14,13,
    12,11,10,9,8,8,8,8,8,8,7,6,6,6,6,6,
     6,5,4,3,2,1
};
const int TRACK_ROW[52] = {
     6,6,6,6,6,6,5,4,3,2,1,0,0,1,
     2,3,4,5,6,6,6,6,6,6,7,8,9,10,11,12,
    13,14,14,13,12,11,10,9,8,7,8,8,9,10,11,12,
    13,14,14,13,12,11
};

// Аюулгүй нүднүүд
const int SAFE[8] = { 0,8,13,21,26,34,39,47 };

// Тоглогч эхлэх байрлал
const int START_POS[4] = { 0,13,26,39 };

const int NUM_TOKENS = 4;
const int FINISH_POS = 57;

// ─────────────────────────────────────────────
//  Тоглоомын төлөв
// ─────────────────────────────────────────────
int  tokenPos[4][4];    // -1=гэрт, 0..56=зам, 57=дуусгасан
bool tokenDone[4][4];
int  finCount[4];
int  numPlayers;

// ─────────────────────────────────────────────
//  Туслах функцүүд
// ─────────────────────────────────────────────
int rollDice() { return rand() % 6 + 1; }

bool isSafe(int absPos) {
    for (int i=0;i<8;i++) if (SAFE[i]==absPos) return true;
    return false;
}

// Алхам + тоглогч → нүдний (col, row)
void stepToCell(int player, int step, int &col, int &row) {
    col = row = -1;
    if (step < 0 || step >= FINISH_POS) return;
    if (step < 52) {
        int a = (step + START_POS[player]) % 52;
        col = TRACK_COL[a]; row = TRACK_ROW[a];
        return;
    }
    int hs = step - 52;
    switch (player) {
        case 0: col=7;      row=5-(hs);   break;  // Red   : col7 дээш
        case 1: col=7;      row=9+(hs);   break;  // Green : col7 доош
        case 2: col=8+(hs); row=7;        break;  // Yellow: row7 баруун
        case 3: col=6-(hs); row=7;        break;  // Blue  : row7 зүүн
    }
}

// Нүдний (col,row) → дэлгэцийн төв (px, py)
void cellToPixel(int col, int row, int &px, int &py) {
    px = BOARD_X + col * CELL + CELL/2;
    py = BOARD_Y + row * CELL + CELL/2;
}

// ─────────────────────────────────────────────
//  Самбар зурах
// ─────────────────────────────────────────────

void fillRect(int col, int row, int cols, int rows, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(BOARD_X + col*CELL, BOARD_Y + row*CELL,
        BOARD_X + (col+cols)*CELL - 1,
        BOARD_Y + (row+rows)*CELL - 1);
}

void drawStar(int px, int py, int r) {
    setcolor(YELLOW);
    int pts[18];
    for (int i=0;i<8;i++) {
        double ang = i * 3.14159/4;
        int ri = (i%2==0) ? r : r/2;
        pts[i*2]   = px + (int)(ri * sin(ang));
        pts[i*2+1] = py - (int)(ri * cos(ang));
    }
    pts[16] = pts[0]; pts[17] = pts[1];
    setfillstyle(SOLID_FILL, YELLOW);
    fillpoly(9, pts);
}

void drawBoard() {
    setfillstyle(SOLID_FILL, WHITE);
    bar(BOARD_X, BOARD_Y, BOARD_X+BOARD_SIZE, BOARD_Y+BOARD_SIZE);

    fillRect(0,0,6,6, RED);
    setfillstyle(SOLID_FILL, WHITE);
    bar(BOARD_X+CELL, BOARD_Y+CELL, BOARD_X+5*CELL, BOARD_Y+5*CELL);

    fillRect(9,0,6,6, GREEN);
    setfillstyle(SOLID_FILL, WHITE);
    bar(BOARD_X+10*CELL, BOARD_Y+CELL, BOARD_X+14*CELL, BOARD_Y+5*CELL);

    fillRect(0,9,6,6, YELLOW);
    setfillstyle(SOLID_FILL, WHITE);
    bar(BOARD_X+CELL, BOARD_Y+10*CELL, BOARD_X+5*CELL, BOARD_Y+14*CELL);

    fillRect(9,9,6,6, BLUE);
    setfillstyle(SOLID_FILL, WHITE);
    bar(BOARD_X+10*CELL, BOARD_Y+10*CELL, BOARD_X+14*CELL, BOARD_Y+14*CELL);

    fillRect(6,0,3,6, WHITE);
    fillRect(7,1,1,5, GREEN);
    fillRect(6,9,3,6, WHITE);
    fillRect(7,9,1,5, YELLOW);
    fillRect(0,6,6,3, WHITE);
    fillRect(1,7,5,1, RED);
    fillRect(9,6,6,3, WHITE);
    fillRect(9,7,5,1, BLUE);

    int cx = BOARD_X + 6*CELL, cy = BOARD_Y + 6*CELL;
    int cs = 3*CELL;
    int tri1[8] = {cx,cy, cx+cs/2,cy+cs/2, cx,cy+cs, cx,cy};
    setfillstyle(SOLID_FILL, RED);   fillpoly(4, tri1);
    int tri2[8] = {cx+cs,cy, cx+cs/2,cy+cs/2, cx,cy, cx+cs,cy};
    setfillstyle(SOLID_FILL, GREEN); fillpoly(4, tri2);
    int tri3[8] = {cx+cs,cy+cs, cx+cs/2,cy+cs/2, cx+cs,cy, cx+cs,cy+cs};
    setfillstyle(SOLID_FILL, BLUE);  fillpoly(4, tri3);
    int tri4[8] = {cx,cy+cs, cx+cs/2,cy+cs/2, cx+cs,cy+cs, cx,cy+cs};
    setfillstyle(SOLID_FILL, YELLOW);fillpoly(4, tri4);

    for (int i=0;i<8;i++) {
        int px = BOARD_X + TRACK_COL[SAFE[i]]*CELL + CELL/2;
        int py = BOARD_Y + TRACK_ROW[SAFE[i]]*CELL + CELL/2;
        drawStar(px, py, 10);
    }

    setcolor(DARKGRAY);
    for (int c=6;c<=9;c++) {
        line(BOARD_X+c*CELL, BOARD_Y, BOARD_X+c*CELL, BOARD_Y+6*CELL);
    }
    for (int r=0;r<=6;r++) {
        line(BOARD_X+6*CELL, BOARD_Y+r*CELL, BOARD_X+9*CELL, BOARD_Y+r*CELL);
    }
    for (int c=6;c<=9;c++) {
        line(BOARD_X+c*CELL, BOARD_Y+9*CELL, BOARD_X+c*CELL, BOARD_Y+15*CELL);
    }
    for (int r=9;r<=15;r++) {
        line(BOARD_X+6*CELL, BOARD_Y+r*CELL, BOARD_X+9*CELL, BOARD_Y+r*CELL);
    }
    for (int r=6;r<=9;r++) {
        line(BOARD_X, BOARD_Y+r*CELL, BOARD_X+6*CELL, BOARD_Y+r*CELL);
    }
    for (int c=0;c<=6;c++) {
        line(BOARD_X+c*CELL, BOARD_Y+6*CELL, BOARD_X+c*CELL, BOARD_Y+9*CELL);
    }
    for (int r=6;r<=9;r++) {
        line(BOARD_X+9*CELL, BOARD_Y+r*CELL, BOARD_X+15*CELL, BOARD_Y+r*CELL);
    }
    for (int c=9;c<=15;c++) {
        line(BOARD_X+c*CELL, BOARD_Y+6*CELL, BOARD_X+c*CELL, BOARD_Y+9*CELL);
    }

    setcolor(BROWN);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    rectangle(BOARD_X, BOARD_Y, BOARD_X+BOARD_SIZE, BOARD_Y+BOARD_SIZE);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    setcolor(WHITE);
}

void drawYardTokens() {
    int offsets[4][2] = {{1,1},{10,1},{1,10},{10,10}};

    for (int p=0;p<numPlayers;p++) {
        int cnt = 0;
        for (int t=0;t<NUM_TOKENS;t++) {
            if (tokenPos[p][t]==-1 && !tokenDone[p][t]) {
                int dx = cnt%2, dy = cnt/2;
                int px = BOARD_X + (offsets[p][0]+dx*2)*CELL + CELL/2 + (dx?10:-10);
                int py = BOARD_Y + (offsets[p][1]+dy*2)*CELL + CELL/2 + (dy?10:-10);
                setfillstyle(SOLID_FILL, PLR_COLOR[p]);
                setcolor(WHITE);
                fillellipse(px, py, CELL/2-4, CELL/2-4);
                char buf[3]; sprintf(buf,"%d",t+1);
                setcolor(BLACK);
                outtextxy(px-4, py-7, buf);
                cnt++;
            }
        }
    }
}

void drawBoardTokens() {
    for (int p=0;p<numPlayers;p++) {
        for (int t=0;t<NUM_TOKENS;t++) {
            int step = tokenPos[p][t];
            if (step < 0 || tokenDone[p][t]) continue;
            int col, row;
            stepToCell(p, step, col, row);
            if (col<0||col>14||row<0||row>14) continue;
            int px, py;
            cellToPixel(col, row, px, py);
            int ox = (t%2==0?-8:8), oy = (t<2?-8:8);
            setfillstyle(SOLID_FILL, PLR_COLOR[p]);
            setcolor(BLACK);
            fillellipse(px+ox, py+oy, CELL/2-6, CELL/2-6);
            char buf[3]; sprintf(buf,"%d",t+1);
            setcolor(BLACK);
            outtextxy(px+ox-4, py+oy-7, buf);
        }
    }
}

void drawInfoPanel(int curPlayer, int dice, const char* msg) {
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, BOARD_Y+BOARD_SIZE+5, WIN_W, WIN_H);

    int y = BOARD_Y + BOARD_SIZE + 15;

    for (int p=0;p<numPlayers;p++) {
        setcolor(PLR_COLOR[p]);
        char buf[64];
        sprintf(buf, "%s:", PLR_NAME[p]);
        outtextxy(10 + p*170, y, buf);
        for (int t=0;t<NUM_TOKENS;t++) {
            if (tokenDone[p][t]) {
                setcolor(YELLOW); outtextxy(10+p*170 + t*30, y+18, "H");
            } else if (tokenPos[p][t]<0) {
                setcolor(DARKGRAY); outtextxy(10+p*170 + t*30, y+18, "Y");
            } else {
                setcolor(PLR_COLOR[p]);
                char s[4]; sprintf(s,"%d",tokenPos[p][t]);
                outtextxy(10+p*170 + t*35, y+18, s);
            }
        }
    }

    setcolor(WHITE);
    char info[80];
    if (dice > 0)
        sprintf(info, ">>> %s   Шоо: %d   %s", PLR_NAME[curPlayer], dice, msg);
    else
        sprintf(info, ">>> %s   %s", PLR_NAME[curPlayer], msg);
    outtextxy(10, y+45, info);
}

void drawDice(int val) {
    int x = WIN_W-110, y = BOARD_Y+BOARD_SIZE+10;
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(DARKGRAY);
    bar(x, y, x+80, y+70);
    rectangle(x, y, x+80, y+70);
    if (val<=0) return;
    setfillstyle(SOLID_FILL, BLACK);
    int dots[6][6][2] = {
        {{40,35}},
        {{20,20},{60,50}},
        {{20,20},{40,35},{60,50}},
        {{20,20},{60,20},{20,50},{60,50}},
        {{20,20},{60,20},{40,35},{20,50},{60,50}},
        {{20,20},{60,20},{20,35},{60,35},{20,50},{60,50}}
    };
    for (int i=0;i<val;i++) {
        fillellipse(x+dots[val-1][i][0], y+dots[val-1][i][1], 6, 6);
    }
}

int showStartScreen() {
    cleardevice();
    setbkcolor(BLACK);

    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    setcolor(YELLOW);
    outtextxy(200, 80, "LUDO");

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(150, 180, "Хэдэн тоглогч? (2, 3, 4)");

    setcolor(RED);    outtextxy(150, 240, "[2] 2 тоглогч");
    setcolor(GREEN);  outtextxy(150, 280, "[3] 3 тоглогч");
    setcolor(YELLOW); outtextxy(150, 320, "[4] 4 тоглогч");

    setcolor(DARKGRAY);
    outtextxy(150, 400, "Гарнаасаа 2, 3, 4 товчийг дарна уу");

    while (true) {
        if (kbhit()) {
            char c = getch();
            if (c=='2') return 2;
            if (c=='3') return 3;
            if (c=='4') return 4;
        }
    }
}

void checkCapture(int atk, int atkTok) {
    int step = tokenPos[atk][atkTok];
    if (step<0||step>=52) return;
    int absPos = (step+START_POS[atk])%52;
    if (isSafe(absPos)) return;
    for (int v=0;v<numPlayers;v++) {
        if (v==atk) continue;
        for (int t=0;t<NUM_TOKENS;t++) {
            if (tokenPos[v][t]<0||tokenPos[v][t]>=52||tokenDone[v][t]) continue;
            int vAbs=(tokenPos[v][t]+START_POS[v])%52;
            if (vAbs==absPos) tokenPos[v][t]=-1;
        }
    }
}

bool moveToken(int player, int token, int dice) {
    int pos = tokenPos[player][token];
    if (pos==-1) {
        tokenPos[player][token]=0;
        checkCapture(player,token);
        return true;
    }
    int np=pos+dice;
    if (np>FINISH_POS) return false;
    tokenPos[player][token]=np;
    if (np==FINISH_POS) {
        tokenDone[player][token]=true;
        finCount[player]++;
    } else if (np<52) {
        checkCapture(player,token);
    }
    return true;
}

bool getMovable(int player, int dice, bool canMove[]) {
    bool any=false;
    for (int t=0;t<NUM_TOKENS;t++) {
        canMove[t]=false;
        if (tokenDone[player][t]) continue;
        int pos=tokenPos[player][t];
        if (pos==-1) { if(dice==6){canMove[t]=true;any=true;} continue; }
        if (pos+dice<=FINISH_POS){canMove[t]=true;any=true;}
    }
    return any;
}

void showWinner(int winner) {
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    setcolor(PLR_COLOR[winner]);
    char msg[40]; sprintf(msg, "%s WINS!", PLR_NAME[winner]);
    outtextxy(150, 200, msg);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(200, 320, "Дурын товч дарна уу...");
    getch();
}

int main() {
    srand((unsigned)time(0));

    initwindow(WIN_W, WIN_H, "LUDO - graphics.h");

    numPlayers = showStartScreen();

    for (int p=0;p<4;p++) {
        finCount[p]=0;
        for (int t=0;t<NUM_TOKENS;t++) { tokenPos[p][t]=-1; tokenDone[p][t]=false; }
    }

    int cp=0, dice=0, winner=-1;
    bool gameOver=false;
    bool waitingRoll=true;

    drawBoard();
    drawYardTokens();
    drawInfoPanel(cp, 0, "Enter дарж шоо хаяна уу");

    while (!gameOver) {

        if (waitingRoll) {
            if (kbhit()) {
                char c=getch();
                if (c==13||c==' ') {
                    dice = rollDice();
                    bool canMove[4];
                    bool any = getMovable(cp, dice, canMove);

                    if (!any) {
                        drawBoard(); drawYardTokens(); drawBoardTokens();
                        drawDice(dice);
                        drawInfoPanel(cp, dice, "Зөөх боломжгүй - дараагийн тоглогч");
                        delay(1500);
                        cp=(cp+1)%numPlayers;
                        drawBoard(); drawYardTokens(); drawBoardTokens();
                        drawDice(0);
                        drawInfoPanel(cp, 0, "Enter дарж шоо хаяна уу");
                    } else {
                        int movable=0, last=-1;
                        for (int t=0;t<NUM_TOKENS;t++) if(canMove[t]){movable++;last=t;}

                        drawBoard(); drawYardTokens(); drawBoardTokens();
                        drawDice(dice);

                        if (movable==1) {
                            char msg[60];
                            sprintf(msg, "Шоо:%d - Token%d автоматаар зөөнө", dice, last+1);
                            drawInfoPanel(cp, dice, msg);
                            delay(800);
                            moveToken(cp, last, dice);
                            drawBoard(); drawYardTokens(); drawBoardTokens();

                            if (finCount[cp]==NUM_TOKENS) {
                                winner=cp; gameOver=true; break;
                            }
                            if (dice==6) {
                                drawInfoPanel(cp, dice, "6 - Дахин хаяна! Enter дарна уу");
                            } else {
                                cp=(cp+1)%numPlayers;
                                drawDice(0);
                                drawInfoPanel(cp, 0, "Enter дарж шоо хаяна уу");
                            }
                        } else {
                            char msg[80]="Токен сонгоно уу: ";
                            for (int t=0;t<NUM_TOKENS;t++) {
                                if (canMove[t]) {
                                    char s[4]; sprintf(s,"[%d]",t+1);
                                    strcat(msg, s);
                                }
                            }
                            drawInfoPanel(cp, dice, msg);
                            waitingRoll = false;
                            continue;
                        }
                        waitingRoll = true;
                    }
                }
            }
        } else {
            if (kbhit()) {
                char c=getch();
                int chosen = c - '1';
                bool canMove[4];
                getMovable(cp, dice, canMove);
                if (chosen>=0 && chosen<NUM_TOKENS && canMove[chosen]) {
                    moveToken(cp, chosen, dice);
                    drawBoard(); drawYardTokens(); drawBoardTokens();
                    drawDice(dice);

                    if (finCount[cp]==NUM_TOKENS) {
                        winner=cp; gameOver=true; break;
                    }
                    if (dice==6) {
                        drawInfoPanel(cp, dice, "6 - Дахин хаяна! Enter дарна уу");
                    } else {
                        cp=(cp+1)%numPlayers;
                        drawDice(0);
                        drawInfoPanel(cp, 0, "Enter дарж шоо хаяна уу");
                    }
                    waitingRoll = true;
                }
            }
        }
        delay(30);
    }

    showWinner(winner);
    closegraph();
    return 0;
}
