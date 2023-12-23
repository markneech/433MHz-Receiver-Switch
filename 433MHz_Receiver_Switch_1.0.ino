///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//    433MHz Receiver interface using ON and OFF buttons from 433MHz Remote Control Keyfob                                                   //

//    Created by Mark Neech                                                                                                                  //

//    23/12/2023                                                                                                                             //

//    Version 1.0                                                                                                                           //

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 unsigned long receivedSignal; // Signal received from 433MHz remote control Keyfob
 unsigned long onButton = 117510576 ; // Change code for specific 433MHz remote control Keyfob's "ON" button
 unsigned long offButton = 117507336 ; // Change code for specific 433MHz remote control Keyfob's "OFF" button
 
 #include <RCSwitch.h>

 RCSwitch mySwitch = RCSwitch();

 void setup() {
 Serial.begin(9600);
 pinMode(LED_BUILTIN, OUTPUT);
 mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
 }

 void loop() {
 if (mySwitch.available()) {
 receivedSignal = (mySwitch.getReceivedValue());
 mySwitch.resetAvailable();
 
  {
  if (receivedSignal == onButton) // ON 
  {
    digitalWrite(LED_BUILTIN, HIGH); // Builtin LED ON with ON Signal from Keyfob
  }
  if (receivedSignal == offButton) // OFF
  {
    digitalWrite(LED_BUILTIN, LOW); // Builtin LED OFF with OFF Signal from Keyfob                 
  }

  }
  Serial.println (receivedSignal); // Used to "sniff" 433MHz signals from Keyfob

  if (receivedSignal == onButton) // ON
  Serial.println ("ON"); // Successful Received signal for ON when Keyfob ON pressed

  if (receivedSignal == offButton) // OFF
  Serial.println ("OFF"); // Successful Received signal for OFF when Keyfob OFF pressed
   
  }  
}
 
    
