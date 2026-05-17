#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Color

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int color)
{
    SetConsoleTextAttribute(h, color);
}

// CURSOR POSITION

void gotoXY(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(h, coord);
}


// HIDE CURSOR


void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor;

    cursor.dwSize = 100;
    cursor.bVisible = false;

    SetConsoleCursorInfo(h, &cursor);
}


// ASCII OBJECTS


const char* bikeArt[5] =
{
    "  _______  ",
    " /       \\ ",
    "|  () ()  |",
    " \\___|___/ ",
    " (o)---(o) "
};

const char* carArt[3] =
{
    " .------. ",
    " | >  < | ",
    " `OO--OO' "
};

const char* barrierArt[3] =
{
    "|==========|",
    "| !STOP!!! |",
    "|==========|"
};

const char* rockArt[3] =
{
    "  /\\ /\\  ",
    " / ROCK \\ ",
    "/________\\"
};


// BASE GAME CLASS


class Game
{
protected:
    string title;

public:

    Game(string t = "Turbo Rider")
    {
        title = t;
    }

    virtual void play() = 0;
};


// PLAYER CLASS


class Player
{
private:

    string username;
    string password;

    int coins;
    int bestScore;
    int lives;

public:

    Player()
    {
        coins = 1000;
        bestScore = 0;
        lives = 3;
    }


    // SETTERS


    void setUsername(string u)
    {
        username = u;
    }

    void setPassword(string p)
    {
        password = p;
    }

    void setCoins(int c)
    {
        coins = c;
    }

    void setBestScore(int s)
    {
        bestScore = s;
    }

    void setLives(int l)
    {
        lives = l;
    }


    // GETTERS


    string getUsername()
    {
        return username;
    }

    string getPassword()
    {
        return password;
    }

    int getCoins()
    {
        return coins;
    }

    int getBestScore()
    {
        return bestScore;
    }

    int getLives()
    {
        return lives;
    }


    // PROFILE DISPLAY


    void displayProfile()
    {
        setColor(11);

        cout << "\n=====================================";
        cout << "\n            PROFILE";
        cout << "\n=====================================\n";

        cout << "\nUsername   : " << username;
        cout << "\nCoins      : " << coins;
        cout << "\nBest Score : " << bestScore << " sec";
        cout << "\nLives      : " << lives << endl;
    }
};


// OBSTACLE CLASS


class Obstacle
{
public:

    int lane;
    int y;
    int type;

    Obstacle()
    {
        lane = rand() % 3;
        y = 5;
        type = rand() % 3;
    }
};


// BIKE GAME CLASS


class BikeGame : public Game
{
private:

    Player currentPlayer;

    int bikeLane;

    int speed;

    int score;

    vector<Obstacle> obstacles;

    // PREVIOUS VALUES FOR FLICKER-FREE UI
    int prevScore;
    int prevCoins;
    int prevLives;
    int prevSpeed;

public:

    BikeGame(Player p)
    {
        currentPlayer = p;

        bikeLane = 1;

        speed = 120;

        score = 0;

        prevScore = -1;
        prevCoins = -1;
        prevLives = -1;
        prevSpeed = -1;

        prevScore = -1;
        prevCoins = -1;
        prevLives = -1;
        prevSpeed = -1;
    }


    // RETURN UPDATED PLAYER

    Player getUpdatedPlayer()
    {
        return currentPlayer;
    }



    // DRAW ROAD
  


    void drawRoad()
    {
        setColor(7);

        for (int i = 0; i < 25; i++)
        {
            gotoXY(0,  i + 5);
            cout << "|";

            gotoXY(26, i + 5);
            cout << "|";

            gotoXY(52, i + 5);
            cout << "|";

            gotoXY(78, i + 5);
            cout << "|";
        }
    }


    // DRAW UI


