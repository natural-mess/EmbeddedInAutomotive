// Define TX and RX pins for UART (change if needed)
#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);  // UART setup
  
  Serial.println("ESP32 UART Receiver");
}

void loop() {
  // Check if data is available to read
  
  if (mySerial.available()) {
    // Read data and display it
    //Serial.println(int(mySerial.read()));
    String message = mySerial.readStringUntil('\n');
    Serial.println("Received: " + message);
    delay(100);
    
    String sendData = "ESP32ToSTM32";
    Serial.println("Sending: " + sendData);
    mySerial.println(sendData);
  }
  /*
  String sendData = "ESP32ToSTM32";
  Serial.println("Sending: " + sendData);
  mySerial.println(sendData);*/
}