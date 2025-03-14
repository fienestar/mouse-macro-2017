//----------------------------------------------------
//----------------------------------------------------
//
//                        제목 : 메크로
//                        타입 : 콘솔
//                     개발자 : 별
//               개발시작일 : 2017.?.?
//           마지막 수정일 : 2017.4.6
//
//----------------------------------------------------
//----------------------------------------------------



#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define SAVEMAX 1<<16

enum CLICK {
  NOTHING,
  YES,
  NO
};

enum PRINT
{
  MAIN,
  PLAY,
  SAVE,
  PLAYING,
  SAVING,
  INFO
};

static HANDLE OUTHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
static POINT Mouse;
static POINT Save[SAVEMAX];
static CLICK SClick[SAVEMAX];
static CLICK Click_Now = NOTHING;
static long long SavePoint, PlayPoint;
static CONSOLE_CURSOR_INFO Cur_INFO;
static long long PlayCount = 1, NowPlaying = 1, Saved = 0;

void SaveM(bool);
void PlayM();
void PrintT(PRINT);
void Print(PRINT);
void SetMouse(POINT);
int main();
void Cursor(DWORD, BOOL);
void SCR_Set(int, int);

void SCR_Set(int maxX, int maxY)
{
  char Screen[40] = { 0 };
  sprintf(Screen, "mode con: cols=%d lines=%d", maxX, maxY);
  system(Screen);
}

void SaveM(bool sw)
{
  if (sw) {
    SavePoint = 0;
    Saved = 0;
  }
  while (SavePoint < SAVEMAX&&GetKeyState(VK_ESCAPE) >= 0 && GetKeyState(VK_F3) >= 0)
  {
    GetCursorPos(&Save[SavePoint]);
    Print(SAVING);
    if (GetKeyState(VK_LBUTTON) < 0 && Click_Now == NOTHING)
    {
      SClick[SavePoint] = YES;
      Click_Now = YES;
    }
    else if (GetKeyState(VK_LBUTTON) >= 0 && Click_Now == YES)
    {
      SClick[SavePoint] = NO;
      Click_Now = NOTHING;
    }
    Sleep(10); //  100fps
    Saved = SavePoint;
    ++SavePoint;
  }
  return;
}

void PlayM()
{
  PlayPoint = 0;

  while (GetKeyState(VK_F3) >= 0 && GetKeyState(VK_ESCAPE) >= 0)
  {
    if (SClick[PlayPoint] == YES)
    {
      mouse_event(MOUSEEVENTF_LEFTDOWN, Save[PlayPoint].x, Save[PlayPoint].y, 0, 0);
      Click_Now = YES;
    }
    else if (SClick[PlayPoint] == NO)
    {
      mouse_event(MOUSEEVENTF_LEFTUP, Save[PlayPoint].x, Save[PlayPoint].y, 0, 0);
      Click_Now = NOTHING;
    }
    SetMouse(Save[PlayPoint]);
    Print(PLAYING);

    Sleep(10);
    ++PlayPoint;
    if (PlayPoint>Saved)
    {
      PlayPoint = 0;
      ++NowPlaying;
      Print(PLAYING);
    }
    do {
      if (GetKeyState(VK_F3) < 0 || GetKeyState(VK_ESCAPE) < 0)
        return;
      else if (GetKeyState(VK_F2) < 0 && NowPlaying - PlayCount) {
        while (GetKeyState(VK_F2) < 0 && NowPlaying - PlayCount) {
          ++NowPlaying;
          Print(PLAYING);
          Sleep(100);
        }
        Sleep(500);
        Print(PLAYING);
      }
      else if (GetKeyState(VK_F1) < 0) {
        while (GetKeyState(VK_F1) < 0) {
          --NowPlaying;
          Print(PLAYING);
          Sleep(100);
        }
        Sleep(500);
      }
    } while (NowPlaying == PlayCount);
  }
  Print(PLAYING);
  return;
}

