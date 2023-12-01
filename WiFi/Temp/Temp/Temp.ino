//WiFi
/*
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// サーバー設定ポート80で接続
WebServer server(80);
// 入出力ピン番号設定
#define OUTPUT_PIN0 0  //出力ピン
// WiFi SSID パスワード設定（自分の環境に合わせて設定）
const char* ssid     = "poiuytrewq(2)";
const char* password = "poscapod";

*/

//If you use AP
//ここから

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LEAmDNS.h>

#ifndef STASSID
  #define STASSID "RW-Connect"
#define STAPSK "TORICA-EE"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const IPAddress ip(192, 168, 40, 1);
const IPAddress subnet(255, 255, 255, 0);

// サーバー設定ポート80で接続
WebServer server(80);

//ここまで
// 入出力ピン番号設定
#define OUTPUT_PIN0 5  //出力ピン


// 変数宣言
float data;
// htmlデータを文字列に格納 ※R"(ここに書いた文字列は改行を無視して1行の文字列として扱える)"
String html = R"(
<!DOCTYPE html><html lang="jp"><head>
  <meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>REMOTE-CONTROLLER</title>
  <style>
    body{font-family: sans-serif; background-color: #fff; max-width: 480px; margin: 0 auto; align-items: center;}
    h1 {color:#333; text-align: center; font-size: 28px; margin: 10px auto;}
    div {display: flex; flex-direction: row; justify-content: center;}
    .btn {height: 70px; width: 100px; color: rgb(44, 43, 43); background-color: #dddde9; font-size: 18px; font-weight: bold; border-radius: 7%; margin: 0 10px; -webkit-appearance: none;}
    td {width: 110px; color:#333; text-align: center; font-size: 18px;}
  </style></head><body>
    <h1>REMOTE-CONTROLLER</h1>
    <div>
      <button class="btn" id="btn0" onclick="getBtnOn(0) ">ON</button>
      <button class="btn" id="btn1" onclick="getBtnOn(1) ">OFF</button>
    </div>
    <div style = "margin-top: 20px;">
      <table border = "1">
        <tr><td>本体温度</td><td><p id="output_temp"></p></td></tr>
      </table>
    </div>
    <div style = "margin-top: 20px;">
      <table border = "1">
        <tr><td>Time:</td><td><p id="output_time"></p></td></tr>
      </table>
    </div>
  </body>
  
  <script type="text/javascript">
  // ON/OFFボタン操作処理
  async function getBtnOn(i) {
    let link;
    switch (i) {
      case 0: link = "/get/btn_on"; break;
      case 1: link = "/get/btn_off"; break;
    }
    try {
      const response = await fetch(link);
      if (response.ok) {
        const text = await response.text();
        console.log(text);
      }
    } catch (error) { console.log(error); }
  }
  // データ取得、output表示更新
  async function updateOutput_temp() {
    const output_temp = document.getElementById("output_temp");
    try {
      const response = await fetch("/get/data_temp");
      if (response.ok) {
        output_temp.textContent = await response.text();
      } else { throw new Error(); }
    } catch (error) { console.log(error); }
  }
  async function updateOutput_Time() {
    const output_time = document.getElementById("output_time");
    try {
      const response = await fetch("/get/data_time");
      if (response.ok) {
        output_time.textContent = await response.text();
      } else { throw new Error(); }
    } catch (error) { console.log(error); }
  }
  setInterval(updateOutput_temp, 1000);
  setInterval(updateOutput_time, 1000);
  </script></html>

)";
// 関数 ------------------------------------------------------------
/********* クライアントにWebページ（HTML）を返す関数 *********/
void handleRoot() {
  server.send(200, "text/html", html); //レスポンス200を返し、htmlデータ送信
  Serial.println("200, Root Access!");
}
/********** クライアントにエラーメッセージを返す関数 **********/
void handleNotFound() { //ファイルが見つかりません
  server.send(404, "text/plain", "File not found!");
  Serial.println("404, File not found!");
}
/********** ブラウザボタンON/OFF、温度データのレスポンスを返す関数 **********/
// ブラウザボタンON
void BtnOn() {
  digitalWrite(25, HIGH);        // 本体LED ON
  digitalWrite(OUTPUT_PIN0, HIGH);        // GPIO0 HIGH
  server.send(200, "text/html", "ON!");   // レスポンス200を返し、ON！を送信
  Serial.println("200, ON!");
}
// ブラウザボタンOFF
void BtnOff() {
  digitalWrite(25, LOW);         // 本体LED OFF
  digitalWrite(OUTPUT_PIN0, LOW);         // GPIO0 LOW
  server.send(200, "text/html", "OFF!");  // レスポンス200を返し、OFF!を送信
  Serial.println("200, OFF!");
}
// ブラウザデータ表示更新
void getTempData() {
  char buf_temp[10];                           // data文字列格納バッファ
  sprintf(buf_temp, "%.1f℃", data);            // dataの値を文字列として格納
  server.send(200, "text/html", buf_temp);     // レスポンス200を返し、dataを文字列で送信
  //Serial.printf("Temp: %.1f'C\n", data);
}
void getTimeData() {
  char buf_time[30];                           // data文字列格納バッファ
  uint64_t now_time = millis();
  sprintf(buf_time, "%d ms", now_time);            // dataの値を文字列として格納
  server.send(200, "text/html", buf_time);     // レスポンス200を返し、dataを文字列で送信
  //Serial.printf("Temp: %.1f'C\n", data);
}
/********************* Webサーバー設定 ********************/
void serverSetting() {
  server.on("/", handleRoot);         // ルートアクセス時の応答関数を設定
  server.onNotFound(handleNotFound);  // 不正アクセス時の応答関数を設定
  server.on("/get/btn_on", BtnOn);    // ボタンON受信処理
  server.on("/get/btn_off", BtnOff);  // ボタンOFF受信処理
  server.on("/get/data_temp", getTempData);    // データ表示更新受信処理
  server.on("/get/data_time", getTimeData);    // データ表示更新受信処理
  server.begin(); // Webサーバー開始
}
// 初期設定 ------------------------------------------------
void setup() {
  // WiFi接続開始
  //if you use AP
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  //ここまで
  pinMode(OUTPUT_PIN0, OUTPUT);     // LED GP0を出力端子に設定（LEDのWL_GPIO0とは別物）
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //接続完了してなければ
    delay(500);
    Serial.print(".");
  }
  for(int i=0;i<5;i++){
    digitalWrite(OUTPUT_PIN0,HIGH);
    delay(500);
    digitalWrite(OUTPUT_PIN0,LOW);
    delay(500);
  }
  pinMode(32, OUTPUT);     // 本体LEDは「25」で指定（ピン番号だと32で指定）
  digitalWrite(32, HIGH);  // 本体LED ON
  
  Serial.print("\nSSID: ");       // SSID表示
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");           // IPアドレス表示
  Serial.println(WiFi.localIP());
  // サーバー設定
  serverSetting();
  // 出力端子設定
  
}
// メイン --------------------------------------------------
void loop() {
  static int led = 0;
  server.handleClient();    //クライアントからのアクセス処理
  data = analogReadTemp();  // 本体温度データ取得
  digitalWrite(32,led);
  led = !led;
  delay(100);
}
