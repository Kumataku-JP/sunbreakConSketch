/*  
 *  概要／
 *  マカ錬金を10連するマクロ
 *  10回抽選して受取り画面で停止
 *  途中でストップ可
 *  
 *  準備／
 *  「迅錬丹」を使用するを選択しておく
 *  カーソル初期位置共通／「護石を受け取る」に置き実行
 *
 *  1.覇気で使用する素材の位置を指定
 *    ab/cd  1つ目（a=ページ、b=上から何個目）  ／  2つ目（c=ページ、d=上から何個目）
 *    初期値  11/00  （1ページ目の1個目を指定）
 *    2つ目の素材指定なしで1つ目のみでマカ錬金可能
 *
 *  2.覇気     使用素材を1.設定で指定しカーソル初期位置を「護石を受け取る」に置き実行
 *  
 *  3.円環    「傀異マカ錬金・円環」を選択し、錬金したいスキル選択し、
 *            素材にしたいページにカーソルを置き実行
 *            ※ただし、最下段と装備している護石がある場合には最上段列にカーソルを置かない
 *  
 *
 *  4.装備売却  1ページ(50装備)分を売却し前のページ移動して終了
 *             装備BOXに貯まった護石を1.で連続錬金指定した数と同ページ分を売却可能。
 *             最終(護石50コうまっている)ページから前のページに自動売却して行くことで、最初のページ送りになりロックしていない武器等を誤って売却するのを防止
 *             ※画面が高速にチラつくので癲癇等に注意
 *
*/

