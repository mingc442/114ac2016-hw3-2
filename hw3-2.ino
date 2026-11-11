// 腳位設定
const int BUTTON_PIN = 2;  // 按鈕接在 D2
const int LED_PIN = 3;     // LED 接在 D3 (記得串聯 220Ω 電阻保護 LED)

// 狀態控制變數
int state = 0;             // 0: 恆亮, 1: 慢閃, 2: 中閃, 3: 快閃
bool lastButtonState = LOW; // 記錄上一次按鈕狀態
unsigned long lastBlinkTime = 0;
bool ledState = HIGH;

// 按鈕去彈跳（防手震）變數
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50; 

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // 初始狀態：恆亮
}

void loop() {
  // 讀取按鈕狀態並做去彈跳處理
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    static bool buttonState = LOW;
    if (reading != buttonState) {
      buttonState = reading;
      
      // 當按鈕按下（上升沿 HIGH）時切換狀態
      if (buttonState == HIGH) {
        state = (state + 1) % 4; // 0 -> 1 -> 2 -> 3 -> 0 (第4下回到恆亮)
      }
    }
  }
  lastButtonState = reading;

  // 根據目前狀態控制 LED 閃爍頻率
  switch (state) {
    case 0: // 狀態 0：恆亮
      digitalWrite(LED_PIN, HIGH);
      break;

    case 1: // 狀態 1：超慢閃爍 (亮1.5秒 / 暗1.5秒)
      blinkLED(1500);
      break;

    case 2: // 狀態 2：中速閃爍 (亮0.5秒 / 暗0.5秒)
      blinkLED(500);
      break;

    case 3: // 狀態 3：極速爆閃 (亮0.08秒 / 暗0.08秒)
      blinkLED(80);
      break;
  }
}

// 非阻塞式閃爍函式
void blinkLED(int interval) {
  if (millis() - lastBlinkTime >= interval) {
    lastBlinkTime = millis();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}