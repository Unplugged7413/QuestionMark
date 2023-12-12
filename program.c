#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int yMax, xMax;
int language = 1; // 1 - english, 2 - slovak, 3 - french


int menu(char *choices[30], int numberOfChoices, bool isLevels, bool *levelsDone)
{
    WINDOW *menuWin = newwin(yMax/2, xMax/4, yMax/4, xMax/4 + xMax/8);
    box(menuWin, 0, 0);

    // animation
    WINDOW *background1 = newwin(yMax/2 + 2, xMax/4 + 4, yMax/4 - 1, xMax/4 + xMax/8 - 2);
    WINDOW *background2 = newwin(yMax/2 + 4, xMax/4 + 8, yMax/4 - 2, xMax/4 + xMax/8 - 4);
    WINDOW *background3 = newwin(yMax/2 + 8, xMax/4 + 16, yMax/4 - 4, xMax/4 + xMax/8 - 8);



    keypad(menuWin, true);

    int choice, highlight = 0;

    halfdelay(3);

    int randColor;
    chtype wall;
    chtype dash;

    int counter = 1;
    while(1)
    {
        choice = wgetch(menuWin);
        // animation
        randColor = rand() % 4 + 2;
        wall = COLOR_PAIR(randColor) | '$';
        dash = COLOR_PAIR(randColor) | '~';
        wclear(background1);
        wclear(background2);
        wclear(background3);
        switch(counter)
        {
            case 1:
                attron(COLOR_PAIR(randColor));
                box(background1, wall, dash);
                attroff(COLOR_PAIR(randColor));
                wrefresh(background1);
                counter++;
                break;
            case 2:
                wclear(background1);
                wrefresh(background1);
                box(background2, wall, dash);
                wrefresh(background2);
                counter++;
                break;
            case 3:
                wclear(background2);
                wrefresh(background2);
                box(background3, wall, dash);
                wrefresh(background3);
                counter++;
                break;
            case 4:
                wclear(background3);
                wrefresh(background3);
                counter = 1;
                break;
        }



        box(menuWin, 0, 0);
        wrefresh(menuWin);

        for(int i = 0; i < numberOfChoices; ++i)
        {
            if(i == highlight)
                wattron(menuWin, A_REVERSE);
            if(isLevels && i == 3)
                mvwprintw(menuWin, yMax/4 + i -2, xMax/8 - 4, "%s", choices[i]);
            else
                mvwprintw(menuWin, yMax/4 + i - 3, xMax/8 - 4, "%s", choices[i]);
            wattroff(menuWin, A_REVERSE);
            if(isLevels && levelsDone[i])
            {
                wattron(menuWin, COLOR_PAIR(4));
                mvwprintw(menuWin, yMax/4 + i - 3, xMax/8 + 6, "[DONE]");
                wattroff(menuWin, COLOR_PAIR(4));
            }
        }
        if(choice == 10)
        {
            wclear(background1);
            wclear(background2);
            wclear(background3);
            if(isLevels)
            {
//                box(background3, wall, dash);
//                wrefresh(background3);
                wclear(background3);
                wrefresh(background3);
            }
            return highlight;
        }
        switch(choice)
        {
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                    highlight = numberOfChoices-1;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == numberOfChoices)
                    highlight = 0;
                break;
        }
    }
}



