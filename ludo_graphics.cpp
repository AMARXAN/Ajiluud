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
#include <cmath>

const int WIN_W=700,WIN_H=750,BOARD_X=50,BOARD_Y=50,BOARD_SIZE=600,CELL=40;
const int PLR_COLOR[4]={RED,GREEN,YELLOW,BLUE};
const char PLR_NAME[4][8]={"Red","Green","Yellow","Blue"};
const int TRACK_COL[52]={1,2,3,4,5,6,6,6,6,6,6,7,8,8,8,8,8,8,9,10,11,12,13,14,14,14,14,14,14,13,12,11,10,9,8,8,8,8,8,8,7,6,6,6,6,6,6,5,4,3,2,1};
const int TRACK_ROW[52]={6,6,6,6,6,6,5,4,3,2,1,0,0,1,2,3,4,5,6,6,6,6,6,6,7,8,9,10,11,12,13,14,14,13,12,11,10,9,8,7,8,8,9,10,11,12,13,14,14,13,12,11};
const int SAFE[8]={0,8,13,21,26,34,39,47};
const int START_POS[4]={0,13,26,39};
const int NUM_TOKENS=4,FINISH_POS=57;

int tokenPos[4][4];
bool tokenDone[4][4];
int finCount[4],numPlayers;

int rollDice(){return rand()%6+1;}
bool isSafe(int a){for(int i=0;i<8;i++)if(SAFE[i]==a)return true;return false;}

void stepToCell(int p,int s,int&col,int&row){
  col=row=-1;
  if(s<0||s>=FINISH_POS)return;
  if(s<52){int a=(s+START_POS[p])%52;col=TRACK_COL[a];row=TRACK_ROW[a];return;}
  int h=s-52;
  switch(p){case 0:col=7;row=5-h;break;case 1:col=7;row=9+h;break;case 2:col=8+h;row=7;break;case 3:col=6-h;row=7;break;}
}
void cellToPixel(int col,int row,int&px,int&py){px=BOARD_X+col*CELL+CELL/2;py=BOARD_Y+row*CELL+CELL/2;}

void fillRect(int col,int row,int cols,int rows,int color){
  setfillstyle(SOLID_FILL,color);
  bar(BOARD_X+col*CELL,BOARD_Y+row*CELL,BOARD_X+(col+cols)*CELL-1,BOARD_Y+(row+rows)*CELL-1);
}

void drawStar(int px,int py,int r){
  setcolor(YELLOW);setfillstyle(SOLID_FILL,YELLOW);
  int pts[18];
  for(int i=0;i<8;i++){double a=i*3.14159/4;int ri=(i%2==0)?r:r/2;pts[i*2]=px+(int)(ri*sin(a));pts[i*2+1]=py-(int)(ri*cos(a));}
  pts[16]=pts[0];pts[17]=pts[1];fillpoly(9,pts);
}