    void drawUI()
    {
        // BORDERS — only draw once (they never change)
        if (prevScore == -1)
        {
            setColor(11);
            gotoXY(0, 0);
            cout << "===============================================================================";
            gotoXY(0, 3);
            cout << "===============================================================================";

            // Static labels that never change
            gotoXY(0, 1);
            cout << "Player : " << currentPlayer.getUsername();

            gotoXY(55, 2);
            setColor(11);
            cout << "Controls : A/D  Q=Quit";
        }

        // SCORE — only redraw if changed
        if (score != prevScore)
        {
            setColor(11);
            gotoXY(30, 1);
            cout << "Score : " << score << " sec   ";
            prevScore = score;
        }

        // COINS — only redraw if changed
        if (currentPlayer.getCoins() != prevCoins)
        {
            setColor(11);
            gotoXY(55, 1);
            cout << "Coins : " << currentPlayer.getCoins() << "    ";
            prevCoins = currentPlayer.getCoins();
        }

        // LIVES — only redraw if changed
        if (currentPlayer.getLives() != prevLives)
        {
            setColor(12);
            gotoXY(0, 2);
            cout << "Lives : " << currentPlayer.getLives();
            prevLives = currentPlayer.getLives();
        }

        // LEVEL — only redraw if speed changed
        if (speed != prevSpeed)
        {
            setColor(10);
            gotoXY(30, 2);

            if (speed <= 80)
            {
                cout << "Level : HARD  ";
            }

            else if (speed <= 100)
            {
                cout << "Level : MEDIUM";
            }

            else
            {
                cout << "Level : EASY  ";
            }

            prevSpeed = speed;
        }
    }


    // ERASE BIKE (clear all 3 possible lane positions to remove ghost)


    void eraseBike()
    {
        int laneX[3] = {2, 28, 54};

        for (int lane = 0; lane < 3; lane++)
        {
            for (int i = 0; i < 5; i++)
            {
                clearRow(laneX[lane], 24 + i, 24);
            }
        }
    }


    // DRAW BIKE


    void drawBike()
    {
        int x;

        if (bikeLane == 0)
        {
            x = 2;
        }

        else if (bikeLane == 1)
        {
            x = 28;
        }

        else
        {
            x = 54;
        }

        int y = 24;

        setColor(14);

        for (int i = 0; i < 5; i++)
        {
            gotoXY(x, y + i);

            cout << bikeArt[i];
        }
    }

  
    // DRAW OBSTACLE


    void drawObstacle(Obstacle& o)
    {
        int x;

        if (o.lane == 0)
        {
            x = 2;
        }

        else if (o.lane == 1)
        {
            x = 28;
        }

        else
        {
            x = 54;
        }

      
        // CAR
     

        if (o.type == 0)
        {
            setColor(12);

            for (int i = 0; i < 3; i++)
            {
                gotoXY(x, o.y + i);

                cout << carArt[i];
            }
        }

        // BARRIER
      

        else if (o.type == 1)
        {
            setColor(14);

            for (int i = 0; i < 3; i++)
            {
                gotoXY(x, o.y + i);

                cout << barrierArt[i];
            }
        }


        // ROCK


        else
        {
            setColor(8);

            for (int i = 0; i < 3; i++)
            {
                gotoXY(x, o.y + i);

                cout << rockArt[i];
            }
        }
    }

    
    // HANDLE INPUT
  
    void handleInput()
    {
        if (_kbhit())
        {
            char ch = _getch();

            
            // MOVE LEFT
            

            if ((ch == 'a' || ch == 'A') &&
                bikeLane > 0)
            {
                bikeLane--;
            }

            
            // MOVE RIGHT
            

            else if ((ch == 'd' || ch == 'D') &&
                     bikeLane < 2)
            {
                bikeLane++;
            }

            
            // QUIT GAME
            

            else if (ch == 'q' || ch == 'Q')
            {
                currentPlayer.setLives(0);
            }
        }
    }

    
    // COLLISION DETECTION
    

