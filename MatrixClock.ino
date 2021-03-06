/*
  亮度设置
  滚动开关
*/
#include <SPI.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <time.h>
#define SDA        5
#define SCL        4
#define CS         15
#define anzMAX     4

#define REVERSE_HORIZONTAL //使用v2.2版本的硬件需删除该行
#define REVERSE_VERTICAL //使用v2.2版本的硬件需删除该行

char ssid[] = ""; //你的WiFi名称
char pass[] = ""; //你的WiFi密码
unsigned short maxPosX = anzMAX * 8 - 1;
unsigned short LEDarr[anzMAX][8];
unsigned short helpArrMAX[anzMAX * 8];
unsigned short helpArrPos[anzMAX * 8];
unsigned int z_PosX = 0;
unsigned int d_PosX = 0;
bool f_tckr1s = false;
bool f_tckr50ms = false;
unsigned long epoch = 0;
unsigned int localPort = 2390;
const char* ntpServerName = "time1.aliyun.com";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
IPAddress timeServerIP;
tm *tt, ttm;
const unsigned char DS3231_ADDRESS = 0x68;
const unsigned char secondREG = 0x00;
const unsigned char minuteREG = 0x01;
const unsigned char hourREG = 0x02;
const unsigned char WTREG = 0x03;
const unsigned char dateREG = 0x04;
const unsigned char monthREG = 0x05;
const unsigned char yearREG = 0x06;
const unsigned char alarm_min1secREG = 0x07;
const unsigned char alarm_min1minREG = 0x08;
const unsigned char alarm_min1hrREG = 0x09;
const unsigned char alarm_min1dateREG = 0x0A;
const unsigned char alarm_min2minREG = 0x0B;
const unsigned char alarm_min2hrREG = 0x0C;
const unsigned char alarm_min2dateREG = 0x0D;
const unsigned char controlREG = 0x0E;
const unsigned char statusREG = 0x0F;
const unsigned char ageoffsetREG = 0x10;
const unsigned char tempMSBREG = 0x11;
const unsigned char tempLSBREG = 0x12;
const unsigned char _24_hour_format = 0;
const unsigned char _12_hour_format = 1;
const unsigned char AM = 0;
const unsigned char PM = 1;
struct DateTime {
  unsigned short sek1, sek2, sek12, min1, min2, min12, std1, std2, std12;
  unsigned short tag1, tag2, tag12, mon1, mon2, mon12, jahr1, jahr2, jahr12, WT;
} MEZ;
Ticker tckr;
WiFiUDP udp;

