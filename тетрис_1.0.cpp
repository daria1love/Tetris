#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;
const int L = 3;
int field[M][N] = { 0 };
int w = 34;
int Counter = 0;
int Counter_D = 0;
bool isFalling;

struct Point
{
    int x, y;
}a[4], b[4];

struct MenuInfo
{
    RenderWindow Window;
    Sprite Background;
    vector<int> Records;
};

int figures[7][4] = {
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5,
};

bool check() {
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;
        else if (field[a[i].y][a[i].x] && a[i].y == M)
            return 0;

    return 1;
}

void statMenu(RenderWindow& window, MenuInfo& menuInfo, bool& stopFlag) {
    Texture menuTexture4;
    menuTexture4.loadFromFile("Button4.png");
    Sprite menu4(menuTexture4);
    menu4.setScale(Vector2f(0.5, 0.5));
    menu4.setPosition(20, 20);
    bool isMenu = true;
    Font font;
    font.loadFromFile("20db.otf");
    int l_y = 250;
    int l_x = 50;
    window.draw(menuInfo.Background);
    for (int i = 0; i < 3; i++) {
        Text recordText;

        recordText.setFont(font);
        recordText.setFillColor(Color::Color(255, 205, 115));
        recordText.setCharacterSize(30);
        recordText.setString(to_string(i + 1) + " PLACE " + to_string(menuInfo.Records[i]));
        recordText.setPosition(l_x, l_y);
        l_y += 60;
        window.draw(recordText);
    }
    window.draw(menu4);
    window.display();
    Event ev;
    while (isMenu&& window.isOpen()) {
        if (Mouse::isButtonPressed(Mouse::Left)) {

            if (IntRect(menu4.getPosition().x, menu4.getPosition().y, menu4.getTexture()->getSize().x / 2.0f, menu4.getTexture()->getSize().y / 2.0f).contains(Mouse::getPosition(window))) {
                isMenu = false;

            }

        }
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
                stopFlag = true;
            }
        }
    }
}

void menu(RenderWindow & window, MenuInfo& info, bool& stopFlag) {
    Texture menuTexture1, menuTexture2;
    Music music;
    music.openFromFile("music.ogg");
    menuTexture1.loadFromFile("Button.png");
    menuTexture2.loadFromFile("Button2.png");
    Sprite playButton(menuTexture1), menu2(menuTexture2);
    playButton.setScale(Vector2f(0.5, 0.5));
    menu2.setScale(Vector2f(0.5, 0.5));

    bool isMenu = 1;
    bool isSetting = 0;
    int numMenu = 0; 
        playButton.setPosition(50, 340 - playButton.getTexture()->getSize().y * playButton.getScale().y * 0.5);
        menu2.setPosition(250 - menu2.getTexture()->getSize().x * menu2.getScale().x* 0.5, 340 - menu2.getTexture()->getSize().y * menu2.getScale().y * 0.5);
        int counter = 0;
        if (numMenu == 0) {
            window.draw(info.Background);
            window.draw(playButton);
            window.draw(menu2);
        }
        music.play();
        window.display();
        while( isMenu && window.isOpen()) {
           
            Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                {
                    stopFlag = true;
                    window.close();
                }
                if (Event::MouseButtonPressed){

                    if (IntRect(playButton.getPosition().x, playButton.getPosition().y, playButton.getTexture()->getSize().x / 2.0f, playButton.getTexture()->getSize().y / 2.0f).contains(Mouse::getPosition(window))) { numMenu = 1; }
                    if (IntRect(menu2.getPosition().x, menu2.getPosition().y, menu2.getTexture()->getSize().x / 2.0f, menu2.getTexture()->getSize().y / 2.0f).contains(Mouse::getPosition(window))) { numMenu = 2; }

                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        if (numMenu == 1) isMenu = false;
                        if (numMenu == 2) {
                            statMenu(window, info, stopFlag);
                            if (stopFlag) return;
                            window.clear(Color::White);
                            window.draw(info.Background);
                            window.draw(playButton);
                            window.draw(menu2);
                            window.display();
                            numMenu = 0;
                            
                        }
                    }

                }
            

            }
        }
}