    bool checkCollision(Obstacle& o)
    {
        if (o.y >= 20 &&
            o.y <= 24)
        {
            if (o.lane == bikeLane)
            {
                return true;
            }
        }

        return false;
    }

    
    // UPDATE BEST SCORE
    

    void updateBestScore()
    {
        if (score >
            currentPlayer.getBestScore())
        {
            currentPlayer.setBestScore(score);
        }
    }

    
    // GAME OVER MENU
    

    bool gameOverMenu()
    {
        system("cls");

        setColor(12);

        cout << "\n\n\t====================================";
        cout << "\n\t            GAME OVER";
        cout << "\n\t====================================\n";

        setColor(11);

        cout << "\n\tYour Score : "
             << score << " sec";

        cout << "\n\tBest Score : "
             << currentPlayer.getBestScore()
             << " sec";

        cout << "\n\tCoins : "
             << currentPlayer.getCoins();

        cout << "\n\n\t1. Buy Extra Life (550 Coins)";
        cout << "\n\t2. Return To Menu";

        cout << "\n\n\tChoice : ";

        char choice;
        cin >> choice;

        
        // BUY LIFE
        

        if (choice == '1')
        {
            if (currentPlayer.getCoins() >= 550)
            {
                currentPlayer.setCoins(
                    currentPlayer.getCoins() - 550);

                currentPlayer.setLives(1);

                return true;
            }

            else
            {
                setColor(12);

                cout << "\n\nNot enough coins!";

                Sleep(1500);
            }
        }

        return false;
    }

    
    // CLEAR A ROW ON SCREEN (used to erase old obstacle art)


    void clearRow(int x, int y, int width)
    {
        gotoXY(x, y);

        for (int i = 0; i < width; i++)
        {
            cout << " ";
        }
    }


    // ERASE OBSTACLE FROM SCREEN (overwrite with spaces)


    void eraseObstacle(Obstacle& o)
    {
        int x;

        if (o.lane == 0)
        {
            x = 2;
        }

        else if (o.lane == 1)
        {
            x = 28;
        }

        else
        {
            x = 54;
        }

        for (int i = 0; i < 3; i++)
        {
            clearRow(x, o.y + i, 16);
        }
    }


    // PLAY GAME


