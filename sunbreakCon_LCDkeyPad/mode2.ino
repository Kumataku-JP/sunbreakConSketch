/*  モンハンライズサンブレイク  マカ錬金
 *  
 *  概要／
 *  マカ錬金を10連するマクロ
 *  10回抽選して受取り画面で停止
 *  途中でストップ可
 *  
 *  準備／
 *  「迅錬丹」を使用するを選択しておく
 *  カーソル初期位置共通／「護石を受け取る」に置き実行
 *
 *  1.覇気     使用素材を6で指定セットしカーソル初期位置を「護石を受け取る」に置き実行
 *  
 *  2.円環    「傀異マカ錬金・円環」を選択し、錬金したいスキル選択し、
 *            素材にしたいページにカーソルを置き実行
 *            ※ただし、最下段と装備している護石がある場合には最上段列にカーソルを置かない
 *  
 *  3.覇気で使用する素材の位置を指定
 *    ab/cd  1つ目（a=ページ、b=上から何個目）  ／  2つ目（c=ページ、d=上から何個目）
 *    初期値  11/00  （1ページ目の1個目を指定）
 *    2つ目の素材指定なしで1つ目のみでマカ錬金可能
 *
 *  4.装備売却  1ページ(50装備)分を売却
 *            画面が高速にチラつくので癲癇等に注意
 *
*/

void mode2() {

  keys = read_LCD_buttons(analogRead(0));
  /*  マカ錬金  */
  //マカ錬金選択
  if (!setupMode) {
    if (keys == btnRIGHT || keys == btnLEFT) {
      value += (keys == btnRIGHT) ? 1 : -1;
      value = toggleValue(value, 1, 4);  // 1-4トグル
      lcd.clear();
      lcdMelding();  // LCD表示
      delay(300);
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      runMode = !runMode;
    }
    keysOld = keys;

    if (runMode) {
      /* 素材複数選択設定 */
      if (value == 1) {
        setupMode = true;
        meldingStop = 4;
      } else if (value == 2) {
        if (repeatCount == 0) {
          meldingStop = 1;
          lcd.setCursor(10, 0);
          lcd.print("Run");
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          meldingVigor();
        }
        if (repeatCount > 0) {
          meldingStop = 1;
          lcd.noCursor();
          lcd.setCursor(7, 0);
          lcd.print("  ");
          lcd.setCursor(9, 0);
          lcd.print("S");
          showLcdCountMelding(repeatCount);  // カウントLCD表示
          lcd.setCursor(12, 0);
          lcd.print("R");
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          meldingVigor();
          if (lap % 10 == 0) {
            repeatCount--;  // カウントをデクリメント
            lap = 0;
          }
          if (repeatCount == 0) {
            meldingStop = 5;
            repeatCount = numDate;
            runMode = false;
          }
        }
      } else if (value >= 3 && value <= 4) {
        lcd.setCursor(10, 0);
        lcd.print("Run");
        lcd.setCursor(10, 1);
        lcd.print(" STP>S");
      }
      if (value == 3) {
        meldingStop = 1;
        meldingCyclus();
      } else if (value == 4) {
        meldingStop = 2;
        sellEquipment();  // 装備売却
      }
    }
    /* ストップ後のLCD表示 */
    if (!runMode) {
      switch (meldingStop) {
        case 0:
          break;
        case 1:
          lcd.setCursor(10, 1);
          lcd.print("RSRT>S");
          lap = 0;
          delay(500);
          meldingStop = 0;
          break;
        case 2:
          lcdMelding();  //LCD表示
          lcd.setCursor(10, 0);
          lcd.print("End");
          lcd.setCursor(10, 1);
          lcd.print(" SRT>S");
          lap = 0;
          delay(500);
          meldingStop = 0;
          break;
        case 3:
          lcdSetMaterial();
          lcd.setCursor(10, 0);
          lcd.print("End");
          lap = 0;
          delay(500);
          meldingStop = 0;
          break;
        case 4:
          lcd.setCursor(10, 1);
          lcd.print("C");
          delay(500);
          meldingStop = 0;
          break;
        case 5:
          lcdMelding();  //LCD表示
          lap = 0;
          delay(500);
          meldingStop = 0;
          break;
      }
    }
  }  //  !setupModeここまで

  /* 素材複数選択設定*/
  if (setupMode) {
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      curPos += (keys == btnRIGHT) ? 1 : -1;
      curPos = toggleCurPos(curPos, 1, 6);  // 1-6トグル
    }
    if (keys == btnUP || keys == btnDOWN) {
      int increment = (keys == btnUP) ? 1 : -1;
      char *matl = nullptr;  // ポインタを使用して対象変数を指す
      switch (curPos) {
        case 1: matl = &page_1; break;
        case 2: matl = &line_1; break;
        case 3: matl = &page_2; break;
        case 4: matl = &line_2; break;
        case 5:
        case 6:
          digitsM[curPos - 5] = (digitsM[curPos - 5] + increment + 10) % 10;
          delay(100);
          return;
      }
      if (matl) {
        *matl += increment;
        if (*matl > ((*matl == page_2 || *matl == line_2) ? 9 : 7))
          *matl = ((*matl == page_2 || *matl == line_2) ? 0 : 1);
        else if (*matl < ((*matl == page_2 || *matl == line_2) ? 0 : 1))
          *matl = ((*matl == page_2 || *matl == line_2) ? 9 : 7);
      }
      delay(100);
    }
    lcd.setCursor(14, 0);
    updateCountMelding();
    lcd.setCursor(3, 1);
    if (languageFlag == 0) lcd.print("MATL > ");
    else if (languageFlag == 1) lcd.print(jp("ｿｻﾞｲｲﾁ>"));
    lcd.setCursor(10, 1);
    lcd.print("S");
    lcdSetMaterial();  // LCDに表示する
    delay(100);
    /* 設定終了 */
    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      if (curPos) {
        setupMode = false;
        runMode = false;
        repeatCount = numDate;
        lcd.noCursor();
        lcdMelding();
        lcd.setCursor(9, 1);
        lcd.print(" ");
      }
    }
    keysOld = keys;  // 前回のキー状態を記録
  }                  // setupModeここまで
}  // mode2ここまで

