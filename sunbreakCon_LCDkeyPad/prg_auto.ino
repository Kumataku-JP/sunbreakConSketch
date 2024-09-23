/* 特産品採集====================================================== */
/* 城塞高地特産品 固定採集 */
void repeatA() {
  startTime = millis();
  if (isFirstRun) {
    isFirstRun = false;
    startTimeA = startTime;
  }
  // 2秒おきに実行
  if (startTime - startTimeA >= 2000) {
    startTimeA = startTime;
    pushButton(Button::A);
  }
}

/* 城塞高地特産品 移動採集 */
void repeatMove() {
  // 2分おきに移動
  if (!skipExec && startTime - startTimeB >= 120000) {
    startTimeB = startTime;
    skipExec = true;
    lcd.setCursor(10, 1);
    lcd.cursor();
  } else if (skipExec && startTime - startTimeB >= 5000) {
    stickTilt(Stick::LEFT, 180, 100, 1100);
    stickNeutral(Stick::LEFT);
    pushButton(Button::L, 40, 40, 2);  // カメラ補整
    skipExec = false;
    lcd.noCursor();
  }
}

/* 闘技場自動 狩猟笛================================================ */
/* 闘技場 */
void autoArena() {
  startTime = millis();
  stickTilt(Stick::LEFT, 5, 100);  // 移動方向
  silkbind_4();           // 抜刀共鳴音珠・震打
  silkbind_5();           // スライドビート・鉄蟲糸響打
  melodyType();           // 旋律
  times++;
}

/* 極泉郷 */
void autoInfernal() {
  startTime = millis();
  stickTilt(Stick::LEFT, 30, 100);  // 移動方向
  silkbind_4();            // 抜刀共鳴音珠・震打
  Sheathe();               // 納刀A
  silkbind_5();            // スライドビート・鉄蟲糸響打
  melodyType();            // 旋律
  times++;
}

/* 塔の秘境 */
void autoForlorn() {
  startTime = millis();
  stickTilt(Stick::LEFT, 25, 100);  // 移動方向
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
    startTimeA = startTime;
  }
  isFirstRun = false;
}
// スライドビート・鉄蟲糸響打
void silkbind_5() {
  if (!skipSheathe && startTime - startTimeA >= 30000) {
    skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeA = startTime;
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
  if (startTime - startTimeB >= 43000 && startTime - startTimeS > 300000) {
    skipSheathe = skipExec = true;  // 納刀Aが実行中、他のコマンド不実行
    startTimeB = startTime;
    delay(500);
    holdButton(mappingR1, 2500);
    if (value == 2) {
      stickTilt(Stick::LEFT, 90, 100);
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
  if (times % 8 == 0 && startTime - startTimeS > 420000) {
    pushButton(Button::RCLICK);
  }
}

/* ============================================================== */
/* クエスト準備開始まで */
void autoQuestCruising() {
  switch (prg) {
    case 0:
      processPlaza();
      prg++;
      break;
    case 1:
      processAccept();
      prg++;
      break;
    case 2:
      processTeaShop();
      prg++;
      break;
    case 3:
      processDango();
      prg++;
      break;
    case 4:
      processLoading();
      prg++;
      break;
    case 5:
      processStartQuest();
      if (value == 1 || value == 4) processArena();
      if (value == 2) processInfernal();
      if (value == 3) processForlorn();
      prg++;
      break;
  }
}
//観測拠点 中央広場に移動 0
void processPlaza() {
  pushButton(minusButton, 100, 300);
  pushButton(confirmButton, 100, 500);
  stickTilt(Stick::LEFT, 30, 100, 1200);  // チッチェまで移動
  stickNeutral(Stick::LEFT);
}
//クエスト選択 1
void processAccept() {
  pushButton(confirmButton, 100, 1000);
  pushButton(confirmButton, 100, 100, 5);
  pushHat(Hat::RIGHT, 100, 100);         // ロンディーネ
  pushButton(confirmButton);             // 盟友選択
  pushHat(Hat::DOWN, 50, 50, 2);         // ヒノエ
  pushButton(confirmButton, 50, 50, 2);  // 盟友選択
  delay(2000);
}
//茶屋前に移動 2
void processTeaShop() {
  pushButton(minusButton, 100, 300);
  pushHat(Hat::DOWN, 50, 50, 2);
  pushButton(confirmButton, 100, 500);
  stickTilt(Stick::LEFT, -20, 100, 1000);
  stickNeutral(Stick::LEFT);
}
//だんご選択 3
void processDango() {
  pushButton(confirmButton, 100, 3000);
  pushButton(confirmButton, 100, 100, 3);
  pushButton(Button::X);  // 謹製おだんご券選択
  pushHat(Hat::DOWN);     // 2段目のだんご選択
  pushButton(confirmButton, 100);
  pushButton(confirmButton, 100, 3000);
  pushButton(cancelButton, 50, 5000);
  pushButton(Button::R, 100);
  pushButton(confirmButton);
}
// ロード時間 4
void processLoading() {
  if (consoleType == 0) {
    delay(20000);  // Switchロード時間
  } else if (consoleType == 1) {
    delay(5000);  // PS5ロード時間
  }
}
// クエスト開始時挙動 5
void processStartQuest() {
  holdButton(Button::ZL, 200);  // 早替え
  holdButton(Button::X);
  holdButton(Button::A, 200);
  releaseButton(Button::ZL);
  releaseButton(Button::X);
  releaseButton(Button::A, 200);
  pushButton(Button::RCLICK);  // ターゲットオン
}
// 闘技場の移動 6
void processArena() {
  holdButton(mappingR1);
  stickTilt(Stick::LEFT, 20, 100, 1500);  // 虹ヒトダマドリ移動
  stickTilt(Stick::LEFT, 0, 100, 10000);  // モンスターまで移動
  releaseButton(mappingR1);
}
// 極泉郷の移動 6
void processInfernal() {
  holdButton(mappingR1);
  stickTilt(Stick::LEFT, -45, 100, 2400);      // 虹ヒトダマドリまで移動
  stickTilt(Stick::LEFT, 50, 100, 3000);       // 大翔蟲まで移動
  releaseButton(mappingR1);
  stickTilt(Stick::LEFT, 90, 100, 500);       // 大翔蟲まで移動
  pushButton(Button::A, 100, 50, 6);  // 大翔蟲
  holdButton(mappingR1);
  stickTilt(Stick::LEFT, 0, 100, 10000);       // モンスターまで移動
  releaseButton(mappingR1);
}
// 塔の秘境の移動 6
void processForlorn() {
  holdButton(mappingR1);
  stickTilt(Stick::LEFT, -10, 100, 3500);      // 虹ヒトダマドリ大翔蟲まで移動
  pushButton(Button::A, 200, 40, 2);  // 大翔蟲
  stickTilt(Stick::LEFT, 0, 100, 10000);       // モンスターまで移動
  releaseButton(mappingR1);
}