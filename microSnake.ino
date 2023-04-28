/*
  Скетч к проекту "Классический pong на oled дисплее"
  Страница проекта (схемы, описания): https://alexgyver.ru/microPong/
  Исходники на GitHub: https://github.com/AlexGyver/microPong/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2019
  https://AlexGyver.ru/
*/

#define GAME_SPEED 100    // стандартный период движения шарика
#define SNAKE_MAX_LEN 64  // максимальная длина гусеницы
#define SNAKE_START_LEN 5 // Стартовая длина гусеницы

#define SNAKE_START_X   32 //
#define SNAKE_START_Y   8  //

#define GAME_HEIGHT     16
#define GAME_WIDTH      64
// пины
#define BTN_UP 3
#define BTN_DWN 4
// другие настройки


int8_t snake_x[SNAKE_MAX_LEN];
int8_t snake_y[SNAKE_MAX_LEN];
byte snake_len = SNAKE_START_LEN;
byte snake_dir = 0;       // Направление движения головы гусеницы
byte food_x = 0;
byte food_y = 0;

uint32_t snakeTimer;
boolean btnFlag1, btnFlag2, actFlag;
byte count;
byte speedIncr = 0;

#include <Wire.h>

void setup() {
  
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DWN, INPUT_PULLUP);
  Wire.begin();
  
  oledInit();
  oledClear();
  gameInit();
  randomSeed(analogRead(0));

  snakeTimer = millis();
}

void snake_init() {
  int i;
  for (i = 0; i <= snake_len; i ++ ) {
    snake_x[i] = SNAKE_START_X - i;
    snake_y[i] = 8;
  }
  snake_dir = 0;
}

void snake_draw_full() {
  int i;
  for (i = 0; i < snake_len; i ++) {
    dotSet(snake_x[i], snake_y[i]);
  }
}

void snake_clear_full() {
  int i;
  for (i = 0; i < snake_len; i ++) {
    dotClear(snake_x[i], snake_y[i]);
  }
}


void snake_draw_part() {
  dotSet(snake_x[0], snake_y[0]);
  dotSet(snake_x[snake_len-1], snake_y[snake_len-1]);
  dotClear(snake_x[snake_len], snake_y[snake_len]);
}

void snake_eat() {
  if (snake_len+1 < SNAKE_MAX_LEN) {
    snake_len ++;
  }
}

void snake_move_head() {
  int8_t x = snake_x[0];
  int8_t y = snake_y[0];
  
  if (snake_dir == 0) {
    x ++;
    if (x == GAME_WIDTH) x = 0;
  } else if (snake_dir == 1) {
    y ++;
    if (y == GAME_HEIGHT) y = 0;
  } else if (snake_dir == 2) {
    x --;
    if (x < 0) x = GAME_WIDTH-1;
  } else if (snake_dir == 3) {
    y --;
    if (y < 0) y = GAME_HEIGHT-1;
  }
  snake_x[0] = x;
  snake_y[0] = y;
}

void snake_move() {
  int i;
  for (i = snake_len; i > 0; i --) {
    snake_x[i] = snake_x[i-1];
    snake_y[i] = snake_y[i-1];
  }
}

bool snake_is_loop() {
  int i;
  for (i = 1; i < snake_len; i ++) {  
    if (snake_x[i] == snake_x[0] && snake_y[i] == snake_y[0]) return true;    
  }
  return false;
}

void food_generate() {
  boolean is_in_snake = false;
  int i;
  
  do {
    food_x = random(0, 64);
    food_y = random(0, 16);
    
    is_in_snake = false;
    
    for (i = 0; i <= snake_len; i ++) {
      if (snake_x[i] == food_x && snake_y[i] == food_y) {
        is_in_snake = true;
        break;
      }
    }
  } while(is_in_snake);
}

void food_clear() {
  dotClear(food_x, food_y);
}

void food_draw() {
  dotSet(food_x, food_y);
}

void loop() {
  buttonTick();
  gameTick();
}

void gameInit() {
  snake_init();
  food_generate();
  
  oledClear();
  snake_draw_full();
  food_draw();  
}

void gameTick() {
  if ((millis() - snakeTimer) >= (GAME_SPEED - speedIncr)) {
    snakeTimer = millis();
  
    snake_move();
    snake_move_head();
    
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
      snake_eat();
      food_generate();
      food_draw();
    } 
    
    if (snake_is_loop()) { //в петле  уменьшаем жизни. начинаем заново
      
      delay(1000);
      gameInit();
       
    } else {
      snake_draw_part();
    }
    actFlag = false;
  }
  snake_draw_part();
}

void buttonTick() {
  if (!digitalRead(BTN_DWN) && !btnFlag1 && !actFlag) {
    btnFlag1 = true;
    actFlag = true;
    if (snake_dir == 0) snake_dir = 3;
    else snake_dir --;
  }
  if (digitalRead(BTN_DWN) && btnFlag1) {
    btnFlag1 = false;
  }
  
  if (!digitalRead(BTN_UP) && !btnFlag2 && !actFlag) {
    btnFlag2 = true;
    actFlag = true;
    if (snake_dir == 3) snake_dir = 0;
    else snake_dir ++;
  }
  if (digitalRead(BTN_UP) && btnFlag2) {
    btnFlag2 = false;
  }
}
