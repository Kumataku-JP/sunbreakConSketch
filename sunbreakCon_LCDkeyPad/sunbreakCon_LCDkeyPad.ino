/*  ArduinoとLCD keypad Shield端末を使用して実行するプログラム
 *  Nintendo Switch
 *  PlayStation 5
 *  モンハンライズサンブレイク  Ver.16.0.x
 *  プログラムセット Ver.4.0.0
 *
 *  DOWN-UPボタンでモードの切り替え
 *  モード0  システム設定  / System Setting
 *  モード1  防具傀異錬成  / Qurious Armor Crafting.
 *  モード2  マカ連金  / Melding
 *  モード3  放置用自動マクロ  / Auto Play.
 *  モード4  amiiboによる福引  / Repeat loading of amiibo.
 *
 *
 *  左右のボタンで各種項目を選択しSELECTボタンで実行・停止または設定モードへ
 *  LCD表示を参照／画面右隅の「>R」(RIGHT)など
 *
 */

#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <NSControlLibrary.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/* ============================================================== */
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
#define ERRORR 6

int read_LCD_buttons(int adc_key_in) {
  if (adc_key_in < 50) return btnRIGHT;    //0   , 戻り値0, 0V
  if (adc_key_in < 200) return btnUP;      //143 , 戻り値1, 0.70V
  if (adc_key_in < 400) return btnDOWN;    //344 , 戻り値2, 1.68V
  if (adc_key_in < 600) return btnLEFT;    //513 , 戻り値3, 2.50V
  if (adc_key_in < 800) return btnSELECT;  //732 , 戻り値4, 3.57V
  if (adc_key_in > 950) return btnNONE;    //1023, 戻り値5, 5.00V
  return btnNONE;                          //
}
/* ============================================================== */

char mode = 0;
char value = 0;
// プログラム制御
unsigned char keys;
unsigned char keysOld;

// 各種初期値
bool initialLcd = false;
bool joinExecuted = false;
bool isFirstRun = true;
bool firstRun = true;
bool melding = false;

// マカ錬金
char meldingStop = 0;
char LotteryStop = 0;
char line_1 = 1;
char line_2 = 0;
char page_1 = 1;
char page_2 = 0;

// 自動制御
bool repeatLaps = false;
int times = 0;
unsigned long elapsed;
unsigned long startTime;
unsigned long startTimeA = 0;
unsigned long startTimeB = 0;
unsigned long startTimeS = 0;
unsigned long stopTime;
unsigned int repeatCount;
unsigned char prg = 0;
bool pause;
bool reStart = true;
bool display = true;
bool revert = false;
bool skipExec = false;
bool skipSheathe = false;
char setPos = 0;
char curPos = 1;
char ess = 0;
char melody = 0;
char area = 0;
bool scroll = 0;
char point = 1;
char areaPick = 0;
char melodyPick = 0;
char scrollPick = 0;
char pointPick = 1;
// コロンの表示制御
unsigned long referenceTime = 0;
unsigned long preMillis = 0;              // 前回の時間
const unsigned long interval = 1000;      // コロンを点滅させる間隔（ミリ秒）
const unsigned long dispInterval = 2000;  // 2秒ごとに表示を切り替え
unsigned long minutes;
bool colonVisible = false;        // コロンの表示状態
long dispIndex = -1;              // 現在表示中の項目インデックス
unsigned long preDispMillis = 0;  // 前回の時間

// 制御
bool runMode;
bool targetOn;
bool setupMode = false;
bool closeLottery = false;
bool timesLeft;

// カウンター制御
unsigned int countR = 1;  // 傀異錬成セットカウント
unsigned int countM;      // マカ錬金セットカウント

int numValue;
int numDate;
int digitsQ[4];  // 各桁の数字を格納
int digitsL[4];  // 各桁の数字を格納
int digitsM[2];  // 各桁の数字を格納

// カレンダー制御
unsigned char lastDay = 31;
unsigned char leap;  //2月の繰り返し回数
unsigned char dayDate = 1;
unsigned char monthDate = 1;
unsigned char yearDate = 24;
unsigned char savedDayDate;
unsigned char savedMonthDate;
unsigned char savedYearDate;
// 設定制御

