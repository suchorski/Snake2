/** Snake2 Info
 *
 *  Primeiro quadrado: 	 0, 0
 *        Coordenadas: 155, 5
 *  Último 	 quadrado: 	43, 28
 *		  Coordenadas: 585, 285
 *
 */

#ifndef SNAKE_HPP
#define SNAKE_HPP

// Informação
#define NAME "Snake"
#define VERSION "2.0"

// Cores
#define COLOR_BORDER COLOR(0, 0, 255)
#define COLOR_TEXT COLOR(255, 255, 255)
#define COLOR_SNAKE COLOR(0, 255, 0)
#define COLOR_SKIN COLOR(0, 200, 0)
#define COLOR_APPLE COLOR(255, 0, 0)

// Textos

#define GAME_TITLE NAME " v" VERSION
#define GAME_LOADING "Carregando..."

#define MENU_SCORE "Pontuação"
#define MENU_LEVEL "Nível"
#define MENU_MAX "Recorde"

// Definições
#define SNAKE_DB ".\\Snake2.db"
#define SNAKE_ADD 1
#define SNAKE_MOVE 2
#define SNAKE_CLEAR 3
#define SNAKE_BODY 1
#define SNAKE_APPLE 2

// Comandos
#define sleep(a) Sleep(a)

// Teclas
#define KEY_ESC 27

// Direções
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// NameSpace
using namespace std;
using namespace boost::random;

// Semente
mt19937 random;

// Variáveis
HANDLE movingSnake;
unsigned short int score = 0;
short int level = 0;
unsigned short int maxscore = 0;
bool maxnew = false;
bool alive = false;
bool life = true;

int keyMove = KEY_RIGHT;
int keyDir = RIGHT;

char *mainmenu[] = {"Começar", "Dificuldade", "Sobre", "Sair"};
char *levelname[] = {"Fácil", "Normal", "Difícil", "Insano", "Nab Rab"};
char *about[] = {"Criado por Thiago", "Versão: 2.0", "Espero que gostem!"};
char *gameover[] = {"Você perdeu!", "", "Mais sorte", "na próxima vez..."};
char *youwin[] = {"PARABÉNS!", "", "Você ganhou!"};
char *gamepaused[] = {"Game Pausado!", "", "Pressione ENTER para", "continuar..."};

short int levelspeed[] = {300, 200, 100, 50, 25};

struct cell
{
	short int x;
	short int y;
} apple;

vector <cell> snake;
vector <cell> blank;

// Fix pra winBGIm
void swapBuffer(void);

// Headers de desenho do jogo
void drawGame(void);
short int drawMenu(short int sel, short int itens, char *item[]);
void drawBox(short int lines, char *item[], bool confirm);
void drawItem(short int x, short int y, short int item);
void drawSnake(void);
void drawApple(void);

// Headers do jogo
void changeSnake(short int x, short int y, short int cmd);
bool changeApple(void);
void changeEmpty(short int x, short int y, bool cmd);
void fillEmpty(void);
bool isSnake(short int x, short int y);
void gameStart(void);
DWORD WINAPI moveSnake(LPVOID lpParam);

#endif // SNAKE_HPP
