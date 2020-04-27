#include <YoutubeApi.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "ap";
char password[] = "12345678";

#define API_KEY "AIzaSyAbnQYnu8c9deR1-IxJdCVe706W4FG-Avw"
#define CHANNEL_ID "UCOONBzaKXvoWvHfQ7F38p9Q"

// NodeMCU Dev Kit => D1 = SCL, D2 = SDA
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 15000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long subs = 0;

void setup() {

  Serial.begin(115200);
  Serial.println();
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  // connect to wifi.
  WiFi.begin(ssid, password);
  Serial.print("connecting");
  lcd.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(WiFi.localIP());
  lcd.setCursor(2, 1);
  lcd.print("wait for data");

}

void loop() {

  if (millis() - api_lasttime > api_mtbs)  {
    if (api.getChannelStatistics(CHANNEL_ID))
    {
      lcd.clear();
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);

      lcd.setCursor(0, 0);
      lcd.print("SUB: ");
      lcd.print(api.channelStats.subscriberCount);

      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);

      lcd.setCursor(0, 1);
      lcd.print("View ");
      lcd.print(api.channelStats.viewCount);

      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }

}