void drawBoard(){
  setfillstyle(SOLID_FILL,WHITE);bar(BOARD_X,BOARD_Y,BOARD_X+BOARD_SIZE,BOARD_Y+BOARD_SIZE);
  fillRect(0,0,6,6,RED);setfillstyle(SOLID_FILL,WHITE);bar(BOARD_X+CELL,BOARD_Y+CELL,BOARD_X+5*CELL,BOARD_Y+5*CELL);
  fillRect(9,0,6,6,GREEN);setfillstyle(SOLID_FILL,WHITE);bar(BOARD_X+10*CELL,BOARD_Y+CELL,BOARD_X+14*CELL,BOARD_Y+5*CELL);
  fillRect(0,9,6,6,YELLOW);setfillstyle(SOLID_FILL,WHITE);bar(BOARD_X+CELL,BOARD_Y+10*CELL,BOARD_X+5*CELL,BOARD_Y+14*CELL);
  fillRect(9,9,6,6,BLUE);setfillstyle(SOLID_FILL,WHITE);bar(BOARD_X+10*CELL,BOARD_Y+10*CELL,BOARD_X+14*CELL,BOARD_Y+14*CELL);
  fillRect(6,0,3,6,WHITE);fillRect(7,1,1,5,GREEN);
  fillRect(6,9,3,6,WHITE);fillRect(7,9,1,5,YELLOW);
  fillRect(0,6,6,3,WHITE);fillRect(1,7,5,1,RED);
  fillRect(9,6,6,3,WHITE);fillRect(9,7,5,1,BLUE);
  int cx=BOARD_X+6*CELL,cy=BOARD_Y+6*CELL,cs=3*CELL;
  int t1[8]={cx,cy,cx+cs/2,cy+cs/2,cx,cy+cs,cx,cy};
  setfillstyle(SOLID_FILL,RED);fillpoly(4,t1);
  int t2[8]={cx+cs,cy,cx+cs/2,cy+cs/2,cx,cy,cx+cs,cy};
  setfillstyle(SOLID_FILL,GREEN);fillpoly(4,t2);
  int t3[8]={cx+cs,cy+cs,cx+cs/2,cy+cs/2,cx+cs,cy,cx+cs,cy+cs};
  setfillstyle(SOLID_FILL,BLUE);fillpoly(4,t3);
  int t4[8]={cx,cy+cs,cx+cs/2,cy+cs/2,cx+cs,cy+cs,cx,cy+cs};
  setfillstyle(SOLID_FILL,YELLOW);fillpoly(4,t4);
  for(int i=0;i<8;i++)drawStar(BOARD_X+TRACK_COL[SAFE[i]]*CELL+CELL/2,BOARD_Y+TRACK_ROW[SAFE[i]]*CELL+CELL/2,10);
  setcolor(DARKGRAY);
  for(int c=6;c<=9;c++){line(BOARD_X+c*CELL,BOARD_Y,BOARD_X+c*CELL,BOARD_Y+6*CELL);line(BOARD_X+c*CELL,BOARD_Y+9*CELL,BOARD_X+c*CELL,BOARD_Y+15*CELL);}
  for(int r=0;r<=6;r++)line(BOARD_X+6*CELL,BOARD_Y+r*CELL,BOARD_X+9*CELL,BOARD_Y+r*CELL);
  for(int r=9;r<=15;r++)line(BOARD_X+6*CELL,BOARD_Y+r*CELL,BOARD_X+9*CELL,BOARD_Y+r*CELL);
  for(int r=6;r<=9;r++){line(BOARD_X,BOARD_Y+r*CELL,BOARD_X+6*CELL,BOARD_Y+r*CELL);line(BOARD_X+9*CELL,BOARD_Y+r*CELL,BOARD_X+15*CELL,BOARD_Y+r*CELL);}
  for(int c=0;c<=6;c++)line(BOARD_X+c*CELL,BOARD_Y+6*CELL,BOARD_X+c*CELL,BOARD_Y+9*CELL);
  for(int c=9;c<=15;c++)line(BOARD_X+c*CELL,BOARD_Y+6*CELL,BOARD_X+c*CELL,BOARD_Y+9*CELL);
  setcolor(BROWN);setlinestyle(SOLID_LINE,0,THICK_WIDTH);
  rectangle(BOARD_X,BOARD_Y,BOARD_X+BOARD_SIZE,BOARD_Y+BOARD_SIZE);
  setlinestyle(SOLID_LINE,0,NORM_WIDTH);setcolor(WHITE);
}

void drawYardTokens(){
  int off[4][2]={{1,1},{10,1},{1,10},{10,10}};
  for(int p=0;p<numPlayers;p++){
    int cnt=0;
    for(int t=0;t<NUM_TOKENS;t++){
      if(tokenPos[p][t]==-1&&!tokenDone[p][t]){
        int dx=cnt%2,dy=cnt/2;
        int px=BOARD_X+(off[p][0]+dx*2)*CELL+CELL/2+(dx?10:-10);
        int py=BOARD_Y+(off[p][1]+dy*2)*CELL+CELL/2+(dy?10:-10);
        setfillstyle(SOLID_FILL,PLR_COLOR[p]);setcolor(WHITE);
        fillellipse(px,py,CELL/2-4,CELL/2-4);
        char b[3];sprintf(b,"%d",t+1);setcolor(BLACK);outtextxy(px-4,py-7,b);
        cnt++;
      }
    }
  }
}

