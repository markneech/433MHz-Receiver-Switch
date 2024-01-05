///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    433MHz Receiver interface using ON and OFF buttons from 433MHz Remote Control Keyfob                                                   //

//    Created by Mark Neech                                                                                                                  //

//    04/01/2024                                                                                                                             //

//    Version 2.1                                                                                                                            //

//    Add Relay Output on D3                                                                                                                 //

//    Add I2C OLED Display                                                                                                                   //

//    Add Run Time Code                                                                                                                      //

//    Remove SeeedOLED and exclusively use Adafruit_SSD1306.h                                                                                //

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 unsigned long receivedSignal; // Signal received from 433MHz remote control Keyfob
 unsigned long onButton = 117510576 ; // Change code for specific 433MHz remote control Keyfob's "ON" button
 //unsigned long onButton = 11345144; // Change code for specific 433MHz remote control Keyfob's "ON" button
 unsigned long offButton = 117507336 ; // Change code for specific 433MHz remote control Keyfob's "OFF" button
 //unsigned long offButton = 11345140 ; // Change code for specific 433MHz remote control Keyfob's "OFF" button

 const int RELAY = 3;

 double startTime = 0;  
 double stopTime = 0;  
 double runTime = 0;  
 //double totTime = 0; // Uncomment to reset accumulated time 
 double totTime;
   
 int Demand; // Demand for Timer  
 boolean DemandLastState = false; // Was there demand?  
 boolean Timing = false; // Is the Timer running?

 #include <EEPROM.h> 
 #include <SPI.h>
 #include <Wire.h>
 #include <RCSwitch.h>
 #include <Adafruit_SSD1306.h>

 #define SCREEN_WIDTH 128 // OLED display width, in pixels
 #define SCREEN_HEIGHT 32 // OLED display height, in pixels

 // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
 #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 RCSwitch mySwitch = RCSwitch();

 void setup() {
 Serial.begin(9600);

 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
 Serial.println(F("SSD1306 allocation failed"));
 for(;;); // Don't proceed, loop forever
 }

 // Clear the buffer
 display.clearDisplay();
 display.clearDisplay();
 display.setTextSize(1);             // Normal 1:1 pixel scale
 display.setTextColor(WHITE);        // Draw white text
 display.setCursor(0,0);             // Start at top-left corner
 display.println(F("Hello, world!")); 
 Wire.begin();                 //initialize I2C in master mode
       //Set addressing mode to Page Mode
 
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(RELAY, OUTPUT);
 mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  
 totTime = EEPROM.read(0) ; // this is where you could read from an EEPROM to continue after a restart  
 //EEPROM.write(0,0);
 }

 void loop() {
 if (mySwitch.available()) {
 receivedSignal = (mySwitch.getReceivedValue());
 mySwitch.resetAvailable();
 
 {
 if (receivedSignal == onButton) // ON 
 {
 Demand = HIGH;
 digitalWrite(LED_BUILTIN, HIGH); // Builtin LED ON with ON Signal from Keyfob
 digitalWrite(RELAY, HIGH); // Builtin LED ON with ON Signal from Keyfob
    
 }
 if (receivedSignal == offButton) // OFF
 {
 Demand = LOW;
 digitalWrite(LED_BUILTIN, LOW); // Builtin LED OFF with OFF Signal from Keyfob  
 digitalWrite(RELAY, LOW); // Builtin LED ON with ON Signal from Keyfob   
 }
 }
 //Serial.println (receivedSignal); // Used to "sniff" 433MHz signals from Keyfob
  
 if (receivedSignal == onButton) // ON
 Serial.println ("ON"); // Successful Received signal for ON when Keyfob ON pressed

 if (receivedSignal == offButton) // OFF
 Serial.println ("OFF"); // Successful Received signal for OFF when Keyfob OFF pressed

 DemandLastState = Demand; // read if the demand should be HIGH  
 
 if (Timing == false && DemandLastState == HIGH)  
 {  
          
 Timing = true;  
 runTime = 0; // reset as its already added to accumulator 
 startTime = millis();  
 Serial.print("Starting Timer. ");
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Heater Running"));
  display.display();
 }  
 else if (Timing == true && DemandLastState ==LOW) // sense LOW prevents entering this more than once  
 {  
        
 Timing = false;  
 stopTime = millis();  
 runTime +=(stopTime - startTime)/60000; // last run time in minute 
 totTime += (runTime); // add to accumulator 
 
 EEPROM.update(0,totTime);
 Serial.print("Heater ran for ");Serial.print(runTime);Serial.print(" Minutes. ");  
 Serial.print("Total time = ");Serial.println(totTime);  
 display.clearDisplay(); 
 display.setTextSize(1);             
 display.setTextColor(WHITE);        
 display.setCursor(0,0); 
 display.println(F("Heater ran for")); 
 display.setCursor(0,15);   
 display.print(String(runTime ).c_str()); //runTime data converted to a c string
 //SeeedOled.putString(String(totTime).c_str()); //totTime data converted to a c string
 display.println(F(" Minutes"));
 display.display();
  }  
 }}
 
    
