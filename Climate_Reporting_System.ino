#include <WiFi.h> //wifi's library.
#include <DHT.h> // DHT22 sensor's library.
#include <ThingSpeak.h> // THing speak's library ( the platform which is used to build our GUI(Graphical User Interface)).

int blue=16; //the pin number which the blue led is connected with.
int green=17; //the pin number which the green led is connected with.

int dry=2640; //from the calibration for the capacitive soil moisture sensor, it was the maximum value which idicates that 0% of water ardound it.  
int wet=1850; //from the calibration for the capacitive soil moisture sensor, it was the minimum value which idicates that 100% of water ardound it.

#define DHTPIN 4 // Digital pin connected to the DHT sensor.
#define DHTTYPE DHT22   // DHT 22.
DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.

#define CHANNEL_ID 1999564 // the channel ID of the channel of our GUI.
#define CHANNEL_API_KEY "J5H0SBCRV1DS1HMS" // the channel API KEY of the channel of our GUI.
WiFiClient client;//Creates a client that can connect to ESP32.

const char* ssid = "Group19326"; // Nework name which ESP32 will connect.
const char* password = "19326CAP"; // Network passowrd. 
unsigned long interval = 30000; // how long ESP32 can attampt to connect to the wifi network.
unsigned long currentMillis = millis(); //storing the time which ESP32 startted its connection attampt. 
//function to connect to the WiFi.
void connecToWiFi() {
  WiFi.mode(WIFI_AP);//The Access Point(AP) mode allows you to use the ESP32 to create a WiFi network to connect.
  WiFi.begin(ssid, password); //calling the ESP32 to connect to حthe network we declared before.
  
  while (WiFi.status() != WL_CONNECTED && (currentMillis < interval)) /* if the ESP32 hasn't connected to the wifi network yet and the
  time which started its attampt is less than the time which should finish its attampt, make the green led on and when it has been connecting
  to wifi open the blue led and close the other one.*/{
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    delay(100);
  }
 digitalWrite(green, LOW);
  
 digitalWrite(blue, HIGH);
 }

void setup() {
  //Declare the LEDs as an output.
  pinMode(blue, OUTPUT); 
  pinMode(green, OUTPUT);
  connecToWiFi(); // call connecToWiFi function to start connecting to wifi.
  dht.begin(); //Start and initialize the dht22 sensor.
  ThingSpeak.begin(client);// Initialize ThingSpeak.
}

void loop() {
  if(WiFi.status() != WL_CONNECTED)/* check wifi connection and if it was lost, close the blue led, open green 
  led and reconnect with again then open the blue led and close the other one.*/  {
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED && (currentMillis < interval)) {
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);
    delay(100);
  }
 }
  else{
 digitalWrite(green, LOW);
 digitalWrite(blue, HIGH);
  }
  float h = dht.readHumidity(); // Read humidity.
  float t = dht.readTemperature(); // Read temperature as Celsius (the default).

  int val = analogRead(36);// store the reading of the capacitive soil moisture sensor in val variable.
  // Sensor has a range of 1024to 633.
  // We want to translate this to a scale or 0% to 100%.
  int percentageHumididy = map(val, wet, dry, 100, 0);  
  ThingSpeak.setField(1, h); //send the DHT22 readings (humidity) to field 1 in thing speak.
  ThingSpeak.setField(2, t);//send the DHT22 readings (temperature) to field 2 in thing speak.
  ThingSpeak.setField(3, percentageHumididy);//send the capacitive soil moisture sensor to field 3 in thing speak.
  ThingSpeak.writeFields( CHANNEL_ID, CHANNEL_API_KEY);// determine the channel which ESP32 will send the data.
  delay(15000);// stop for 15 sec and update thing speak fields.
  // our GUI link:https://thingspeak.com/channels/1976108
}
//if u want to have a look on system's cirduit, u can visit this link: https://drive.google.com/drive/folders/1oadIBi9AAuemlmkpC5C__7qEIJRDxS6W?usp=share_link