int level1()
{
    int windowY = 21;
    int windowX = 42;
    WINDOW *mapWin = newwin(windowY, windowX, (yMax - windowY) / 2, (xMax - windowX) / 2);
    box(mapWin, '|', '~');

    bool hintMes = 1;

    WINDOW *hintWin = newwin(9, 28, 2, 2);
    WINDOW *statsWin = newwin(5, 38, (yMax - windowY) / 2 - 4, (xMax - windowX) / 2 + 2);
    box(statsWin, '|', '~');
    wrefresh(statsWin);

    nocbreak();
    if(language == 1)
    {
        mvwprintw(mapWin, 8, 11, "Welcome to LEVEL 1");
        mvwprintw(mapWin, 9, 11, "To win it you need to");
        mvwprintw(mapWin, 10, 11, "take three coins");
        mvwprintw(mapWin, 12, 11, "press any key to start...");
    } else if(language == 2)
    {
        mvwprintw(mapWin, 8, 8, "Vitajte na UROVNI 1");
        mvwprintw(mapWin, 9, 8, "Ak chcete vyhrať,");
        mvwprintw(mapWin, 10, 8, "musíte si vziať tri mince");
        mvwprintw(mapWin, 12, 8, "stlacte ľubovoľnú");
        mvwprintw(mapWin, 13, 11, "klavesu...");
    } else if(language == 3)
    {
        mvwprintw(mapWin, 8, 9, "Bienvenue au NIVEAU 1"); // 24
        mvwprintw(mapWin, 9, 9, "Pour gagner, vous devez");
        mvwprintw(mapWin, 10, 9, "prendre trois pièces");
        mvwprintw(mapWin, 12, 7, "appuyez sur n'importe quelle");
        mvwprintw(mapWin, 13, 8, "touche pour commencer...");
    }


    wrefresh(mapWin);
    cbreak();

    wgetch(mapWin);
    wclear(mapWin);
    clear();
    box(mapWin, '|', '~');
    halfdelay(1);


    for(int i = 1; i < windowY-1; ++i)
    {
        for(int j = 1; j < windowX-1; j+=2)
        {
            if(i < 7 && j == 35)
            {
                mvwaddch(mapWin, i, j, '|');
            }
            else if(i == 6 && j > 35)
            {
                mvwaddch(mapWin, i, j, '_');
                mvwaddch(mapWin, i, j - 1, '_');
            }
            else if(i == 4 && j < 16)
            {
                mvwaddch(mapWin, i, j, '_');
                mvwaddch(mapWin, i, j + 1, '_');
            }
            else
                mvwaddch(mapWin, i, j, '.');
        }
    }

    // hint
    chtype hint = A_REVERSE | '?';
    mvwaddch(mapWin, 15, 31, hint);

    // coins
    chtype coin = COLOR_PAIR(3) | '*';
    int coinCounter = 0;
    bool coinCheck1 = 1;
    bool coinCheck2 = 1;
    bool coinCheck3 = 1;

    mvwaddch(mapWin, 2, 3, coin);
    mvwaddch(mapWin, 2, 39, coin);

    keypad(mapWin, TRUE);

    int ch;
    int playerY = windowY-2;
    int playerX = windowX/2;
    chtype player = COLOR_PAIR(1) | '?';

    int enemyY = windowY/2 + windowY/4;
    int enemyX = 1;
    int enemyDir = 1; // 1 - left, 0 - rigth

    int enemy2Y = 1;
    int enemy2X = 15;
    int enemy2Dir = 1;

    do
    {
        if(ch != ERR)
        {
            nodelay(mapWin, TRUE);
            flushinp();
            usleep(100000); // 0.1 sec
            flushinp();
            nodelay(mapWin, FALSE);
        }
        // hint updater
        mvwaddch(mapWin, 15, 31, hint);

        // hint message
        if(playerY == 15 && playerX == 31 && hintMes)
        {
            hintMes = 0;
            if(language == 1)
            {
                mvwprintw(hintWin, 0, 0, "in order to take the");
                mvwprintw(hintWin, 2, 6, "coin you need to enter");
                mvwprintw(hintWin, 4, 2, "the room through the");
                mvwprintw(hintWin, 5, 5, "upper wall");
                mvwprintw(hintWin, 7, 1, "(it is not real)");
            } else if(language == 2)
            {
                mvwprintw(hintWin, 0, 0, "aby ste si mohli");
                mvwprintw(hintWin, 2, 6, "vziať mincu, musíte");
                mvwprintw(hintWin, 4, 2, "vstúpiť do miestnosti");
                mvwprintw(hintWin, 5, 5, "cez hornú stenu");
                mvwprintw(hintWin, 7, 1, "(nie je skutočná)");
            } else if(language == 3)
            {
                mvwprintw(hintWin, 0, 0, "pour prendre la pièce,");
                mvwprintw(hintWin, 2, 6, "il faut entrer dans");
                mvwprintw(hintWin, 4, 2, "la pièce par");
                mvwprintw(hintWin, 5, 5, "le mur du haut");
                mvwprintw(hintWin, 7, 1, "(il n'est pas réel)");
            }

            mvwaddch(mapWin, 7, 13, coin);
            wrefresh(hintWin);
        } else
        {
            hintMes = 1;
            mvwaddch(mapWin, 7, 13, '.');
            wclear(hintWin);
            wrefresh(hintWin);
            wrefresh(mapWin);
        }

        // coin track
        if( (playerY == 2 && playerX == 39) && coinCheck1)
        {
            coinCheck1 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        }
        if( (playerY == 2 && playerX == 3) && coinCheck2)
        {
            coinCheck2 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        }
        if( (playerY == 7 && playerX == 13) && coinCheck3)
        {
            coinCheck3 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        }

        // check win(3 coins)
        if(coinCounter > 2)
            return 1;

        // enemy
        if(enemyDir)
        {
            mvwaddch(mapWin, enemyY, enemyX, '.');
            enemyX += 2;
            if(enemyX > windowX - 4)
                enemyDir = 0;
        } else
        {
            mvwaddch(mapWin, enemyY, enemyX, '.');
            enemyX -= 2;
            if(enemyX < 3)
                enemyDir = 1;
        }
        chtype enemy = COLOR_PAIR(2) | '!';
        mvwaddch(mapWin, enemyY, enemyX, enemy);

        // enemy 2
        if(enemy2Dir == 1)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2Y++;
            if(enemy2Y > 2)
                enemy2Dir = 2;
        } else if(enemy2Dir == 2)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2X-=2;
            if(enemy2X < 8)
                enemy2Dir = 3;
        } else if(enemy2Dir == 3)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2Y--;
            if(enemy2Y < 2)
                enemy2Dir = 4;
        } else if(enemy2Dir == 4)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2X+=2;
            if(enemy2X == 15)
                enemy2Dir = 1;
        }
        chtype enemy2 = COLOR_PAIR(2) | '!';
        mvwaddch(mapWin, enemy2Y, enemy2X, enemy2);


        chtype chT;
        chtype underline = '_';
        chtype wall = '|';
        switch(ch)
        {
            case KEY_UP:
                if(playerY > 1)
                {
                    chT = mvwinch(mapWin, playerY-1, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerY--;
                }
                break;
            case KEY_DOWN:
                if(playerY < windowY - 2)
                {
                    chT = mvwinch(mapWin, playerY+1, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerY++;
                }
                break;
            case KEY_LEFT:
                if(playerX > 1)
                {
                    chT = mvwinch(mapWin, playerY, playerX-2);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerX-=2;
                }
                break;
            case KEY_RIGHT:
                if(playerX < windowX - 3)
                {
                    chT = mvwinch(mapWin, playerY, playerX+2);
                    if(chT == wall && playerX == 33 && playerY == 1)
                    {
                        mvwprintw(mapWin, playerY, playerX, ".");
                        playerX+=2;
                    }
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerX+=2;
                }
                break;
        }


        // check if player lose
        chtype playerCh = mvwinch(mapWin, playerY, playerX);
        chtype enemyCh = COLOR_PAIR(2) | '!';
        if(playerCh == enemyCh)
            return 0;

        mvwaddch(mapWin, playerY, playerX, player);


        wrefresh(mapWin);
    } while((ch = wgetch(mapWin)) != 'q');

    if(ch == 'q')
    {
        wclear(statsWin);
        wrefresh(statsWin);
        wclear(mapWin);
        wrefresh(mapWin);
        return 2;
    }

    wrefresh(mapWin);
    wgetch(mapWin);
    timeout(-1);
    return 1;
}

