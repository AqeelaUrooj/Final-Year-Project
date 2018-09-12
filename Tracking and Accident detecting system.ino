#include<LiquidCrystal.h>
#include <TinyGPS.h>
#include<SoftwareSerial.h>
#define Relay 13
#define vib_pin 10
#define button A4
#define Buzzer_pin A3
SoftwareSerial Serial1(11,12);//when we made RX-TX-GSM
LiquidCrystal lcd (2,3,4,5,6,7);//VDD-power,VSS-VEE-RW-GROUND
TinyGPS gps;
unsigned long chars;
unsigned short sentences, failed_checksum;
char new1[15];
char new2[15];
String a; 
float lati,lon;
void setup() 
{
pinMode(vib_pin,INPUT);
pinMode(button,INPUT);
pinMode(Buzzer_pin,OUTPUT);
Serial.begin(9600);
Serial1.begin(9600);
lcd.begin(16,2);  
lcd.clear();
lcd.setCursor(3,0);
lcd.print("WELCOME TO");
//Serial.println("WELCOME TO");
lcd.setCursor(0,1);
lcd.print("TRACKING SYSTEM");
//Serial.println("TRACKING SYSTEM");
delay(3000); 
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Avoid High Speed");
lcd.setCursor(0,1);
lcd.print ("Speed Limit:80");
delay(3000);
}
String sms;
int i=0;
const int Xpin=A2;
const int Ypin=A1;
const int Zpin=A0;
int X_diff,Y_diff,Z_diff,m;
int x,c,d,t,s=0;
String num[50]={"","+923482297895","+923482297895","+923482297895","+923482297895"};
//String num[50]={"","+923482297895","+923442825783","+923352428093","+923410259114"};
void loop()
{
Extract_sms();
GPS();
vibration();
if (sms=="TRACK")
            { 
                a="CURRENT LOCATION\nLATITUDE:24.61718\nLONGITUDE:67.89933\n";//FOR TESTING
                Send_sms(a);
           }
else if (sms=="ON")
            {
                digitalWrite(Relay,HIGH);  
                lcd.clear(); 
                lcd.print("SYSTEM ON NOW"); 
                delay(1000); 
                lcd.clear();
            }
else if (sms=="OFF")
            {
                digitalWrite(Relay,LOW);  
                lcd.clear(); 
                lcd.print("SYSTEM OFF NOW"); 
                delay(1000); 
                lcd.clear();
            }


}
void GPS(){

while(Serial.available()>0){
byte  c=Serial.read();
          if (gps.encode(c))
                  {
                   gps.f_get_position(&lati,&lon);
                   gps.stats(&chars, &sentences, &failed_checksum);
                  dtostrf(lati,7, 6, new1);
                  dtostrf(lon,7, 6, new2);
                  a="CURRENT LOCATION\n""LATITUDE:"+(String)new1+"\n"+"LONGITUDE:"+(String)new2;
                  //a="lat:"+(String)new1+","+"lon:"+(String)new2+"";
                    lcd.setCursor(0,0);
                    //Serial.print("LAT: ");
                    //Serial.print(lat,6);
                    lcd.print("LAT: ");
                    lcd.print(lati,6);
                    lcd.setCursor(0,1);
                    //Serial.print("LONG:");
                    //Serial.print(lon,6);  
                    lcd.print("LONG:");
                    lcd.print(lon,6);   
                    delay(5000);
                    lcd.clear();
                   
                  } }} 
void Extract_sms(){
   String ID;
  String cell_no;
              if (Serial1.available()>0)
            {
             char a=Serial1.read();
             
             if(a=='+')
             {
              ID=Serial1.readStringUntil(':');
              if(ID=="CMT"){
              Serial1.readStringUntil('"');
              cell_no=Serial1.readStringUntil('"');
              Serial1.readStringUntil('\r');
              if(cell_no=="+923482297895")
              {
              Serial1.readStringUntil('$');
              sms=Serial1.readStringUntil('#');//condition for safety
              Serial1.readStringUntil('\n');
              Serial1.println("AT+CMGD=1,4");
              //Serial.print(">>>>>>>>>>>>>>>>>>>>>>>> SMS RECEIVED <<<<<<<<<<<<<<<<<<<<<<<<\n\n");
              //Serial.println("SMS Received from Phone No: "+cell_no);
              //Serial.println(sms);
              lcd.clear();
             lcd.print(">>SMS RECEIVED<<");
            
              }
              else{
                //Serial.println("Your not allowed to Track");
                sms="Not Allowed";
                lcd.clear();
                lcd.print(sms);
                  } }} 

  }}

void Send_sms(String mesg)
{
while(i<=4)
              {
                for(i=0;i<5;i++)
                {
              Serial1.println("AT+CMGS=\""+num[i]+"\"\r");
              delay(2000);
              //Serial.println("AT+CMGS=\""+num[i]+"\"\r");
              Serial1.println(mesg);//the content of the message
              delay(200);
              Serial1.println((char)26);//the stopping character
              delay(3000);
              Serial1.println("AT+CMGD=1,4");
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Message sent....");
              //Serial.println("Message sent....");
              //Serial.println("Sent msg to "+num[i]);
              delay(3000);
              lcd.clear();
              }}

              
   }
void vibration()
{ 

long shock_value=pulseIn (vib_pin,HIGH);
  Serial.println (shock_value);
  delay(10);
  if (shock_value > 100000   ) {

    x = 1;
    c=millis();
  digitalWrite(Buzzer_pin,HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Confirmation delay");
  lcd.setCursor(0,1);
  lcd.print("Press Button...");
  lcd.clear();    
    }
  if(x==1){
    
    d=millis();
    t=d-c;
    s=digitalRead(button);
      if(s>0 && t<10000){
        digitalWrite(Buzzer_pin,LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("False Alarm");
        lcd.setCursor(0,1);
        lcd.print("Acknowledged");
        lcd.clear();
        //Serial.println("False Alarm");
        x=0; 
        }
      if(t>=10000){
        
        x=0;
        digitalWrite(Buzzer_pin,LOW);
              GPS();
             Send_sms(a);
             //Serial.println("No Alarm");
             //Serial.println ("SEND SMS");
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("SMS SEND");
             lcd.clear();
      }
  }

}
void Acc()
{
//Serial.println(analogRead(Xpin));
X_diff=abs(analogRead(Xpin)-803);
Serial.print(e);
Serial.print("\t");
//Serial.println(analogRead(Ypin));
Y_diff=abs(analogRead(Ypin)-909);
Serial.print(f);
Serial.print("\t");
//Serial.println(analogRead(Zpin));
Z_diff=abs(analogRead(Zpin)-904);
Serial.print(g);
Serial.print("\t");
 Serial.print("\n");
  delay(2000);
 if (X_diff<40 && Y_diff<40 )
  {
    m=0;
    Serial.print("s");
  }
  if(X_diff>40 || Y_diff>40 ){
    
    if( m==0)
    {    
    digitalWrite(Buzzer_pin,HIGH);
    delay(2000);
  // a="CURRENT LOCATION\nLATITUDE:24.61718\nLONGITUDE:67.89933\n";//FOR TESTING
                //a="lat:24.61718,lon:67.89933";
                GPS();
                Serial.println(a);
//                Send_sms(a);
             Serial.println ("SEND SMS");
             lcd.clear();
             lcd.setCursor(0,0);
             lcd.print("SMS SEND");
             lcd.clear();
             
           digitalWrite(Buzzer_pin,LOW);
  m=1;
  }
  }
             

  }
  

