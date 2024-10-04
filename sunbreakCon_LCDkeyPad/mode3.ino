/*  
 *  概要／
 *  決戦場や採取素材前で放置で各種コマンドを進めるプログラム
 *  
 *  種類／
 *  1.  オートクエストの準備設定  *は初期値
 *      1. 決戦場の場所／闘技場*、極泉郷、塔の秘境
 *      2. 体力回復(大)の旋律ボタン／X*、A、XA
 *      3. 疾替えの書の色／蒼*、朱
 *      4. オートマクロ開始位置／クエスト受注から、団子食事から*、クエスト出発から、モンスター前から
 *      
 *  2.  1で設定した項目でオート開始
 *      抜刀後に旋律タイプの変更可、複数多頭クエストの場合ターゲット変更マクロ指定
 *      クエスト中または討伐後selectボタンで停止、1分以内は再開可、停止から1分以上で1の設定からオートマクロ可能
 *      抜刀前までのマクロ実行中に停止すると、どの段階からでも抜刀からマクロ開始
 *      ※ 実行中にボタン  上=X旋律、右=A旋律、左=XA旋律、下=多頭クエスト用（ターゲット変更）
 *      
 *  3.  Aボタンのリピート(2秒間隔)単純なマクロ
 *    ※ 実行中に下を押すと採集に移動を含めたマクロ
 *  
*/

