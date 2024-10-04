/* 
 * amiibo福引
*/

/* カレンダー */
void setupModeting() {
  /* 閏年 */
  switch (yearDate % 4) {
    case 0:  //閏年
      leap = 29;
      break;
    case 1:
    case 2:
    case 3:  //閏年以外
      leap = 28;
      break;
  }
  switch (monthDate) {
    //1.3.5.7.8.10月
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
      if (dayDate < lastDay) oneDay();
      else if (dayDate == lastDay) oneMonth();
      break;
    //2月
    case 2:
      if (dayDate < leap) oneDay();
      else if (dayDate == leap) oneMonth();
      break;
    //4.6.9.11月
    case 4:
    case 6:
    case 9:
    case 11:
      if (dayDate < lastDay - 1) oneDay();
      else if (dayDate == lastDay - 1) oneMonth();
      break;
    //12月
    case 12:
      if (dayDate < lastDay) oneDay();
      else if (dayDate == lastDay) oneYear();
      break;
  }
}

/* 日更新関数 */
void oneDay() {
  screenSetting();
  if (prg == 4) {
    switch (languageFlag) {
      case 0:                              // mm-dd-yyyy-hh-mm-AM/PM-confirm
        pushButton(Button::A);             // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushButton(Button::A, 60, 40, 5);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        break;
      case 1:                              // yyyy-mm-dd-hh-mm-confirm
        pushButton(Button::A, 40, 40, 2);  // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushButton(Button::A, 60, 40, 3);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        break;
    }
    delay(200);
    dayDate++;
    prg++;
  }
}
/* 月日更新関数 */
void oneMonth() {
  screenSetting();
  if (prg == 4) {
    switch (languageFlag) {
      case 0:                              // mm-dd-yyyy-hh-mm-AM/PM-confirm
        pushHat(Hat::UP, 40, 40);          // 月更新
        pushButton(Button::A);             // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushButton(Button::A, 60, 40, 5);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        delay(100);
        break;
      case 1:                              // yyyy-mm-dd-hh-mm-confirm
        pushButton(Button::A, 40, 40, 2);  // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushHat(Hat::LEFT, 40, 40);        // 月に移動
        pushHat(Hat::UP, 40, 40);          // 月更新
        pushButton(Button::A, 60, 40, 4);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        delay(200);
        break;
    }
    monthDate++;
    dayDate = 1;
    prg++;
  }
}
/* 年月日更新関数 */
void oneYear() {
  screenSetting();
  if (prg == 4) {
    switch (languageFlag) {
      case 0:                              // mm-dd-yyyy-hh-mm-AM/PM-confirm
        pushHat(Hat::UP, 40, 40);          // 月更新
        pushButton(Button::A);             // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushButton(Button::A);             // 年に移動
        pushHat(Hat::UP, 40, 40);          // 年更新
        pushButton(Button::A, 60, 40, 4);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        delay(200);
        break;
      case 1:                              // yyyy-mm-dd-hh-mm-confirm
        pushHat(Hat::UP, 40, 40);          // 年更新
        pushButton(Button::A);             // 月に移動
        pushHat(Hat::UP, 40, 40);          // 月更新
        pushButton(Button::A);             // 日に移動
        pushHat(Hat::UP, 40, 40);          // 日更新
        pushButton(Button::A, 60, 40, 3);  // OKまで移動
        pushButton(Button::A, 100, 100);   // OK
        delay(200);
        break;
    }
    yearDate++;
    monthDate = 1;
    dayDate = 1;
    prg++;
  }
}

/* ============================================================== */
/*  HOME画面 */
// ゲームから時間設定画面まで
void screenSetting() {
  // ホーム画面から設定画面へ
  if (prg == 0) {
    pushButton(Button::HOME, 50, 600);
    pushHat(Hat::LEFT, 50, 120);
    pushHat(Hat::DOWN, 50, 90);
    pushHat(Hat::LEFT, 50, 90);
    pushButton(Button::A, 60, 400);
    prg++;
  } else if (prg == 1) {
    // 設定画面本体まで
    stickTilt(Stick::LEFT, 180, 100, 50);     // down
    stickTilt(Stick::RIGHT, 180, 100, 1650);  // down
    stickNeutral(Stick::LEFT);
    pushButton(Button::A, 100, 150);
    prg++;
  } else if (prg == 2) {
    // 日付と変更まで
    stickTilt(Stick::RIGHT, 180, 100, 600);  // down
    stickNeutral(Stick::RIGHT);
    pushButton(Button::A, 100, 200);
    prg++;
  } else if (prg == 3) {
    //インターネットで時間を合わせるオンオフ(時刻を現在時間にリセット)
    if (firstRun) {
      pushButton(Button::A, 100, 250);
      firstRun = false;  // 日付自動オン無効
    }
    // 時間設定まで
    stickTilt(Stick::LEFT, 180, 100, 50);    // down
    stickTilt(Stick::RIGHT, 180, 100, 100);  // down
    stickNeutral(Stick::LEFT);
    stickNeutral(Stick::RIGHT);
    pushButton(Button::A, 100, 200);
    prg++;
  }
}

/* ゲームに戻り福引き */
void lottery() {
  if (prg == 5) {
    pushButton(Button::HOME, 50, 600);
    pushButton(Button::HOME, 50, 800);
    // amiiboを読み込む
    pushButton(Button::A, 40, 100, 3);  // 前日の景品受取り処理
    pushButton(Button::A, 2200);        // amiibo読み込み中
    prg++;
  } else if (prg == 6) {
    pushButton(Button::A, 40, 200, 4);  // amiiboを読み込みました,amiibo福引,このamiiboで福引をおこないますか
    lcd.setCursor(10, 1);
    lcd.blink();
    pushButton(Button::A, 40, 100, 4);  // スキップ
    pushHat(Hat::DOWN, 50, 50);         // 受取り
    pushButton(Button::A, 40, 50);      // 閉じる（景品受取りは日付変更後
    prg++;
  }
}
/* ============================================================== */

// amiibo福引終了で日付を元に戻す。
void screenSettingClose() {
  // ホーム画面から設定画面へ
  pushButton(Button::HOME, 50, 600);
  pushHat(Hat::LEFT, 50, 120);
  pushHat(Hat::DOWN, 50, 90);
  pushHat(Hat::LEFT, 50, 90);
  pushButton(Button::A, 60, 400);
  // 設定画面本体まで
  stickTilt(Stick::RIGHT, 180, 100, 1680);  // down
  stickNeutral(Stick::RIGHT);
  // 日付と変更まで
  pushButton(Button::A, 100, 100);
  stickTilt(Stick::RIGHT, 180, 100, 700);  // down
  stickNeutral(Stick::RIGHT);
  pushButton(Button::A, 40, 200);
  //インターネットで時間を合わせるオンオフ(時刻を現在時間にリセット)
  pushButton(Button::A, 50, 200);
  pushButton(Button::HOME, 50, 600);
  pushButton(Button::HOME, 50, 800);
}