/* ============================================================== */
/* 決定Aボタンと×ボタンの決定・キャンセルのキーマッピング */
// Nintendo Switchは"0"、PlayStation 5は"1"
// Aボタンと×ボタンの決定、機種固有の違い
// マイナスボタン：キャプチャーボタン／CREATEボタン：タッチパッド
bool consoleType = 0;
unsigned char confirmButton;
unsigned char cancelButton;
unsigned int minusButton;
unsigned int captureButton;

/* R(R1)ボタンとZR(R2)ボタンを入れ替えたマッピングの場合 =============== */
// ゲームオプション / CONTROLS / L(L1)とR(R1)の入れ替え  →  R(R1)とZR(R2)のみ
// ユーザー環境に合わせて数値変更  デフォルトは"0"  入れ替えた場合は"1"
bool mappingR = 0;  // Rボタンのキーマッピングの有無
unsigned char mappingR1;
unsigned char mappingR2;

/* カナ変換 ======================================================= */
char jpconv[64];
char *jp(const char *text) {
  char *cur = &jpconv[0];
  const char *chk = text;
  while (*chk != 0) {
    if ((*chk & 0xff) == 0xef && (*(chk + 1) & 0xbc) == 0xbc) {
      *cur = (*++chk & 0x02) << 5;
      *cur += *++chk;
      cur++;
    } else {
      *cur++ = *chk;
    }
    chk++;
  }
  *cur = 0;
  return jpconv;
}

/* language ===================================================== */
bool languageFlag = 1;  // 言語のフラグ (0: 英語, 1: 日本語)
// value, mode 0  / 設定
const char *strings_S[][2] = {
  { "CONFIGURATION", "ｼｽﾃﾑｾｯﾃｲ" },           // 0
  { "LANGUAGE", "LANGUAGE" },                // 1
  { "Console Type", "ｹﾞｰﾑ ｷｼｭ" },            // 2
  { "R-BTN Mapping", "Rﾎﾞﾀﾝﾏｯﾋﾟﾝｸﾞ" },       // 3
  { "Update Setting?", "ｾｯﾃｲｦ ｺｳｼﾝｼﾏｽｶ?" },  // 4
};
// value, mode 1  / 傀異錬成
const char *strings_Q[][2] = {
  { "Qurious", "ｶｲｲﾚﾝｾｲ" },      // 0
  { "Config", "ｾｯﾃｲ" },          // 1
  { "Qurious", "ｶｲｲﾚﾝｾｲ" },      // 2
  { "RepeatRec", "ﾚﾝｾｲｷﾛｸ" },    // 3
  { "RepeatQUR", "ﾚﾝｾｲｸﾘｶｴｼ" },  // 4
};
// value, mode 2  / マカ錬金
const char *strings_M[][2] = {
  { "Melding", "ﾏｶﾚﾝｷﾝ" },     // 0
  { "MulTask", "ﾊｷﾏﾙﾁｾｯﾃｲ" },  // 1
  { "Vigor", "ﾊｷ" },           // 2
  { "Cyclus", "ｴﾝｶﾝ" },        // 3
  { "Sell EQPT ", "ﾊﾞｲｷｬｸ" },  // 4
};
// value, mode 3  / 自動プレイ
const char *strings_A[][2] = {
  { "AutoPlay", "ｵｰﾄﾌﾟﾚｲ" },   // 0
  { "Config", "ｼﾞｭﾝﾋﾞｾｯﾃｲ" },  // 1
  { "", "" },                  // 2
  { "ItemGath", "ﾄｸｻﾝﾋﾝ" },    // 3
};
// value, mode 4  / amiibo福引
const char *strings_F[][2] = {
  { "amiibo", "ｱﾐｰﾎﾞ" },   // 0
  { "Config", "ｾｯﾃｲ" },    // 1
  { "Lottery", "ﾌｸﾋﾞｷ" },  // 2
};

// 選択した言語に応じて文字列を表示する関数
void displayString(char index, char set) {
  const char *str = nullptr;
  switch (set) {
    case 0:
      str = (languageFlag == 0) ? strings_S[(int)index][0] : jp(strings_S[(int)index][1]);
      break;
    case 1:
      str = (languageFlag == 0) ? strings_Q[(int)index][0] : jp(strings_Q[(int)index][1]);
      break;
    case 2:
      str = (languageFlag == 0) ? strings_M[(int)index][0] : jp(strings_M[(int)index][1]);
      break;
    case 3:
      str = (languageFlag == 0) ? strings_A[(int)index][0] : jp(strings_A[(int)index][1]);
      break;
    case 4:
      str = (languageFlag == 0) ? strings_F[(int)index][0] : jp(strings_F[(int)index][1]);
      break;
  }
  if (str != nullptr) lcd.print(str);
}

