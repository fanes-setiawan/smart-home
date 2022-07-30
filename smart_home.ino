//library yang akan dibutuhkan
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

DHT dht(4,DHT11); //nama object sensor suhu "dht"
LiquidCrystal_I2C lcd(0x27, 16, 2); //nama object LCD 16x2 "lcd"
FirebaseData firebaseData; //nama object firebase
Servo servo; //nama object dari Servo"servo"

//alamat untuk menyambungkan wifi (seperti nama hotspot dan sandi)
const char* ssid = "AndroidAP";
const char* password = "11111111";

//Variabel untuk sensor suhu
String path = "/dht11";
uint8_t i=0;
float humidity;
float temperature;

void setup(){//fungsi setup akan di baca satu kali
  Serial.begin(115200); //memulai serial monitor
  dht.begin(); //memulai dht/data suhu
  lcd.clear(); // membersikan lcd/menghapus
  lcd.init(); 
  lcd.backlight();
  pinMode(D5,OUTPUT);pinMode(D6,OUTPUT);pinMode(D7,OUTPUT);pinMode(D8,OUTPUT); //bagian mengatur pin Digital LED sebagai OUTPUT
  servo.attach(D2); //pin dari servo
  servo.write(100); //servo memulai dari sudut 0 drajat
  konekWifi(); // memangil fungsi konekWiFi
  Firebase.begin("https://db-home-12141-default-rtdb.firebaseio.com/", "07i4junPRN2Qarb7HMl4YXt6NhaHnigNMYpu3HBf;"); //uuntuk menghubungkan di firebase
  //Firebase.begin("firebase host", "firebase auth database");
}
 
void konekWifi() {//membuat fungsi untuk menghubungkan wemos D1 ke wifi
  WiFi.begin(ssid, password);//memulai menghubungkan ke wifi router
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); //status saat mengkoneksikan
  }
  Serial.println("Sukses terkoneksi wifi!");
  Serial.println("IP Address:"); //alamat ip lokal
  Serial.println(WiFi.localIP());
}

 void loop() {//fungsi loop akan membaca program berulang2
 humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.print("Kirim data ke firebase");
  if(Firebase.setFloat(firebaseData,path +"/suhu",+temperature)){ //mengambil data suhu dan di tampilkan di firebase
    Serial.print("PART: "+ firebaseData.dataPath());
    Serial.print("TYPE: "+ firebaseData.dataType());
    //print di serial monitor
    Serial.print("Temperature: "); 
    Serial.print(" °C ");
    //print pada lcd 16x2
    lcd.setCursor(0,0);
    lcd.print("Suhu  = ");
    lcd.setCursor(7,0);
    lcd.print(temperature);
    lcd.setCursor(13,0);
    lcd.print("C");
  }
  if(Firebase.setFloat(firebaseData,path+"/kelembaban",+humidity)){ //mengambil data kelembaban dan di tampilkan di firebase
    //print pada serial monitor
    Serial.print(" Humidity: ");
    Serial.print(humidity);//mengambil data kelembaban
    Serial.println( "%" );
    //print pada lcd 16x2
    lcd.setCursor(0,1);
    lcd.print("Humid = ");
    lcd.setCursor(7,1);
    lcd.print(humidity);
    lcd.setCursor(13,1);
    lcd.print("%");
  }else{
    Serial.println("Gagal");
    Serial.println("REASON: " +firebaseData.errorReason());
    Serial.println("--------------------------------------");
    Serial.println();
  }
  delay(1000);
Lantai1(); //memangil fungsi "Lantai1" 
Lantai2(); //memangil fungsi "Lantai2" 
Lantai3(); //memangil fungsi "Lantai3" 
Garasi(); //memangil fungsi "Garasi" 
}
void Lantai1(){//fungsi untuk menyalakan danmematikan lampu/led lantai1
  if (Firebase.getString(firebaseData, "/Lantai1")) { //misal database diberikan nama Lantai 1
    if  (firebaseData.dataType() == "string") 
    {
      String A = firebaseData.stringData();
      if (A == "ON") {                                                      
      Serial.println("LANTAI 1 TERANG");                         
      digitalWrite(D5, HIGH); }
        else if (A == "OFF") {                                                  
        Serial.println("LANTAI 1 PADAM");
        digitalWrite(D5, LOW);                                                
        }else {digitalWrite(D5,LOW);}
    }
  }
}
void Lantai2(){ //fungsi untuk menyalakan danmematikan lampu/led lantai2
  if (Firebase.getString(firebaseData, "/Lantai2")) { //misal database diberikan nama Lantai2
    if  (firebaseData.dataType() == "string") 
    {
      String B = firebaseData.stringData();
      if (B == "ON") {                                                         
      Serial.println("LANTAI 2 TERANG");                         
      digitalWrite(D6, HIGH); }
        else if (B == "OFF") {                                                  
        Serial.println("LANTAI 2 PADAM");
        digitalWrite(D6, LOW);                                                
        }else {digitalWrite(D6,LOW);}
    }
  }
}
void Lantai3(){//fungsi untuk menyalakan danmematikan lampu/led Lantai3
  if (Firebase.getString(firebaseData, "/Lantai3")) { 
    if  (firebaseData.dataType() == "string") 
    {
      String C = firebaseData.stringData();
      if (C == "ON") {                                                         
      Serial.println("LANTAI 3 TERANG");                       
      digitalWrite(D7, HIGH); }
        else if (C == "OFF") {                                                  
        Serial.println("LANTAI 3 PADAM");
        digitalWrite(D7, LOW);                                                
        }else {digitalWrite(D7,LOW);}
    }
  }
}
void Garasi(){
  if (Firebase.getString(firebaseData, "/Garasi")) { //misal database diberikan nama Garasi
    if  (firebaseData.dataType() == "string") 
    {
      String D = firebaseData.stringData();
      if (D == "BUKA") { 
           servo.write(0);
           Serial.println("GARASI TERBUKA"); 
           digitalWrite(D8,HIGH);
           }
        else if (D == "TUTUP") {                                                  
        Serial.println("GARASI TUTUP");
        servo.write(100);
        digitalWrite(D8,LOW);                                           
        }else {servo.write(0);}
    }
  }
}

/*NAMA    : FANES SETIAWAN
 *KELAS   : XII AUDIO VIDEO .B
 *NO.ABSEN: 09
 **\
 */
