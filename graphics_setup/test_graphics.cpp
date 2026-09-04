#include <graphics.h>
#include <conio.h>
#include <cstdio>

int main(){
  initwindow(640,480,"graphics.h - Тест");
  setbkcolor(BLACK);cleardevice();
  settextstyle(BOLD_FONT,HORIZ_DIR,3);
  setcolor(YELLOW);outtextxy(100,30,(char*)"graphics.h амжилттай!");
  setcolor(RED);   fillellipse(100,200,60,60);
  setcolor(GREEN); fillellipse(250,200,60,60);
  setcolor(YELLOW);fillellipse(400,200,60,60);
  setcolor(BLUE);  fillellipse(550,200,60,60);
  setcolor(WHITE);rectangle(50,300,590,420);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
  setcolor(WHITE);outtextxy(120,350,(char*)"Дурын товч дарна уу...");
  getch();closegraph();return 0;
}
