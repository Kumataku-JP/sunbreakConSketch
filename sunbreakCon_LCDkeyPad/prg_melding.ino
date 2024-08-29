/* v1.2*/


/*  1.傀異マカ錬金 素材複数選択  覇気  */
void meldingVigor() {
  if (lap == 0) {
    pushHat(Hat::UP, 50, 40, 2);  // 覇気までカーソル移動
  }

  if (lap <= 10) {
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
    pushHat(Hat::DOWN);                           // 個数調整
    if (page_2 != 0) {
      pushHat(Hat::DOWN);  // 個数調整
    }
    pushButton(confirmButton);  // 選択完了

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
    pushButton(Button::MINUS);  // 決定までのカーソル移動
    pushButton(confirmButton);  // 決定
    pushHat(Hat::RIGHT);        // はい  に移動
    pushButton(confirmButton);  // はい  決定
    lap++;
  }
  //護石受取り
  if (lap == 10) {
    pushHat(Hat::DOWN, 50, 50, 2);  // 決定までのカーソル移動
    pushButton(confirmButton);      // 釜選択
    meldingStop = 3;
    repeatMelding();  // 錬成結果画面動作
  }
}

/*  2.傀異マカ錬金 円環  */
void meldingCyclus() {
  if (lap > 0 && lap <= 10) {
    pushButton(confirmButton, 40, 150);  // 釜選択
    pushButton(confirmButton, 40, 90);   // スキル選択
  }
  if (lap <= 10) {
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

    pushButton(Button::MINUS, 50, 90);  // 決定までのカーソル移動
    pushButton(confirmButton, 40, 90);  // 決定
    pushHat(Hat::RIGHT);                // はい  に移動
    pushButton(confirmButton, 40, 90);  // はい  決定
    lap++;
  }
  // 護石受取り
  if (lap == 10) {
    pushHat(Hat::DOWN);         // 決定までのカーソル移動
    pushButton(confirmButton);  // 釜選択
    meldingStop = 2;
    steyMode2 = false;
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
    steyMode2 = false;
  } else {
    steyMode2 = false;
  }
}

/*  装備売却  */
void sellEquipment() {
  if (lap <= 50) {
    lap++;
    lcd.setCursor(13, 0);
    lcd.cursor();
    pushButton(Button::X);      // 選択
    pushButton(confirmButton);  // YES
    lcd.noCursor();
    if (lap % 10 == 0) {
      pushHat(Hat::DOWN);
    } else {
      pushHat(Hat::RIGHT);
    }
  }
  if (lap == 50) {
    pushButton(Button::R, 100);
    meldingStop = 2;
    steyMode2 = false;
    lcd.noCursor();
  }
}