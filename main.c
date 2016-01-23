/*----------------------------------------------------------*/
/*                                                          */
/*                    Final Project                         */
/*                                                          */
/*         Sabaei, Student No. 9431901, 1394/11/02          */
/*                                                          */
/*----------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLACK       0
#define BLUE        1
#define DARK_GREEN  2
#define PURPLE      5
#define GOLD        6
#define GRAY        7
#define DARK_GRAY   8
#define DARK_BLUE   9
#define LIGHT_GREEN 10
#define CYAN        11
#define LIGHT_RED   12
#define PINK        13
#define YELLOW      14
#define WHITE       15

struct block {
    int  block_energy;
    char type;
    int  cell;         // 0 No cell, 1 cell of player 1, cell of player 2 in this block
    int  cell_energy1; //
    int  cell_energy2;
};

struct block **table;
int width, height, score1, score2;
int typecolor[4] = {LIGHT_GREEN, PINK, LIGHT_RED, YELLOW};
bool two_player, map_loaded, GameStrated, player_turn;
int cx, cy;

void play_game();

void gotoxy (int x, int y)
{
	COORD coord; // coordinates
	coord.X = x; coord.Y = y; // X and Y coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void textcolor(int tc, int bc)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, tc+bc*16);
}

;void print_main_menu_item(int i, bool select)
{
    if (select)
        textcolor(BLACK, WHITE);
    else
        textcolor(WHITE, BLACK);
    gotoxy(25, i*2+1);
    switch (i) {
    case 1:
            printf("[1] Load Map\n\n");
            break;
    case 2:
            printf("[2] New Single Player Game\n\n");
            break;
    case 3:
            printf("[3] New Multi Player Game\n\n");
            break;
    case 4:
            printf("[4] Back to Game");
            break;
    case 5:
            printf("[5] Save Map");
            break;
    case 6:
            printf("[6] Load Game");
            break;
    case 7:
            printf("[7] Save Game");
            break;
    case 8:
            printf("[8] Create Map");
            break;
    case 9:
            printf("[9] Exit");
            break;
    }
}

int main_menu(int si)
{
    int i, j;
    char key;
    bool NotEnterPressed = true;
    int ni = 9;

    i = si;
    textcolor(YELLOW, LIGHT_RED);
    gotoxy(25, 1);
    printf("Main Menu");
    for (j = 1; j <= ni; j++)
        print_main_menu_item(j, false);
    print_main_menu_item(i, true);
    gotoxy(0, 23);
    textcolor(GRAY, BLUE);
    for(j=0; j<80; j++)
        printf(" ");
    gotoxy(11, 23);
    printf("Use UP and DOWN Arrow key and then ENTER to select an item");
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                j = i;
                key = getch();
                if (key == 'H')  // UP ARROW
                    if (i > 1)
                        i--;
                    else
                        i = ni;
                if (key == 'P')  // DOWN ARROW
                    if (i < ni)
                        i++;
                    else
                        i = 1;
                if (i != j)
                {
                    print_main_menu_item(j, false);
                    print_main_menu_item(i, true);
                }
            }
        }
    }

    return i;
}

void print_first_menu_item(int i, bool select)
{
    if (select)
        textcolor(BLUE, YELLOW);
    else
        textcolor(YELLOW, BLUE);
    switch (i) {
        case 1:
                gotoxy(1, height*2+2);
                printf("Move");
                break;
        case 2:
                gotoxy(8, height*2+2);
                printf("Split a cell");
                break;
        case 3:
                gotoxy(23, height*2+2);
                printf("Boost energy");
                break;
        case 4:
                gotoxy(38, height*2+2);
                printf("Save");
                break;
        case 5:
                gotoxy(45, height*2+2);
                printf("Exit");
                break;
    }
}

int first_menu(bool draw)
{
    int i=1, j;
    char key;
    bool NotEnterPressed = true;

    if (!draw)
    {
        gotoxy(0, height*2+2);
        textcolor(YELLOW, BLUE);
        for(j=0; j<50; j++)
            printf(" ");
        gotoxy(0, 0);
        return 0;
    }
    print_first_menu_item(1, true);
    for (j = 2; j <= 5; j++)
        print_first_menu_item(j, false);
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                j = i;
                key = getch();
                if (key == 'K')  // Left ARROW
                    if (i > 1)
                        i--;
                    else
                        i = 5;
                if (key == 'M')  // Right ARROW
                    if (i < 5)
                        i++;
                    else
                        i = 1;
                if (i != j)
                {
                    print_first_menu_item(j, false);
                    print_first_menu_item(i, true);
                }
            }
        }
    }

    return i;
}

void print_second_menu_item (int i, bool select)
{
    if (select)
        textcolor(BLUE, YELLOW);
    else
        textcolor(WHITE, LIGHT_RED);
    switch (i) {
    case 0:
            gotoxy(5, height*2+3);
            printf("  N  ");
            break;
    case 1:
            gotoxy(10, height*2+4);
            printf(" NE ");
            break;
    case 2:
            gotoxy(10, height*2+5);
            printf(" SE ");
            break;
    case 3:
            gotoxy(5, height*2+6);
            printf("  S  ");
            break;
    case 4:
            gotoxy(1, height*2+5);
            printf(" SW ");
            break;
    case 5:
            gotoxy(1, height*2+4);
            printf(" NW ");
            break;
    }
}

int second_menu(bool draw)
{
    int i=0, j;
    char key;
    bool NotEnterPressed = true;

    if (!draw)
    {
        textcolor(YELLOW, BLUE);
        gotoxy(0, height*2+3);
        printf("              ");
        gotoxy(0, height*2+4);
        printf("              ");
        gotoxy(0, height*2+5);
        printf("              ");
        gotoxy(0, height*2+6);
        printf("              ");
        gotoxy(0, 0);
        return 0;
    }
    print_second_menu_item(0, true);
    for (j = 1; j <= 6; j++)
        print_second_menu_item(j, false);
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                j = i;
                key = getch();
                if ((key == 'K') || (key == 'H'))  // Left or UP ARROW
                    i = (6+i-1) % 6;
                if ((key == 'M') || (key == 'P'))  // Right or Down ARROW
                    i = (i+1) % 6;
                if (i != j)
                {
                    print_second_menu_item(j, false);
                    print_second_menu_item(i, true);
                }
            }
        }
    }

    return i;
}

void print_cell_enrgy_menu_item(int i, int h, bool select)
{
    switch (i) {
        case 0:
                gotoxy(1, h*2+2);
                textcolor(0, typecolor[0]);
                printf("  ");
                textcolor(YELLOW, BLUE);
                printf(" ");
                if (select)
                    textcolor(BLUE, YELLOW);
                printf("Resource Block"); // len text = 15
                break;
        case 1:
                gotoxy(20, h*2+2);
                textcolor(0, typecolor[1]);
                printf("  ");
                textcolor(YELLOW, BLUE);
                printf(" ");
                if (select)
                    textcolor(BLUE, YELLOW);
                printf("Divide Block"); // len text = 13
                break;
        case 2:
                gotoxy(37, h*2+2);
                textcolor(0, typecolor[2]);
                printf("  ");
                textcolor(YELLOW, BLUE);
                printf(" ");
                if (select)
                    textcolor(BLUE, YELLOW);
                printf("No-entry Block"); // len text = 15
                break;
        case 3:
                gotoxy(56, h*2+2);
                textcolor(0, typecolor[3]);
                printf("  ");
                textcolor(YELLOW, BLUE);
                printf(" ");
                if (select)
                    textcolor(BLUE, YELLOW);
                printf("Normal Block");
                break;
    }
}

int cell_type_menu(int h, bool draw)
{
    int i=0, j;
    char key;
    bool NotEnterPressed = true;

    if (!draw)
    {
        gotoxy(0, h*2+2);
        textcolor(YELLOW, BLUE);
        for(j=0; j<75; j++)
            printf(" ");
        gotoxy(0, 0);
        return 0;
    }
    print_cell_enrgy_menu_item(0, h, true);
    for (j = 1; j <4; j++)
        print_cell_enrgy_menu_item(j, h, false);
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                j = i;
                key = getch();
                if (key == 'K')  // Left ARROW
                    i = (4+(i-1)) % 4;
                if (key == 'M')  // Right ARROW
                    i = (i+1) % 4;
                if (i != j)
                {
                    print_cell_enrgy_menu_item(j, h, false);
                    print_cell_enrgy_menu_item(i, h, true);
                }
            }
        }
    }

    return i+1;
}

void print_escape_menu_item(int i, int h, bool select)
{
    if (select)
        textcolor(BLUE, YELLOW);
    else
        textcolor(YELLOW, BLUE);
    switch (i) {
        case 0:
                gotoxy(1, h*2+2);
                printf("Save");
                break;
        case 1:
                gotoxy(7, h*2+2);
                printf("Save & Exit");
                break;
        case 2:
                gotoxy(20, h*2+2);
                printf("Exit");
                break;
    }
}

int escape_menu(int h, bool draw)
{
    int i=0, j;
    char key;
    bool NotEnterPressed = true;

    if (!draw)
    {
        gotoxy(0, h*2+2);
        textcolor(YELLOW, BLUE);
        for(j=0; j<50; j++)
            printf(" ");
        gotoxy(0, 0);
        return 0;
    }
    print_escape_menu_item(0, h, true);
    for (j = 1; j <3; j++)
        print_escape_menu_item(j, h, false);
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                j = i;
                key = getch();
                if (key == 'K')  // Left ARROW
                    i = (3+(i-1)) % 3;
                if (key == 'M')  // Right ARROW
                    i = (i+1) % 3;
                if (i != j)
                {
                    print_escape_menu_item(j, h, false);
                    print_escape_menu_item(i, h, true);
                }
            }
        }
    }

    return i;
}

int get_enrgy(int h)
{
    int e;

    gotoxy(1, h*2+3);
    textcolor(YELLOW, BLUE);
    printf("Enter block energy: ");
    textcolor(YELLOW, BLUE);
    printf("   ");
    gotoxy(22, h*2+3);
    scanf("%d", &e);
    gotoxy(1, h*2+3);
    textcolor(YELLOW, BLUE);
    printf("                         ");

    return e;
}

void fill_block(struct block **tb, int w, int h, bool ss)
{
    int x, y;

    score1 = 0;
    score2 = 0;
    for (x = 0; x < w; x++)
        for (y = 0; y < h; y++)
        {
            gotoxy(x*4+1, (h-y)*2-1+(x % 2));
            textcolor(0, typecolor[tb[x][y].type-'1']);
            switch (tb[x][y].cell)
            {
                case 0:
                        if (tb[x][y].type == '1')
                            printf("%3d", tb[x][y].block_energy);
                        else
                            printf("   ");
                        break;
                case 1:
                        printf(" %c ", 4);
                        break;
                case 2:
                        printf(" %c ", 5);
                        break;
            }
            score1 = score1 + tb[x][y].cell_energy1;
            score2 = score2 + tb[x][y].cell_energy2;
        }
    gotoxy(1, 24);
    textcolor(YELLOW, BLUE);
    printf("                                                                              ");
    if (ss)
    {
        gotoxy(1, 24);
        if (!player_turn)
            textcolor(YELLOW, LIGHT_RED);
        else
            textcolor(YELLOW, BLUE);
        printf("%c = %d", 4, score1);
        if (two_player)
        {
            gotoxy(10, 24);
            if (player_turn)
                textcolor(YELLOW, LIGHT_RED);
            else
                textcolor(YELLOW, BLUE);
            printf("%c = %d", 5, score2);
        }
    }
}

void draw_table(int w, int h)
{
    int i, j;
    int x, y;
    int z;

    for (i = 0; i < (w+w%2)/2; i++)
        printf("%c%c%c%c%c   ", 218, 196, 196, 196, 191);
    printf("\n");
    printf("%c   ", 179);
    for(i = 0; i < w/2-1; i++)
        printf("%c%c%c%c%c   ", 195, 196, 196, 196, 180);
    if (w%2 == 1)
        printf("%c%c%c%c%c   %c\n", 195, 196, 196, 196, 180, 179);
    else
        printf("%c%c%c%c%c\n", 195, 196, 196, 196, 191);
    for(j = 0; j < h-1; j++)
    {
        for (i = 0; i < w/2; i++)
            printf("%c%c%c%c%c   ", 195, 196, 196, 196, 180);
        if (w%2 == 1)
            printf("%c%c%c%c%c\n", 195, 196, 196, 196, 180);
        else
            printf("%c\n", 179);
        printf("%c   ", 179);
        for(i = 0; i < w/2; i++)
            printf("%c%c%c%c%c   ", 195, 196, 196, 196, 180);
        if (w%2 == 1)
            printf("%c\n", 179);
        else
            printf("\n");
    }
    printf("%c%c%c%c%c   ", 192, 196, 196, 196, 180);
    for(i = 0; i < w/2-1; i++)
        printf("%c%c%c%c%c   ", 195, 196, 196, 196, 180);
    if (w%2 == 1)
        printf("%c%c%c%c%c\n", 195, 196, 196, 196, 217);
    else
        printf("%c\n", 179);
    printf("    ");
    for(i = 0; i < w/2; i++)
        printf("%c%c%c%c%c   ", 192, 196, 196, 196, 217);
    printf("\n");

    gotoxy(1 , 23);
    textcolor(0, typecolor[0]);
    printf("  ");
    textcolor(CYAN, BLUE);
    printf(" Resource Block  ");
    textcolor(0, typecolor[1]);
    printf("  ");
    textcolor(CYAN, BLUE);
    printf(" Divide Block  ");
    textcolor(0, typecolor[2]);
    printf("  ");
    textcolor(CYAN, BLUE);
    printf(" No-entry Block  ");
    textcolor(0, typecolor[3]);
    printf("  ");
    textcolor(CYAN, BLUE);
    printf(" Normal Block");
}

void print_load_save_menu_item(int i, bool select)
{
    if (select)
        textcolor(BLUE, YELLOW);
    else
        textcolor(YELLOW, BLUE);
    switch (i) {
        case 0:
                gotoxy(37, 3);
                printf("Binary");
                break;
        case 1:
                gotoxy(45, 3);
                printf("Text");
                break;
    }
}

int load_save_menu(char *filename, int l_s, int m_g)
{
    int  i=0;
    char key;
    bool NotEnterPressed = true;

    textcolor(CYAN, BLUE);
    system("CLS");
    gotoxy(2, 1);
    textcolor(YELLOW, LIGHT_RED);
    if (l_s == 1)
        printf("Load ");
    else
        printf("Save ");
    if (m_g == 1)
        printf("Map");
    else
        printf("Game");
    textcolor(CYAN, BLUE);
    gotoxy(2, 3);
    printf("Select the format of output file : ");
    print_load_save_menu_item(0, true);
    print_load_save_menu_item(1, false);
    while (NotEnterPressed)
    {
        key = getch();
        if (key == 13)   // Enter is pressed
            NotEnterPressed = false;
        else
        {
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                key = getch();
                if ((key == 'K') || (key == 'M'))  // Left or Right ARROW
                {
                    print_load_save_menu_item(i, false);
                    i = ((i+1) % 2);
                    print_load_save_menu_item(i, true);
                }
            }
        }
    }
    textcolor(CYAN, BLUE);
    gotoxy(2, 5);
    printf("Enter the file name: ");
    textcolor(BLUE, YELLOW);
    printf("               ");
    textcolor(CYAN, BLUE);
    if (i == 0)
        printf(".bin");
    else
        printf(".txt");
    textcolor(BLUE, YELLOW);
    gotoxy(22, 5);
    scanf("%s", filename);
    if (i == 0)
        filename = strcat(filename, ".bin ");
    else
        filename = strcat(filename, ".txt");
    return i;
}

bool load_map()
{
    int n, x, y, tx, ty;
    char filename[80];
    FILE *fp;

    n = load_save_menu(filename, 1, 1);
    fp = fopen(filename, "r");
    if (fp == NULL)
        return false;
    if (map_loaded)
    {
        for(x = 0; x < width; x++)
            free(table[x]);
        free(table);
    }
    if (n == 0)
    {
        fread(&width, sizeof(width), 1, fp);
        fread(&height, sizeof(height), 1, fp);
        table = (struct block **) malloc(width * sizeof(struct block *));
        for(x = 0; x < width; x++)
            table[x] = (struct block *) malloc(height * sizeof(struct block));
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
            {
                fread(&table[x][y].block_energy, sizeof(int), 1, fp);
                fread(&table[x][y].type, sizeof(char), 1, fp);
                table[x][y].cell = 0;
                table[x][y].cell_energy1 = 0;
                table[x][y].cell_energy2 = 0;
            }
    }
    else
    {
        fscanf(fp, "%d", &width);
        fscanf(fp, "%d", &height);
        table = (struct block **) malloc(width * sizeof(struct block *));
        for(x = 0; x < width; x++)
            table[x] = (struct block *) malloc(height * sizeof(struct block));
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
            {
                fscanf(fp, "%d %d %d %c", &tx, &ty, &table[x][y].block_energy, &table[x][y].type,
                        &table[x][y].cell, &table[x][y].cell_energy1, &table[x][y].cell_energy2);
                table[x][y].cell = 0;
                table[x][y].cell_energy1 = 0;
                table[x][y].cell_energy2 = 0;
            }
    }
	fclose(fp);
	map_loaded = true;
	GameStrated = false;
    textcolor(WHITE, BLACK);
    system("CLS");
	return true;
}

bool save_map(struct block **table, int width, int height)
{
    FILE *fp;
    char filename[80];
    int n, x, y;

    n = load_save_menu(filename, 2, 1);
    fp = fopen(filename, "w");
    if (fp == NULL)
        return false;
    if (n == 0)
    {
        fwrite(&width,  sizeof(width),  1, fp);
        fwrite(&height, sizeof(height), 1, fp);
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
            {
                fwrite(&table[x][y].block_energy, sizeof(int), 1, fp);
                fwrite(&table[x][y].type, sizeof(char), 1, fp);
            }
    }
    else
    {
        fprintf(fp, "%d\n", width);
        fprintf(fp, "%d\n", height);
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
                fprintf(fp, "%2d %2d  %3d  %c\n", x, y, table[x][y].block_energy, table[x][y].type);
    }
    fclose(fp);
    textcolor(WHITE, BLACK);
    system("CLS");
    return true;
}

bool load_game()
{
    FILE *fp;
    char filename[80];
    int n, x, y, tx, ty;

    n = load_save_menu(filename, 1, 2);
    fp = fopen(filename, "r");
    if (fp == NULL)
        return false;
    if (map_loaded)
    {
        for(x = 0; x < width; x++)
            free(table[x]);
        free(table);
    }
    if (n == 0)
    {
        fread(&two_player, sizeof(two_player), 1, fp);
        fread(&player_turn, sizeof(player_turn), 1, fp);
        fread(&cx, sizeof(cx), 1, fp);
        fread(&cy, sizeof(cy), 1, fp);
        fread(&width, sizeof(width), 1, fp);
        fread(&height, sizeof(height), 1, fp);
        table = (struct block **) malloc(width * sizeof(struct block *));
        for(x = 0; x < width; x++)
            table[x] = (struct block *) malloc(height * sizeof(struct block));
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
            {
                fread(&table[x][y].block_energy, sizeof(int), 1, fp);
                fread(&table[x][y].type, sizeof(char), 1, fp);
                fread(&table[x][y].cell, sizeof(int), 1, fp);
                fread(&table[x][y].cell_energy1, sizeof(int), 1, fp);
                fread(&table[x][y].cell_energy2, sizeof(int), 1, fp);
            }
    }
    else
    {
        fscanf(fp, "%d", &two_player);
        fscanf(fp, "%d", &player_turn);
        fscanf(fp, "%d", &cx);
        fscanf(fp, "%d", &cy);
        fscanf(fp, "%d", &width);
        fscanf(fp, "%d", &height);
        table = (struct block **) malloc(width * sizeof(struct block *));
        for(x = 0; x < width; x++)
            table[x] = (struct block *) malloc(height * sizeof(struct block));
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
                fscanf(fp, "%d %d %d %c %d %d %d", &tx, &ty, &table[x][y].block_energy, &table[x][y].type,
                        &table[x][y].cell, &table[x][y].cell_energy1, &table[x][y].cell_energy2);
    }
    fclose(fp);
    map_loaded = true;
    play_game();
    return true;
}

bool save_game()
{
    FILE *fp;
    char filename[80];
    int n, x, y;

    n = load_save_menu(filename, 2, 2);
    fp = fopen(filename, "w");
    if (fp == NULL)
        return false;
    if (n == 0)
    {
        fwrite(&two_player, sizeof(two_player), 1, fp);
        fwrite(&two_player, sizeof(player_turn), 1, fp);
        fwrite(&cx, sizeof(cx), 1, fp);
        fwrite(&cy, sizeof(cy), 1, fp);
        fwrite(&width,  sizeof(width),  1, fp);
        fwrite(&height, sizeof(height), 1, fp);
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
            {
                fwrite(&table[x][y].block_energy, sizeof(int), 1, fp);
                fwrite(&table[x][y].type, sizeof(char), 1, fp);
                fwrite(&table[x][y].cell, sizeof(int), 1, fp);
                fwrite(&table[x][y].cell_energy1, sizeof(int), 1, fp);
                fwrite(&table[x][y].cell_energy2, sizeof(int), 1, fp);
            }
    }
    else
    {
        fprintf(fp, "%d\n", two_player);
        fprintf(fp, "%d\n", player_turn);
        fprintf(fp, "%d\n", cx);
        fprintf(fp, "%d\n", cy);
        fprintf(fp, "%d\n", width);
        fprintf(fp, "%d\n", height);
        for (x = 0; x < width; x++)
            for (y = height-1; y >= 0; y--)
                fprintf(fp, "%2d %2d  %3d  %c  %3d  %3d  %3d\n", x, y, table[x][y].block_energy, table[x][y].type,
                        table[x][y].cell, table[x][y].cell_energy1, table[x][y].cell_energy2);
    }
    fclose(fp);
    textcolor(WHITE, BLACK);
    system("CLS");
    return true;
}

void current_block(int x, int y, int width, int height)
{
    textcolor(WHITE, BLUE);
    gotoxy(x*4, (height-y)*2-2+(x % 2));
    printf("%c%c%c%c%c", 218, 196, 196, 196, 191);
    gotoxy(x*4, (height-y)*2-1+(x % 2));
    printf("%c", 179);
    gotoxy(x*4+4, (height-y)*2-1+(x % 2));
    printf("%c", 179);
    gotoxy(x*4, (height-y)*2+(x % 2));
    printf("%c%c%c%c%c", 192, 196, 196, 196, 217);
}

void show_status()
{
    textcolor(YELLOW, BLUE);
    gotoxy(35, 24);
    if ((!player_turn) && (table[cx][cy].cell == 1))
        printf("Cell Energy = %d", table[cx][cy].cell_energy1);
    if ((player_turn) && (table[cx][cy].cell == 2))
        printf("Cell Energy = %d", table[cx][cy].cell_energy2);
    gotoxy(53, 24);
    if (table[cx][cy].type == '1')
        printf("Block Energy = %d", table[cx][cy].block_energy);
}
void draw_and_fill(struct block **tb, int w, int h, bool ss)
{
    textcolor(CYAN, BLUE);
    gotoxy(0, 0);
    draw_table(w, h);
    fill_block(tb, w, h, ss);
}

void print_notification(int tc, int bc, char * msg)
{
    int i, l;

    gotoxy(0, 24);
    textcolor(tc, bc);
    l = strlen(msg) + strlen(" - Press any key ...");
    for (i=0; i<(40-l/2); i++)
        printf(" ");
    printf("%s%s", msg, " - Press any key ...");
    for (i=i+l; i<79; i++)
        printf(" ");
    getch();
    gotoxy(0, 24);
    textcolor(GRAY, BLACK);
    for(i=0; i<79; i++)
        printf(" ");
}

void get_number_of_cell(int np)
{
    int x, y, n;

    textcolor(CYAN, BLUE);
    system("CLS");
    if (np == 1)
    {
        two_player = false;
        printf("\nEnter the number of cell: ");
    }
    else
    {
        two_player = true;
        printf("\nEnter the number of cell for player1: ");
    }
    player_turn = false;
    scanf("%d", &n);
    while( n > 0)
    {
        x = (rand()*width)/(RAND_MAX+1);
        y = (rand()*height)/(RAND_MAX+1);
        if ((table[x][y].type != '3') && (table[x][y].cell == 0))
        {
            table[x][y].cell = 1;
            table[x][y].cell_energy1 = 0;
            n--;
        }
    }
    if (two_player)
    {
        printf("\nEnter the number of cell for player2: ");
        scanf("%d", &n);
        while( n > 0)
        {
            x = (rand()*width)/(RAND_MAX+1);
            y = (rand()*height)/(RAND_MAX+1);
            if ((table[x][y].type != '3') && (table[x][y].cell == 0))
            {
                table[x][y].cell = 2;
                table[x][y].cell_energy2 = 0;
                n--;
            }
        }
    }
    cx = 0;
    cy = height - 1;
}

int check_neighbours(int cx, int cy, int *neighbourtable)
{
    int  ty;

    // North
    if ((cy < height-1) && (table[cx][cy+1].type != '3') && (table[cx][cy+1].cell == 0))  //N
        neighbourtable[0]=1;
    else
        neighbourtable[0]=0;
    // North East
    ty = cy + (1-(cx%2));
    if((cx < width -1) && (ty<height) && (table[cx+1][ty].type != '3') && (table[cx+1][ty].cell == 0))  //NE
        neighbourtable[1]=1;
    else
        neighbourtable[1]=0;

    ty = cy - (cx%2);
    if((cx<width-1) && (ty>=0) && (table[cx+1][ty].type != '3') && (table[cx+1][ty].cell == 0))  //SE
        neighbourtable[2]=1;
    else
        neighbourtable[2]=0;

    if((cy > 0) &&(table[cx][cy-1].type != '3')&& (table[cx][cy-1].cell ==0))    //S
        neighbourtable[3]=1;
    else
        neighbourtable[3]=0;

    ty= cy - (cx%2);
    if((cx>0) && (ty>=0) && (table[cx-1][ty].type != '3') && (table[cx-1][ty].cell == 0))  //SW
        neighbourtable[4]=1;
    else
        neighbourtable[4]=0;

    ty= cy + (1-(cx%2));
    if((cx>0) && (ty<height) && (table[cx-1][ty].type != '3') && (table[cx-1][ty].cell == 0))  //NW
        neighbourtable[5]=1;
    else
        neighbourtable[5]=0;
}

void play_game()
{
    int ty, n, m, neighbourtable[6];
    char key;
    bool NotPressedEscape = true;

    textcolor(CYAN, BLUE);
    system("CLS");
    draw_and_fill(table, width, height, true);
    current_block(cx, cy, width, height);
    show_status();
    while (NotPressedEscape)
    {
        key = getch();
        if ((key == 13) && ( ((!player_turn) && (table[cx][cy].cell == 1)) ||
                             ((player_turn) && (table[cx][cy].cell == 2)) ))
        {
            n = first_menu(true);
            check_neighbours(cx, cy, neighbourtable);
            switch (n)
            {
                case 1: // Move
                        m = second_menu(true);
                        switch (m)
                        {
                            case 0: // North
                                    if (neighbourtable[0] == 1)
                                    {
                                        table[cx][cy].cell = 0;
                                        if (!player_turn)
                                        {
                                            table[cx][cy+1].cell = 1;
                                            table[cx][cy+1].cell_energy1 = table[cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx][cy+1].cell = 2;
                                            table[cx][cy+1].cell_energy2 = table[cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cy = cy + 1;
                                        if (two_player)
                                            player_turn = !player_turn;
                                    }
                                    break;
                            case 1: //northeast
                                    if (neighbourtable[1] == 1)
                                    {
                                        ty = cy + (1-(cx % 2));
                                        table[cx][cy].cell = 0;
                                        if (!player_turn)
                                        {
                                            table[cx+1][ty].cell = 1;
                                            table[cx+1][ty].cell_energy1 = table [cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx+1][ty].cell = 2;
                                            table[cx+1][ty].cell_energy2 = table [cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cx = cx+1;
                                        cy = ty;
                                        if (two_player)
                                            player_turn = !player_turn;
                                    }
                                    break;
                            case 2: //southeast
                                    if (neighbourtable[2] == 1)
                                    {
                                        ty = cy - (cx % 2);
                                        table[cx][cy].cell = 0;
                                        if (!player_turn)
                                        {
                                            table[cx+1][ty].cell = 1;
                                            table[cx+1][ty].cell_energy1 = table [cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx+1][ty].cell = 2;
                                            table[cx+1][ty].cell_energy2 = table [cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cx = cx+1;
                                        cy = ty;
                                        if (two_player)
                                            player_turn = !player_turn;
                                    }
                                    break;
                            case 3: //south
                                    if (neighbourtable[3] == 1)
                                    {
                                        table[cx][cy].cell = 0;
                                        if (!player_turn)
                                        {
                                            table[cx][cy-1].cell = 1;
                                            table[cx][cy-1].cell_energy1 = table [cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx][cy-1].cell = 2;
                                            table[cx][cy-1].cell_energy2 = table [cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cy=cy-1;
                                        if (two_player)
                                            player_turn = !player_turn;
                                    }
                                    break;
                            case 4: //southwest
                                    if (neighbourtable[4] == 1)
                                    {
                                        ty = cy - (cx % 2);
                                        table[cx][cy].cell   = 0;
                                        if (!player_turn)
                                        {
                                            table[cx-1][ty].cell = 1;
                                            table[cx-1][ty].cell_energy1 = table [cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx-1][ty].cell = 2;
                                            table[cx-1][ty].cell_energy2 = table [cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cx = cx-1;
                                        cy = ty;
                                        if (two_player)
                                            player_turn = !player_turn;
                                }
                                break;
                            case 5: //northwest
                                    if (neighbourtable[5] == 1)
                                    {
                                        ty = cy + (1-(cx%2));
                                        table[cx][cy].cell = 0;
                                        if (!player_turn)
                                        {
                                            table[cx-1][ty].cell = 1;
                                            table[cx-1][ty].cell_energy1 = table [cx][cy].cell_energy1;
                                            table[cx][cy].cell_energy1 = 0;
                                        }
                                        else
                                        {
                                            table[cx-1][ty].cell = 2;
                                            table[cx-1][ty].cell_energy2 = table [cx][cy].cell_energy2;
                                            table[cx][cy].cell_energy2 = 0;
                                        }
                                        cx = cx-1;
                                        cy = ty;
                                        if (two_player)
                                            player_turn = !player_turn;
                                    }
                                    break;
                        }
                        second_menu(false);
                        draw_and_fill(table, width, height, true);
                        current_block(cx, cy, width, height);
                        show_status();
                        break;
                case 2: // Split a cell
                        if (table[cx][cy].type == '2')
                            if (((!player_turn) && (table[cx][cy].cell_energy1 >= 80)) ||
                                (( player_turn) && (table[cx][cy].cell_energy2 >= 80)))
                            {
                                int k, fb, rk;

                                fb = 0;
                                for (k=0; k < 6; k++)
                                    fb = fb + neighbourtable[k];
                                if (fb > 0)
                                {
                                    rk = (rand()*fb)/(RAND_MAX+1);
                                    for (k=0; rk>0; k++)
                                        if(neighbourtable[k] == 1)
                                            rk--;
                                    switch (k)
                                    {
                                        case 0: // North
                                            if (!player_turn)
                                            {
                                                table[cx][cy+1].cell = 1;
                                                table[cx][cy].cell_energy1 = table[cx][cy+1].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx][cy+1].cell = 2;
                                                table[cx][cy].cell_energy2 = table[cx][cy+1].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                        case 1: // North East
                                            ty = cy + (1-(cx%2));
                                            if (!player_turn)
                                            {
                                                table[cx+1][ty].cell = 1;
                                                table[cx][cy].cell_energy1 = table [cx+1][ty].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx+1][ty].cell = 2;
                                                table[cx][cy].cell_energy2 = table [cx+1][ty].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                        case 2: // South East
                                            ty = cy - (cx%2);
                                            if (!player_turn)
                                            {
                                                table[cx+1][ty].cell = 1;
                                                table[cx][cy].cell_energy1 = table [cx+1][ty].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx+1][ty].cell = 2;
                                                table[cx][cy].cell_energy2 = table [cx+1][ty].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                        case 3: // South
                                            if (!player_turn)
                                            {
                                                table[cx][cy-1].cell = 1;
                                                table[cx][cy].cell_energy1 = table [cx][cy-1].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx][cy-1].cell = 2;
                                                table[cx][cy].cell_energy2 = table [cx][cy-1].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                        case 4: // South West
                                            ty= cy - (cx%2);
                                            if (!player_turn)
                                            {
                                                table[cx-1][ty].cell = 1;
                                                table[cx][cy].cell_energy1 = table [cx-1][ty].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx-1][ty].cell = 2;
                                                table[cx][cy].cell_energy2 = table [cx-1][ty].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                        case 5: // North West
                                            ty= cy + (1-(cx%2));
                                            if (!player_turn)
                                            {
                                                table[cx-1][ty].cell = 1;
                                                table[cx][cy].cell_energy1 = table[cx-1][ty].cell_energy1 = 40;
                                            }
                                            else
                                            {
                                                table[cx-1][ty].cell = 2;
                                                table[cx][cy].cell_energy2 = table[cx-1][ty].cell_energy2 = 40;
                                            }
                                            if (two_player)
                                                player_turn = !player_turn;
                                            break;
                                    }
                                }
                                else
                                    print_notification(YELLOW, LIGHT_RED, "No neighbour free block exists!");
                            }
                            else
                                print_notification(YELLOW, LIGHT_RED, "No enough energy exits to split the cell!");
                        else
                            print_notification(YELLOW, LIGHT_RED, "No split block!");
                        break;
                case 3: // Boost energy
                        if (!player_turn)
                        {
                            if (table[cx][cy].type == '1')
                            {
                                if (table[cx][cy].cell_energy1 <= 100)
                                {
                                    if (table[cx][cy].block_energy > 0)
                                        if (two_player)
                                            player_turn = !player_turn;
                                    if(table[cx][cy].block_energy >= 15)
                                    {
                                        table[cx][cy].cell_energy1 = table[cx][cy].cell_energy1 + 15;
                                        table[cx][cy].block_energy = table[cx][cy].block_energy - 15;
                                    }
                                    else
                                    {
                                        table[cx][cy].cell_energy1 = table[cx][cy].cell_energy1 + table[cx][cy].block_energy;
                                        table[cx][cy].block_energy = 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (table[cx][cy].type == '1')
                            {
                                if (table[cx][cy].cell_energy2 <= 100)
                                {
                                    if (table[cx][cy].block_energy > 0)
                                        if (two_player)
                                            player_turn = !player_turn;
                                    if(table[cx][cy].block_energy >= 15)
                                    {
                                        table[cx][cy].cell_energy2 = table[cx][cy].cell_energy2 + 15;
                                        table[cx][cy].block_energy = table[cx][cy].block_energy - 15;
                                    }
                                    else
                                    {
                                        table[cx][cy].cell_energy2 = table[cx][cy].cell_energy2 + table[cx][cy].block_energy;
                                        table[cx][cy].block_energy = 0;
                                    }
                                }
                            }
                        }
                        break;
                case 4: // Save
                        save_game();
                        textcolor(CYAN, BLUE);
                        system("CLS");
                        break;
                case 5: // Exit
                        NotPressedEscape = false;
                        break;
            }
            first_menu(false);
            draw_and_fill(table, width, height, true);
            current_block(cx, cy, width, height);
            show_status();
        }
        else
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                key = getch();
                draw_and_fill(table, width, height, true);
                switch (key)
                {
                        case 'M':  // Right
                                    if (cx < width-1)
                                        cx++;
                                    else
                                        cx = 0;
                                    break;
                        case 'K':   // Left
                                    if (cx > 0)
                                        cx--;
                                    else
                                        cx = width-1;
                                    break;
                        case 'H':   //Up
                                    if (cy < height-1)
                                        cy++;
                                    else
                                        cy = 0;
                                    break;
                        case 'P':   // Down
                                    if (cy > 0)
                                        cy--;
                                    else
                                        cy = height-1;
                                    break;
                }
                current_block(cx, cy, width, height);
                show_status();
            }
        if (key == 27)
            NotPressedEscape = false;
    }
    GameStrated = true;
    textcolor(WHITE, BLACK);
    system("CLS");
}

void exit_message()
{
    int i;

    textcolor(GRAY, BLACK);
    system("CLS");
    printf("GOOD BYE!\n\n");
    if (map_loaded)
    {
        for(i = 0; i < width; i++)
            free(table[i]);
        free(table);
    }
}

void create_map()
{
    int x, y, ty, n, m;
    char key;
    bool NotPressedEscape = true;
    int w, h;
    struct block **ctable;

    textcolor(CYAN, BLUE);
    system("CLS");
    gotoxy(1, 1);
    printf("Enter the width: ");
    scanf("%d", &w);
    gotoxy(1, 3);
    printf("Enter the height: ");
    scanf("%d", &h);
    ctable = (struct block **) malloc(w * sizeof(struct block *));
    for(x = 0; x < w; x++)
        ctable[x] = (struct block *) malloc(h * sizeof(struct block));
    for (x = 0; x < w; x++)
        for (y = h-1; y >= 0; y--)
        {
            ctable[x][y].block_energy = 0;
            ctable[x][y].type = '4';
            ctable[x][y].cell = 0;
            ctable[x][y].cell_energy1 = 0;
            ctable[x][y].cell_energy2 = 0;
        }
    system("CLS");
    draw_and_fill(ctable, w, h, false);
    getch();
    x = 0;
    y = h-1;
    current_block(x, y, w, h);
    while (NotPressedEscape)
    {
        key = getch();
        if (key == 13)
        {
            n = cell_type_menu(h, true);
            ctable[x][y].type = '0'+ n;
            if (n == 1)
                ctable[x][y].block_energy = get_enrgy(h);
            n = cell_type_menu(h, false);
            draw_and_fill(ctable, w, h, false);
            current_block(x, y, w, h);
        }
        else
            if ((int) key == -32) // if key == -32, a arrow key was pressed
            {
                key = getch();
                draw_and_fill(ctable, w, h, false);
                switch (key) {
                case 'M':  // Right
                            if (x < w-1)
                                x++;
                            else
                                x = 0;
                            break;
                case 'K':   // Left
                            if (x > 0)
                                x--;
                            else
                                x = w-1;
                            break;
                case 'H':   //Up
                            if (y < h-1)
                                y++;
                            else
                                y = 0;
                            break;
                case 'P':   // Down
                            if (y > 0)
                                y--;
                            else
                                y = h-1;
                            break;
                }
                current_block(x, y, w, h);
            }
        if (key == 27)
        {
            m = escape_menu(h, true);
            switch (m){
                case 0: // save
                     if (save_map(ctable, w, h))
                         print_notification(BLACK, LIGHT_GREEN, "The new map was saved successfully");
                     else
                         print_notification(YELLOW, LIGHT_RED, "The map was not saved!");
                     textcolor(CYAN, BLUE);
                     system("CLS");
                     break;
                case 1: // save & exit
                    if (save_map(ctable, w, h))
                    {
                        print_notification(BLACK, LIGHT_GREEN, "The new map was saved successfully");
                        NotPressedEscape = false;
                    }
                     else
                        print_notification(YELLOW, LIGHT_RED, "The map was not saved!");
                     break;
                case 2: // exit without saving
                     NotPressedEscape = false;
                     break;
            }
            m = escape_menu(h, false);
        }
        draw_and_fill(ctable, w, h, false);
        current_block(x, y, w, h);
    }
    for(x = 0; x < width; x++)
        free(ctable[x]);
    free(ctable);
    textcolor(WHITE, BLACK);
    system("CLS");
}

int main()
{
    int n = 1;
    bool NotQuit = true;
    time_t t = time(NULL);

    srand(t);
    map_loaded = false;
    GameStrated = false;
    cx = 0;
    cy = 0;
    textcolor(WHITE, BLACK);
    system("CLS");
    while (NotQuit)
    {
        n = main_menu(n);
        switch(n) {
        case 1: // Load Map
                if (load_map())
                    print_notification(BLACK, LIGHT_GREEN, "The map file was load.");
                else
                {
                    print_notification(YELLOW, LIGHT_RED, "The map file not find!");
                    textcolor(WHITE, BLACK);
                    system("CLS");
                }
                break;
        case 2: // New Single Player Game
                if (map_loaded)
                {
                    get_number_of_cell(1);
                    play_game();
                }
                else
                    print_notification(YELLOW, LIGHT_RED, "First, the map must be load!");
                break;
        case 3: // New Multi Player Game
                if (map_loaded)
                {
                    get_number_of_cell(2);
                    play_game();
                }
                else
                    print_notification(YELLOW, LIGHT_RED, "First, the map must be load!");
                break;
        case 4: // Back to Game
                if (GameStrated)
                    play_game();
                else
                    print_notification(YELLOW, LIGHT_RED, "The game is not started yet!");
                break;
        case 5: // Save Map
                if (map_loaded)
                    if(save_map(table, width, height))
                        print_notification(BLACK, LIGHT_GREEN, "The map was saved successfully");
                    else
                        print_notification(YELLOW, LIGHT_RED, "The map was not saved!");
                else
                    print_notification(YELLOW, LIGHT_RED, "The map is not load yet, Can't save it!");
                break;
        case 6: // Load Game
                if (!load_game())
                {
                    print_notification(YELLOW, LIGHT_RED, "Can't load the game!");
                    textcolor(WHITE, BLACK);
                    system("CLS");
                }
                break;
        case 7: // Save Game
                if (GameStrated)
                    if(save_game())
                        print_notification(BLACK, LIGHT_GREEN, "The game was saved successfully");
                    else
                        print_notification(YELLOW, LIGHT_RED, "The game was not saved!");
                else
                    print_notification(YELLOW, LIGHT_RED, "The game is not started yet, Can't save it!");
                break;
        case 8: // Create Map
                create_map();
                break;
        case 9: // Exit
                exit_message();
                NotQuit = false;
        }
    }

    return 0;
}
