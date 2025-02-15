#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Khởi tạo U8g2 cho OLED 128x64 I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// --- Định nghĩa hằng số ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BIRD_SIZE 8
#define PIPE_WIDTH 15
#define PIPE_GAP 25  // Khoảng trống giữa ống trên và ống dưới
#define PIPE_SPEED 2
#define GRAVITY 1
#define JUMP_STRENGTH -4 // Giảm giá trị lực nhảy (SỬA ĐỔI)
#define BUTTON_PIN 0 // Thay đổi chân nút nhấn nếu cần

// --- Biến toàn cục ---
int birdX, birdY;
int birdVelocity;
int pipeX[3]; // Mảng lưu vị trí X của 3 cặp ống
int pipeBottomTopY[3]; // Mảng lưu vị trí Y của mép trên ống dưới (SỬA ĐỔI)
int score;
bool gameRunning;
bool gameOver;

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Nút nhấn, sử dụng điện trở kéo lên nội bộ

  startGame(); // Khởi tạo trò chơi khi bắt đầu
}

void loop() {
  u8g2.clearBuffer();

  if (!gameRunning && !gameOver) {
    drawStartScreen(); // Vẽ màn hình bắt đầu
    if (isButtonPressed()) {
      startGame(); // Bắt đầu trò chơi khi nút được nhấn
      gameRunning = true;
    }
  } else if (gameRunning) {
    updateGame();   // Cập nhật logic trò chơi (vị trí chim, ống, va chạm, điểm số)
    drawGame();     // Vẽ các đối tượng trò chơi
    if (gameOver) {
      gameRunning = false; // Dừng trò chơi
    }
  } else if (gameOver) {
    drawGameOverScreen(); // Vẽ màn hình Game Over
    if (isButtonPressed()) {
      startGame();     // Chơi lại khi nút được nhấn
      gameOver = false;
      gameRunning = true;
    }
  }

  u8g2.sendBuffer();
  delay(30); // Điều chỉnh tốc độ trò chơi
}

// --- Hàm khởi tạo trò chơi ---
void startGame() {
  birdX = SCREEN_WIDTH / 4;
  birdY = SCREEN_HEIGHT / 2;
  birdVelocity = 0;
  score = 0;
  gameOver = false;

  // Khởi tạo vị trí ống và khoảng trống ban đầu
  for (int i = 0; i < 3; i++) {
    pipeX[i] = SCREEN_WIDTH + i * (SCREEN_WIDTH / 3); // Ống cách đều nhau
    pipeBottomTopY[i] = random(PIPE_GAP + 10, SCREEN_HEIGHT - 10); // Mép trên ống dưới ngẫu nhiên (SỬA ĐỔI)
  }
}

// --- Hàm cập nhật trạng thái trò chơi mỗi frame ---
void updateGame() {
  // 1. Cập nhật vị trí chim do trọng lực và nhảy
  birdVelocity += GRAVITY;
  birdY += birdVelocity;

  // Giới hạn chim không rơi khỏi màn hình dưới
  if (birdY > SCREEN_HEIGHT - BIRD_SIZE / 2) {
    birdY = SCREEN_HEIGHT - BIRD_SIZE / 2;
    gameOver = true; // Game Over khi chạm đất
  }
  // Giới hạn chim không bay lên quá màn hình trên
  if (birdY < BIRD_SIZE / 2) {
    birdY = BIRD_SIZE / 2;
    birdVelocity = 0; // Ngăn vận tốc cộng dồn khi chạm trần
  }


  // 2. Di chuyển ống và tạo ống mới
  for (int i = 0; i < 3; i++) {
    pipeX[i] -= PIPE_SPEED; // Ống di chuyển sang trái

    if (pipeX[i] < -PIPE_WIDTH) { // Ống ra khỏi màn hình
      pipeX[i] = SCREEN_WIDTH;    // Đặt lại ống ở bên phải
      pipeBottomTopY[i] = random(PIPE_GAP + 10, SCREEN_HEIGHT - 10); // Mép trên ống dưới mới (SỬA ĐỔI)
      score++; // Tăng điểm khi vượt qua ống
    }
  }

  // 3. Phát hiện va chạm
  if (checkCollision()) {
    gameOver = true;
    gameRunning = false; // Dừng game
  }

  // 4. Xử lý nút nhấn (nhảy)
  if (isButtonPressed()) {
    birdVelocity = JUMP_STRENGTH; // Chim vỗ cánh/nhảy lên
  }
}