    void play()
    {
        system("cls");

        time_t startTime = time(0);

        int rewardTimer = 0;

        currentPlayer.setLives(3);

        obstacles.clear();

        bikeLane = 1;

        speed = 120;

        score = 0;

        prevScore = -1;
        prevCoins = -1;
        prevLives = -1;
        prevSpeed = -1;

        prevScore = -1;
        prevCoins = -1;
        prevLives = -1;
        prevSpeed = -1;


        // DRAW STATIC ELEMENTS ONCE


        drawRoad();


        // MAIN GAME LOOP


        while (true)
        {

            // SCORE SYSTEM


            score = (int)(time(0) - startTime);


            // DRAW UI (repositions cursor, no cls)


            drawUI();


            // DRAW ROAD BORDERS


            drawRoad();


            // ERASE OLD BIKE POSITION (prevents ghost on lane change)


            eraseBike();


            // DRAW BIKE


            drawBike();


            // INPUT


            handleInput();


            // REWARD SYSTEM


            if (score != 0 &&
                score % 30 == 0)
            {
                if (rewardTimer != score)
                {
                    currentPlayer.setCoins(
                        currentPlayer.getCoins() + 50);

                    rewardTimer = score;
                }
            }


            // GENERATE OBSTACLES (spawn rate scales with score)


            int spawnChance;

            if (score < 60)
            {
                spawnChance = 30;
            }

            else if (score < 90)
            {
                spawnChance = 22;
            }

            else if (score < 120)
            {
                spawnChance = 16;
            }

            else if (score < 150)
            {
                spawnChance = 12;
            }

            else
            {
                spawnChance = 9;
            }

            if (rand() % spawnChance == 0)
            {
                obstacles.push_back(Obstacle());
            }


            // MOVE AND DRAW OBSTACLES


            for (int i = 0; i < (int)obstacles.size(); i++)
            {
                // ERASE OLD POSITION BEFORE MOVING


                eraseObstacle(obstacles[i]);


                // MOVE DOWN


                obstacles[i].y++;


                // REMOVE OFFSCREEN OBSTACLES


                if (obstacles[i].y > 29)
                {
                    obstacles.erase(obstacles.begin() + i);

                    i--;

                    continue;
                }


                // COLLISION CHECK


                if (checkCollision(obstacles[i]))
                {
                    eraseObstacle(obstacles[i]);

                    obstacles.erase(obstacles.begin() + i);

                    i--;

                    currentPlayer.setLives(
                        currentPlayer.getLives() - 1);

                    Beep(700, 200);


                    // GAME OVER


                    if (currentPlayer.getLives() <= 0)
                    {
                        updateBestScore();

                        bool continueGame = gameOverMenu();


                        // BUY EXTRA LIFE - CONTINUE


                        if (continueGame)
                        {
                            startTime = time(0) - score;

                            obstacles.clear();

                            prevScore = -1;
                            prevCoins = -1;
                            prevLives = -1;
                            prevSpeed = -1;

                            system("cls");

                            drawRoad();

                            continue;
                        }


                        // EXIT GAME


                        else
                        {
                            return;
                        }
                    }

                    continue;
                }


                // DRAW OBSTACLE AT NEW POSITION


                drawObstacle(obstacles[i]);
            }


            // DIFFICULTY SCALING
            // EASY: 0-60s, MEDIUM: 60-90s, HARD: 90s+
            // Every 30s after 90s speed increases further


            if (score > 60 && speed > 100)
            {
                speed = 100;
            }

            if (score > 90 && speed > 80)
            {
                speed = 80;
            }

            if (score > 120 && speed > 65)
            {
                speed = 65;
            }

            if (score > 150 && speed > 55)
            {
                speed = 55;
            }

            if (score > 180 && speed > 45)
            {
                speed = 45;
            }


            // CHECK QUIT (Q key sets lives to 0)


            if (currentPlayer.getLives() <= 0)
            {
                updateBestScore();

                bool continueGame = gameOverMenu();

                if (continueGame)
                {
                    startTime = time(0) - score;

                    obstacles.clear();

                    prevScore = -1;
                    prevCoins = -1;
                    prevLives = -1;
                    prevSpeed = -1;

                    system("cls");

                    drawRoad();
                }

                else
                {
                    return;
                }
            }


            // FRAME DELAY


            Sleep(speed);
        }
    }
};

// GLOBAL USERS VECTOR

vector<Player> users;

// LOAD USERS

void loadUsers()
{
    ifstream file("users.txt");

    if (!file)
    {
        return;
    }

    string username;
    string password;

    int coins;
    int bestScore;
    int lives;

    while (getline(file, username, '|')
           && getline(file, password, '|')
           && (file >> coins >> bestScore >> lives)
           && file.ignore())
    {
        Player p;

        p.setUsername(username);
        p.setPassword(password);

        p.setCoins(coins);
        p.setBestScore(bestScore);
        p.setLives(lives);

        users.push_back(p);
    }

    file.close();
}

// SAVE USERS

void saveUsers()
{
    ofstream file("users.txt");

    for (int i = 0;
         i < users.size();
         i++)
    {
        file << users[i].getUsername() << "|";

        file << users[i].getPassword() << "|";

        file << users[i].getCoins() << " ";

        file << users[i].getBestScore() << " ";

        file << users[i].getLives();

        file << endl;
    }

    file.close();
}

// REGISTER USER