void mode2() {
  /*  マカ錬金  */
  //マカ錬金選択
  if (!setupMode) {
    if (keys == btnRIGHT || keys == btnLEFT) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      value += direction;
      if (value < 1) value = 4;
      else if (value > 4) value = 1;
      delay(300);
      lcd.clear();   // LCD初期化
      lcdMelding();  // LCD表示
    }

    if (keys == btnSELECT && keysOld == btnNONE && value != 0) {
      runMode = !runMode;
    }
    keysOld = keys;

    if (runMode) {
      switch (value) {
        /* 素材複数選択設定 */
        case 1:
          setupMode = true;
          break;
          /* 神気マカ錬金 */
        case 2:
          // 周回なし
          if (!repeatLaps) {
            meldingStop = 1;
            lcd.clear();                 // LCD初期化
            commonLcdRow1();             // 1列目LCD0-1
            displayString(value, mode);  // 1列目LCD2-
            lcd.setCursor(10, 0);
            lcd.print("Run");
            commonLcdRow2();         // 2列目LCD0-2
            displayString(0, mode);  // 2列目LCD3-
            lcd.setCursor(10, 1);
            lcd.print(" STP>S");
            meldingVigor();
          }
          // 周回あり
          else if (repeatLaps) {
            meldingStop = 1;
            lcd.clear();                 // LCD初期化
            commonLcdRow1();             // 1列目LCD0-1
            displayString(value, mode);  // 1列目LCD2-
            lcd.setCursor(9, 0);
            lcd.print("S");
            showLcdCountMelding(repeatCount);  // カウントLCD表示
            lcd.setCursor(12, 0);
            lcd.print("R");
            commonLcdRow2();         // 2列目LCD0-2
            displayString(0, mode);  // 2列目LCD3-
            lcd.setCursor(10, 1);
            lcd.print(" STP>S");
            meldingVigor();
            if (times % 10 == 0) {
              repeatCount--;  // カウントをデクリメント
              times = 0;
            }
            if (repeatCount == 0) {
              meldingStop = 3;
              repeatCount = numDate;
              runMode = false;
            }
          }
          break;
          /* 円環マカ錬金 */
        case 3:
          meldingStop = 1;
          lcd.setCursor(10, 0);
          lcd.print("Run");
          lcd.setCursor(10, 1);
          lcd.print(" STP>S");
          meldingCyclus();
          break;
          /* 装備売却 */
        case 4:
          // 売却周回なし
          if (!repeatLaps) {
            meldingStop = 1;
            lcd.setCursor(10, 0);
            lcd.print("Run");
            lcd.setCursor(10, 1);
            lcd.print(" STP>S");
            sellEquipment();  // 装備売却
          }
          // 売却周回あり
          else if (repeatLaps) {
            meldingStop = 1;
            lcd.noCursor();
            lcd.setCursor(10, 0);
            lcd.print("Run ");
            showLcdCountMelding(repeatCount);  // カウントLCD表示
            lcd.setCursor(10, 1);
            lcd.print(" STP>S");
            sellEquipment();  // 装備売却
            if (times % 50 == 0) {
              repeatCount--;  // カウントをデクリメント
              times = 0;
            }
            if (repeatCount == 0) {
              meldingStop = 3;
              repeatCount = numDate;
              runMode = false;
            }
          }
          break;
      }
    }
    /* ストップ後のLCD表示 */
    if (!runMode) {
      if (meldingStop != 0) {
        switch (value) {
          case 3:
            lcdMelding();  //LCD表示
            lcd.setCursor(10, 0);
            lcd.print("End");
            times = 0;
            delay(500);
            meldingStop = 0;
            break;
        }
      }
      switch (meldingStop) {
        case 0:
          break;
        /* 中断 */
        case 1:
          lcd.setCursor(9, 0);
          lcd.print(" STP");
          lcd.setCursor(10, 1);
          lcd.print("RSRT>S");
          times = 0;
          meldingStop = 0;
          runMode = false;
          delay(500);
          break;
        /* マカ錬金終了 */
        case 2:
          lcd.setCursor(10, 0);
          lcd.print("End");
          lcd.setCursor(10, 1);
          lcd.print(" SRT>S");
          times = 0;
          meldingStop = 0;
          runMode = false;
          delay(500);
          break;
        /* 周回終了 */
        case 3:
          lcd.setCursor(9, 0);
          lcd.print(" End");
          lcd.setCursor(10, 1);
          lcd.print(" SRT>S");
          times = 0;
          meldingStop = 0;
          runMode = false;
          delay(500);
          break;
        case 4:
          runMode = false;
          break;
      }
    }
  }  //  !setupModeここまで

  /* 素材複数選択設定*/
  if (setupMode) {
    if ((keys == btnRIGHT || keys == btnLEFT) && keysOld == btnNONE) {
      int direction = (keys == btnRIGHT) ? 1 : -1;
      curPos += direction;
      if (curPos < 1) curPos = 6;
      else if (curPos > 6) curPos = 1;
    }
    if (keys == btnUP || keys == btnDOWN) {
      int increment = (keys == btnUP) ? 1 : -1;
      char *matl = nullptr;  // ポインタを使用して対象変数を指す
      switch (curPos) {
        /* 使用素材数設定 */
        case 1: matl = &page_1; break;
        case 2: matl = &line_1; break;
        case 3: matl = &page_2; break;
        case 4: matl = &line_2; break;
        /* 周回数設定 */
        case 5:
        case 6:
          int &digit = digitsM[curPos - 5];  // curPos - 5を一度だけ計算
          digit = (digit + increment + 10) % 10;
          break;
      }
      /* 使用素材数 */
      if (matl) {
        *matl += increment;
        // line_1の処理を個別に扱う
        if (matl == &line_1) {
          if (*matl > 7) *matl = 1;       // 7より大きくなったら1に戻す
          else if (*matl < 1) *matl = 7;  // 1より小さくなったら7に戻す
        } else {
          char maxVal = (*matl == page_1 || *matl == page_2) ? 9 : 7;
          char minVal = (*matl == page_1 || *matl == line_1) ? 1 : 0;
          if (*matl > maxVal) *matl = minVal;  // 上限チェック
          if (*matl < minVal) *matl = maxVal;  // 下限チェック
        }
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
        if (numDate > 0) repeatLaps = true;
        else repeatLaps = false;
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
  lcd.print((int)page_1);  // 数値を直接表示
  lcd.print((int)line_1);  // 数値を直接表示
  lcd.print("/");          // 区切り文字を表示
  lcd.print((int)page_2);  // 数値を直接表示
  lcd.print((int)line_2);  // 数値を直接表示
  cursorPosition();        // カーソル位置を更新
}

/* のこりマカ錬金周回数をディスプレイに表示 */
void updateCountMelding() {
  updateCountGeneric(numDate, digitsM, 2);
}

/* のこりマカ回数LCD表示 */
void showLcdCountMelding(int count) {
  if (count < 10) lcd.print("0");  // 1桁の場合は先頭に'0'を追加
  lcd.print(count);                // countの値をそのまま表示
}

/* LCD表示 */
void lcdMelding() {
  commonLcdRow1();             // 1列目LCD0-1
  displayString(value, mode);  // 1列目LCD2-
  commonLcdRow2();             // 2列目LCD0-2
  displayString(0, mode);      // 2列目LCD3-
  lcd.setCursor(10, 1);
  if (value >= 1 && value <= 3) lcd.print(" SRT>S");
  if (value == 1) {
    lcd.setCursor(11, 0);
    lcd.print("> T");
    updateCountMelding();
    lcd.setCursor(10, 1);
    lcd.print("C");
    lcdSetMaterial();
  }
  if (value == 2) {
    lcd.setCursor(10, 0);
    lcd.print("M");
    lcdSetMaterial();
    if (repeatLaps) {
      lcd.setCursor(7, 0);
      lcd.print("S");
      updateCountMelding();
    }
  }
  if (value == 4) {
    lcd.print("SELL>S");
    if (repeatLaps) {
      lcd.setCursor(13, 0);
      lcd.print("S");
      updateCountMelding();
    }
  }
}