bool StartGame() {
    bool stopFlag = false;
    string l;
    RenderWindow window(VideoMode(N * w, M * w), "Tetris!");
    MenuInfo Info;
    srand(time(0));
    Texture menuBackground;
    menuBackground.loadFromFile("BG.jpg");
    Sprite menuBG(menuBackground);
    menuBG.setScale(Vector2f(0.7, 0.7));
    menuBG.setPosition(0, 0);
    Info.Background = menuBG;
    ifstream fout_ou("results.txt");
    if (fout_ou.is_open()) {
        while (getline(fout_ou, l)) {
            Info.Records.push_back(stoi(l));
        }
    }
    fout_ou.close();
    sort(Info.Records.begin(), Info.Records.end());
    reverse(Info.Records.begin(), Info.Records.end());

    menu(window, Info, stopFlag);
    if (stopFlag) return false;
    Texture t, Background2, GO, Put;
    Background2.loadFromFile("BG2.jpg");
    t.loadFromFile("tiles.png");
    GO.loadFromFile("gameover.png");
    Put.loadFromFile("Putin.png");
    Sprite tiles(t), BG(Background2), Game_Over(GO), Putin(Put);
    int dx = 0, colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;
    bool ad = true;
    SoundBuffer buffer;
    Sound click;
    buffer.loadFromFile("clear.ogg");
    click.setBuffer(buffer);
    BG.setPosition(0, 0);
    Music music2;
    music2.openFromFile("gameov.ogg");
    Music sound;
    sound.openFromFile("sound.ogg");
    sound.play();
    ofstream fout_in("results.txt", ios_base::app);
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Right)
                    dx = 1;
                else if (event.key.code == Keyboard::Left)
                    dx = -1;
        }


        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }

        if (!check()) {
            for (int i = 0; i < 4; i++)
                a[i] = b[i];
        }

        if (rotate) {
            Point p = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;

                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }

            if (!check()) {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }

        }

        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check()) {
                if (!isFalling)
                {
                    if (Counter != 0) fout_in << Counter << endl;
                    Counter_D = Counter;
                    Counter = 0;
                    fout_in.close();
                    sound.stop();
                    memset(field, 0, M * N * sizeof field[0][0]);
                    break;

                }
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
                isFalling = false;
            }
            else
            {
                isFalling = true;
            }
            timer = 0;
        }

        if (ad) {
            int n = rand() % 7;
            if (a[0].x == 0)
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            ad = false;
        }

        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j])
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N)
                k--;
            else {
                click.play();
                Counter += 1;
            }
        }

        dx = 0;
        rotate = false;
        delay = 0.3;

        window.draw(Info.Background);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0)
                    continue;
                tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
                tiles.setPosition(j * w, i * w);
                window.draw(tiles);
            }

        for (int i = 0; i < 4; i++) {
            tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
            tiles.setPosition(a[i].x * w, a[i].y * w);
            window.draw(tiles);
        }
        Font font;
        font.loadFromFile("20db.otf");
        Text counterText;
        counterText.setFont(font);
        counterText.setCharacterSize(40);
        counterText.setFillColor(Color::Color(18, 41, 120));
        counterText.setString(std::to_string(Counter));
        counterText.setPosition(260, 40);
        window.draw(counterText);
        window.display();

    }
    music2.play();
    while (window.isOpen())
    {
        window.clear(Color::Black);
        Game_Over.setScale(Vector2f(0.71, 1));
        //Putin.setScale(Vector2f(0.3, 0.3));
        window.draw(Game_Over);
        //window.draw(Putin);
        Game_Over.setPosition(0, 0);
        //Putin.setPosition(0, 680 - Putin.getTexture()->getSize().y / 4.0f);
        Font font;
        font.loadFromFile("Arial.ttf");
        Text counterText, text;
        counterText.setFont(font);
        counterText.setCharacterSize(50);
        counterText.setFillColor(Color::White);
        counterText.setString(std::to_string(Counter_D));
        text.setFont(font);
        text.setCharacterSize(60);
        text.setFillColor(Color::White);
        text.setString("Your score");
        text.setPosition(40, 50);
        counterText.setPosition(160, 130);
        window.draw(text);
        window.draw(counterText);
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
                return false;
               
            }
            if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
            if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры
        }
        window.display();
    }

}

void gameRunning() {//ф-ция перезагружает игру , если это необходимо
    if (StartGame()) { gameRunning(); }////если startGame() == true, то вызываем занова ф-цию isGameRunning, которая в свою очередь опять вызывает startGame() 
}
int main()
{
    gameRunning();
    return 0;
}