void drawBoardTokens(){
  for(int p=0;p<numPlayers;p++){
    for(int t=0;t<NUM_TOKENS;t++){
      int s=tokenPos[p][t];
      if(s<0||tokenDone[p][t])continue;
      int col,row;stepToCell(p,s,col,row);
      if(col<0||col>14||row<0||row>14)continue;
      int px,py;cellToPixel(col,row,px,py);
      int ox=(t%2==0?-8:8),oy=(t<2?-8:8);
      setfillstyle(SOLID_FILL,PLR_COLOR[p]);setcolor(BLACK);
      fillellipse(px+ox,py+oy,CELL/2-6,CELL/2-6);
      char b[3];sprintf(b,"%d",t+1);setcolor(BLACK);outtextxy(px+ox-4,py+oy-7,b);
    }
  }
}

void drawInfoPanel(int cp,int dice,const char*msg){
  setfillstyle(SOLID_FILL,BLACK);bar(0,BOARD_Y+BOARD_SIZE+5,WIN_W,WIN_H);
  int y=BOARD_Y+BOARD_SIZE+15;
  for(int p=0;p<numPlayers;p++){
    setcolor(PLR_COLOR[p]);
    char buf[16];sprintf(buf,"%s:",PLR_NAME[p]);outtextxy(10+p*170,y,buf);
    for(int t=0;t<NUM_TOKENS;t++){
      if(tokenDone[p][t]){setcolor(YELLOW);outtextxy(10+p*170+t*30,y+18,(char*)"H");}
      else if(tokenPos[p][t]<0){setcolor(DARKGRAY);outtextxy(10+p*170+t*30,y+18,(char*)"Y");}
      else{setcolor(PLR_COLOR[p]);char s[5];sprintf(s,"%d",tokenPos[p][t]);outtextxy(10+p*170+t*35,y+18,s);}
    }
  }
  setcolor(WHITE);
  char info[120];
  if(dice>0)sprintf(info,">>> %s  Шоо:%d  %s",PLR_NAME[cp],dice,msg);
  else sprintf(info,">>> %s  %s",PLR_NAME[cp],msg);
  outtextxy(10,y+45,info);
}

void drawDice(int val){
  int x=WIN_W-110,y=BOARD_Y+BOARD_SIZE+10;
  setfillstyle(SOLID_FILL,WHITE);setcolor(DARKGRAY);
  bar(x,y,x+80,y+70);rectangle(x,y,x+80,y+70);
  if(val<=0)return;
  setfillstyle(SOLID_FILL,BLACK);
  int dots[6][6][2]={{{40,35}},{{20,20},{60,50}},{{20,20},{40,35},{60,50}},{{20,20},{60,20},{20,50},{60,50}},{{20,20},{60,20},{40,35},{20,50},{60,50}},{{20,20},{60,20},{20,35},{60,35},{20,50},{60,50}}};
  for(int i=0;i<val;i++)fillellipse(x+dots[val-1][i][0],y+dots[val-1][i][1],6,6);
}

int showStartScreen(){
  cleardevice();setbkcolor(BLACK);
  settextstyle(BOLD_FONT,HORIZ_DIR,4);setcolor(YELLOW);outtextxy(200,80,(char*)"LUDO");
  settextstyle(DEFAULT_FONT,HORIZ_DIR,2);setcolor(WHITE);outtextxy(120,180,(char*)"Хэдэн тоглогч? (2,3,4)");
  setcolor(RED);outtextxy(150,240,(char*)"[2] 2 тоглогч");
  setcolor(GREEN);outtextxy(150,280,(char*)"[3] 3 тоглогч");
  setcolor(YELLOW);outtextxy(150,320,(char*)"[4] 4 тоглогч");
  while(true){if(kbhit()){char c=getch();if(c=='2')return 2;if(c=='3')return 3;if(c=='4')return 4;}}
}

void checkCapture(int atk,int atkTok){
  int s=tokenPos[atk][atkTok];
  if(s<0||s>=52)return;
  int ap=(s+START_POS[atk])%52;
  if(isSafe(ap))return;
  for(int v=0;v<numPlayers;v++){
    if(v==atk)continue;
    for(int t=0;t<NUM_TOKENS;t++){
      if(tokenPos[v][t]<0||tokenPos[v][t]>=52||tokenDone[v][t])continue;
      if((tokenPos[v][t]+START_POS[v])%52==ap)tokenPos[v][t]=-1;
    }
  }
}

bool moveToken(int p,int tok,int dice){
  int pos=tokenPos[p][tok];
  if(pos==-1){tokenPos[p][tok]=0;checkCapture(p,tok);return true;}
  int np=pos+dice;
  if(np>FINISH_POS)return false;
  tokenPos[p][tok]=np;
  if(np==FINISH_POS){tokenDone[p][tok]=true;finCount[p]++;}
  else if(np<52)checkCapture(p,tok);
  return true;
}