/* ============================================================== */
void lcdSelect() {
  const char *strings[][2] = {
    { "MENU", "ﾒﾆｭｰ" },
    { "SelectEssence", "ｺﾊｸｦ ｾﾝﾀｸ" },
    { "SelectMelding", "ﾚﾝｷﾝｦ ｾﾝﾀｸ" },
    { "SelectAutoTyp", "ｵｰﾄｺｳﾓｸｦ ｾﾝﾀｸ" },
    { "SelectLottery", "ﾌｸﾋﾞｷｦ ｾﾝﾀｸ" },
  };
  lcd.setCursor(0, 0);
  lcd.print((languageFlag == 0) ? strings[(int)mode][0] : jp(strings[(int)mode][1]));

  lcd.setCursor(13, 0);
  lcd.print(">LR");
}

/* ============================================================== */
/*　各モードのデータ設定時のカーソル表示位置*/
void cursorPosition() {
  switch (mode) {
    case 1:
      if (curPos == 1) lcd.setCursor(12, 0);       // 1000の位
      else if (curPos == 2) lcd.setCursor(13, 0);  // 100の位
      else if (curPos == 3) lcd.setCursor(14, 0);  // 10の位
      else if (curPos == 4) lcd.setCursor(15, 0);  // 1の位
      else if (curPos == 5) lcd.setCursor(15, 1);  // 使用琥珀
      break;
    case 2:
      if (curPos == 1) lcd.setCursor(11, 1);       // 1つめ使用素材ページ
      else if (curPos == 2) lcd.setCursor(12, 1);  // 1つめ使用素材か所
      else if (curPos == 3) lcd.setCursor(14, 1);  // 2つめ使用素材ページ
      else if (curPos == 4) lcd.setCursor(15, 1);  // 2つめ使用素材か所
      else if (curPos == 5) lcd.setCursor(14, 0);  // 周回数十の位
      else if (curPos == 6) lcd.setCursor(15, 0);  // 周回数一の位
      break;
    case 4:
      if (curPos == 1) lcd.setCursor(9, 0);        // 年の位置
      else if (curPos == 2) lcd.setCursor(12, 0);  // 月の位置
      else if (curPos == 3) lcd.setCursor(15, 0);  // 日の位置
      else if (curPos == 4) lcd.setCursor(12, 1);  // 1000の位
      else if (curPos == 5) lcd.setCursor(13, 1);  // 100の位
      else if (curPos == 6) lcd.setCursor(14, 1);  // 10の位
      else if (curPos == 7) lcd.setCursor(15, 1);  // 1の位
      break;
  }
  if (setupMode) {
    lcd.cursor();  // カーソル常時表示
  }
}

/* LCD制御=========================================================== */
/* 1列目LCD */
void commonLcdRow1() {
  lcd.setCursor(0, 0);
  lcd.print((int)value);
  lcd.print(".");
}
/* 2列目LCD */
void commonLcdRow2() {
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print((int)mode);
  lcd.print(".");
}
/* LCD日付データ表示 */
void showLcdDate() {
  char text[6];
  if (languageFlag == 0) {
    sprintf(text, setupMode ? "M%02dD%02dY%02d" : " %02d/%02d/%02d", monthDate, dayDate, yearDate);  // "MM/DD/YY"
  } else if (languageFlag == 1) {
    sprintf(text, setupMode ? "Y%02dM%02dD%02d" : " %02d/%02d/%02d", yearDate, monthDate, dayDate);  // "YY/MM/DD"
  }
  lcd.print(text);
}

