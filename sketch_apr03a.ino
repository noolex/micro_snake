#include <Wire.h>
void setup() {
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  oledInit();
  oledClear();
  game_init();
}

//SDA-A4 
//SCL-A5
#define GAME_WIDTH 64
#define GAME_HEIGHT 16

#define GAME_SPEED 500
#define SNAKE_LEN 64
#define SNAKE_HEAD_X 16 
#define SNAKE_HEAD_Y 8

int snake_x[SNAKE_LEN];
int snake_y[SNAKE_LEN];

int snake_len = 4;
int food_x = 0;
int food_y = 0;

int snake_dir = 0;
int btnFlag1 = 0, btnFlag2 = 0, actFlag = 0;

uint32_t snakeTimer = 0;

void snake_init() {
  int i;
  for (i = 0; i < SNAKE_LEN; i ++) {
    snake_x[i] = SNAKE_HEAD_X - i;
    snake_y[i] = SNAKE_HEAD_Y;
  }
}

void snake_move_head() {
  if (snake_dir == 0) {
    snake_y[0]--;
    if (snake_y[0] == -1) snake_y[0] = GAME_HEIGHT-1;
  }
  if (snake_dir == 1) {
    snake_x[0]++;
    if (snake_x[0] == GAME_WIDTH) snake_x[0] = 0;
  }
  if (snake_dir == 2) {
    snake_y[0]++;
    if (snake_y[0] == GAME_HEIGHT) snake_y[0] = 0;
  }
  if (snake_dir == 3) {
    snake_x[0]--;
    if (snake_x[0] == -1) snake_x[0] = GAME_WIDTH-1;
  }
}

void snake_move() {
  int i;
  for (i = snake_len; i > 0; i --) {
    snake_x[i] = snake_x[i-1];
    snake_y[i] = snake_y[i-1];
  }
}

void snake_draw_part() {
  dotSet(snake_x[0], snake_y[0]);
  dotSet(snake_x[snake_len-1], snake_y[snake_len-1]);
  dotClear(snake_x[snake_len], snake_y[snake_len]);
}

void snake_draw() {
  int i;
  oledClear();
  for (i = 0; i < snake_len; i ++) {
     dotSet(snake_x[i], snake_y[i]);
  }
}

void snake_eat() {
  if (snake_len + 1 < SNAKE_LEN) {
    snake_len ++;
  }   
}

void food_generate() {
  int isin = 0; //0  если мы вне змеи, 1 если на змее
  int i;
  
  for (isin = 1; isin == 1; ) {
    food_x = random(0, GAME_WIDTH);
    food_y = random(0, GAME_HEIGHT);
    
    isin = 0;
    for (i = 0; i < snake_len; i ++) {
      if (snake_x[i] == food_x && snake_y[i] == food_y) {
        isin = 1;
        break;
      }
    } 
  }
  
}

void food_draw() {
  dotSet(food_x, food_y);
}

void food_clear() {
  dotClear(food_x, food_y);
}

void gameTick() {
  if ((millis() - snakeTimer) >= GAME_SPEED) {
      snake_move();
      snake_move_head();
      actFlag = 0;
      
      if (snake_x[0] == food_x && snake_y[0] == food_y) {
        snake_eat();
        food_generate();  
      }
      
      snake_draw_part();
      food_draw();
      snakeTimer = millis();

      if (snake_is_loop()) {
        delay(2000);
        snake_len = 4;
        game_init();
      } 
  }
}

void game_init() {
  snake_init();
  food_generate();
  snake_draw();
  food_draw();  
}

int snake_is_loop () {
  int i;
  for (i = 1; i < snake_len; i++) {
    if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
    {
      return 1; 
    }
  }
  return 0;
}


void loop() {
  gameTick();
  
  if (digitalRead(3) == LOW) {
    if (btnFlag1 == 0 && actFlag == 0) {
      btnFlag1 = 1;
      snake_dir ++;
      actFlag = 1;
      if (snake_dir == 4) snake_dir = 0;    
    }
  } else {
    btnFlag1 = 0;
  }

  if (digitalRead(4) == LOW) {
    if (btnFlag2 == 0 && actFlag == 0) {
      btnFlag2 = 1;
      snake_dir --;
      actFlag = 1;
      if (snake_dir < 0) snake_dir = 3;
    }
  } else {
    btnFlag2 = 0;
  }
}
