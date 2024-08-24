/*  傀異錬成  */
// 傀異錬成センタク
void choiceQurious() {
  pushButton(confirmButton, 60, 200);  // 傀異強化の種類を選択
}
// 錬成強化しない
void choiceCurrent() {
  pushButton(cancelButton, 60, 200);  // 強化前のステータスを選択
  pushHat(Hat::RIGHT, 50, 60);        // はいに移動
  pushButton(confirmButton, 60);      // 「続けて、傀異強化をおこないますか?」
  pushButton(confirmButton, 150);     // はい
}

/*  使用琥珀  */
// 精気琥珀

// 精気琥珀・上
void amberEssencePlus() {
  pushHat(Hat::DOWN);  // 精気琥珀・上まで
}
// 精気琥珀・尖
void primeAmberEssence() {
  pushHat(Hat::DOWN, 40, 40, 2);  // 精気琥珀・尖まで
}
// 精気琥珀・王
void royalAmberEssence() {
  pushHat(Hat::DOWN, 40, 40, 3);  // 精気琥珀・王まで
}
// 精気琥珀・真
void pureAmberEssence() {
  pushHat(Hat::UP, 40, 40, 2);  // 精気琥珀・真まで
}

void choiceEssence() {
  pushButton(confirmButton);           // 精気琥珀・真選択
  pushHat(Hat::DOWN);                  // 使用数MAX指定
  pushButton(confirmButton);           // 選択完了
  pushButton(Button::MINUS);   // 決定までカーソル移動
  pushButton(confirmButton, 40, 100);  // 決定
  pushButton(confirmButton);   // 確認画面
}
// キャプチャー
void capture() {
  if (consoleType == 0) delay(700);
  else if (consoleType == 1) delay(500);
  pushButton(Button::CAPTURE, 100, 100);
}