int level2()
{
    int windowY = 21;
    int windowX = 42;
    WINDOW *mapWin = newwin(windowY, windowX, (yMax - windowY) / 2, (xMax - windowX) / 2);
    box(mapWin, '|', '~');

    WINDOW *statsWin = newwin(5, 38, (yMax - windowY) / 2 - 4, (xMax - windowX) / 2 + 2);
    box(statsWin, '|', '~');
    wrefresh(statsWin);

    nocbreak();
    if(language == 1)
    {
        mvwprintw(mapWin, 8, 11, "Welcome to LEVEL 2");
        mvwprintw(mapWin, 9, 11, "To win it you need to");
        mvwprintw(mapWin, 10, 11, "take six coins");
        mvwprintw(mapWin, 12, 11, "press any key to start...");
    } else if(language == 2)
    {
        mvwprintw(mapWin, 8, 8, "Vitajte na UROVNI 2");
        mvwprintw(mapWin, 9, 8, "Ak chcete vyhrať,");
        mvwprintw(mapWin, 10, 6, "musíte si vziať sesť minc");
        mvwprintw(mapWin, 12, 8, "stlacte ľubovoľnú");
        mvwprintw(mapWin, 13, 11, "klavesu...");
    } else if(language == 3)
    {
        mvwprintw(mapWin, 8, 9, "Bienvenue au NIVEAU 2"); // 24
        mvwprintw(mapWin, 9, 9, "Pour gagner, vous devez");
        mvwprintw(mapWin, 10, 9, "prendre six pièces");
        mvwprintw(mapWin, 12, 7, "appuyez sur n'importe quelle");
        mvwprintw(mapWin, 13, 8, "touche pour commencer...");
    }

    wrefresh(mapWin);
    cbreak();

    wgetch(mapWin);
    wclear(mapWin);
    clear();
    box(mapWin, '|', '~');
    halfdelay(1);

    chtype weakWall = COLOR_PAIR(5) | '|';

    for(int i = 1; i < windowY-1; ++i)
    {
        for(int j = 1; j < windowX-1; j+=2)
        {
            if(i > 6 && i < 12 && j == 15)
                mvwaddch(mapWin, i, j, '|');
            else if(i > 6 && i < 12 && j == 27)
                mvwaddch(mapWin, i, j, '|');
            else if(i > 7 && i < 11 && j == 19)
                mvwaddch(mapWin, i, j, '|');
            else if(i > 7 && i < 11 && j == 23 && i != 9)
                mvwaddch(mapWin, i, j, '|');
            else if(i == 9 && j == 23)
                mvwaddch(mapWin, i, j, weakWall);
            else if(i == 7 && j == 21)
                mvwprintw(mapWin, i, j - 1, "___");
            else if(i == 10 && j == 21)
                mvwprintw(mapWin, i, j - 1, "___");
            else
                mvwaddch(mapWin, i, j, '.');
        }
    }

    keypad(mapWin, TRUE);

    // coins
    chtype coin = COLOR_PAIR(3) | '*';
    int coinCounter = 0;
    bool coinCheck1 = 1;
    bool coinCheck2 = 1;
    bool coinCheck3 = 1;
    bool coinCheck4 = 1;
    bool coinCheck5 = 1;
    bool coinCheck6 = 1;
    mvwaddch(mapWin, 3, 17, coin);
    mvwaddch(mapWin, 9, 1, coin);
    mvwaddch(mapWin, 2, 35, coin);
    mvwaddch(mapWin, 7, 29, coin);
    mvwaddch(mapWin, 10, 13, coin);
    mvwaddch(mapWin, 8, 21, coin);

    // key
    chtype key = COLOR_PAIR(5) | 'K';
    bool isKeyPresent = 0;
    mvwaddch(mapWin, 2, 21, key);

    int ch;
    int playerY = windowY-2;
    int playerX = windowX/2;
    chtype player = COLOR_PAIR(1) | '?';

    // enemy
    chtype enemy = COLOR_PAIR(2) | '!';
    int enemyY = 5;
    int enemyX = 9;
    int enemyDir = 1;

    chtype enemyDot = COLOR_PAIR(2) | '.';

    // enemy 2
    int enemy2Y = 5;
    int enemy2X = 31;
    int enemy2Dir = 1;

    do
    {
        if(ch != ERR)
        {
            nodelay(mapWin, TRUE);
            flushinp();
            usleep(100000); // 0.1 sec
            flushinp();
            nodelay(mapWin, FALSE);
        }

        // check win
        if(coinCounter > 5)
            return 1;

        // enemy
        enemyDir = rand() % 4 + 1;
        switch(enemyDir)
        {
            case 1:
                if((mvwinch(mapWin, enemyY, enemyX + 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyX += 2;
                }
                break;
            case 2:
                if((mvwinch(mapWin, enemyY, enemyX - 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyX -= 2;
                }
                break;
            case 3:
                if((mvwinch(mapWin, enemyY + 1, enemyX) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyY++;
                }
                break;
            case 4:
                if((mvwinch(mapWin, enemyY - 1, enemyX) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyY--;
                }
                break;
        }

        enemy2Dir = rand() % 4 + 1;
        switch(enemy2Dir)
        {
            case 1:
                if((mvwinch(mapWin, enemy2Y, enemy2X + 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemy2Y, enemy2X, '.');
                    enemy2X += 2;
                }
                break;
            case 2:
                if((mvwinch(mapWin, enemy2Y, enemy2X - 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemy2Y, enemy2X, '.');
                    enemy2X -= 2;
                }
                break;
            case 3:
                if((mvwinch(mapWin, enemy2Y + 1, enemy2X) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemy2Y, enemy2X, '.');
                    enemy2Y++;
                }
                break;
            case 4:
                if((mvwinch(mapWin, enemy2Y - 1, enemy2X) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemy2Y, enemy2X, '.');
                    enemy2Y--;
                }
                break;
        }

        // key track
        if( (playerY == 2 && playerX == 21 && isKeyPresent == 0) )
        {
            isKeyPresent = 1;
            if(language == 1)
                mvwprintw(statsWin, 2, 22, "key in bag");
            else if(language == 2)
                mvwprintw(statsWin, 2, 22, "kľúc v taske");
            else if(language == 3)
            {
                mvwprintw(statsWin, 2, 24, "clé dans le");
                mvwprintw(statsWin, 3, 25, "sac");
            }
            wrefresh(statsWin);
        }

        // coin track
        if( (playerY == 3 && playerX == 17) && coinCheck1)
        {
            coinCheck1 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        } else if( (playerY == 9 && playerX == 1) && coinCheck2 )
        {
            coinCheck2 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        } else if( (playerY == 2 && playerX == 35) && coinCheck3 )
        {
            coinCheck3 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        } else if( (playerY == 7 && playerX == 29) && coinCheck4 )
        {
            coinCheck4 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        } else if( (playerY == 10 && playerX == 13) && coinCheck5 )
        {
            coinCheck5 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        } else if( (playerY == 8 && playerX == 21) && coinCheck6 )
        {
            coinCheck6 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(statsWin, 2, 2, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(statsWin, 2, 2, "mince vo vrecku: %d", coinCounter);
            else if(language == 3)
                mvwprintw(statsWin, 2, 2, "pièces dans le sac: %d", coinCounter);
            wrefresh(statsWin);
        }

        // enemy 1
        mvwaddch(mapWin, enemyY, enemyX, enemy);
        // print red dots
        if((mvwinch(mapWin, enemyY, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX + 2, enemyDot);
        if((mvwinch(mapWin, enemyY, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX + 2, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX + 2, enemyDot);


        // clear
        if((mvwinch(mapWin, enemyY - 2, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX - 2, '.');
        if((mvwinch(mapWin, enemyY - 2, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX, '.');
        if((mvwinch(mapWin, enemyY - 2, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX + 2, '.');

        if((mvwinch(mapWin, enemyY + 2, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX - 2, '.');
        if((mvwinch(mapWin, enemyY + 2, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX, '.');
        if((mvwinch(mapWin, enemyY + 2, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX + 2, '.');

        if((mvwinch(mapWin, enemyY - 1, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX - 4, '.');
        if((mvwinch(mapWin, enemyY, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX - 4, '.');
        if((mvwinch(mapWin, enemyY + 1, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX - 4, '.');

        if((mvwinch(mapWin, enemyY - 1, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX + 4, '.');
        if((mvwinch(mapWin, enemyY, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX + 4, '.');
        if((mvwinch(mapWin, enemyY + 1, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX + 4, '.');

        // if player lose
        if(mvwinch(mapWin, enemyY, enemyX + 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX) == player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX + 2)== player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX + 2) == player)
            return 0;

        // enemy 2
        mvwaddch(mapWin, enemy2Y, enemy2X, enemy);
        // print red dots
        if((mvwinch(mapWin, enemy2Y, enemy2X + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y, enemy2X + 2, enemyDot);
        if((mvwinch(mapWin, enemy2Y, enemy2X - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y, enemy2X - 2, enemyDot);
        if((mvwinch(mapWin, enemy2Y + 1, enemy2X) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 1, enemy2X, enemyDot);
        if((mvwinch(mapWin, enemy2Y - 1, enemy2X) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 1, enemy2X, enemyDot);
        if((mvwinch(mapWin, enemy2Y + 1, enemy2X + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 1, enemy2X + 2, enemyDot);
        if((mvwinch(mapWin, enemy2Y + 1, enemy2X - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 1, enemy2X - 2, enemyDot);
        if((mvwinch(mapWin, enemy2Y - 1, enemy2X - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 1, enemy2X - 2, enemyDot);
        if((mvwinch(mapWin, enemy2Y - 1, enemy2X + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 1, enemy2X + 2, enemyDot);


        // clear
        if((mvwinch(mapWin, enemy2Y - 2, enemy2X - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 2, enemy2X - 2, '.');
        if((mvwinch(mapWin, enemy2Y - 2, enemy2X) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 2, enemy2X, '.');
        if((mvwinch(mapWin, enemy2Y - 2, enemy2X + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 2, enemy2X + 2, '.');

        if((mvwinch(mapWin, enemy2Y + 2, enemy2X - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 2, enemy2X - 2, '.');
        if((mvwinch(mapWin, enemy2Y + 2, enemy2X) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 2, enemy2X, '.');
        if((mvwinch(mapWin, enemy2Y + 2, enemy2X + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 2, enemy2X + 2, '.');

        if((mvwinch(mapWin, enemy2Y - 1, enemy2X - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 1, enemy2X - 4, '.');
        if((mvwinch(mapWin, enemy2Y, enemy2X - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y, enemy2X - 4, '.');
        if((mvwinch(mapWin, enemy2Y + 1, enemy2X - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 1, enemy2X - 4, '.');

        if((mvwinch(mapWin, enemy2Y - 1, enemy2X + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y - 1, enemy2X + 4, '.');
        if((mvwinch(mapWin, enemy2Y, enemy2X + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y, enemy2X + 4, '.');
        if((mvwinch(mapWin, enemy2Y + 1, enemy2X + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemy2Y + 1, enemy2X + 4, '.');

        // if player lose
        if(mvwinch(mapWin, enemy2Y, enemy2X + 2) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y, enemy2X - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y + 1, enemy2X) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y - 1, enemy2X) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y + 1, enemy2X + 2)== player)
            return 0;
        if(mvwinch(mapWin, enemy2Y + 1, enemy2X - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y - 1, enemy2X - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemy2Y - 1, enemy2X + 2) == player)
            return 0;


        chtype chT;
        chtype underline = '_';
        chtype wall = '|';
        switch(ch)
        {
            case KEY_UP:
                if(playerY > 1)
                {
                    chT = mvwinch(mapWin, playerY-1, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerY--;
                }
                break;
            case KEY_DOWN:
                if(playerY < windowY - 2)
                {
                    chT = mvwinch(mapWin, playerY+1, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerY++;
                }
                break;
            case KEY_LEFT:
                if(playerX > 1)
                {
                    chT = mvwinch(mapWin, playerY, playerX-2);
                    if(chT == underline || chT == wall)
                        break;
                    if(playerY == 9 && playerX == 25 && isKeyPresent == 0)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerX-=2;
                }
                break;
            case KEY_RIGHT:
                if(playerX < windowX - 3)
                {
                    chT = mvwinch(mapWin, playerY, playerX+2);
                    if(chT == underline || chT == wall)
                        break;
                    mvwprintw(mapWin, playerY, playerX, ".");
                    playerX+=2;
                }
                break;
        }


        // check if player lose
        if(mvwinch(mapWin, playerY, playerX)  == enemyDot)
            return 0;

        mvwaddch(mapWin, playerY, playerX, player);


        wrefresh(mapWin);
    } while((ch = wgetch(mapWin)) != 'q');

    if(ch == 'q')
    {
        wclear(statsWin);
        wrefresh(statsWin);
        wclear(mapWin);
        wrefresh(mapWin);
        return 2;
    }

    wgetch(mapWin);
    return 1;
}

int level3()
{
    int windowY = 25;
    int windowX = 42;
    WINDOW *mapWin = newwin(windowY, windowX, (yMax - 21) / 2 - 4, (xMax - windowX) / 2);

    for(int y = 0; y < windowY; ++y)
    {
        for(int x = 0; x < windowX; ++x)
        {
            if(y == 4)
                mvwaddch(mapWin, y, x, '~');
            else if(y == windowY-1)
                mvwaddch(mapWin, y, x, '~');
            else if((y > 4 && x == 0) || (y > 4 && x == windowX - 1))
                mvwaddch(mapWin, y, x, '|');
            else if(y > 0 && y < 4 && (x == 2 || x == 39))
                mvwaddch(mapWin, y, x, '|');
            else if(y == 0 && x > 1 && x < 40)
                mvwaddch(mapWin, y, x, '~');
        }
    }

    nocbreak();
    if(language == 1)
    {
        mvwprintw(mapWin, 8, 11, "Welcome to LEVEL 3");
        mvwprintw(mapWin, 9, 11, "To win it you need to");
        mvwprintw(mapWin, 10, 11, "take ten coins");
        mvwprintw(mapWin, 12, 11, "press any key to start...");
    } else if(language == 2)
    {
        mvwprintw(mapWin, 8, 8, "Vitajte na UROVNI 3");
        mvwprintw(mapWin, 9, 8, "Ak chcete vyhrať,");
        mvwprintw(mapWin, 10, 6, "musíte si vziať desať minc");
        mvwprintw(mapWin, 12, 8, "stlacte ľubovoľnú");
        mvwprintw(mapWin, 13, 11, "klavesu...");
    } else if(language == 3)
    {
        mvwprintw(mapWin, 8, 9, "Bienvenue au NIVEAU 3"); // 24
        mvwprintw(mapWin, 9, 9, "Pour gagner, vous devez");
        mvwprintw(mapWin, 10, 9, "prendre dix pièces");
        mvwprintw(mapWin, 12, 7, "appuyez sur n'importe quelle");
        mvwprintw(mapWin, 13, 8, "touche pour commencer...");
    }

    wrefresh(mapWin);
    cbreak();

    wgetch(mapWin);
    wclear(mapWin);
    clear();
    halfdelay(1);

    // draw a map
    for(int y = 0; y < windowY; ++y)
    {
        for(int x = 0; x < windowX; x++)
        {
            if(y == 4 && x != 31)
                mvwaddch(mapWin, y, x, '_');
            else if(y == windowY-1)
                mvwaddch(mapWin, y, x, '~');
            else if((y > 4 && x == 0) || (y > 4 && x == windowX - 1))
                mvwaddch(mapWin, y, x, '|');
            else if(y > 0 && y < 4 && (x == 2 || x == 39))
                mvwaddch(mapWin, y, x, '|');
            else if(y == 0 && x > 1 && x < 40)
                mvwaddch(mapWin, y, x, '~');
            else if(y > 4 && y < 10 && x == 5)
                mvwaddch(mapWin, y, x, '|');
            else if(y == 12 && x > 4 && x < 36)
                mvwaddch(mapWin, y, x, '_');
            else if(y > 6 && y < 11 && x == 9)
                mvwaddch(mapWin, y, x, '|');
            else if(y == 21 && (x < 8 || x > 32))
                mvwaddch(mapWin, y, x, '_');
            else if( y == 7 && x > 0 && x < 6)
                mvwaddch(mapWin, y, x, '_');
            else if( y > 4 && (x % 2 == 1))
                mvwaddch(mapWin, y, x, '.');
        }
    }


    keypad(mapWin, TRUE);

    // coins
    chtype coin = COLOR_PAIR(3) | '*';
    int coinCounter = 0;
    bool coinCheck1 = 1;
    bool coinCheck2 = 1;
    bool coinCheck3 = 1;
    bool coinCheck4 = 1;
    bool coinCheck5 = 1;
    bool coinCheck6 = 1;
    bool coinCheck7 = 1;
    bool coinCheck8 = 1;
    bool coinCheck9 = 1;

    mvwaddch(mapWin, 20, 1, coin);
    mvwaddch(mapWin, 17, 19, coin);
    mvwaddch(mapWin, 15, 23, coin);
    mvwaddch(mapWin, 13, 7, coin);
    mvwaddch(mapWin, 9, 25, coin);
    mvwaddch(mapWin, 8, 13, coin);

    mvwaddch(mapWin, 16, 11, coin);
    mvwaddch(mapWin, 18, 33, coin);
    mvwaddch(mapWin, 23, 5, coin);

    mvwaddch(mapWin, 5, 3, coin);

    // three
    mvwaddch(mapWin, 7, 7, '0');
    int threeY = 7;
    int threeX = 7;

    // player
    int ch;
    int playerY = windowY - 2;
    int playerX = windowX/2;
    chtype player = COLOR_PAIR(1) | '?';

    // enemies
    chtype enemyDot = COLOR_PAIR(2) | '.';
    chtype enemy = COLOR_PAIR(2) | '!';
    int enemyY = 7;
    int enemyX = 17;
    int enemyDir = 1;

    int enemy2Y = 20;
    int enemy2X = 7;
    int enemy2Dir = 1;

    int enemy3Y = 19;
    int enemy3X = 33;
    int enemy3Dir = 1;

    int enemy4Y = 18;
    int enemy4X = 21;
    int enemy4Dir = 1;

    int enemy5Y = 17;
    int enemy5X = 21;
    int r;

    int enemy6Y = 16;
    int enemy6X = 1;
    int enemy6Dir = 1;

    int enemy7Y = 16;
    int enemy7X = 39;


    do
    {
        if(ch != ERR)
        {
            nodelay(mapWin, TRUE);
            flushinp();
            usleep(100000); // 0.1 sec
            flushinp();
            nodelay(mapWin, FALSE);
        }

        // coin track
        if(coinCheck2)
            mvwaddch(mapWin, 17, 19, coin);
        if(coinCheck7)
            mvwaddch(mapWin, 16, 11, coin);
        if(coinCheck8)
            mvwaddch(mapWin, 18, 33, coin);
        if(coinCheck9)
            mvwaddch(mapWin, 23, 5, coin);


        if(playerY == 20 && playerX == 1 && coinCheck1)
        {
            coinCheck1 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 17 && playerX == 19 && coinCheck2)
        {
            coinCheck2 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 15 && playerX == 23 && coinCheck3)
        {
            coinCheck3 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 13 && playerX == 7 && coinCheck4)
        {
            coinCheck4 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 9 && playerX == 25 && coinCheck5)
        {
            coinCheck5 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 8 && playerX == 13 && coinCheck6)
        {
            coinCheck6 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 16 && playerX == 11 && coinCheck7)
        {
            coinCheck7 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 18 && playerX == 33 && coinCheck8)
        {
            coinCheck8 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        } else if(playerY == 23 && playerX == 5 && coinCheck9)
        {
            coinCheck9 = 0;
            coinCounter++;
            if(language == 1)
                mvwprintw(mapWin, 2, 5, "coins in the bag: %d", coinCounter);
            else if(language == 2)
                mvwprintw(mapWin, 2, 5, " mince vo vrecku: %d", coinCounter);
            else if(language == 3)
            {
                mvwprintw(mapWin, 1, 11, "pièces");
                mvwprintw(mapWin, 2, 5, "     dans le sac: %d", coinCounter);
            }
        }

        // three
        mvwaddch(mapWin, threeY, threeX, '0');

        // enemy
        enemyDir = rand() % 4 + 1;
        switch(enemyDir)
        {
            case 1:
                if((mvwinch(mapWin, enemyY, enemyX + 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyX += 2;
                }
                break;
            case 2:
                if((mvwinch(mapWin, enemyY, enemyX - 2) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyX -= 2;
                }
                break;
            case 3:
                if((mvwinch(mapWin, enemyY + 1, enemyX) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyY++;
                }
                break;
            case 4:
                if((mvwinch(mapWin, enemyY - 1, enemyX) & A_CHARTEXT) == '.')
                {
                    mvwaddch(mapWin, enemyY, enemyX, '.');
                    enemyY--;
                }
                break;
        }



        // enemy 1
        mvwaddch(mapWin, enemyY, enemyX, enemy);
        // print red dots
        if((mvwinch(mapWin, enemyY, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX + 2, enemyDot);
        if((mvwinch(mapWin, enemyY, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX + 2, enemyDot);
        if((mvwinch(mapWin, enemyY + 1, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX - 2, enemyDot);
        if((mvwinch(mapWin, enemyY - 1, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX + 2, enemyDot);


        // clear
        if((mvwinch(mapWin, enemyY - 2, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX - 2, '.');
        if((mvwinch(mapWin, enemyY - 2, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX, '.');
        if((mvwinch(mapWin, enemyY - 2, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 2, enemyX + 2, '.');

        if((mvwinch(mapWin, enemyY + 2, enemyX - 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX - 2, '.');
        if((mvwinch(mapWin, enemyY + 2, enemyX) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX, '.');
        if((mvwinch(mapWin, enemyY + 2, enemyX + 2) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 2, enemyX + 2, '.');

        if((mvwinch(mapWin, enemyY - 1, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX - 4, '.');
        if((mvwinch(mapWin, enemyY, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX - 4, '.');
        if((mvwinch(mapWin, enemyY + 1, enemyX - 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX - 4, '.');

        if((mvwinch(mapWin, enemyY - 1, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY - 1, enemyX + 4, '.');
        if((mvwinch(mapWin, enemyY, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY, enemyX + 4, '.');
        if((mvwinch(mapWin, enemyY + 1, enemyX + 4) & A_CHARTEXT) == '.')
            mvwaddch(mapWin, enemyY + 1, enemyX + 4, '.');

        // if player lose
        if(mvwinch(mapWin, enemyY, enemyX + 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX) == player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX + 2)== player)
            return 0;
        if(mvwinch(mapWin, enemyY + 1, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX - 2) == player)
            return 0;
        if(mvwinch(mapWin, enemyY - 1, enemyX + 2) == player)
            return 0;

        // enemy 2
        if(enemy2Dir == 1)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2X += 2;
            if(enemy2X == 33)
                enemy2Dir = 0;
        } else if(enemy2Dir == 0)
        {
            mvwaddch(mapWin, enemy2Y, enemy2X, '.');
            enemy2X -= 2;
            if(enemy2X == 7)
                enemy2Dir = 1;
        }
        mvwaddch(mapWin, enemy2Y, enemy2X, enemy);

        // enemy 3
        if(enemy3Dir == 0)
        {
            mvwaddch(mapWin, enemy3Y, enemy3X, '.');
            enemy3X += 2;
            if(enemy3X == 39)
                enemy3Dir = 1;
        } else if(enemy3Dir == 1)
        {
            mvwaddch(mapWin, enemy3Y, enemy3X, '.');
            enemy3X -= 2;
            if(enemy3X == 1)
                enemy3Dir = 0;
        }
        mvwaddch(mapWin, enemy3Y, enemy3X, enemy);

        // enemy 4
        if(enemy4Dir == 1)
        {
            mvwaddch(mapWin, enemy4Y, enemy4X, '.');
            enemy4X += 2;
            if(enemy4X == 27)
                enemy4Dir = 0;
        } else if(enemy4Dir == 0)
        {
            mvwaddch(mapWin, enemy4Y, enemy4X, '.');
            enemy4X -= 2;
            if(enemy4X == 13)
                enemy4Dir = 1;
        }
        mvwaddch(mapWin, enemy4Y, enemy4X, enemy);

        // enemy 5
        r = rand() % 2;
        if(r)
        {
            mvwaddch(mapWin, enemy5Y, enemy5X, '.');
            if(enemy5X < 39)
                enemy5X += 2;
        } else
        {
            mvwaddch(mapWin, enemy5Y, enemy5X, '.');
            if(enemy5X > 1)
                enemy5X -= 2;
        }
        mvwaddch(mapWin, enemy5Y, enemy5X, enemy);

        // enemy 6 and 7
        if(enemy6Dir == 1)
        {
            mvwaddch(mapWin, enemy6Y, enemy6X, '.');
            mvwaddch(mapWin, enemy7Y, enemy7X, '.');
            enemy6X += 2;
            enemy7X -= 2;
            if(enemy6X == 21)
                enemy6Dir = 0;
        } else
        {
            mvwaddch(mapWin, enemy6Y, enemy6X, '.');
            mvwaddch(mapWin, enemy7Y, enemy7X, '.');
            enemy6X -= 2;
            enemy7X += 2;
            if(enemy6X == 1)
                enemy6Dir = 1;
        }
        mvwaddch(mapWin, enemy6Y, enemy6X, enemy);
        mvwaddch(mapWin, enemy7Y, enemy7X, enemy);


        chtype chT;
        chtype chT2;
        chtype underline = '_';
        chtype wall = '|';
        switch(ch)
        {
            case KEY_UP:
                if(playerY > 1)
                {
                    chT = mvwinch(mapWin, playerY-1, playerX);
                    chT2 = mvwinch(mapWin, playerY-2, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    if(chT == 'c' || chT == 'i' || chT == 's' || chT == 'h' || chT == 'a' || chT == ':')
                        break;
                    if(chT == 'k' || chT == 'e' || chT == 'v' || chT == 'o' || chT == 'l' || chT == 'p')
                        break;
                    if(chT == '1' || chT == '2' || chT == '3' || chT == '4' || chT == '5')
                        break;
                    if(chT == '6' || chT == '7' || chT == '8' || chT == '9')
                        break;
                    if(chT == '0' && (chT2 == '.' || chT2 == ' '))
                        threeY--;
                    else if(chT == '0')
                        break;
                    if(playerY > 4)
                        mvwaddch(mapWin, playerY, playerX, '.');
                    else
                        mvwaddch(mapWin, playerY, playerX, ' ');
                    playerY--;
                }
                break;
            case KEY_DOWN:
                if(playerY < windowY - 2)
                {
                    chT = mvwinch(mapWin, playerY+1, playerX);
                    chT2 = mvwinch(mapWin, playerY+2, playerX);
                    if(chT == underline || chT == wall)
                        break;
                    if(chT == 'c' || chT == 'i' || chT == 's' || chT == 'h' || chT == 'a' || chT == ':')
                        break;
                    if(chT == 'k' || chT == 'e' || chT == 'v' || chT == 'o' || chT == 'l' || chT == 'p')
                        break;
                    if(chT == '1' || chT == '2' || chT == '3' || chT == '4' || chT == '5')
                        break;
                    if(chT == '6' || chT == '7' || chT == '8' || chT == '9')
                        break;
                    if(chT == '0' && (chT2 == '.' || chT2 == ' '))
                        threeY++;
                    else if(chT == '0')
                        break;
                    if(playerY > 4)
                        mvwaddch(mapWin, playerY, playerX, '.');
                    else
                        mvwaddch(mapWin, playerY, playerX, ' ');
                    playerY++;
                }
                break;
            case KEY_LEFT:
                if(playerX > 1)
                {
                    chT = mvwinch(mapWin, playerY, playerX-2);
                    chT2 = mvwinch(mapWin, playerY, playerX-4);
                    chtype chT3 = mvwinch(mapWin, playerY, playerX-1);
                    if(chT == underline || chT == wall || chT3 == wall)
                        break;
                    if(chT == 'c' || chT == 'i' || chT == 's' || chT == 'h' || chT == 'a' || chT == ':')
                        break;
                    if(chT == 'k' || chT == 'e' || chT == 'v' || chT == 'o' || chT == 'l' || chT == 'p')
                        break;
                    if(chT == '1' || chT == '2' || chT == '3' || chT == '4' || chT == '5')
                        break;
                    if(chT == '6' || chT == '7' || chT == '8' || chT == '9')
                        break;
                    if(chT == '0' && (chT2 == '.' || chT2 == ' '))
                        threeX-=2;
                    else if(chT == '0')
                        break;
                    if(playerY > 4)
                        mvwaddch(mapWin, playerY, playerX, '.');
                    else
                        mvwaddch(mapWin, playerY, playerX, ' ');
                    playerX-=2;
                }
                break;
            case KEY_RIGHT:
                if(playerX < windowX - 3)
                {
                    chT = mvwinch(mapWin, playerY, playerX+2);
                    chT2 = mvwinch(mapWin, playerY, playerX+4);
                    if(chT == underline || chT == wall)
                        break;
                    if(chT == 'c' || chT == 'i' || chT == 's' || chT == 'h' || chT == 'a' || chT == ':')
                        break;
                    if(chT == 'k' || chT == 'e' || chT == 'v' || chT == 'o' || chT == 'l' || chT == 'p')
                        break;
                    if(chT == '1' || chT == '2' || chT == '3' || chT == '4' || chT == '5')
                        break;
                    if(chT == '6' || chT == '7' || chT == '8' || chT == '9')
                        break;
                    if(chT == '0' && (chT2 == '.' || chT2 == ' '))
                        threeX+=2;
                    else if(chT == '0')
                        break;
                    if(playerY > 4)
                        mvwaddch(mapWin, playerY, playerX, '.');
                    else
                        mvwaddch(mapWin, playerY, playerX, ' ');
                    playerX+=2;
                }
                break;
        }

        // check if player lose
        if(mvwinch(mapWin, playerY, playerX)  == enemyDot)
            return 0;

        // check if player lose by enemy
        if(mvwinch(mapWin, playerY, playerX) == enemy)
            return 0;

        // check win
        if(threeY == 2 && threeX == 25 && coinCounter > 0)
            return 1;

        mvwaddch(mapWin, playerY, playerX, player);

        mvwaddch(mapWin, threeY, threeX, '0');

        wrefresh(mapWin);
    } while((ch = wgetch(mapWin)) != 'q');


    wclear(mapWin);
    wrefresh(mapWin);
    return 2;

}

void game() // question mark
{
    srand(time(NULL));
    initscr();
    start_color();
    noecho();
    cbreak();
    curs_set(0);
    getmaxyx(stdscr, yMax, xMax);
    keypad(stdscr, TRUE);


    init_pair(1, COLOR_BLUE, COLOR_YELLOW);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    int numOfLevels = 4; // 4 because of "back" button
    bool levelsDone[numOfLevels];
    for(int i = 0; i < numOfLevels; ++i)
        levelsDone[i] = 0;

    menu:

    char *choices[30];
    if(language == 1)
    {
        choices[0] = "Play";
        choices[1] = "Language";
        choices[2] = "Exit";
    }
    else if(language == 2)
    {
        choices[0] = "Hrať";
        choices[1] = "Jazyk";
        choices[2] = "Ukonciť";
    }
    else if(language == 3)
    {
        choices[0] = "Jouer";
        choices[1] = "Langue";
        choices[2] = "Sortie";
    }
    bool *empty = NULL;

    char *levelChoices[30];
    if(language == 1)
    {
        levelChoices[0] = "LEVEL 1";
        levelChoices[1] = "LEVEL 2";
        levelChoices[2] = "LEVEL 3";
        levelChoices[3] = "back";
    }
    else if(language == 2)
    {
        levelChoices[0] = "UROVEN 1";
        levelChoices[1] = "UROVEN 2";
        levelChoices[2] = "UROVEN 3";
        levelChoices[3] = "spat";
    }
    else if(language == 3)
    {
        levelChoices[0] = "NIVEAU 1";
        levelChoices[1] = "NIVEAU 2";
        levelChoices[2] = "NIVEAU 3";
        levelChoices[3] = "retour";
    }

    char *languages[30];
    if(language == 1)
    {
        languages[0] = "English";
        languages[1] = "Slovak";
        languages[2] = "French";
    }
    else if(language == 2)
    {
        languages[0] = "anglictina";
        languages[1] = "slovencina";
        languages[2] = "francuzstina";
    }
    else if(language == 3)
    {
        languages[0] = "anglais";
        languages[1] = "slovaque";
        languages[2] = "français";
    }
    int numOfLanguages = 3;


    int menuChoice = menu(choices, 3, 0, empty);

    if(menuChoice == 2) // exit
    {
        endwin();
        return;
    }

    if(menuChoice == 0) // play
    {
        int levelMenuChoice = menu(levelChoices, numOfLevels, 1, levelsDone);

        if(levelMenuChoice == 0) // level 1
        {
            int isVictory = level1();
            if(isVictory == 1)
            {
                attron(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU WON");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "VYHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS AVEZ GAGNE");
                attroff(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                levelsDone[0] = 1; // mark that level 1 done
                clear();
                refresh();
                goto menu;
            } else if(isVictory == 0)
            {
                attron(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU LOSE");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "PREHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS PERDEZ");
                attroff(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                clear();
                refresh();
                goto menu;
            } else if(isVictory == 2)
                goto menu;
        } else if(levelMenuChoice == 1) // LEVEL 2
        {
            int isVictory = level2();
            if(isVictory == 1)
            {
                attron(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU WON");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "VYHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS AVEZ GAGNE");
                attroff(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                levelsDone[1] = 1; // mark that level 2 done
                clear();
                refresh();
                goto menu;
            } else if(isVictory == 0)
            {
                attron(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU LOSE");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "PREHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS PERDEZ");
                attroff(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                clear();
                refresh();
                goto menu;
            } else if(isVictory == 2)
                goto menu;
        } else if(levelMenuChoice == 2) // LEVEL 3
        {
            int isVictory = level3();
            if(isVictory == 1)
            {
                attron(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU WON");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "VYHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS AVEZ GAGNE");
                attroff(COLOR_PAIR(4));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                clear();
                refresh();
                levelsDone[2] = 1; // mark that level 2 done
                goto menu;
            } else if(isVictory == 0)
            {
                attron(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax/2, xMax/2 - 4, "YOU LOSE");
                else if(language == 2)
                    mvprintw(yMax/2, xMax/2 - 4, "PREHRALI STE");
                else if(language == 3)
                    mvprintw(yMax/2, xMax/2 - 4, "VOUS PERDEZ");
                attroff(COLOR_PAIR(2));
                if(language == 1)
                    mvprintw(yMax-2, xMax/2 - 8, "(wait 3 seconds)");
                else if(language == 2)
                    mvprintw(yMax-2, xMax/2 - 8, "(pockajte 3 sekundy)");
                else if(language == 3)
                    mvprintw(yMax-2, xMax/2 - 8, "(attendre 3 secondes)");
                refresh();
                usleep(3000000);
                flushinp();
                clear();
                refresh();
                goto menu;
            } else if(isVictory == 2)
                goto menu;
        } else // back
            goto menu;
    } else if(menuChoice == 1) // language
    {
        int languageChoice = menu(languages, numOfLanguages, 0, empty);
        if(languageChoice == 0)
            language = 1;
        else if(languageChoice == 1)
            language = 2;
        else if(languageChoice == 2)
            language = 3;
        goto menu;
    }

    getch();
    endwin();
}

int main()
{
    game();
    return 0;
}