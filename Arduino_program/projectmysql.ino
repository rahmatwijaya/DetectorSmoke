#include <MQ2.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define FIREBASE_HOST "iotproject-7f7d4-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "4PpCsBK91O3fqMimzfT7qwN2FSgvXjK5OdlKjije"
//firebase project setting/ service accounts/secret

const byte asap = A0;
//const byte led = 4;
const byte buzzer = 5;
int batasacuan =200;

const long utcOffsetInSeconds = 25200;  // set offset
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);

char Time[ ] = "00:00:00";
char Date[ ] = "00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
String kondisi;

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Jay";
const char* password = "jjjjaayy";
const char* host = "192.168.43.2";


void setup()
{
    Serial.begin(115200);
    dht.begin();
    pinMode(asap, INPUT);
//    pinMode(led, OUTPUT);
    pinMode(asap, OUTPUT);
    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
    timeClient.begin();
    delay(100);
}

void loop()
{
  float sensor = analogRead(asap);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();

  second_ = second(unix_epoch);
  if (last_second != second_) {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
    
    Time[7] = second_ % 10 + 48;
    Time[6] = second_ / 10 + 48;
    Time[4]  = minute_ % 10 + 48;
    Time[3]  = minute_ / 10 + 48;
    Time[1]  = hour_   % 10 + 48;
    Time[0]  = hour_   / 10 + 48;

    Date[0]  = day_   / 10 + 48;
    Date[1]  = day_   % 10 + 48;
    Date[3]  = month_  / 10 + 48;
    Date[4]  = month_  % 10 + 48;
    Date[8] = (year_   / 10) % 10 + 48;
    Date[9] = year_   % 10 % 10 + 48;
    
    Serial.println(Time);
    Serial.println(Date);
    last_second = second_;
    
    Serial.print("connecting to ");
    Serial.println(host);
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Kelembapan:");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Sensor Asap:");
    Serial.println(sensor);
    
    Firebase.setFloat("Temperature", temperature);
    Firebase.setFloat("Humidity", humidity);
    Firebase.setFloat("Sensor Api", sensor);
    Firebase.setString("Tanggal", Date);
    Firebase.setString("Waktu", Time);
    
   if(temperature<=34){
    Firebase.setString("Kondisi", kondisi = "Suhu Normal");
  } else{
    Firebase.setString("Kondisi", kondisi = "Suhu Panas");
  }
  
  
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    HTTPClient http;
    
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    String Link;
    Link = "http://" + String(host) + "/iottugasversi2/datasensor.php?temperature=" + String(temperature) + "&kelembapan=" + String(humidity) + "&sensorapi=" + String(sensor);

    http.begin(Link);
    http.GET();

    String respon = http.getString();
    Serial.println(respon);
    http.end();
  }
  
    if (sensor > batasacuan)
   {
//     digitalWrite(led, HIGH);
     kirimnotifikasi("Alarm Apps", "Terjadi Suhu Extrem");
     tone(buzzer, 1000);
   }
   else
   {
//     digitalWrite(led, LOW);
     noTone(buzzer);
   }
   if (Firebase.failed()){
    Serial.print("koneksi firebase error");
    Serial.println(Firebase.error());
    return;
  }
}

void kirimnotifikasi(String judul, String isi)
{
  judul.replace(" ", "%20");
  isi.replace(" ", "%20");
  const char* host = "192.168.43.2";
  WiFiClient client;
  HTTPClient http;
  if(!client.connect(host, 80)){
    Serial.println("Connection failed");
    return;
    }   
    String Link;
    Link = "http://192.168.43.2/iottugasversi2/notif.php?judul=" + judul + "&isi=" + isi;

    http.begin(Link);
    http.GET();
    String respon = http.getString();
    Serial.println(respon);
    http.end();
}