/* LCD制御=========================================================== */
/* 素材設定LCD表示 */
void lcdSetMaterial() {
  char text[2];
  sprintf(text, "%01d", page_1);
  lcd.print(text);
  sprintf(text, "%01d", line_1);
  lcd.print(text);
  lcd.print("/");
  sprintf(text, "%01d", page_2);
  lcd.print(text);
  sprintf(text, "%01d", line_2);
  lcd.print(text);
  cursorPosition();
}

// のこりマカ錬金周回数をディスプレイに表示
void updateCountMelding() {
  numDate = digitsM[0] * 10 + digitsM[1];
  for (int i = 0; i < 2; i++) {
    lcd.print(digitsM[(int)i]);
  }
}
/* のこりマカ回数LCD表示 */
void showLcdCountMelding(int count) {
  char text[3];
  sprintf(text, "%02d", count);
  lcd.print(text);
}

/* LCD表示 */
void lcdMelding() {
  commonLcdRow1();  // 1列目LCD
  commonLcdRow2();  // 2列目LCD
  lcd.setCursor(10, 1);
  if (value >= 1 && value <= 3) lcd.print(" SRT>S");
  if (value == 1) {
    lcd.setCursor(11, 0);
    lcd.print("> T");
    for (int i = 0; i < 2; i++) lcd.print(digitsM[(int)i]);
    lcd.setCursor(10, 1);
    lcd.print("C");
    lcdSetMaterial();
  }
  if (value == 2) {
    lcd.setCursor(10, 0);
    lcd.print("M");
    lcdSetMaterial();
    if (repeatCount > 0) {
      lcd.setCursor(7, 0);
      lcd.print("S");
      updateCountMelding();
    }
  }
  if (value == 4) lcd.print("SELL>S");
}