void Print(PRINT M)
{
  switch (M)
  {
    case PLAY:
      SetConsoleCursorPosition(OUTHANDLE, { 11,4 });
      printf("%-3d", PlayCount);
      break;
    case PLAYING:
      SetConsoleCursorPosition(OUTHANDLE, { 7,2 });
      printf("%5.2lf초/%4.1lf초", (double)PlayPoint / 100, (double)Saved / 100);
      SetConsoleCursorPosition(OUTHANDLE, { 5,3 });
      printf("(%04d,%03d) / ", Save[PlayPoint - 1].x, Save[PlayPoint - 1].y);
      if (Click_Now == YES) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("%-10s", "클릭하는중");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
      }
      else
        printf("%-10s", "클릭안함");
      SetConsoleCursorPosition(OUTHANDLE, { 10,4 });
      printf("%3d회 남음", PlayCount - NowPlaying);
      break;
    case SAVING:
      SetConsoleCursorPosition(OUTHANDLE, { 7,3 });
      printf("%5.2lf초/%u초", (double)Saved / 100, SAVEMAX);//2
      SetConsoleCursorPosition(OUTHANDLE, { 5,4 });
      printf("(%04d,%03d) / ", Save[SavePoint - 1].x, Save[SavePoint - 1].y);
      if (Click_Now == YES) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        printf("클릭하는중");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
      }
      else
        printf("클릭안함  ");
      break;

  }
}

void PrintT(PRINT M)
{
  SetConsoleCursorPosition(OUTHANDLE, { 0,0 });
  volatile static const char t[] = "┃　   제작      : 별         ┃";
  switch (M)
  {
    case PLAYING:
      puts("  　　　　　　　　　　　　　　  ");//1
      puts("┏━━━━━━━━━━━━━━┓");//0
      puts("┃　　　　　　　　　　　　　　┃");//1
      puts("┃　　　　　　　　　　　　　　┃");//2
      puts("┃　　　　　　　　　　　　　　┃");//3
      puts("┃　　　　　　　　　　　　　　┃");//4
      puts("┃　　　F1 : 반복추가   　　　┃");//5
      puts("┃　　　F2 : 반복감소   　　　┃");//6
      puts("┃　　　F3 : 돌아가기　 　　　┃");//7
      puts("┗━━━━━━━━━━━━━━┛");//8
      puts("  　　　　　　　　　　　　　　  ");//10
      break;
    case MAIN:
      puts("  　　　　　　　　　　　　　　  ");//0
      puts("┏━━━━━━━━━━━━━━┓");//1
      puts("┃　　　　　　　　　　　　　　┃");//2
      puts("┃　　　　　+메크로+　　　　　┃");//3
      puts("┃　　　　　　　　　　　　　　┃");//4
      puts("┃　　　F1 : 매크로 녹화　　　┃");//5
      puts("┃　　　F2 : 매크로 시작　　　┃");//6
      puts("┃      Esc : 종료하기　　　　┃");//7
      puts("┃　　　F3 : 정보       　　　┃");//8
      puts("┃　　　　　　　　　　　　　　┃");//8
      puts("┗━━━━━━━━━━━━━━┛");//9
      break;
    case PLAY:
      puts("  　　　　　　　　　　　　　　  ");//0
      puts("┏━━━━━━━━━━━━━━┓");//1
      puts("┃　　　　　　　　　　　　　　┃");//2
      puts("┃　　　　　반복횟수　　　　　┃");//3
      puts("┃　　　　　  　　　　　　　　┃");//4
      puts("┃　　　: 　   　 　 　 　 　 ┃");//5
      puts("┃　　　F1 : 반복추가   　　　┃");//6
      puts("┃　　　F3 : 반복감소   　　　┃");//7
      puts("┃　　　F4 : 시작하기   　　　┃");//8
      puts("┗━━━━━━━━━━━━━━┛");//9
      puts("  　　　　　　　　　　　　　　  ");//10
      break;
    case SAVE:
      puts("  　　　　　　　　　　　　　　  ");//0
      puts("┏━━━━━━━━━━━━━━┓");//1
      puts("┃　　　　　　　　　　　　　　┃");//2
      puts("┃　　　　　어디부터　　　　　┃");//3
      puts("┃　　　　　　　　　　　　　　┃");//4
      puts("┃　　　 F2 : 처음부터　　　　┃");//5
      puts("┃　　　 F4 : 이어하기　　　　┃");//6
      puts("┃                    　　　　┃");//7
      puts("┃　　　　　　　　　　　　　　┃");//8
      puts("┗━━━━━━━━━━━━━━┛");//9
      puts("  　　　　　　　　　　　　　　  ");//10
      break;
    case SAVING:
      puts("  　　　　　　　　　　　　　　  ");//0
      puts("┏━━━━━━━━━━━━━━┓");//1
      puts("┃　　　　　　　　　　　　　　┃");//2
      puts("┃　　      녹화중...       　┃");//3
      puts("┃　　                      　┃");//4
      puts("┃　　                      　┃");//5
      puts("┃　　F3 : 돌아가기(+저장)　　┃");//6
      puts("┃　　　                　　　┃");//7
      puts("┃　　　             　 　　　┃");//8
      puts("┗━━━━━━━━━━━━━━┛");//9
      puts("  　　　　　　　　　　　　　　  ");//10
      break;
    case INFO:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
      puts("  　　　　　　　　　　　　　　  ");//0
      puts("┏━━━━━━━━━━━━━━┓");//1
      puts("┃　　                    　　┃");//2
      puts("┃     이름      : 메크로     ┃");//3
      puts((const char *)t);
       puts("┃　　 제작일    : 4/7      　┃");//5
      puts("┃　   소요시간  : 2~7일    　┃");//6
      puts("┃　　 제작 목적 : 연습　　   ┃");//7
      puts("┃달성한 목표-게임에 이용가능 ┃");//8
      puts("┃　　　　　　　　　　　　　  ┃");//9
      puts("┗━━━━━━━━━━━━━━┛");//10
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
      break;
  }
}

