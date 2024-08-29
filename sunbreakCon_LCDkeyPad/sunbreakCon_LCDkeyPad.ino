/*  ArduinoとLCD keypad Shield端末を使用して実行するプログラム
 *  Nintendo Switch
 *  PlayStation 5
 *  モンハンライズサンブレイク  Ver.16.0.x
 *  プログラムセット Ver.3.8
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
  if (adc_key_in < 50)
    return btnRIGHT;  // 0   , 戻り値0, 0V
  if (adc_key_in < 250)
    return btnUP;  // 131 , 戻り値1, 0.64V
  if (adc_key_in < 450)
    return btnDOWN;  // 307 , 戻り値2, 1.50V
  if (adc_key_in < 650)
    return btnLEFT;  // 479 , 戻り値3, 2.34V
  if (adc_key_in < 850)
    return btnSELECT;  // 721 , 戻り値4, 3.52V
  if (adc_key_in > 1000)
    return btnNONE;  // 1023, 戻り値5, 5.00V
  return btnNONE;    //
}
/* ============================================================== */

int mode = 0;
// プログラム制御
unsigned char keys;
unsigned char keysOld;
unsigned char modeNow;
unsigned char modeOld;

// 各種初期値
bool initialLcd = false;
bool joinExecuted = false;
bool isFirstRun = true;
bool firstRun = true;
bool melding = false;
bool QuriousRun;

// マカ錬金
int meldingStop = 0;
int LotteryStop = 0;
int lap = 0;
char line_1 = 1;
char page_1 = 1;
char line_2 = 0;
char page_2 = 0;

// 自動制御
int times = 0;
unsigned char melody = 0;
unsigned long now;
unsigned long elapsed;
unsigned long startTime = 0;
unsigned long startTimeA = 0;
unsigned long startTimeB = 0;
unsigned long startTimeS = 0;
unsigned long stopTime;
unsigned int repeatCount;
unsigned char prg = 0;
bool reStart;
bool skipExec = false;
bool skipSheathe = false;
// コロンの表示制御
unsigned long referenceTime = 0;
unsigned long previousMillis = 0;     // 前回の時間
const unsigned long interval = 1000;  // コロンを点滅させる間隔（ミリ秒）
bool colonVisible = false;            // コロンの表示状態

// 制御
int value;
bool steyMode2;
bool steyMode3;
bool steyMode4;
bool targetOn;
bool dateSet = false;
bool memory = false;
bool closeLottery = false;
bool runStop = true;
bool timesLeft;

// カウンター制御
unsigned int countR = 1;  // 傀異錬成セットカウント
unsigned int countM;      // マカ錬金セットカウント

int numValue;
int numDate;
int digitsQ[4];  // 各桁の数字を格納
int digitsL[4];  // 各桁の数字を格納
int digitsM[2];

// カレンダー制御
unsigned char day31 = 31;
unsigned char day30 = 30;
unsigned char leap;  //2月の繰り返し回数
unsigned char dayDate = 1;
unsigned char monthDate = 1;
unsigned char yearDate = 24;
unsigned char savedDayDate;
unsigned char savedMonthDate;
unsigned char savedYearDate;
unsigned char curPos = 1;  // カーソル位置

/* ============================================================== */
/* 決定Aボタンと×ボタンの決定・キャンセルのキーマッピング */
// Nintendo Switchは"0"、PlayStation 5は"1"
bool consoleType = 0;  // Aボタンと×ボタンの決定、機種固有の違い
unsigned char confirmButton;
unsigned char cancelButton;

/* R(R1)ボタンとZR(R2)ボタンを入れ替えたマッピングの場合 =============== */
// ゲームオプション / CONTROLS / L(L1)とR(R1)の入れ替え  →  R(R1)とZR(R2)のみ
// ユーザー環境に合わせて数値変更  デフォルトは"0"  入れ替えた場合は"1"
bool mappingR = 1;  // Rボタンのキーマッピングの有無
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
  *cur = 0;  // 終端に null 文字を追加
  return jpconv;
}

