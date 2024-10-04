# sunbreakCon_LCDkeyPad
本プログラムは、**Arduino Leonardo**に**LCD KeyPad Shield**を接続し、**モンスターハンターライズ：サンブレイク**の各種反復する作業を簡略負担軽減しゲームのプレイ時間を有効に活用することを目的とした**オールインワン型**のマクロコントローラーです。Switch及びPS5で動作。
- バージョン　4.0.0

## マニュアルについて
noteにてマニュアルを公開予定
簡易的な説明はmode0-4の各ファイル先頭にあり

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
- [Arduino IDE](https://www.arduino.cc/en/software)
- Arduino AVR Boards（1.8.6）
- PS5の場合、コンバーター等諸設備が必須[PS5にArduinoをコントローラーとして接続する方法
](https://note.com/kumataku/n/n6cb0367d6341)

## 準備
- [Arduino IDE](https://www.arduino.cc/en/software)をインストール。WindowsまたはmacOS。
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

	|    | 1  | 2  | 3  | 4  |
	|:-|:--|:---|:----|:-----|
	| Mode 0	システム設定 | 言語 | ゲーム機 | Rボタンマッピング | 設定更新 |
	| Mode 1	傀異錬成 | 設定 | 傀異錬成 | 錬成記録 | 錬成繰返し |
	| Mode 2	マカ錬金 | 設定 | 覇気錬金 | 円環錬金 | 売却 |
	| Mode 3	オート | 設定 | 決戦場 | 特産品 |
	| Mode 4	amiibo福引 | 設定 | amiibo福引 |
## 修正履歴
- v4.0.0  M1.傀異錬成及びM3.オートマクロの項目仕様変更、その他コードの改修
- v3.9.3  オートマクロの修正
- v3.9.2  NSControlLibraryのバージョンアップ(1.2.0)に伴いアナログスティックのコードを適応<br>amiibo福引のカレンダー変更までの調整
- v3.9.1  sunbreakCon_LCDkeyPad公開

# 利用規約
- 本プログラムの一部または全部の再配布、本プログラムのソフト化及びハード化し無断販売することを一切禁じる。
- 本プログラムの改変は、利用者個人の環境に合わせたコマンド修正等のみ許可する。
- 本プログラム使用で生じた責任は自己で持つこと。