void mode3() {
  if (keys == btnSELECT && keysOld == btnNONE) {
    delay(250);
    if (value == 1) {
      setupMode = !setupMode;
      lcd.clear();  // LCD初期化
      lcd.setCursor(3, 1);
      if (setupMode) {
        lcdArea();
        setPos = 0;
      }
    }
    if (value == 2) pause = !pause;
    if (value >= 2 && value <= 3) {
      if (display) {
        if (point == 0) prg = 0;
        else if (point == 1) prg = 2;
        else if (point == 2) prg = 4;
        else if (point == 3) prg = 6;
      }
      runMode = !runMode;
      display = false;
      lcd.setCursor(11, 1);
      lcd.print(runMode ? "STP>S" : "SRT>S");
    }
  }
  /* オート設定 */
  if (!setupMode && !runMode && value != 0) {
    lcdAuto();
  }
  if (setupMode && !runMode && value != 0) {
    /* 設定終了 */
    lcd.setCursor(3, 1);
    switch (setPos) {
      case 0:
        lcdArea();  // LCD表示
        break;
      case 1:
        lcdMelody();  // LCD表示
        break;
      case 2:
        lcdScroll();  // LCD表示
        break;
      case 3:
        lcdPoint();  // LCD表示
        break;
    }
    /* 各設定 */
    if (keys == btnUP || keys == btnDOWN) {
      int increment = (keys == btnUP) ? 1 : -1;
      setPos += increment;
      if (setPos < 0) setPos = 3;
      else if (setPos > 3) setPos = 0;
      delay(300);
      lcd.clear();  // LCD初期化
    }
    if (keys == btnRIGHT || keys == btnLEFT) {
      delay(300);
      lcd.clear();  // LCD初期化
      lcd.setCursor(3, 1);
      switch (setPos) {
        case 0:
          areaPick += (keys == btnRIGHT) ? 1 : -1;  // areaPickを増減
          if (areaPick > 2) areaPick = 0;           // 0-2トグル
          else if (areaPick < 0) areaPick = 2;
          area = areaPick;  // area に反映
          lcdArea();        // LCD表示
          break;
        case 1:
          melodyPick += (keys == btnRIGHT) ? 1 : -1;  // melodyPickを増減
          if (melodyPick > 2) melodyPick = 0;         // 0-2トグル
          else if (melodyPick < 0) melodyPick = 2;
          melody = melodyPick;  // melody に反映
          lcdMelody();          // LCD表示
          break;
        case 2:
          scrollPick += (keys == btnRIGHT) ? 1 : -1;  // scrollPickを増減
          if (scrollPick > 1) scrollPick = 0;         // 0-1トグル
          else if (scrollPick < 0) scrollPick = 1;
          scroll = scrollPick;  // scroll に反映
          lcdScroll();          // LCD表示
          break;
        case 3:
          pointPick += (keys == btnRIGHT) ? 1 : -1;  // pointPickを増減
          if (pointPick > 3) pointPick = 0;          // 0-3トグル
          else if (pointPick < 0) pointPick = 3;
          point = pointPick;  // point に反映
          lcdPoint();         // LCD表示
          break;
      }
    }
    /* 設定LCD表示 */
    lcd.setCursor(0, 0);
    lcd.print(setPos + 1);
    lcd.print(".");
    const char* setStrings[] = {
      "AREA", "MELODY", "SkillScroll", "Start Point"
    };
    lcd.print(setStrings[(int)setPos]);  // 文字列を配列で管理
    lcd.setCursor(13, 0);
    lcd.print(">UD");
    commonLcdRow2();  // 2列目LCD0-2
    lcd.setCursor(13, 1);
    lcd.print(">LR");
  }

  /*  オートプレイ  */
  if (!runMode && !setupMode) {
    if ((keys == btnRIGHT || keys == btnLEFT)) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      value += direction;
      if (value < 1) value = 3;
      else if (value > 3) value = 1;
      delay(300);
      lcd.noCursor();
      lcd.clear();  // LCD初期化
      lcdAuto();    // LCD表示
    }
  }  // !runModeここまで
  /* オート実行中のモーション切り替え */
  if (runMode && !setupMode) {
    // runModeがtrueの時に下ボタンでtargetOnを切り替え
    if (keys == btnDOWN && keysOld == btnNONE && value >= 2 && value <= 3) targetOn = !targetOn;
    // runModeがtrueの時に上左右ボタンでmelodyTypeを切り替え
    if (keys == btnUP && value == 2) melody = 0;     // X旋律
    if (keys == btnRIGHT && value == 2) melody = 1;  // A旋律
    if (keys == btnLEFT && value == 2) melody = 2;   // XA旋律
  }
  keysOld = keys;

  /* オート実行中 */
  if (runMode && value >= 2 && value <= 3) {
    if (isFirstRun) {
      // stopTimeが60秒経過しているかどうかをチェックしてから初期化
      if (!reStart || startTimeS == 0) {
        startTimeS = startTime = referenceTime = millis();
        elapsed = 0;
      }
      startTimeA = startTimeB = millis();  //　startTimeA startTimeB の初期化
      pause = false;
    }
    pause = false;
    switch (value) {
      case 2:
        lcd.setCursor(2, 0);
        lcdArea();
        if (prg >= 0 && prg <= 5) {
          lcdCruise();          // 実行中項目LCD表示
          autoQuestCruising();  // 団子からオート
        }
        if (prg == 6) {
          commonLcdMacro();  // LCD共通マクロ
          autoArea();        // 決戦場
          if (targetOn) target();
        }
        break;
      case 3:
        lcdItem();                   // 特産品LCD
        elapsedTime();               // LCD表示
        if (targetOn) repeatMove();  // 2分置きに移動
        repeatA();
        break;
    }
  }
  /* オートストップ */
  if (!runMode && value >= 2 && value <= 3) {
    if (!reStart && !revert) {
      lcd.clear();    // 画面をクリア
      lcdAuto();      // LCD表示
      prg = point;    // プログラムを初期化
      revert = true;  // 処理が実行されたことを記録
    }
    if (reStart) {
      revert = false;  // 60秒経過中ならフラグをリセット
    }
    if (pause) {
      stickNeutral(Stick::LEFT);
      lcd.setCursor(value == 2 ? 10 : 9, 0);  // 決戦場と特産品の場合とのLCD位置の調整
      lcd.print("STOP");
      lcd.setCursor(10, 1);
      lcd.print("RSRT>S");
      skipExec = false;
      pause = false;
      isFirstRun = true;
      times = 0;
      prg = 6;
      stopTime = millis();  // ストップ時にstopTimeを計測開始
      delay(400);
    }
    reStart = (millis() - stopTime <= 60000);  // ストップから60秒経過中
    if (!reStart) {
      startTime = 0;
      display = true;
    }  // ストップして60秒経過でstartTimeをリセット
  }
}