// --- Hàm vẽ các đối tượng trò chơi ---
void drawGame() {
  // Vẽ chim (hình tròn đơn giản)
  u8g2.drawCircle(birdX, birdY, BIRD_SIZE / 2);

  // Vẽ các ống
  for (int i = 0; i < 3; i++) {
    drawPipe(pipeX[i], pipeBottomTopY[i]); // Sửa đổi để truyền pipeBottomTopY
  }

  // Vẽ mặt đất (đường kẻ ngang)
  u8g2.drawLine(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, SCREEN_HEIGHT - 5);

  // Vẽ điểm số
  u8g2.setFont(u8g_font_5x7);
  u8g2.setCursor(5, 10);
  u8g2.print("Score: ");
  u8g2.print(score);
}

// --- Hàm vẽ một cặp ống tại vị trí x, pipeBottomTopY (SỬA ĐỔI HÀM) ---
void drawPipe(int x, int pipeBottomTopY) { // Sửa đổi tham số thứ hai
  // Ống trên (SỬA ĐỔI)
  u8g2.drawBox(x, 0, PIPE_WIDTH, pipeBottomTopY - PIPE_GAP); // Vẽ từ 0 đến vị trí mép trên ống dưới trừ khoảng trống
  // Ống dưới (SỬA ĐỔI)
  u8g2.drawBox(x, pipeBottomTopY, PIPE_WIDTH, SCREEN_HEIGHT - pipeBottomTopY); // Vẽ từ mép trên ống dưới đến đáy màn hình
}

// --- Hàm kiểm tra va chạm ---
bool checkCollision() {
  // 1. Va chạm với mặt đất (đã xử lý trong updateGame)

  // 2. Va chạm với ống
  for (int i = 0; i < 3; i++) {
    if (birdX + BIRD_SIZE / 2 > pipeX[i] && birdX - BIRD_SIZE / 2 < pipeX[i] + PIPE_WIDTH) { // Chim nằm trong khoảng X của ống
      // Kiểm tra va chạm với ống trên (SỬA ĐỔI: dùng pipeBottomTopY)
      if (birdY - BIRD_SIZE / 2 < pipeBottomTopY[i] - PIPE_GAP) return true;
      // Kiểm tra va chạm với ống dưới (SỬA ĐỔI: dùng pipeBottomTopY)
      if (birdY + BIRD_SIZE / 2 > pipeBottomTopY[i] + 0) return true; // +0 vì mép trên ống dưới là pipeBottomTopY
    }
  }
  return false; // Không va chạm
}

// --- Hàm vẽ màn hình bắt đầu ---
void drawStartScreen() {
  u8g2.setFont(u8g_font_helvB18);
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Flappy Bird")) / 2, SCREEN_HEIGHT / 2 - 10, "Flappy Bird");
  u8g2.setFont(u8g_font_5x7);
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Press button to start")) / 2, SCREEN_HEIGHT / 2 + 10, "Press button to start");
}

// --- Hàm vẽ màn hình Game Over ---
void drawGameOverScreen() {
  u8g2.setFont(u8g_font_helvB18);
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Game Over")) / 2, SCREEN_HEIGHT / 2 - 20, "Game Over");
  u8g2.setFont(u8g_font_5x7);
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Score: ")) / 2, SCREEN_HEIGHT / 2, "Score: ");
  u8g2.print(score);
  u8g2.setFont(u8g_font_5x7);
  u8g2.drawStr((SCREEN_WIDTH - u8g2.getStrWidth("Press button to restart")) / 2, SCREEN_HEIGHT / 2 + 20, "Press button to restart");
}


// --- Hàm đọc trạng thái nút nhấn ---
bool isButtonPressed() {
  return digitalRead(BUTTON_PIN) == LOW; // Nút nhấn thường LOW khi được nhấn (PULLUP)
}