/* 
 * 傀異マカ錬金
 */
 
void meldingVigor() {
  if (times == 0) {
    pushHat(Hat::UP, 50, 40, 2);  // 覇気までカーソル移動
  }

  if (times <= 10) {
    //マカ錬金回数
    countM++;
    char text[4];
    lcd.setCursor(13, 0);
    sprintf(text, "%3d", countM);
    lcd.print(text);

    pushButton(confirmButton, 40, 100);  // 釜選択
    /* 1つ目の素材選択まで */
    pushButton(Button::R, 40, 40, (page_1 - 1));  // 1素材目ページ移動
    pushHat(Hat::DOWN, 40, 40, (line_1 - 1));     // 素材まで移動
    pushButton(confirmButton);                    // 素材選択
    pushHat(Hat::DOWN);                           // 最大個数
    if (page_2 != 0) pushHat(Hat::DOWN);          // 個数調整
    pushButton(confirmButton);                    // 選択完了

    /* 2つ目の素材選択まで */
    if (page_2 != 0) {
      // 2素材目ページ移動
      if (page_2 - page_1 < 0) {
        pushButton(Button::L, 40, 40, (page_1 - page_2));
      } else {
        pushButton(Button::R, 40, 40, (page_2 - page_1));
      }
      // 2素材目の選択
      char materialDigit = line_2 - line_1;
      if (line_2 < line_1) {
        pushHat(Hat::UP, 40, 40, abs(materialDigit));  // 素材まで移動
      } else {
        pushHat(Hat::DOWN, 40, 40, materialDigit);  // 素材まで移動
      }
      pushButton(confirmButton);  // 素材選択
      pushHat(Hat::DOWN);         // 指定個数
      pushButton(confirmButton);  // 選択完了
    }
    pushButton(minusButton);    // 決定までのカーソル移動
    pushButton(confirmButton);  // 決定
    pushHat(Hat::RIGHT);        // はい  に移動
    pushButton(confirmButton);  // はい  決定
    times++;
  }
  //護石受取り
  if (times == 10) {
    pushHat(Hat::DOWN, 50, 50, 2);  // 決定までのカーソル移動
    pushButton(confirmButton);      // 釜選択
    meldingStop = 2;
    repeatMelding();  // 錬成結果画面動作
  }
}

/*  2.傀異マカ錬金 円環  */
void meldingCyclus() {
  if (times > 0 && times <= 10) {
    pushButton(confirmButton, 40, 150);  // 釜選択
    pushButton(confirmButton, 40, 90);   // スキル選択
  }
  if (times <= 10) {
    // マカ錬金回数
    countM++;
    char text[4];
    lcd.setCursor(13, 0);
    sprintf(text, "%3d", countM);
    lcd.print(text);

    for (int i = 0; i < 7; i++) {
      pushButton(confirmButton);
      pushHat(Hat::RIGHT, 50, 90);
    }
    pushButton(confirmButton);
    pushHat(Hat::DOWN, 50, 90);
    pushButton(confirmButton);  // 9
    pushHat(Hat::RIGHT, 50, 90);
    pushButton(confirmButton);  // 10

    pushButton(minusButton, 50, 90);    // 決定までのカーソル移動
    pushButton(confirmButton, 40, 90);  // 決定
    pushHat(Hat::RIGHT);                // はい  に移動
    pushButton(confirmButton, 40, 90);  // はい  決定
    times++;
  }
  // 護石受取り
  if (times == 10) {
    pushHat(Hat::DOWN);         // 決定までのカーソル移動
    pushButton(confirmButton);  // 釜選択
    meldingStop = 2;
    runMode = !runMode;
  }
}
/*  護石受取り画面での処理  */
void repeatMelding() {
  if (repeatCount > 0) {
    pushHat(Hat::UP);           //
    pushButton(confirmButton);  // 全て受け取る
    melding = true;
  } else if (melding && repeatCount == 0) {
    melding = false;
    runMode = !runMode;
  } else {
    runMode = !runMode;
  }
}

/*  装備売却  */
void sellEquipment() {
  if (times <= 50) {
    times++;
    lcd.setCursor(13, 0);
    lcd.cursor();
    pushButton(Button::X);      // 選択
    pushButton(confirmButton);  // YES
    lcd.noCursor();
    if (times % 10 == 0) {
      pushHat(Hat::DOWN);
    } else {
      pushHat(Hat::RIGHT);
    }
  }
  if (times == 50) {
    pushButton(Button::L, 100);
    meldingStop = 3;
    lcd.noCursor();
    if (repeatCount > 0) {
      melding = true;
    } else if (melding && repeatCount == 0) {
      melding = false;
      runMode = !runMode;
    } else {
      runMode = !runMode;
    }
  }
}