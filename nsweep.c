#include <ncurses.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** boarda, ** boardb;
int rows, cols, mines;

void makeBoard(void);
void fillBoard(void);
void mineBoard(void);
void wipeBoard(void);
void printBoard(void);
void revealBoard(void);
int checkPoint(int, int);
void helpScreen(char *);

int main(int argc, char ** argv)
{
    char *input, *arrows;
    int flags, cov;
    int xc, yc;
    rows=0;
    cols=0;
    mines=0;
    input=malloc(3);
    arrows=malloc(5);
    strcpy(arrows, "hjkl");
    if (argc>1)
    {
        if (strcmp(*(argv+1),"-a")==0)
            strcpy(arrows, "aswd");
    }
    srand(time(NULL));
    while (rows<9||cols<9)
    {
        printf("Type in a the size of the grid by rows and columns (limit 26x26): ");
        scanf ("%d%d", &cols, &rows);
        if (rows<9||cols<9)
            printf("Warning, size must be greater than 9\n\n");
    }
    while (mines<((rows<cols)?rows:cols)||mines> (rows-1) * (cols-1))
    {
        printf("\nType in the number of mines you want to have: ");
        scanf("%d", &mines);
        if (mines<((rows<cols)?rows:cols)||mines> (rows-1) * (cols-1))
            printf("Warning: number is out of scope\n\n");
    }
    initscr();
    curs_set(TRUE);
    noecho();
    clear();
    do
    {
        cov =0;
        flags=mines;
        makeBoard();
        fillBoard();
        mineBoard();
        xc = 1;
        yc = 0;
        while (1)
        {
            printBoard();
            move(rows+3, 0);
            printw("Mines: %d", flags);
            move(0,cols);
            printw(":)");
            move(yc+2,2*(xc+1));
            refresh();
            *input=getch();
            if (*input=='i')
            {
                helpScreen(arrows);
                continue;
            }
            if (*input=='q')
            {
                move(rows+3, 0);
                printw("Do you really want to quit? [y/n]: ");
                while(1)
                {
                    *input=getch();
                    if (*input=='n'||*input=='y') break;
                }    
                if (*input=='y')
                {
                    printw("Quitting...\n\n");
                    break;
                }
            }
            if (*input==(*(arrows+1)))
                yc+=(yc<(rows-1));
            if (*input==(*(arrows+3)))
                xc+=(xc<(cols-1));
            if (*input==(*(arrows+2)))
                yc-=(yc>0);
            if (*input==*arrows)
                xc-=(xc>0);
            if (yc>=rows||xc>=cols)
            {
                printw("Invalid Move");
                continue;
            }
            if (*input=='u')
            {
                if ((*(*(boarda+yc)+xc))=='x')
                {
                    revealBoard();
                    move(0,cols);
                    printw("X(");
                    break;
                }
                else if((*(*(boardb+yc)+xc))!='_')
                {
                    printw("That location is already selected\n\n");
                    continue;
                }
                else
                    checkPoint(xc, yc);
            }
            else if (*input=='f')
            {
                if (*(*(boardb+yc)+xc)=='_')
                {
                    *(*(boardb+yc)+xc)='F';
                    flags--;
                    if (*(*(boarda+yc)+xc)=='x')
                        cov++;
                }
                else if( *(*(boardb+yc)+xc)=='F')
                {
                    *(*(boardb+yc)+xc)='_';
                    flags++;
                    if (*(*(boarda+yc)+xc)=='x')
                       cov--;
                }
                else
                    printw("That place is not valid\n");
                if (cov==mines)
                {
                    printBoard();
                    move(0,cols);
                    printw("B) Good Job!");
                    getch();
                    break;
                }
            }
            refresh();
        }
        wipeBoard();
        move (rows+3,0);
        printw("\n\nQuit? [y/n] ");
        while (*input!='y'&&*input!='n')
        {
            *input=getch();
        }
        
    } while (*input!='y');
    free((void *) input);
    free((void *) arrows);
    
    printw("\nGood bye\n");
    endwin();
    return 0;
}