void registerUser()
{
    system("cls");

    Player p;

    string username;
    string password;

    setColor(11);

    cout << "=====================================\n";
    cout << "             REGISTER";
    cout << "\n=====================================\n";

    cout << "\nEnter Username : ";
    cin.ignore(1000, '\n');
    getline(cin, username);

    // CHECK EXISTING USER

    for (int i = 0;
         i < users.size();
         i++)
    {
        if (users[i].getUsername() ==
            username)
        {
            setColor(12);

            cout << "\nUsername already exists!";

            Sleep(1500);

            return;
        }
    }

    cout << "Enter Password : ";
    getline(cin, password);

    p.setUsername(username);
    p.setPassword(password);

    users.push_back(p);

    saveUsers();

    setColor(10);

    cout << "\nRegistration Successful!";

    Sleep(1500);
}

// LOGIN USER

int loginUser()
{
    system("cls");

    string username;
    string password;

    setColor(11);

    cout << "=====================================\n";
    cout << "               LOGIN";
    cout << "\n=====================================\n";

    cout << "\nEnter Username : ";
    cin.ignore(1000, '\n');
    getline(cin, username);

    cout << "Enter Password : ";
    getline(cin, password);

    // SEARCH USER

    for (int i = 0;
         i < users.size();
         i++)
    {
        if (users[i].getUsername() ==
            username
            &&
            users[i].getPassword() ==
            password)
        {
            setColor(10);

            cout << "\nLogin Successful!";

            Sleep(1000);

            return i;
        }
    }

    setColor(12);

    cout << "\nInvalid Username or Password!";

    Sleep(1500);

    return -1;
}

// LEADERBOARD

void leaderboard()
{
    system("cls");

    setColor(14);

    cout << "=====================================\n";
    cout << "            LEADERBOARD";
    cout << "\n=====================================\n\n";

    // DISPLAY USERS

    for (int i = 0;
         i < users.size();
         i++)
    {
        cout << i + 1 << ". ";

        cout << setw(15)
             << left
             << users[i].getUsername();

        cout << users[i].getBestScore()
             << " sec";

        cout << endl;
    }

    cout << "\n\nPress any key to continue...";

    _getch();
}

// USER MENU

void userMenu(int index)
{
    while (true)
    {
        system("cls");

        setColor(11);

        cout << "=====================================\n";

        cout << "Welcome, "
             << users[index].getUsername();

        cout << "\n=====================================\n\n";

        cout << "1. Start Game\n";
        cout << "2. View Profile\n";
        cout << "3. Logout\n\n";

        cout << "Coins      : "
             << users[index].getCoins();

        cout << "\nBest Score : "
             << users[index].getBestScore()
             << " sec\n";

        cout << "\nChoice : ";

        char choice;
        cin >> choice;

        // START GAME

        if (choice == '1')
        {
            BikeGame game(users[index]);

            game.play();

            users[index] =
                game.getUpdatedPlayer();

            saveUsers();
        }

        // PROFILE

        else if (choice == '2')
        {
            system("cls");

            users[index].displayProfile();

            cout << "\n\nPress any key to continue...";

            _getch();
        }

        // LOGOUT

        else if (choice == '3')
        {
            break;
        }
    }
}

// MAIN FUNCTION

int main()
{
    // CONSOLE SETUP

    SetConsoleTitleA("Turbo Rider");

    srand(time(0));

    hideCursor();

    loadUsers();

    // MAIN MENU LOOP

    while (true)
    {
        system("cls");

        setColor(11);

        cout << "=====================================\n";
        cout << "           TURBO RIDER";
        cout << "\n=====================================\n\n";

        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Leaderboard\n";
        cout << "4. Exit\n\n";

        cout << "Choice : ";

        char choice;
        cin >> choice;

        // LOGIN

        if (choice == '1')
        {
            int index = loginUser();

            if (index != -1)
            {
                userMenu(index);
            }
        }

        // REGISTER

        else if (choice == '2')
        {
            registerUser();
        }

        // LEADERBOARD

        else if (choice == '3')
        {
            leaderboard();
        }

        // EXIT

        else if (choice == '4')
        {
            break;
        }
    }

    return 0;
}