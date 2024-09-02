/* 特産品採集====================================================== */
/* 城塞高地特産品 固定採集 */
void repeatA() {
  now = millis();
  // 2秒おきに実行
  if (now - startTimeA >= 2000) {
    startTimeA = now;
    pushButton(Button::A);
  }
}

/* 城塞高地特産品 移動採集 */
void repeatMove() {
  // 2分おきに移動
  if (!skipExec && now - startTimeB >= 120000) {
    startTimeB = now;
    skipExec = true;
    lcd.setCursor(10, 1);
    lcd.cursor();
  } else if (skipExec && now - startTimeB >= 5000) {
    leftStickTilt(180, 100, 1100);
    leftStickNeutral();
    pushButton(Button::L, 40, 40, 2);  // カメラ補整
    skipExec = false;
    lcd.noCursor();
  }
}

/* 闘技場自動 狩猟笛================================================ */
/* 闘技場 */
void autoArena() {
  now = millis();
  leftStickTilt(5, 100);  // 移動方向
  silkbind_4();           // 抜刀共鳴音珠・震打
  silkbind_5();           // スライドビート・鉄蟲糸響打
  melodyType();           // 旋律
  times++;
}

/* 極泉郷 */
void autoInfernal() {
  now = millis();
  leftStickTilt(40, 100);  // 移動方向
  silkbind_4();            // 抜刀共鳴音珠・震打
  Sheathe();               // 納刀A
  silkbind_5();            // スライドビート・鉄蟲糸響打
  melodyType();            // 旋律
  times++;
}

/* 塔の秘境 */
void autoForlorn() {
  now = millis();
  leftStickTilt(25, 100);  // 移動方向
  silkbind_4();            // 抜刀共鳴音珠・震打
  Sheathe();               // 納刀A
  silkbind_5();            // スライドビート・鉄蟲糸響打
  melodyType();            // 旋律
  times++;
}
/* ============================================================== */

// 共鳴音珠・震打
void silkbind_4() {
  if (isFirstRun) {
    pushButton(Button::X, 60, 900);
    holdButton(Button::ZL);
    holdButton(Button::A, 200);
    releaseButton(Button::ZL);
    releaseButton(Button::A);
    startTimeA = now;
  }
  isFirstRun = false;
}
// スライドビート・鉄蟲糸響打
void silkbind_5() {
  if (!skipSheathe && now - startTimeA >= 30000) {
    skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeA = now;
    delay(500);
    holdButton(Button::ZL);
    holdButton(Button::X, 200);
    releaseButton(Button::ZL);
    releaseButton(Button::X, 900);
    pushButton(mappingR2, 700, 50);
    skipExec = false;
  }
}
// 納刀
void Sheathe() {
  if (now - startTimeB >= 43000 && now - startTimeS > 300000) {
    skipSheathe = skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeB = now;
    delay(500);
    holdButton(mappingR1, 2500);
    if (value == 2) {
      leftStickTilt(90, 100);
    }
    pushButton(Button::A, 200, 40, 4);
    releaseButton(mappingR1);
    skipSheathe = skipExec = false;
  }
}
// 回復大旋律タイプ
void melodyType() {
  if (!skipExec) {
    switch (melody) {
      case 0:  // X旋律
        pushButton(mappingR2, 60, 50, 2);
        pushButton(Button::X, 50, 800);
        break;
      case 1:  // A旋律
        pushButton(mappingR2, 100, 50, 2);
        pushButton(Button::A, 50, 800);
        break;
      case 2:  // XA旋律
        pushButton(mappingR2, 400, 50, 2);
        holdButton(Button::X);
        holdButton(Button::A, 100);
        releaseButton(Button::A);
        releaseButton(Button::X, 500);
        break;
    }
  }
}
// ターゲットチェンジ
void target() {
  if (times % 8 == 0 && now - startTimeS > 420000) {
    pushButton(Button::RCLICK);
  }
}

/* ============================================================== */
/* クエスト準備開始まで */
void autoArenaCruising() {
  if (prg == 0) {
    pushButton(minusButton, 100, 500);  //観測拠点 中央広場
    pushButton(confirmButton, 100, 500);
    leftStickTilt(30, 100, 1200);  //チッチェまで移動
    leftStickNeutral();
    prg++;
  } else if (prg == 1) {
    pushButton(confirmButton, 100, 1000);  //クエスト選択
    pushButton(confirmButton, 100, 100, 5);
    pushHat(Hat::RIGHT, 100, 100);         // ロンディーネ
    pushButton(confirmButton);             //盟友選択
    pushHat(Hat::DOWN, 50, 50, 2);         //ヒノエ
    pushButton(confirmButton, 50, 50, 2);  //盟友選択
    delay(2000);
    prg++;
  } else if (prg == 2) {
    pushButton(minusButton, 100, 500);  //茶屋前
    pushHat(Hat::DOWN, 50, 50, 2);
    pushButton(confirmButton, 100, 500);
    leftStickTilt(-20, 100, 1000);
    leftStickNeutral();
    prg++;
  } else if (prg == 3) {
    pushButton(confirmButton, 100, 3000);  //だんご選択
    pushButton(confirmButton, 100, 100, 3);
    pushButton(Button::X);  //謹製おだんご券選択
    pushHat(Hat::DOWN);     //2段目のだんご選択
    pushButton(confirmButton, 100);
    pushButton(confirmButton, 100, 3000);
    pushButton(cancelButton, 50, 5000);
    pushButton(Button::R, 100);
    pushButton(confirmButton);
    prg++;
  } else if (prg == 4) {
    if (consoleType == 0) {
      delay(20000);  //Switchロード時間
    } else if (consoleType == 1) {
      delay(5000);  //PS5ロード時間
    }
    prg++;
  } else if (prg == 5) {
    holdButton(Button::ZL, 200);  // 早替え
    holdButton(Button::X);
    holdButton(Button::A, 200);
    releaseButton(Button::ZL);
    releaseButton(Button::X);
    releaseButton(Button::A, 200);
    pushButton(Button::RCLICK);  // ターゲットオン
    holdButton(mappingR1);
    leftStickTilt(20, 100, 1500);  // 移動
    leftStickTilt(0, 100, 10000);  // 移動
    releaseButton(mappingR1);
    prg++;
  }
}