#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include<ncurses.h>
#include<string>
using namespace std;
int Philosoph_number;
mutex widelce[5];
mutex printf1, printf2;
int left[5];
int right[5];
using Clock = chrono::system_clock;
using namespace chrono_literals;
bool enable[5];
enum States
{
    THINK = 1,
    HUNGER = 2,
    EAT = 3,
    HUNGER_TAKES_LEFT = 4,
    HUNGER_TAKES_RIGHT = 5
    
};
class Philosph
{
    public:
    int id;
    int state;
    int number;
    int starttime = 0;
    float time;

    
};

Philosph philosops[5];
void Work(Philosph philosopher[], int c, bool enable[], int left, int right)//int random
{
    int random = 300 + rand() % 500;
    while(1){
 
    //srand(time(NULL));
    //int random = 300 + rand() % 500;
    //int random = 1;
    if(philosopher[c].id != philosopher[c].number-1)
    {
       
        widelce[left].lock();
        philosopher[c].state = HUNGER_TAKES_LEFT;
        widelce[right].lock();
    } 
    else
    {
        widelce[right].lock();
        philosopher[c].state = HUNGER_TAKES_RIGHT;
        widelce[left].lock();
    }

        philosopher[c].state = EAT;
        while(philosopher[c].starttime <= random)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
            philosopher[c].starttime +=100;
            philosopher[c].time = philosopher[c].starttime;
        }
        philosopher[c].starttime = 0;
        widelce[right].unlock();
        widelce[left].unlock();
        philosopher[c].state = THINK;
        while(philosopher[c].starttime <= random)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
            philosopher[c].starttime+=100;
            philosopher[c].time = philosopher[c].starttime;
        }
        philosopher[c].starttime = 0;
        philosopher[c].state = HUNGER;
    }
}

void Printing(int number,Philosph philosops[], bool enable[])//Philosph philosophs[])
{

    int i = 0;
    int rows,columns = 0;
    getmaxyx(stdscr,rows, columns);
    initscr();
    mvprintw(2,2, "Numer filozofa");
    mvprintw(2,20, "Status");
    mvprintw(2,40, "Zużyte widelce");
    mvprintw(2,60, "Postęp stanu");
    for(int i = 0; i < number; i++)
    {
    mvprintw(3 + i,2,  "%d", philosops[i].id);
    }
    while(1)
    {
    int left = i;
    int right = (i+1)%number;
        if(philosops[i].state == 1)
        {
        mvprintw(3 + i,20,  "THINK                  ");
        mvprintw(3 + i,40,  " ");
        mvprintw(3 + i,40 + 5,  " ");
        }
        else if(philosops[i].state == 3)
        {
        mvprintw(3 + i,20,  "EAT                    ");
        mvprintw(3 + i, 40, "%d ", left);
        mvprintw(3 + i, 40 + 2, "%d ",  right);

        }
        else if(philosops[i].state == 2)
        {
        mvprintw(3 + i,20,  "HUNGER                  ");
        mvprintw(3 + i,40,  " ");
        mvprintw(3 + i,40 + 2,  " ");
        }
        else if(philosops[i].state == 4)
        {
        mvprintw(3 + i,20,  "HUNGER-TAKES LEFT      ");
        mvprintw(3 + i, 40, "%d ", left);
        mvprintw(3 + i, 40 + 2, " ", left);

        }
        else if(philosops[i].state == 5)
        {
        mvprintw(3 + i,20,  "HUNGER-TAKES RIGHT     ");
        mvprintw(3+i, 40, "%d ", right);
        mvprintw(3+i, 40+2, " ");

        }
        if(philosops[i].state == 2 || philosops[i].state == 4 || philosops[i].state == 5)
        mvprintw(3 + i, 60, "WAIT   ");
        else
        mvprintw(3 + i, 60, "%d     ", (int) philosops[i].time);
    i++;
    if(i == number)i = 0;
    refresh();
    }
    endwin();
}
int main(int argc, char *argv[])
{
    if (argc == 1) 
        Philosoph_number = 5;
    else if (argc > 1 && atoi(argv[1]) >= 5 && atoi(argv[1]) <= 10)
    {
        Philosoph_number = atoi(argv[1]);
    }
    else
    {
        printf("Podano złe argumenty");
        exit(0);
    }
    using namespace chrono_literals;

   mutex *widelce = new mutex[Philosoph_number];
   thread Philosoph_thread[Philosoph_number];
   Philosph *philosops = new Philosph[Philosoph_number];
   //bool *enable = new bool[Philosoph_number];
   int *left = new int[Philosoph_number];
   int *right = new int[Philosoph_number];
   int random[Philosoph_number];
   for(int i = 0; i < Philosoph_number; i++)
   {
    enable[i] = true;
   }
   //thread Print(Printing);
   for(int i = 0; i <Philosoph_number; i++)
   {
    philosops[i].id= i;
    philosops[i].state = THINK;
    philosops[i].number = Philosoph_number;
    //random[i]= rand
    left[i] = i;
    right[i] = (i+1)%Philosoph_number;
    Philosoph_thread[i] = thread(Work,philosops, i, enable, left[i], right[i]);//random[i]);
   }
    //initscr();
    thread Screen(Printing,Philosoph_number, philosops, enable);
    Screen.join();
   for(int i = 0; i <Philosoph_number; i++)
   {
    Philosoph_thread[i].join();
   }
   delete[] widelce;
   delete[] philosops;
   delete[] left;
   delete[] right;
   //delete[] enable;
}