/* LCD制御=========================================================== */
/* 共通マクロ */
void commonLcdMacro() {
  lcd.noBlink();
  lcd.setCursor(3, 1);
  lcdMelody();  // LCD表示旋律タイプ
  lcd.print(" ");
  elapsedTime();  // LCD表示経過時間
}
/* 実行中LCD */
const char* autoStrings[] = {
  "Plaza..", "Accept.", "TeaShop", "Dango..", "Loading", "Hunting"
};
void lcdCruise() {
  lcd.setCursor(2, 0);
  displayString(value, 3);
  lcd.setCursor(3, 1);
  if (prg >= 0 && prg <= 5) {
    lcd.print(autoStrings[prg]);  // 文字列を配列で管理
    lcd.blink();
  }
}
/* エリアタイプLCD */
void lcdArea() {
  const char* areaStrings[][2] = {
    { "Arena", "ﾄｳｷﾞｼﾞｮｳ" },     // area == 0
    { "Infernal", "ｺﾞｸｾﾝｷｮｳ" },  // area == 1
    { "Forlorn", "ﾄｳﾉﾋｷｮｳ" },    // area == 2
  };
  lcd.print((languageFlag == 0) ? areaStrings[(int)area][0] : jp(areaStrings[(int)area][1]));
}
/* 旋律タイプLCD */
void lcdMelody() {
  const char* melodyStrings[][2] = {
    { "MelodyX", "ｾﾝﾘﾂ X" },    // melody == 0
    { "MelodyA", "ｾﾝﾘﾂ A" },    // melody == 1
    { "MelodyXA", "ｾﾝﾘﾂ XA" },  // melody == 2
  };
  lcd.print((languageFlag == 0) ? melodyStrings[(int)melody][0] : jp(melodyStrings[(int)melody][1]));
}
/* 疾替えの書タイプLCD */
void lcdScroll() {
  const char* scrollStrings[][2] = {
    { "BLUE", "ｱｵﾉｼｮ" },  // Scroll == 0
    { "RED", "ｼｭﾉｼｮ" },   // Scroll == 1
  };
  lcd.print((languageFlag == 0) ? scrollStrings[(int)scroll][0] : jp(scrollStrings[(int)scroll][1]));
}
/* 開始位置LCD */
void lcdPoint() {
  const char* pointStrings[][2] = {
    { "Accept.Q", "ｼﾞｭﾁｭｳｶﾗ" },  // point == 0
    { "Dango", "ｼｮｸｼﾞｶﾗ" },      // point == 1
    { "Depart.Q", "ｼｭｯﾊﾟﾂｶﾗ" },  // point == 2
    { "Hunting", "ﾓﾝｽﾀｰﾏｴｶﾗ" },     // point == 3
  };
  lcd.print((languageFlag == 0) ? pointStrings[(int)point][0] : jp(pointStrings[(int)point][1]));
}
/* 特産品LCD */
void lcdItem() {
  lcd.setCursor(3, 1);
  const char* text = (languageFlag == 0)
                       ? (targetOn ? "OnTheGo" : "RepeatA")
                       : (targetOn ? jp("ｲﾄﾞｳｻｲｼｭ") : jp("ﾘﾋﾟｰﾄA  "));
  lcd.print(text);
}
/* 経過時間LCD表示 */
void elapsedTime() {
  unsigned long curMillis = millis();  // 現在の時間を取得
  // 1秒ごとに点滅を切り替える
  if (curMillis - preMillis >= interval) {
    colonVisible = !colonVisible;  // 表示状態を反転させる
    preMillis = curMillis;         // 前回の時間を更新
  }
  // 経過時間を計算
  unsigned long elapsed = curMillis - referenceTime;
  unsigned long minutes = elapsed / 60000;  // 経過時間を分に変換
  // 経過時間表示位置
  lcd.setCursor((value == 2) ? 10 : 9, 0);
  lcd.print(targetOn && (value >= 2 && value <= 3) ? "TGT" : "RUN");
  lcd.print(colonVisible ? ":" : " ");
  char text[4];
  sprintf(text, (value == 2) ? "%2lu" : "%3lu", minutes);  // 経過時間を表示
  lcd.print(text);
}
// 表示切り替え関数
void displayPrev() {
  if (display) {
    lcd.setCursor(2, 0);
    switch (dispIndex) {
      case 0:
        lcdArea();
        break;
      case 1:
        lcdMelody();
        break;
      case 2:
        lcdScroll();
        break;
      case 3:
        lcdPoint();
        break;
    }
    lcd.print("    ");
    unsigned long curDispMillis = millis();
    if (curDispMillis - preDispMillis >= dispInterval) {
      dispIndex = (dispIndex + 1) % 4;  // 0〜3の範囲でインデックスを循環
      preDispMillis = curDispMillis;    // 前回の時間を更新
    }
  }
}
/* LCD表示 */
void lcdAuto() {
  // 1列目LCD
  if (value >= 1 && value <= 3) {
    commonLcdRow1();  // 1列目LCD0-1
  }
  if (value == 2) displayPrev();
  else displayString(value, mode);  // 1列目LCD2-
  // 2列目LCD
  commonLcdRow2();         // 2列目LCD0-2
  displayString(0, mode);  // 2列目LCD3-
  lcd.setCursor(11, 1);
  if (value == 1) {
    lcd.print("SET>S");
  }
  if (value >= 2 && value <= 3) {
    lcd.print("SRT>S");
  }
}