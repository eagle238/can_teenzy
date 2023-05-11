#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can3;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can4;

#include <SPI.h>
#include <ST7735_t3.h> // Hardware-specific library
#include <ST7789_t3.h> // Hardware-specific library
#include <ST7735_t3_font_Arial.h>

#define TFT_RST    32   // chip reset
#define TFT_DC     9   // tells the display if you're sending data (D) or commands (C)   --> WR pin on TFT
#define TFT_MOSI   11  // Data out    (SPI standard)
#define TFT_SCLK   13  // Clock out   (SPI standard)  
#define TFT_CS     10  // chip select (SPI standard)

int LCD_BL = 33;       // LCD back light control

ST7789_t3 tft = ST7789_t3(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
IntervalTimer timer;
uint8_t d=0;
uint8_t i=0;
uint8_t y=0;

void setup(void) {
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);  // Turn LCD backlight on
  Serial.begin(115200); delay(1000);
  Serial.println("CANBUS_2");
 
  
//can3 params

  can3.begin();
  can3.setBaudRate(500000);
  can3.setMBFilter(ACCEPT_ALL);
  can3.distribute();
  can3.mailboxStatus();
  
 //can4 params 
 
  can4.begin();
  can4.setMB(MB6,RX,STD);
  can4.setBaudRate(500000);
  //can4.setMBFilter(ACCEPT_ALL);
  can4.setMBFilter(REJECT_ALL);
  can4.enableMBInterrupts();
  can4.onReceive(MB6,canSniff);
  can4.setMBFilterRange(MB6, 0x101,0x104);
  can4.enhanceFilter(MB6);
  can4.distribute();
  can4.mailboxStatus();

  
//screen start message
  tft.init(240, 240);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLUE);
  tft.setCursor(5,5);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setFont(Arial_32);
  tft.println("Start...");
  delay(1000);

  timer.begin(sendframe, 500000); // Send frame every 500ms

}
void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(Arial_20);
  tft.setCursor(10,10);
  tft.println("CAN 1");
  i++ ; 
  tft.println(i);//count received messages on can4
  tft.println(msg.id, HEX);
  //delay(1000);
}
void sendframe()
{
  CAN_message_t msg2;
  msg2.id = 0x122;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg2.buf[i] = i + 1;
  }
  
  msg2.buf[0] = 2;
  msg2.seq =1;
  CAN_message_t msg3;
  msg3.id = 0x123;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg3.buf[i] = i + 1;
  }
  
  msg3.buf[0] =3;
  msg3.seq =1;
 CAN_message_t msg5;
  msg5.id = 0x125;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg5.buf[i] = i + 1;
  }
  
  msg5.buf[0] =6;
  msg5.seq =1;
  can3.write( msg3); //send message from can3
  can3.write( msg2);//send message from can3
  can3.write( msg5);
  
 
  CAN_message_t msg1;
  msg1.id = 0x141;
  msg1.buf[0] = 1;
  msg1.seq =1;
  CAN_message_t msg4;
  msg4.id = 0x144;
  msg4.buf[0] = 4;
  msg4.seq =1;
  can4.write(msg1);//send message from can4
  can4.write( msg4);//send message from can4
}
  


void loop() {
  tft.fillScreen(ST7735_BLACK);
  can4.events();
  CAN_message_t msgcan3;

 /*  if(can4.read(msgcan4))
  {    check if we received a CAN frame 
      tft.fillScreen(ST7735_BLACK);
      Serial.println("Got can4");
      Serial.print("MB: "); Serial.print(msgcan4.mb);
      Serial.print("  OVERRUN: "); Serial.print(msgcan4.flags.overrun);
      Serial.print("  ID: 0x"); Serial.print(msgcan4.id, HEX );
      Serial.print("  EXT: "); Serial.print(msgcan4.flags.extended );
      Serial.print("  LEN: "); Serial.print(msgcan4.len);
      
      Serial.print(" DATA: ");
      for ( uint8_t i = 0; i <msgcan4.len ; i++ ) {
        Serial.print(msgcan4.buf[i]); Serial.print(" ");
      }
      Serial.print("  TS: "); Serial.println(msgcan4.timestamp);
      //tft.fillScreen(ST7735_BLACK); 
      tft.setFont(Arial_20);
      tft.setCursor(10,150);
      tft.println("CAN 1");
      i++ ; 
      tft.println(i);//count received messages on can4
     
    
  }
    */
  if(can3.read(msgcan3))
  {   /* check if we received a CAN frame */
      
      Serial.println("Got can3");
      Serial.print("MB: "); Serial.print(msgcan3.mb);
      Serial.print("  OVERRUN: "); Serial.print(msgcan3.flags.overrun);
      Serial.print("  ID: 0x"); Serial.print(msgcan3.id, HEX );
      Serial.print("  EXT: "); Serial.print(msgcan3.flags.extended );
      Serial.print("  LEN: "); Serial.print(msgcan3.len);
      
      Serial.print(" DATA: ");
      for ( uint8_t i = 0; i <msgcan3.len ; i++ ) {
        Serial.print(msgcan3.buf[i]); Serial.print(" ");
      }
      Serial.print("  TS: "); Serial.println(msgcan3.timestamp);
      
      tft.setCursor(120,10);
      tft.setFont(Arial_20);
      tft.println("CAN 2");
      y++ ; 
      tft.setCursor(120,30);
      tft.println(y); //count received messages on can4
      tft.setCursor(120,55);
      tft.println(msgcan3.id, HEX);
      ///delay(1000);
    
  }
  delay(500);
 

}
