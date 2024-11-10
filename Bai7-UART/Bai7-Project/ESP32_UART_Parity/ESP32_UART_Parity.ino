// Define TX and RX pins for UART (change if needed)
#define TXD1 19
#define RXD1 21

// Use Serial1 for UART communication
HardwareSerial mySerial(2);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8E1, RXD1, TXD1);  // UART setup
  
  Serial.println("ESP32 UART Receiver");
}

void loop() {
  // Check if data is available to read
  
  if (mySerial.available()) 
  {
    // Read data and display it
    /*
    int receivedData = mySerial.read();  // Chỉ lấy 8 bit thấp
    
    Serial.print("Received (8-bit data): ");
    Serial.println(receivedData);*/
    //Serial.println(int(mySerial.read()));
    
    String message = mySerial.readStringUntil('\n');
    Serial.println("Received: " + message);
    delay(100);
    
    String sendData = "ESP32ToSTM32";
    Serial.println("Sending: " + sendData);
    mySerial.println(sendData);
  }
  
  /*String sendData = "ESP32ToSTM32";
  String sendData = "E";
  Serial.println("Sending: " + sendData);
  mySerial.println(sendData);
  delay(2000);*/
}