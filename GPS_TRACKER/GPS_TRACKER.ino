#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2, 3); //TX, RX
LiquidCrystal_I2C lcd(0x27,20,4);

#define M0 8    
#define M1 9             

float llat, llon;
float latRad, lonRad;
float flat, flon;
float distance_km;
float LatTotal, LongTotal;
#define Buzzer 4 

String targetLat = "-8.074912";
String targetLon = "111.921845";

void setup() {
  lcd.init();
  pinMode (Buzzer, OUTPUT);
  lcd.backlight();
  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(M0, OUTPUT);        
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);     
  digitalWrite(M1, LOW);    
  
}

void loop() {
  if(mySerial.available()){
    digitalWrite (Buzzer, HIGH);
    delay(100);
    digitalWrite (Buzzer, LOW);
    String input = mySerial.readString();
    Serial.print("Data Input : ");Serial.print(input);
    String lattitude = getValue(input, '#', 2);
    Serial.print("Lattitude : ");Serial.println(lattitude);
    String longitude = getValue(input, '#', 3);
    Serial.print("Longitude : ");Serial.println(longitude);

    LatTotal = parsLat(lattitude, targetLat);
    Serial.println();
    LongTotal = parsLong(longitude, targetLon);
    float ecludian = (sqrt(LatTotal + LongTotal))/1000 ;
    distance_km = (ecludian )*111;
    Serial.println(); 
    Serial.println(ecludian);
    Serial.print("Jarak : ");Serial.println(distance_km, 3);
    lcd.setCursor(0, 0);
    lcd.print(lattitude);
    lcd.setCursor(12, 0);
    lcd.print(distance_km);
    lcd.setCursor(0, 1);
    lcd.print(longitude);
    
    
    Serial.println("=============================================");
  }  
  digitalWrite (Buzzer, LOW);
}


float parsLat (String Lattitude, String TargetLat){
    // Lattitude saat ini
    String LatA = Lattitude.substring(0, 5);
    float fLatA = LatA.toFloat();
    Serial.print("LatA = ");Serial.println(LatA);
    String LatB = Lattitude.substring(5, 9);
    float fLatB = LatB.toFloat();
    Serial.print("LatB = ");Serial.println(LatB);
    Serial.println();
    
    // Lattitude target
    String TLatA = TargetLat.substring(0, 5);
    float fTLatA = TLatA.toFloat();
    Serial.print("LatA_penyebrangan = ");Serial.println(TLatA);
    String TLatB = TargetLat.substring(5, 9);
    float fTLatB = TLatB.toFloat();
    Serial.print("LatB_penyebrangan = ");Serial.println(TLatB);

    //hitung Lat
    float HLatA = (fTLatA - fLatA)*1000000; // dikali satu juta
    float HLatB = fTLatB - fLatB;
    Serial.print("LatA - LatA_penyebrangan = ");Serial.println(HLatA);
    Serial.print("LatB - LatB_penyebrangan = ");Serial.println(HLatB); 
    float totLat = sq(HLatA - HLatB)/1000000;
    Serial.println(totLat);
    
    return totLat;
}

float parsLong (String Longitude, String TargetLong){
    // Longitude saat ini
    String LongA = Longitude.substring(0, 3);
    float fLongA = LongA.toFloat();
    Serial.print("LongA = ");Serial.println(LongA);
    String LongB = Longitude.substring(4, 7);
    float fLongB = LongB.toFloat();
    Serial.print("LongB = ");Serial.println(LongB);
    String LongC = Longitude.substring(7, 11);
    float fLongC = LongC.toFloat();
    Serial.print("LongC = ");Serial.println(LongC);
    Serial.println();
    
    // Longitude target
    String TLongA = TargetLong.substring(0, 3);
    float fTLongA = TLongA.toFloat();
    Serial.print("LongA_penyebrangan = ");Serial.println(TLongA);
    String TLongB = TargetLong.substring(4, 7);
    float fTLongB = TLongB.toFloat();
    Serial.print("LongB_penyebrangan = ");Serial.println(TLongB);
    String TLongC = TargetLong.substring(7, 11);
    float fTLongC = TLongC.toFloat();
    Serial.print("LongC_penyebrangan = ");Serial.println(TLongC);

    //hitung Long
    float HLongA = (fTLongA - fLongA)*1000000;
    float HLongB = (fTLongB - fLongB)*1000;
    float HLongC = fTLongC - fLongC;
    Serial.print("LongA - LongA_penyebrangan = ");Serial.println(HLongA);
    Serial.print("LongB - LongB_penyebrangan = ");Serial.println(HLongB); 
    Serial.print("LongC - LongC_penyebrangan = ");Serial.println(HLongC); 
    float totLong = sq((HLongA - HLongB)-HLongC)/1000000;
    Serial.println(totLong);

    return totLong;
}
 
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}