/* ============================================================== */
void connectUp() {
  if (!consoleType) {
    for (char i = 0; i < 5; i++) {
      holdButton(Button::L);
      holdButton(Button::R, 100);
      releaseButton(Button::L);
      releaseButton(Button::R, 100);
    }
  } else {
    pushButton(Button::HOME, 50, 600);
  }
}
/* 実行前にSwitchにUSB接続を処理 */
void join() {
  if (joinExecuted) return;  // 一度実行されていたら処理をスキップ
  connectUp();
  joinExecuted = true;  // 実行後にフラグをセット
}
/* ============================================================== */
void updateCountGeneric(int &numResult, const int digits[], int numDigits) {
  numResult = 0;
  for (int i = 0; i < numDigits; i++) {
    numResult = numResult * 10 + digits[i];
    lcd.print(digits[i]);
  }
}
/* setup========================================================= */
// int analogValue = 0;  // アナログ値を格納する変数
void setup() {
  // Serial.begin(9600);  // シリアルポートを9600bpsで開く
  /* EEPROMからデータを読み込む */
  if ((EEPROM.read(0) == 0 || EEPROM.read(0) == 1) && (EEPROM.read(1) == 0 || EEPROM.read(1) == 1) && (EEPROM.read(2) == 0 || EEPROM.read(2) == 1)) {
    bool savedLanguageFlag = EEPROM.read(0);
    bool savedConsoleType = EEPROM.read(1);
    bool savedMappingR = EEPROM.read(2);
    // 読み込んだデータを使って初期設定などを行う
    languageFlag = savedLanguageFlag;
    consoleType = savedConsoleType;
    mappingR = savedMappingR;
  }
  /* 決定ボタン */
  confirmButton = (consoleType == 0) ? Button::A : Button::B;  // Switch
  cancelButton = (consoleType == 0) ? Button::B : Button::A;   // PS5
  /* マイナスボタン：キャプチャーボタン／CREATEボタン：タッチパッド */
  minusButton = (consoleType == 0) ? Button::MINUS : Button::CAPTURE;    // Switch
  captureButton = (consoleType == 0) ? Button::CAPTURE : Button::MINUS;  // PS5
  /* Rボタン入れ替え */
  mappingR1 = (mappingR == 0) ? Button::R : Button::ZR;  // Switch
  mappingR2 = (mappingR == 0) ? Button::ZR : Button::R;  // PS5
  /* コントローラー接続 */
  connectUp();
  /* LCDの初期値 */
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(F("MONSTER HUNTER"));
  delay(1000);
  lcd.clear();  // LCD初期化
  startTime = millis();
}
/* loop========================================================== */
void loop() {
  // analogValue = analogRead(0);  // アナログピン0から読み取る
  // Serial.println(analogValue);
  keys = read_LCD_buttons(analogRead(0));
  // DOWN - UPでモード切り替え
  if (!setupMode && !runMode) {
    if ((keys == btnDOWN || keys == btnUP) && !closeLottery) {
      mode += (keys == btnUP) ? 1 : -1;
      // ゲーム機種によるモード制限
      if (consoleType == 0) {
        mode = (mode > 4) ? 0 : (mode < 0) ? 4
                                           : mode;
      } else if (consoleType == 1) {
        mode = (mode > 3) ? 0 : (mode < 0) ? 3
                                           : mode;
      }
      prg = 0;
      // モード切り替え時の設定を保存
      value = 0;
      setupMode = false;
      initialLcd = true;
      if (mode != 3) prg = 0;  // 闘技場オートクエスト準備初期化
      lcd.clear();             // LCD初期化
      lcdSelect();             // value = 0 1列目LCD0-
      commonLcdRow2();         // value = 0 2列目LCD0-1
      displayString(0, mode);  // 2列目LCD3-
      join();
      delay(300);
    }
  }

  /* マクロ項目*/
  switch (mode) {
    // モード0の動作
    case 0:
      if (!initialLcd) {
        lcd.setCursor(0, 0);
        lcd.print(F("RISE:SUNBREAK_"));
        lcd.print(consoleType == 0 ? F("NS") : F("PS"));
        lcd.setCursor(0, 1);
        lcd.print(F("v4.0 MODE>"));
        lcd.print(languageFlag == 0 ? "UP-DWN" : jp("ｳｴorｼﾀ"));
      } else if (value == 0) {
        lcd.setCursor(3, 1);
        displayString(mode, 5);  // configuration
      }
      mode0();
      break;
    // モード1の動作
    case 1:
      mode1();
      break;
    // モード2の動作
    case 2:
      mode2();
      break;
    // モード3の動作
    case 3:
      mode3();
      break;
    // モード4の動作
    case 4:
      mode4();
      break;
  }
}