/* language ===================================================== */
bool languageFlag = 1;  // 言語のフラグ (0: 英語, 1: 日本語)
// value, mode 0  / 設定
const char *strings_S[][2] = {
  { "CONFIGURATION", "ｼｽﾃﾑｾｯﾃｲ" },         // 0
  { "VideoGame Type", "ｹﾞｰﾑ ｷｼｭ" },        // 1
  { "R-BTN Mapping", "Rﾎﾞﾀﾝﾏｯﾋﾟﾝｸﾞ" },     // 2
  { "DATE Setting", "ﾋﾂﾞｹ ｾｯﾃｲ" },         // 3
  { "SystemLANGUAGE", "SystemLANGUAGE" },  // 4
};
// value, mode 1  / 傀異錬成
const char *strings_Q[][2] = {
  { "Qurious", "ｶｲｲﾚﾝｾｲ" },      // 0
  { "Amber", "ｾｲｷ" },            // 1
  { "Plus", "ｼﾞｮｳ" },            // 2
  { "Prime", "ｾﾝ" },             // 3
  { "Royal", "ｵｳ" },             // 4
  { "Pure", "ｼﾝ" },              // 5
  { "RepeatRec", "ﾚﾝｾｲｷﾛｸ" },    // 6
  { "RepeatQUR", "ﾚﾝｾｲｸﾘｶｴｼ" },  // 7
  { "CountSet", "ｶｲｽｳｾｯﾃｲ" },    // 8
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
  { "AutoPlay", "ｵｰﾄﾌﾟﾚｲ" },           // 0
  { "Arena", "ﾄｳｷﾞｼﾞｮｳ" },             // 1
  { "Infernal", "ｺﾞｸｾﾝｷｮｳ" },          // 2
  { "Forlorn", "ﾄｳﾉﾋｷｮｳ" },            // 3
  { "ArenaAuto   ", "ﾄｳｷﾞｼﾞｮｳｵｰﾄ " },  // 4
  { "ItemGath", "ﾄｸｻﾝﾋﾝ" },            // 5
};
// value, mode 4  / amiibo福引
const char *strings_F[][2] = {
  { "amiibo", "ｱﾐｰﾎﾞ" },     // 0
  { "Lottery", "ﾌｸﾋﾞｷ  " },  // 1
  { "Config", "ｾｯﾃｲ   " },   // 2
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
void startUp() {
  switch (consoleType) {
    case 0:
      for (char i = 0; i < 5; i++) {
        holdButton(Button::L, 0);
        holdButton(Button::R, 100);
        releaseButton(Button::L, 0);
        releaseButton(Button::R, 100);
      }
      break;
    case 1:
      pushButton(Button::HOME, 50, 600);
      break;
  }
}

/* ============================================================== */
void setup() {
  // EEPROMからデータを読み込む
  if ((EEPROM.read(0) == 0 || EEPROM.read(0) == 1) && (EEPROM.read(1) == 0 || EEPROM.read(1) == 1) && (EEPROM.read(2) == 0 || EEPROM.read(2) == 1)) {
    bool savedConsoleType = EEPROM.read(0);
    bool savedMappingR = EEPROM.read(1);
    bool savedLanguageFlag = EEPROM.read(2);
    savedDayDate = EEPROM.read(3);
    savedMonthDate = EEPROM.read(4);
    savedYearDate = EEPROM.read(5);
    // 読み込んだデータを使って初期設定などを行う
    consoleType = savedConsoleType;
    mappingR = savedMappingR;
    languageFlag = savedLanguageFlag;
    dayDate = savedDayDate;
    monthDate = savedMonthDate;
    yearDate = savedYearDate;
  }
  /* 決定ボタン */
  confirmButton = (consoleType == 0) ? Button::A : Button::B;
  cancelButton = (consoleType == 0) ? Button::B : Button::A;
  /* Rボタン入れ替え */
  mappingR1 = (mappingR == 0) ? Button::R : Button::ZR;
  mappingR2 = (mappingR == 0) ? Button::ZR : Button::R;

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("MONSTER HUNTER");

  /* コントローラー接続 */
  startUp();

  /* LCDの初期値 */
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RISE:SUNBREAK_");
  lcd.print(consoleType == 0 ? "NS" : "PS");

  startTime = millis();
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
  lcd.print((languageFlag == 0) ? strings[mode][0] : jp(strings[mode][1]));

  lcd.setCursor(13, 0);
  lcd.print(">LR");
}

/* ============================================================== */
/*　各モードのデータ設定時のカーソル表示位置*/
void cursorPosition() {
  switch (mode) {
    case 0:
      if (curPos == 1) lcd.setCursor(6, 1);        // 年の位置
      else if (curPos == 2) lcd.setCursor(9, 1);   // 月の位置
      else if (curPos == 3) lcd.setCursor(12, 1);  // 日の位置
      break;
    case 1:
      if (curPos == 1) lcd.setCursor(12, 1);       // 1000の位
      else if (curPos == 2) lcd.setCursor(13, 1);  // 100の位
      else if (curPos == 3) lcd.setCursor(14, 1);  // 10の位
      else if (curPos == 4) lcd.setCursor(15, 1);  // 1の位
      break;
    case 2:
      if (curPos == 1) lcd.setCursor(11, 1);       //
      else if (curPos == 2) lcd.setCursor(12, 1);  //
      else if (curPos == 3) lcd.setCursor(14, 1);  //
      else if (curPos == 4) lcd.setCursor(15, 1);  //
      else if (curPos == 5) lcd.setCursor(14, 0);  //
      else if (curPos == 6) lcd.setCursor(15, 0);  //
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
  if (dateSet) lcd.cursor();  // カーソル常時表示
}

/* ============================================================== */
/* 1列目LCD */
void commonLcdRow1() {
  lcd.setCursor(0, 0);
  lcd.print(value);
  lcd.print(".");
  displayString(value, mode);
}
/* 2列目LCD */
void commonLcdRow2() {
  lcd.setCursor(0, 1);
  lcd.print("M");
  lcd.print(mode);
  lcd.print(".");
  displayString(0, mode);
}

/* LCD日付データ表示 */
void showLcdDate() {
  if (languageFlag == 0) {
    char text[10];
    sprintf(text, "%02d/%02d/%02d", monthDate, dayDate, yearDate);  // "MM/DD/YY"
    lcd.print(text);
  } else if (languageFlag == 1) {
    char text[10];
    sprintf(text, "%02d/%02d/%02d", yearDate, monthDate, dayDate);  // "YY/MM/DD"
    lcd.print(text);
  }
}


/* ============================================================== */
void loop() {

  modeNow = read_LCD_buttons(analogRead(0));

  // DOWN - UPでモード切り替え
  if (!dateSet && (modeNow == btnDOWN || modeNow == btnUP) && modeOld == btnNONE && value != -1) {
    if (!memory) {
      if (modeNow == btnUP) mode++;
      else if (modeNow == btnDOWN) mode--;

      // ゲーム機種によるモード制限
      if (consoleType == 0) mode = (mode > 4) ? 0 : (mode < 0) ? 4
                                                               : mode;
      else if (consoleType == 1) mode = (mode > 3) ? 0 : (mode < 0) ? 3
                                                                    : mode;
      prg = 0;
      delay(250);
    }

    settingMemory();  // EEPROMに書き込む(モード0。valueが1から4、設定変更をした場合)
    value = 0;
    dateSet = false;
    initialLcd = true;
    if (mode != 3) prg = 0;  // 闘技場オートクエスト準備初期化

    // 2列目LCD表示
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("M");
    lcd.print(mode);
    lcd.print(".");
    displayString(0, mode);  // マクロモード項目
  }
  modeOld = modeNow;

  /* マクロ項目*/
  switch (mode) {
    // モード0の動作
    case 0:
      if (!initialLcd) {
        lcd.setCursor(0, 1);
        lcd.print("v3.8");
        if (languageFlag == 0) lcd.print(" MODE>UP-DWN");
        else if (languageFlag == 1) lcd.print(jp(" ﾓｰﾄﾞ>ｳｴorｼﾀ"));
      } else if (value == 0) {
        lcdSelect();
        lcd.setCursor(3, 1);
        displayString(mode, 5);  // configuration
      }
      mode0();
      break;
    // モード1の動作
    case 1:
      if (value == 0) lcdSelect();
      mode1();
      break;
    // モード2の動作
    case 2:
      if (value == 0) lcdSelect();
      mode2();
      break;
    // モード3の動作
    case 3:
      if (value == 0) lcdSelect();
      mode3();
      break;
    // モード4の動作
    case 4:
      if (value == 0) lcdSelect();
      mode4();
      break;
  }
}
