/* 城塞高地特産品 固定採集 */
void repeatA() {
  now = millis();
  if (!isFirstRun) {
    isFirstRun = true;
    startTimeA = now;
  }
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
/* 闘技場 X旋律モード */
void autoArenaX() {
  leftStickTilt(20, 100);
  now = millis();
  if (!isFirstRun) {
    silkbind_4();  // 抜刀共鳴音珠・震打
    isFirstRun = true;
    startTimeA = now;
  }
  silkbind_5();  // スライドビート・鉄蟲糸響打
  melodyType();  // 旋律
  times++;
}


/* 極泉郷 */
void autoInfernal() {
  leftStickTilt(40, 100);
  now = millis();
  // 抜刀共鳴音珠・震打
  if (!isFirstRun) {
    silkbind_4();  // 抜刀共鳴音珠・震打
    isFirstRun = true;
    startTimeA = now;
  }
  // 納刀A
  if (now - startTimeB >= 43000 && now - startTimeS > 300000) {
    skipSheathe = true;
    skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeB = now;
    delay(500);
    holdButton(mappingR1, 2500);
    releaseButton(mappingR1);
    leftStickTilt(90, 100);
    pushButton(Button::A, 200, 40, 4);
    leftStickNeutral();
    skipSheathe = false;
    skipExec = false;
  }
  silkbind_5();  // スライドビート・鉄蟲糸響打
  melodyType();  // 旋律
  times++;
}


/* 塔の秘境 */
void autoForlorn() {
  leftStickTilt(30, 100);
  now = millis();
  if (!isFirstRun) {
    silkbind_4();  // 抜刀共鳴音珠・震打
    isFirstRun = true;
    startTimeA = now;
  }
  // 納刀A
  if (now - startTimeB >= 43000 && now - startTimeS > 300000) {
    skipSheathe = true;
    skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeB = now;
    delay(500);
    holdButton(mappingR1, 2500);
    pushButton(Button::A, 200, 40, 4);
    releaseButton(mappingR1);
    skipSheathe = false;
    skipExec = false;
  }
  silkbind_5();  // スライドビート・鉄蟲糸響打
  melodyType();  // 旋律
  times++;
}

// 共鳴音珠・震打
void silkbind_4() {
  pushButton(Button::X, 60, 900);
  holdButton(Button::ZL, 0);
  holdButton(Button::A, 200);
  releaseButton(Button::ZL);
  releaseButton(Button::A);
}
// スライドビート・鉄蟲糸響打
void silkbind_5() {
  if (!skipSheathe && now - startTimeA >= 30000) {
    skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeA = now;
    delay(500);
    holdButton(Button::ZL, 0);
    holdButton(Button::X, 200);
    releaseButton(Button::ZL, 0);
    releaseButton(Button::X, 900);
    pushButton(mappingR2, 700, 50);
    skipExec = false;
  }
}
// 回復大旋律タイプ
void melodyType() {
  if (!skipExec) {
    switch (melody) {
      case 0:  // X旋律
        pushButton(Button::X, 50, 800);
        pushButton(mappingR2, 60, 50, 2);
        break;
      case 1:  // A旋律
        pushButton(Button::A, 50, 800);
        pushButton(mappingR2, 100, 50, 2);
        break;
      case 2:  // XA旋律
        holdButton(Button::X, 0);
        holdButton(Button::A, 100);
        releaseButton(Button::A, 0);
        releaseButton(Button::X, 500);
        pushButton(mappingR2, 400, 50, 2);
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