bool getMovable(int p,int dice,bool cm[]){
  bool any=false;
  for(int t=0;t<NUM_TOKENS;t++){cm[t]=false;if(tokenDone[p][t])continue;int pos=tokenPos[p][t];if(pos==-1){if(dice==6){cm[t]=true;any=true;}continue;}if(pos+dice<=FINISH_POS){cm[t]=true;any=true;}}
  return any;
}

void showWinner(int w){
  cleardevice();settextstyle(BOLD_FONT,HORIZ_DIR,4);setcolor(PLR_COLOR[w]);
  char msg[40];sprintf(msg,"%s WINS!",PLR_NAME[w]);outtextxy(150,200,msg);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,2);setcolor(WHITE);outtextxy(180,320,(char*)"Дурын товч...");getch();
}

int main(){
  srand((unsigned)time(0));
  initwindow(WIN_W,WIN_H,"LUDO - graphics.h");
  numPlayers=showStartScreen();
  for(int p=0;p<4;p++){finCount[p]=0;for(int t=0;t<NUM_TOKENS;t++){tokenPos[p][t]=-1;tokenDone[p][t]=false;}}
  int cp=0,dice=0,winner=-1;
  bool gameOver=false,waitingRoll=true;
  drawBoard();drawYardTokens();drawBoardTokens();
  drawInfoPanel(cp,0,(char*)"Enter дарж шоо хаяна уу");
  while(!gameOver){
    if(waitingRoll){
      if(kbhit()){
        char c=getch();
        if(c==13||c==' '){
          dice=rollDice();
          bool cm[4];bool any=getMovable(cp,dice,cm);
          if(!any){
            drawBoard();drawYardTokens();drawBoardTokens();drawDice(dice);
            drawInfoPanel(cp,dice,(char*)"Зөөх боломжгүй - дараагийн тоглогч");
            delay(1500);cp=(cp+1)%numPlayers;
            drawBoard();drawYardTokens();drawBoardTokens();drawDice(0);
            drawInfoPanel(cp,0,(char*)"Enter дарж шоо хаяна уу");
          } else {
            int movable=0,last=-1;
            for(int t=0;t<NUM_TOKENS;t++)if(cm[t]){movable++;last=t;}
            drawBoard();drawYardTokens();drawBoardTokens();drawDice(dice);
            if(movable==1){
              char msg[60];sprintf(msg,"Token%d автоматаар зөөнө",last+1);
              drawInfoPanel(cp,dice,msg);delay(800);
              moveToken(cp,last,dice);
              drawBoard();drawYardTokens();drawBoardTokens();
              if(finCount[cp]==NUM_TOKENS){winner=cp;gameOver=true;break;}
              if(dice==6){drawInfoPanel(cp,dice,(char*)"6 - Дахин! Enter");}
              else{cp=(cp+1)%numPlayers;drawDice(0);drawInfoPanel(cp,0,(char*)"Enter дарж шоо хаяна уу");}
            } else {
              char msg[80]="Токен: ";
              for(int t=0;t<NUM_TOKENS;t++)if(cm[t]){char s[4];sprintf(s,"[%d]",t+1);strcat(msg,s);}
              drawInfoPanel(cp,dice,msg);waitingRoll=false;continue;
            }
          }
        }
      }
    } else {
      if(kbhit()){
        char c=getch();int chosen=c-'1';
        bool cm[4];getMovable(cp,dice,cm);
        if(chosen>=0&&chosen<NUM_TOKENS&&cm[chosen]){
          moveToken(cp,chosen,dice);
          drawBoard();drawYardTokens();drawBoardTokens();drawDice(dice);
          if(finCount[cp]==NUM_TOKENS){winner=cp;gameOver=true;break;}
          if(dice==6){drawInfoPanel(cp,dice,(char*)"6 - Дахин! Enter");}
          else{cp=(cp+1)%numPlayers;drawDice(0);drawInfoPanel(cp,0,(char*)"Enter дарж шоо хаяна уу");}
          waitingRoll=true;
        }
      }
    }
    delay(30);
  }
  showWinner(winner);closegraph();return 0;
}