unsigned short const font1[96][9] = {
  { 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x20, Space
  { 0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00 },   // 0x21, !
  { 0x07, 0x09, 0x09, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x22, "
  { 0x07, 0x0a, 0x0a, 0x1f, 0x0a, 0x1f, 0x0a, 0x0a, 0x00 },   // 0x23, #
  { 0x07, 0x04, 0x0f, 0x14, 0x0e, 0x05, 0x1e, 0x04, 0x00 },   // 0x24, $
  { 0x07, 0x19, 0x19, 0x02, 0x04, 0x08, 0x13, 0x13, 0x00 },   // 0x25, %
  { 0x07, 0x04, 0x0a, 0x0a, 0x0a, 0x15, 0x12, 0x0d, 0x00 },   // 0x26, &
  { 0x07, 0x04, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x27, '
  { 0x07, 0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00 },   // 0x28, (
  { 0x07, 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08, 0x00 },   // 0x29, )
  { 0x07, 0x04, 0x15, 0x0e, 0x1f, 0x0e, 0x15, 0x04, 0x00 },   // 0x2a, *
  { 0x07, 0x00, 0x04, 0x04, 0x1f, 0x04, 0x04, 0x00, 0x00 },   // 0x2b, +
  { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 },   // 0x2c, ,
  { 0x07, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00 },   // 0x2d, -
  { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00 },   // 0x2e, .
  { 0x07, 0x01, 0x01, 0x02, 0x04, 0x08, 0x10, 0x10, 0x00 },   // 0x2f, /
  { 0x07, 0x0F, 0x09, 0x09, 0x09, 0x09, 0x09, 0x0F, 0x00 },   // 0x30, 0
  { 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00 },   // 0x31, 1
  { 0x07, 0x0F, 0x01, 0x01, 0x0F, 0x08, 0x08, 0x0F, 0x00 },   // 0x32, 2
  { 0x07, 0x0F, 0x01, 0x01, 0x0F, 0x01, 0x01, 0x0F, 0x00 },   // 0x33, 3
  { 0x07, 0x09, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x01, 0x00 },   // 0x34, 4
  { 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x01, 0x01, 0x0F, 0x00 },   // 0x35, 5
  { 0x07, 0x0F, 0x08, 0x08, 0x0F, 0x09, 0x09, 0x0F, 0x00 },   // 0x36, 6
  { 0x07, 0x0F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00 },   // 0x37, 7
  { 0x07, 0x0F, 0x09, 0x09, 0x0F, 0x09, 0x09, 0x0F, 0x00 },   // 0x38, 8
  { 0x07, 0x0F, 0x09, 0x09, 0x0F, 0x01, 0x01, 0x0F, 0x00 },   // 0x39, 9
  { 0x04, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x00 },   // 0x3a, :
  { 0x07, 0x00, 0x0c, 0x0c, 0x00, 0x0c, 0x04, 0x08, 0x00 },   // 0x3b, ;
  { 0x07, 0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 0x00 },   // 0x3c, <
  { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x3d, =
  { 0x07, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x00 },   // 0x3e, >
  { 0x07, 0x0e, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04, 0x00 },   // 0x3f, ?
  { 0x07, 0x0e, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0f, 0x00 },   // 0x40, @
  { 0x07, 0x04, 0x0a, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x00 },   // 0x41, A
  { 0x07, 0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e, 0x00 },   // 0x42, B
  { 0x07, 0x0e, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0e, 0x00 },   // 0x43, C
  { 0x07, 0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E, 0x00 },   // 0x44, D
  { 0x07, 0x1f, 0x10, 0x10, 0x1c, 0x10, 0x10, 0x1f, 0x00 },   // 0x45, E
  { 0x07, 0x1f, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10, 0x00 },   // 0x46, F
  { 0x07, 0x0e, 0x11, 0x10, 0x10, 0x13, 0x11, 0x0f, 0x00 },   // 0x37, G
  { 0x07, 0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x00 },   // 0x48, H
  { 0x07, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e, 0x00 },   // 0x49, I
  { 0x07, 0x1f, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0c, 0x00 },   // 0x4a, J
  { 0x07, 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11, 0x00 },   // 0x4b, K
  { 0x07, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x00 },   // 0x4c, L
  { 0x07, 0x11, 0x1b, 0x15, 0x11, 0x11, 0x11, 0x11, 0x00 },   // 0x4d, M
  { 0x07, 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x00 },   // 0x4e, N
  { 0x07, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },   // 0x4f, O
  { 0x07, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10, 0x00 },   // 0x50, P
  { 0x07, 0x0e, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0d, 0x00 },   // 0x51, Q
  { 0x07, 0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11, 0x00 },   // 0x52, R
  { 0x07, 0x0e, 0x11, 0x10, 0x0e, 0x01, 0x11, 0x0e, 0x00 },   // 0x53, S
  { 0x07, 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00 },   // 0x54, T
  { 0x07, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e, 0x00 },   // 0x55, U
  { 0x07, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04, 0x00 },   // 0x56, V
  { 0x07, 0x11, 0x11, 0x11, 0x15, 0x15, 0x1b, 0x11, 0x00 },   // 0x57, W
  { 0x07, 0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11, 0x00 },   // 0x58, X
  { 0x07, 0x11, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x04, 0x00 },   // 0x59, Y
  { 0x07, 0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f, 0x00 },   // 0x5a, Z
  { 0x07, 0x0e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0e, 0x00 },   // 0x5b, [
  { 0x07, 0x10, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x00 },   // 0x5c, '\'
  { 0x07, 0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x0e, 0x00 },   // 0x5d, ]
  { 0x07, 0x04, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x5e, ^
  { 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00 },   // 0x5f, _
  { 0x07, 0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x60, `
  { 0x07, 0x00, 0x0e, 0x01, 0x0d, 0x13, 0x13, 0x0d, 0x00 },   // 0x61, a
  { 0x07, 0x10, 0x10, 0x10, 0x1c, 0x12, 0x12, 0x1c, 0x00 },   // 0x62, b
  { 0x07, 0x00, 0x00, 0x0E, 0x10, 0x10, 0x10, 0x0E, 0x00 },   // 0x63, c
  { 0x07, 0x01, 0x01, 0x01, 0x07, 0x09, 0x09, 0x07, 0x00 },   // 0x64, d
  { 0x07, 0x00, 0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0f, 0x00 },   // 0x65, e
  { 0x07, 0x06, 0x09, 0x08, 0x1c, 0x08, 0x08, 0x08, 0x00 },   // 0x66, f
  { 0x07, 0x00, 0x0e, 0x11, 0x13, 0x0d, 0x01, 0x01, 0x0e },   // 0x67, g
  { 0x07, 0x10, 0x10, 0x10, 0x16, 0x19, 0x11, 0x11, 0x00 },   // 0x68, h
  { 0x05, 0x00, 0x02, 0x00, 0x06, 0x02, 0x02, 0x07, 0x00 },   // 0x69, i
  { 0x07, 0x00, 0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0c },   // 0x6a, j
  { 0x07, 0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12, 0x00 },   // 0x6b, k
  { 0x05, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00 },   // 0x6c, l
  { 0x07, 0x00, 0x00, 0x0a, 0x15, 0x15, 0x11, 0x11, 0x00 },   // 0x6d, m
  { 0x07, 0x00, 0x00, 0x16, 0x19, 0x11, 0x11, 0x11, 0x00 },   // 0x6e, n
  { 0x07, 0x00, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e, 0x00 },   // 0x6f, o
  { 0x07, 0x00, 0x00, 0x1c, 0x12, 0x12, 0x1c, 0x10, 0x10 },   // 0x70, p
  { 0x07, 0x00, 0x00, 0x07, 0x09, 0x09, 0x07, 0x01, 0x01 },   // 0x71, q
  { 0x07, 0x00, 0x00, 0x16, 0x19, 0x10, 0x10, 0x10, 0x00 },   // 0x72, r
  { 0x07, 0x00, 0x00, 0x0f, 0x10, 0x0e, 0x01, 0x1e, 0x00 },   // 0x73, s
  { 0x07, 0x08, 0x08, 0x1c, 0x08, 0x08, 0x09, 0x06, 0x00 },   // 0x74, t
  { 0x07, 0x00, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d, 0x00 },   // 0x75, u
  { 0x07, 0x00, 0x00, 0x11, 0x11, 0x11, 0x0a, 0x04, 0x00 },   // 0x76, v
  { 0x07, 0x00, 0x00, 0x11, 0x11, 0x15, 0x15, 0x0a, 0x00 },   // 0x77, w
  { 0x07, 0x00, 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00 },   // 0x78, x
  { 0x07, 0x00, 0x00, 0x11, 0x11, 0x0f, 0x01, 0x11, 0x0e },   // 0x79, y
  { 0x07, 0x00, 0x00, 0x1f, 0x02, 0x04, 0x08, 0x1f, 0x00 },   // 0x7a, z
  { 0x07, 0x06, 0x08, 0x08, 0x10, 0x08, 0x08, 0x06, 0x00 },   // 0x7b, {
  { 0x07, 0x04, 0x04, 0x04, 0x00, 0x04, 0x04, 0x04, 0x00 },   // 0x7c, |
  { 0x07, 0x0c, 0x02, 0x02, 0x01, 0x02, 0x02, 0x0c, 0x00 },   // 0x7d, }
  { 0x07, 0x08, 0x15, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0x7e, ~
  { 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00 }    // 0x7f, DEL
};

//**************************************************************************************************
bool autoConfig()
{
  WiFi.begin();

  for (int i = 0; i < 10; i++)
  {
    clear_Display();
    char2Arr('W', 26, 0);
    char2Arr('i', 19, 0);
    char2Arr('F', 13, 0);
    char2Arr('i', 6, 0);
    refresh_display();

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      clear_Display();
      char2Arr('O', 26, 0);
      char2Arr('K', 19, 0);
      char2Arr('!', 12, 0);
      char2Arr('!', 6, 0);
      refresh_display();

      Serial.println("WiFi connected");
      Serial.println(WiFi.localIP());
      Serial.println("Starting UDP");
      udp.begin(localPort);
      Serial.print("Local port: ");
      Serial.println(udp.localPort());
      return true;
    }
    else {
      Serial.print("AutoConfig Waiting......");
      Serial.println(WiFi.status());
      delay(1000);
    }
  }
  clear_Display();
  char2Arr('E', 25, 0);
  char2Arr('r', 19, 0);
  char2Arr('r', 12, 0);
  char2Arr('!', 6, 0);
  refresh_display();
  delay(1000);
  Serial.println("AutoConfig Faild!" );
  return false;
}
void smartConfig()
{
  unsigned int count = 0;
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  clear_Display();
  char2Arr('S', 29, 0);
  char2Arr('e', 23, 0);
  char2Arr('t', 17, 0);
  char2Arr('u', 12, 0);
  char2Arr('p', 6, 0);
  refresh_display();
  count = 0;
  while (1)
  {
    Serial.print(".");
    if (count >= 60)
    {
      clear_Display();
      char2Arr('R', 25, 0);
      char2Arr('T', 19, 0);
      char2Arr('C', 12, 0);
      char2Arr('!', 6, 0);
      refresh_display();
      delay(4000);
      break;
    }

    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      udp.begin(localPort);
      WiFi.setAutoConnect(true);
      break;
    }
    delay(1000);
    count++;
  }
}

//**************************************************************************************************
tm* connectNTP() {
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.println(timeServerIP);
  Serial.println("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  udp.beginPacket(timeServerIP, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  delay(1000);
  int cb = udp.parsePacket();
  udp.read(packetBuffer, NTP_PACKET_SIZE);
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  const unsigned long seventyYears = 2208988800UL;
  epoch = secsSince1900 - seventyYears + 3600 * 8 + 2;
  time_t t;
  t = epoch;
  tm* tt;
  tt = localtime(&t);
  Serial.println(epoch);
  Serial.println(asctime(tt));
  if (cb == 48)
    return (tt);
  else
    return (NULL);
}
//**************************************************************************************************
void rtc_init(unsigned char sda, unsigned char scl) {
  Wire.begin(sda, scl);
  rtc_Write(controlREG, 0x00);
}
//**************************************************************************************************
unsigned char dec2bcd(unsigned char x) {
  unsigned char z, e, r;
  e = x % 10;
  z = x / 10;
  z = z << 4;
  r = e | z;
  return (r);
}
unsigned char bcd2dec(unsigned char x) {
  int z, e;
  e = x & 0x0F;
  z = x & 0xF0;
  z = z >> 4;
  z = z * 10;
  return (z + e);
}
//**************************************************************************************************
unsigned char rtc_Read(unsigned char regaddress) {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(regaddress);
  Wire.endTransmission();
  Wire.requestFrom((unsigned char) DS3231_ADDRESS, (unsigned char) 1);
  return (Wire.read());
}
void rtc_Write(unsigned char regaddress, unsigned char value) {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(regaddress);
  Wire.write(value);
  Wire.endTransmission();
}
//**************************************************************************************************
unsigned char rtc_sekunde() {
  return (bcd2dec(rtc_Read(secondREG)));
}
unsigned char rtc_minute() {
  return (bcd2dec(rtc_Read(minuteREG)));
}
unsigned char rtc_stunde() {
  return (bcd2dec(rtc_Read(hourREG)));
}
unsigned char rtc_wochentag() {
  return (bcd2dec(rtc_Read(WTREG)));
}
unsigned char rtc_tag() {
  return (bcd2dec(rtc_Read(dateREG)));
}
unsigned char rtc_monat() {
  return (bcd2dec(rtc_Read(monthREG)));
}
unsigned char rtc_jahr() {
  return (bcd2dec(rtc_Read(yearREG)));
}
void rtc_sekunde(unsigned char sek) {
  rtc_Write(secondREG, (dec2bcd(sek)));
}
void rtc_minute(unsigned char min) {
  rtc_Write(minuteREG, (dec2bcd(min)));
}
void rtc_stunde(unsigned char std) {
  rtc_Write(hourREG, (dec2bcd(std)));
}
void rtc_wochentag(unsigned char wt) {
  rtc_Write(WTREG, (dec2bcd(wt)));
}
void rtc_tag(unsigned char tag) {
  rtc_Write(dateREG, (dec2bcd(tag)));
}
void rtc_monat(unsigned char mon) {
  rtc_Write(monthREG, (dec2bcd(mon)));
}
void rtc_jahr(unsigned char jahr) {
  rtc_Write(yearREG, (dec2bcd(jahr)));
}
//**************************************************************************************************
void rtc_set(tm* tt) {
  rtc_sekunde((unsigned char) tt->tm_sec);
  rtc_minute((unsigned char) tt->tm_min);
  rtc_stunde((unsigned char) tt->tm_hour);
  rtc_tag((unsigned char) tt->tm_mday);
  rtc_monat((unsigned char) tt->tm_mon + 1);
  rtc_jahr((unsigned char) tt->tm_year - 100);
  if (tt->tm_wday == 0)
  {
    rtc_wochentag(7);
  }
  else
    rtc_wochentag((unsigned char) tt->tm_wday);
}
//**************************************************************************************************
float rtc_temp() {
  float t = 0.0;
  unsigned char lowByte = 0;
  signed char highByte = 0;
  lowByte = rtc_Read(tempLSBREG);
  highByte = rtc_Read(tempMSBREG);
  lowByte >>= 6;
  lowByte &= 0x03;
  t = ((float) lowByte);
  t *= 0.25;
  t += highByte;
  return (t);
}
//**************************************************************************************************
void rtc2mez() {

  unsigned short Jahr, Tag, Monat, WoTag, Stunde, Minute, Sekunde;

  Jahr = rtc_jahr();
  if (Jahr > 99)
    Jahr = 0;
  Monat = rtc_monat();
  if (Monat > 12)
    Monat = 0;
  Tag = rtc_tag();
  if (Tag > 31)
    Tag = 0;
  WoTag = rtc_wochentag();
  if (WoTag == 7)
    WoTag = 0;
  Stunde = rtc_stunde();
  if (Stunde > 23)
    Stunde = 0;
  Minute = rtc_minute();
  if (Minute > 59)
    Minute = 0;
  Sekunde = rtc_sekunde();
  if (Sekunde > 59)
    Sekunde = 0;

  MEZ.WT = WoTag;
  MEZ.sek1 = Sekunde % 10;
  MEZ.sek2 = Sekunde / 10;
  MEZ.sek12 = Sekunde;
  MEZ.min1 = Minute % 10;
  MEZ.min2 = Minute / 10;
  MEZ.min12 = Minute;
  MEZ.std1 = Stunde % 10;
  MEZ.std2 = Stunde / 10;
  MEZ.std12 = Stunde;
  MEZ.tag12 = Tag;
  MEZ.tag1 = Tag % 10;
  MEZ.tag2 = Tag / 10;
  MEZ.mon12 = Monat;
  MEZ.mon1 = Monat % 10;
  MEZ.mon2 = Monat / 10;
  MEZ.jahr12 = Jahr;
  MEZ.jahr1 = Jahr % 10;
  MEZ.jahr2 = Jahr / 10;
}

//*************************************************************************************************
const unsigned short InitArr[7][2] = { { 0x0C, 0x00 },
  { 0x00, 0xFF },
  { 0x09, 0x00 },
  { 0x0F, 0x00 },
  { 0x0B, 0x07 },
  { 0x0A, 0x04 },
  { 0x0C, 0x01 }
};
//**************************************************************************************************
void max7219_init()
{
  unsigned short i, j;
  for (i = 0; i < 7; i++) {
    digitalWrite(CS, LOW);
    delayMicroseconds(1);
    for (j = 0; j < anzMAX; j++) {
      SPI.write(InitArr[i][0]);
      SPI.write(InitArr[i][1]);
    }
    digitalWrite(CS, HIGH);
  }
}
//**************************************************************************************************
void max7219_set_brightness(unsigned short br)
{
  unsigned short j;
  if (br < 16) {
    digitalWrite(CS, LOW);
    delayMicroseconds(1);
    for (j = 0; j < anzMAX; j++) {
      SPI.write(0x0A);
      SPI.write(br);
    }
    digitalWrite(CS, HIGH);
  }
}
//**************************************************************************************************
void helpArr_init(void)
{
  unsigned short i, j, k;
  j = 0;
  k = 0;
  for (i = 0; i < anzMAX * 8; i++) {
    helpArrPos[i] = (1 << j);
    helpArrMAX[i] = k;
    j++;
    if (j > 7) {
      j = 0;
      k++;
    }
  }
}
//**************************************************************************************************
void clear_Display()
{
  unsigned short i, j;
  for (i = 0; i < 8; i++)
  {
    digitalWrite(CS, LOW);
    delayMicroseconds(1);
    for (j = anzMAX; j > 0; j--) {
      LEDarr[j - 1][i] = 0;
      SPI.write(i + 1);
      SPI.write(LEDarr[j - 1][i]);
    }
    digitalWrite(CS, HIGH);
  }
}
//*********************************************************************************************************
void rotate_90()
{
  for (uint8_t k = anzMAX; k > 0; k--) {

    uint8_t i, j, m, imask, jmask;
    uint8_t tmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (  i = 0, imask = 0x01; i < 8; i++, imask <<= 1) {
      for (j = 0, jmask = 0x01; j < 8; j++, jmask <<= 1) {
        if (LEDarr[k - 1][i] & jmask) {
          tmp[j] |= imask;
        }
      }
    }
    for (m = 0; m < 8; m++) {
      LEDarr[k - 1][m] = tmp[m];
    }
  }
}
//**************************************************************************************************
void refresh_display()
{
  unsigned short i, j;

#ifdef ROTATE_90
  rotate_90();
#endif

  for (i = 0; i < 8; i++)
  {
    digitalWrite(CS, LOW);
    delayMicroseconds(1);
    for (j = 1; j <= anzMAX; j++) {
      SPI.write(i + 1);

#ifdef REVERSE_HORIZONTAL
      SPI.setBitOrder(LSBFIRST);
#endif

#ifdef REVERSE_VERTICAL
      SPI.write(LEDarr[j - 1][7 - i]);
#else
      SPI.write(LEDarr[j - 1][i]);
#endif

#ifdef REVERSE_HORIZONTAL
      SPI.setBitOrder(MSBFIRST);
#endif
    }
    digitalWrite(CS, HIGH);
  }
}
//**************************************************************************************************
void char2Arr(unsigned short ch, int PosX, short PosY) {
  int i, j, k, l, m, o1, o2, o3, o4;
  PosX++;
  k = ch - 32;
  if ((k >= 0) && (k < 96))
  {
    o4 = font1[k][0];
    o3 = 1 << (o4 - 2);
    for (i = 0; i < o4; i++) {
      if (((PosX - i <= maxPosX) && (PosX - i >= 0))
          && ((PosY > -8) && (PosY < 8)))
      {
        o1 = helpArrPos[PosX - i];
        o2 = helpArrMAX[PosX - i];
        for (j = 0; j < 8; j++) {
          if (((PosY >= 0) && (PosY <= j)) || ((PosY < 0) && (j < PosY + 8)))
          {
            l = font1[k][j + 1];
            m = (l & (o3 >> i));
            if (m > 0)
              LEDarr[o2][j - PosY] = LEDarr[o2][j - PosY] | (o1);
            else
              LEDarr[o2][j - PosY] = LEDarr[o2][j - PosY] & (~o1);
          }
        }
      }
    }
  }
}


//**************************************************************************************************
void timer50ms() {
  static unsigned int cnt50ms = 0;
  f_tckr50ms = true;
  cnt50ms++;
  if (cnt50ms == 20) {
    f_tckr1s = true;
    cnt50ms = 0;
  }
}
//**************************************************************************************************
void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  Serial.begin(115200);
  SPI.begin();
  helpArr_init();
  max7219_init();
  max7219_set_brightness(0); //亮度设置 0~15
  rtc_init(SDA, SCL);
  clear_Display();
  refresh_display();
  tckr.attach(0.05, timer50ms);
  if (!autoConfig())
  {
    Serial.println("Start module");
    smartConfig();
  }
  tm* tt;
  tt = connectNTP();
  if (tt != NULL)
    rtc_set(tt);
  else
    Serial.println("no timepacket received");
}
//**************************************************************************************************
void loop() {
  unsigned int sek1 = 0, sek2 = 0, min1 = 0, min2 = 0, std1 = 0, std2 = 0, count = 0, sj = 0;
  unsigned int sek11 = 0, sek12 = 0, sek21 = 0, sek22 = 0;
  unsigned int min11 = 0, min12 = 0, min21 = 0, min22 = 0;
  unsigned int std11 = 0, std12 = 0, std21 = 0, std22 = 0;
  signed int x = 0;
  signed int y = 0, y1 = 0, y2 = 0, y3 = 0;
  bool updown = false;
  unsigned int sc1 = 0, sc2 = 0, sc3 = 0, sc4 = 0, sc5 = 0, sc6 = 0;
  bool f_scrollend_y = false;
  unsigned int f_scroll_x = false;
  int sensorValue = 0;
  z_PosX = maxPosX;
  d_PosX = -8;
  refresh_display();
  updown = true;
  if (updown == false) {
    y2 = -9;
    y1 = 8;
  }
  if (updown == true) {
    y2 = 8;
    y1 = -8;
  }
  while (true) {
    yield();
    if ( MEZ.std12 == 0 && MEZ.min12 == 10 && MEZ.sek12 == 0 )
    {
      clear_Display();
      delay(500);
      ESP.restart();
    }
    if (f_tckr1s == true)
    {
      rtc2mez();
      sek1 = MEZ.sek1;
      sek2 = MEZ.sek2;
      min1 = MEZ.min1;
      min2 = MEZ.min2;
      std1 = MEZ.std1;
      std2 = MEZ.std2;
      y = y2;
      sc1 = 1;
      sek1++;
      sensorValue = analogRead(A0);
      float voltage = sensorValue * (2.3 / 1023.0);
      Serial.println(sensorValue);
      Serial.println(16 - sensorValue / 64);
      if (sek1 == 10) {
        sc2 = 1;
        sek2++;
        sek1 = 0;
      }
      if (sek2 == 6) {
        min1++;
        sek2 = 0;
        sc3 = 1;
      }
      if (min1 == 10) {
        min2++;
        min1 = 0;
        sc4 = 1;
      }
      if (min2 == 6) {
        std1++;
        min2 = 0;
        sc5 = 1;
      }
      if (std1 == 10) {
        std2++;
        std1 = 0;
        sc6 = 1;
      }
      if ((std2 == 2) && (std1 == 4)) {
        std1 = 0;
        std2 = 0;
        sc6 = 1;
      }

      sek11 = sek12;
      sek12 = sek1;
      sek21 = sek22;
      sek22 = sek2;
      min11 = min12;
      min12 = min1;
      min21 = min22;
      min22 = min2;
      std11 = std12;
      std12 = std1;
      std21 = std22;
      std22 = std2;
      f_tckr1s = false;
      count++;
      if (count == 1)
      {
        sj = random(15, 30);
      }
      if (count == sj)
      {
        f_scroll_x = false; //滚动开关 true or false
        count = 0;
      }

    }
    if (f_tckr50ms == true) {
      f_tckr50ms = false;
      if (f_scroll_x == true) {
        z_PosX++;
        d_PosX++;
        if (d_PosX == 101)
          z_PosX = 0;
        if (z_PosX == maxPosX) {
          f_scroll_x = false;
          d_PosX = -8;
        }
      }
      if (sc1 == 1) {
        if (updown == 1)
          y--;
        else
          y++;
        y3 = y;
        if (y3 > 0) {
          y3 = 0;
        }
        char2Arr(48 + sek12, 4, y3);
        char2Arr(48 + sek11, 4, y + y1);
        if (y == 0) {
          sc1 = 0;
          f_scrollend_y = true;
        }
      }
      else
        char2Arr(48 + sek1, 4, 0);

      if (sc2 == 1) {
        char2Arr(48 + sek22, 9, y3);
        char2Arr(48 + sek21, 9, y + y1);
        if (y == 0)
          sc2 = 0;
      }
      else
        char2Arr(48 + sek2, 9, 0);

      if (sc3 == 1) {
        char2Arr(48 + min12, 15, y3);
        char2Arr(48 + min11, 15, y + y1);
        if (y == 0)
          sc3 = 0;
      }
      else
        char2Arr(48 + min1, 15, 0);

      if (sc4 == 1) {
        char2Arr(48 + min22, 20, y3);
        char2Arr(48 + min21, 20, y + y1);
        if (y == 0)
          sc4 = 0;
      }
      else
        char2Arr(48 + min2, 20, 0);

      if (sc5 == 1) {
        char2Arr(48 + std12, 26, y3);
        char2Arr(48 + std11, 26, y + y1);
        if (y == 0)
          sc5 = 0;
      }
      else
        char2Arr(48 + std1, 26, 0);

      if (sc6 == 1) {
        char2Arr(48 + std22, 31, y3);
        char2Arr(48 + std21, 31, y + y1);
        if (y == 0)
          sc6 = 0;
      }
      else
        char2Arr(48 + std2, 31, 0);

      refresh_display();
      if (f_scrollend_y == true) {
        f_scrollend_y = false;
      }
    }
    if (y == 0) {
    }
  }
}