void SetMouse(POINT T) { SetCursorPos(T.x, T.y); }

void Cursor(DWORD dwSize, BOOL bVisible)
{
  Cur_INFO = { dwSize, bVisible };
  SetConsoleCursorInfo(OUTHANDLE, &Cur_INFO);
}

int main()
{
  Cursor(1, FALSE);
  SCR_Set(33, 11);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
  PrintT(MAIN);
  while (GetKeyState(VK_ESCAPE) >= 0)
  {
    if (GetKeyState(VK_F1) < 0)
    {
      PrintT(SAVE);
      while (GetKeyState(VK_F2) >= 0 && GetKeyState(VK_F4) >= 0 && GetKeyState(VK_ESCAPE) >= 0);
      PrintT(SAVING);
      SaveM(GetKeyState(VK_F2) < 0);
      PrintT(MAIN);
    }
    else if (GetKeyState(VK_F2) < 0)
    {
      PrintT(PLAY);
      Print(PLAY);
      NowPlaying = 0;
      while (GetKeyState(VK_F4) >= 0 && GetKeyState(VK_ESCAPE) >= 0)
      {
        if (GetKeyState(VK_F1) < 0) {
          ++PlayCount;
          Sleep(100);
          Print(PLAY);
        }
        else if (GetKeyState(VK_F3) < 0 && PlayCount > 1) {
          --PlayCount;
          Sleep(100);
          Print(PLAY);
        }
      }
      PrintT(PLAYING);
      PlayM();
      PrintT(MAIN);
    }
    else if (GetKeyState(VK_F3) < 0)
    {
      PrintT(INFO);
      getch();
      PrintT(MAIN);
    }
  }
  system("cls");
  return 0;
}
