#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include<math.h>

// connecting to the firebase.
#define FIREBASE_HOST "water-level-6e159.firebaseio.com"    
#define FIREBASE_AUTH "SbNhbiTYUAfnYTLsRRIVrNTDYHh6GZ03pVvqru5q"
#define WIFI_SSID "JioFi 5g"
#define WIFI_PASSWORD "Jaisingh@123#"


const int Relay1 = 14 ; //D5

// defines pins numbers
const int trigPin = 4;  //D2
const int echoPin = 5;  //D1


// defines variables
long duration;
int distance=0;
int rel1=0;
int total;
long divi;
int percentage;

void setup() 
{
  
 Serial.begin(9600);   // Starts the serial communication.
 

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(Relay1,OUTPUT);  //set the relay as an output.
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output.
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input.

  Firebase.set("percentage",0);   
  Firebase.set("Relay",0);    //Here the varialbe  needs to be  used in our Firebase and App.
  
}


void loop()
{

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);


// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);


   
// Calculating the distance
distance = duration * 0.034 / 2;


// Setting the level and calculating the percentage.
total=19-distance;
divi=(total*100)/16;
percentage=round(divi);


// Sends the distance value to Firebase.
 Firebase.setInt("percentage", percentage);

 
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
Serial.print("Percentage : ");
Serial.println(percentage);
delay(1000);

if( percentage>=100)
   {
        Firebase.setInt("Relay",0); 
         rel1=0;                  //Setting  relay 0 when percentage equal or greater then 100.
   }

 rel1=Firebase.getInt("Relay");      //Reading the value of the varialble Status from the firebase.
                                 

 if(rel1==1)          // If, the Status is 1, turn on moter.
     {
      digitalWrite(Relay1,LOW);
      Serial.println("Moter ON");
    }

    
 if(rel1==0 )               // If, the Status is 0, turn Off moter .
    {                                      
      digitalWrite(Relay1,HIGH);
      Serial.println("Moter OFF");
  }  

}
