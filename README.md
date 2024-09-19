# sunbreakCon_LCDkeyPad
本プログラムは、**Arduino Leonardo**に**LCD KeyPad Shield**を接続し、**モンスターハンターライズ：サンブレイク**の各種反復する作業を簡略負担軽減しゲームのプレイ時間を有効に活用することを目的とした**オールインワン型**のマクロコントローラーです。Switch及びPS5で動作。
- Sketchバージョン　3.9.1公開

## マニュアルについて
noteにてマニュアルを公開予定（完全版はたぶん有料）

## 出来ること
- 傀異錬成
- 傀異錬成の記録、連続錬成
- 覇気・円環マカ錬金、覇気の連続マカ錬金
- 連続売却
- 狩猟笛による各決戦場の放置討伐
- カムラポイント等の放置採取
- メルゼナamiibo福引
- 言語、ゲーム機、RボタンをZRボタンとの入れ替え設定


## 動作環境
動作環境は下記の通りです。
- [Arduino Leonardo](https://amzn.to/3TvliXZ)  または[互換機](https://amzn.to/47vEI4y)
- [LCD KeyPad Shield](https://amzn.to/3AsXnla)
- [Arduino IDE](https://www.arduino.cc/en/software)（2.3.2）
- Arduino AVR Boards（1.8.6）


## 準備
- [Arduino IDE](https://www.arduino.cc/en/software)（2.3.2）をインストール。WindowsまたはmacOS。
- [NSControlLibrary](https://github.com/Kumataku-JP/NSControlLibrary.git) を /document/Arduino フォルダ内にある libraries フォルダに入れる。
- sunbreakCon_LCDkeyPad フォルダを sketch フォルダに入れる。
- Arduino Leonardo に LCD KeyPad Shieldを接続する。
- WindowsまたはmacOSにArduino LeonardoをUSB接続する。
- Arduino IDEを立ち上げArduino AVR Boards 1.8.6をインストールし、`./Arduino15/packages/arduino/hardware/avr/1.8.6/boards.txt`を書き換え。※boards.txtを書き換える際はArduino IDEを終了しておく。
	```
	leonardo.name=Switch Controller　// ← boardで認識しやすいように任意で化。

	leonardo.vid.1=0x0f0d　// ← 書換え
	leonardo.pid.1=0x00c1　// ← 書換え

	leonardo.upload_port.1.vid=0x0f0d　// ← 書換え
	leonardo.upload_port.1.pid=0x00c1　// ← 書換え

	leonardo.build.vid=0x0f0d　// ← 書換え
	leonardo.build.pid=0x00c1　// ← 書換え
	leonardo.build.usb_product="Switch Controller"　// ← 変更しなくてもよい
	```
- Arduino IDEからsunbreakCon_LCDkeyPadをArduino Leonardoに書き込み。
- SwitchにUSB接続でコントローラーとして接続。
- ゲーム機及びゲーム設定。詳細はマニュアル（公開予定）参照。

## LCD KeyPadのマトリックス
- 上下ボタン＝Mode変更　左右ボタン＝メニュー変更　セレクトボタン＝実行または設定モード移行

	|    | 1  | 2  | 3  | 4  | 5 | 6 | 7 | 8 |
	|:-|:--|:---|:----|:-----|:------|:-------|:--------|:---------|
	| Mode 0	システム設定 | 言語設定 | ゲーム機 | Rボタンマッピング | 設定更新 |     |     |     |
	| Mode 1	傀異錬成 | 精気琥珀使用 | 精気琥珀・上 | 精気琥珀・尖 | 精気琥珀・王 | 精気琥珀・真 | 錬成記録 | 錬成繰返し |	錬成回数設定	|
	| Mode 2	マカ錬金 | 覇気錬金用設定 | 覇気錬金 | 円環錬金 | 売却 |     |     |     |
	| Mode 3	オート | 闘技場 | 極泉郷 | 塔の秘境 | 闘技場オート | 特産品    |     |     |
	| Mode 4	amiibo福引 | 日付・回数設定 | amiibo福引 |     |     |     |     |     |


# 利用規約
- 本プログラムの一部または全部の再配布、本プログラムのソフト化及びハード化し販売することを一切禁じる。
- 本プログラムの有料マニュアルの再配付、無断販売を禁じる。
- 本プログラムの改変は、個人利用の環境に合わせたコマンド修正等のみ許可する。
- 本プログラム使用で起きたことの責任は自己で持つこと。