void makeBoard(void)
{
    int x;
    boarda=malloc(sizeof(char *) * rows);
    boardb=malloc(sizeof(char *) * rows);
    for (x=0;x<rows;x++)
    {
        *(boarda+x)=malloc(cols);
        *(boardb+x)=malloc(cols);
    }
}

void fillBoard(void)
{
    int x, y;
    for (y=0;y<rows;y++)
        for (x=0;x<cols;x++)
        {
            *(*(boarda+y)+x)='.';
            *(*(boardb+y)+x)='_';
        }
}

void mineBoard(void)
{
    int x, y, z;
    z=mines; 
    while(z>0)
    {
        x=rand()%cols;
        y=rand()%rows;
        if (*(*(boarda+y)+x)=='.')
        {
            *(*(boarda+y)+x)='x';
            z--;
        }
    }
}

void wipeBoard(void)
{
    int x;
    for (x=0;x<rows;x++)
    {
        free((void *) (*(boarda+x)));
        free((void *) (*(boardb+x)));
    }
    free((void *) boarda);
    free((void *) boardb);
}

void printBoard(void)
{
    char a;
    int x, y;
    clear();
    move(0,0);
    printw("\n ");
    for (x=0, a='A';x<cols; x++, a++)
        printw(" %c", a);
    for (y=0, a='A';y<rows; y++, a++)
    {
        printw("\n%c", a);
        for (x=0; x<cols; x++)
            printw(" %c", *(*(boardb+y)+x));
    }
    refresh();
} 

void revealBoard(void)
{
    char a;
    int x, y;
    move (0,0);
    clear();
    printw("\n ");
    for (x=0, a='A';x<cols; x++, a++)
        printw(" %c", a);
    for (y=0, a='A';y<rows; y++, a++)
    {
        printw("\n%c", a);
        for (x=0; x<cols; x++)
            printw(" %c", (*(*(boarda+y)+x)=='.')?(*(*(boardb+y)+x)):(*(*(boarda+y)+x)));
    }
}

int checkPoint(int x, int y)
{
    int h=0;
    if (x>0)
    {
        h=(*(*(boarda+y)+x-1)=='x');
        if (y>0)
            h+=(*(*(boarda+y-1)+x-1)=='x');
    }
    if (y>0)
    {
        h+=(*(*(boarda+y-1)+x)=='x');
        if (x<(cols-1))
            h+=(*(*(boarda+y-1)+x+1)=='x');
    }
    if (x<(cols-1))
    {
        h+=(*(*(boarda+y)+x+1)=='x');
        if (y<(rows-1))
            h+=(*(*(boarda+y+1)+x+1)=='x');
    }
    if (y<(rows-1))
    {
        h+=(*(*(boarda+y+1)+x)=='x');
        if (x>0)
            h+=(*(*(boarda+y+1)+x-1)=='x');
    }
    *(*(boardb+y)+x)=(h>0)?('0'+h):' ';
    if (h==0)
    {
        if (x>0)
        {
            if (*(*(boardb+y)+x-1)=='_')
                checkPoint(x-1,y);
            if (y>0)
                if (*(*(boardb+y-1)+x-1)=='_')
                    checkPoint(x-1,y-1);
        }
        if (y>0)
        {
            if (*(*(boardb+y-1)+x)=='_')
                checkPoint(x, y-1);
            if (x<(cols-1))
                if (*(*(boardb+y-1)+x+1)=='_')
                    checkPoint(x+1,y-1);
        }
        if (x<(cols-1))
        {
            if (*(*(boardb+y)+x+1)=='_')
                checkPoint(x+1, y);
            if (y<(rows-1))
                if (*(*(boardb+y+1)+x+1)=='_')
                    checkPoint(x+1,y+1);
        }
        if (y<(rows-1))
        {
            if (*(*(boardb+y+1)+x)=='_')
                checkPoint(x, y+1);
            if (x>0)
                if (*(*(boardb+y+1)+x-1)=='_')
                    checkPoint(x-1,y+1);
        }
    }
}

void helpScreen(char * txt)
{
    clear();
    move (0,0);
    printw("Instructions:\n\n%c - left\n%c - down\n%c - up\n%c - right\nu - uncover\nf - flag/deflag\n\nPress any button to continue: ", *txt, *(txt+1), *(txt+2), *(txt+3));
    getch();
}
