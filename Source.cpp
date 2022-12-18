#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include "iostream"
#include <fstream>


using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int record = 0;
int last = 0;


int state = 0; //0 - Menu, 1 - results, 2 - Game;


int field[M][N] = { 0 };
int w = 34;

int couter_points = 0;

class MenuItems {
public:
    Sprite menuSprite, resultSprite, gameSprite;
};

struct Point
{
    int x, y;
}a[4], b[4];

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

    return 1;
}



void UpdateData(int count = -1) {
    ifstream fin("data.txt");
    if (!fin.is_open()) {
        ofstream fout("data.txt");
        fout << "0 0";
        fout.close();
    }
    else {
        fin >> record;
        fin >> last;
        fin.close();
    }

    if (count != -1) {
        ofstream fout("data.txt");
        last = count;
        if (record < last) { record = last; }
        fout << to_string(record) << " " << to_string(last);

        fout.close();

    }
}

int main()
{
    RenderWindow window(VideoMode(N * w, M * w), "TETRIS");

    Music music;
    music.openFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Downloads\\Mikhail_SHufutinskijj_-_3-e_sentyabrya_48105361 (1).ogg");
    music.setLoop(true);
    music.play();

    UpdateData();
    MenuItems mi;
    Texture t;
    t.loadFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Downloads\\MAINCART.png");
    mi.menuSprite.setTexture(t);
    Texture t2;
    t2.loadFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Downloads\\backfont.png");
    mi.gameSprite.setTexture(t2);

    Texture t3;
    t3.loadFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Downloads\\results.png");
    mi.resultSprite.setTexture(t3);

    mi.menuSprite.setPosition(0, 0);
    mi.gameSprite.setPosition(0, 0);
    mi.resultSprite.setPosition(0, 0);


    Font font;
    font.loadFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Desktop\\PakenhamBlItalic.ttf");

    Text recordCount(to_string(record), font, 50);


    Text lastCount(to_string(last), font, 50);

    Text lastCount1(to_string(0), font, 50);
    lastCount1.setPosition(N * w - 50, 0);


    recordCount.setFillColor(Color::Black);
    lastCount.setFillColor(Color::Black);
    lastCount1.setFillColor(Color::Black);

    recordCount.setPosition(w * 7, w * 5 - 20);
    lastCount.setPosition(w * 7, w * 7 - 20);



    srand(time(0));
    Texture t1;
    t1.loadFromFile("C:\\Users\\ÍÈÊÈÒÀ\\Downloads\\tiles.png");
    Sprite tiles(t1);

    int dx = 0, colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;
    bool ad = true;
    int n;


    while (window.isOpen()) {
        Event event;
        window.clear(Color::White);

        if (state == 0) {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::MouseButtonPressed) {
                    if (event.key.code == Mouse::Left) {
                        Vector2i pos = Mouse::getPosition(window);

                        if (pos.y >= w * 5 and pos.y <= w * 6) state = 2;
                        else if (pos.y >= w * 7 and pos.y <= w * 8) state = 1;
                        else if (pos.y >= w * 9 and pos.y <= w * 10) window.close();

                    }
                }
            }


            window.draw(mi.menuSprite);


        }
        else if (state == 1) {
            UpdateData();
            recordCount.setString(to_string(record));
            lastCount.setString(to_string(last));


            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::MouseButtonPressed) {
                    if (event.key.code == Mouse::Left) {
                        Vector2i pos = Mouse::getPosition(window);

                        if (pos.y <= w) state = 0;


                    }
                }
            }




            window.draw(mi.resultSprite);
            window.draw(lastCount);
            window.draw(recordCount);

        }
        else {


            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::MouseButtonPressed) {
                    if (event.key.code == Mouse::Left) {
                        Vector2i pos = Mouse::getPosition(window);

                        if (pos.y <= w) {
                            state = 0;

                            dx = 0;
                            rotate = false;
                            timer = 0, delay = 0.3;
                            ad = true;


                            for (int x = 0; x < M; x++) {
                                for (int y = 0; y < N; y++) field[x][y] = 0;
                            }

                            n = rand() % 7;
                            colorNum = 1 + rand() % 7;
                            for (int i = 0; i < 4; i++) {
                                a[i].x = figures[n][i] % 2;
                                a[i].y = figures[n][i] / 2;
                            }
                            UpdateData(couter_points);
                            couter_points = 0;
                            ad = false;
                        }
                    }
                }

                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::KeyPressed)
                    if (event.key.code == Keyboard::Up)
                        rotate = true;
                    else if (event.key.code == Keyboard::Right)
                        dx = 1;
                    else if (event.key.code == Keyboard::Left)
                        dx = -1;


                if (Keyboard::isKeyPressed(Keyboard::Down))
                    delay = 0.05;
            }

            int h = 0;
            for (int x = 0; x < M; x++) {
                for (int y = 0; y < N; y++) {
                    if (field[x][y] != 0) {
                        h++;
                        break;
                    }
                }
            }

            if (h >= 16) {

                dx = 0;
                rotate = false;
                timer = 0, delay = 0.3;
                ad = true;


                for (int x = 0; x < M; x++) {
                    for (int y = 0; y < N; y++) field[x][y] = 0;
                }

                n = rand() % 7;
                colorNum = 1 + rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
                UpdateData(couter_points);
                couter_points = 0;
                ad = false;

                state = 1;
            }

            lastCount1.setString(to_string(couter_points));

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
                    for (int i = 0; i < 4; i++)
                        field[b[i].y][b[i].x] = colorNum;
                    colorNum = 1 + rand() % 7;
                    n = rand() % 7;
                    for (int i = 0; i < 4; i++) {
                        a[i].x = figures[n][i] % 2;
                        a[i].y = figures[n][i] / 2;
                    }
                }

                timer = 0;
            }

            if (ad) {
                n = rand() % 7;
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
                    if (field[i][j]) {
                        count++;
                    }

                    field[k][j] = field[i][j];
                }
                if (count < N) {
                    k--;
                }
                else {
                    couter_points += 1;

                }

            }

            dx = 0;
            rotate = false;
            delay = 0.3;



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


            window.draw(mi.gameSprite);
            window.draw(lastCount1);

        }

        window.display();



    }




    return 0;
}