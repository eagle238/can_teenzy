#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

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
  Serial.println("CANBUS_1");
   delay(3000);
  
  
//can 2 params

  can2.begin();
  can2.setBaudRate(500000);
  can2.enableFIFO();
  can2.setMBFilter(ACCEPT_ALL);
  can2.distribute();
  can2.mailboxStatus();
  
 //can1 params 
 
  can1.begin();
  can1.setMB(MB6,RX,STD);
  can1.setBaudRate(500000);
  //can1.setMBFilter(ACCEPT_ALL);
  can1.setMBFilter(REJECT_ALL);
  can1.enableMBInterrupts();
  can1.onReceive(MB6,canSniff);
  can1.setMBFilterRange(MB6, 0x122,0x125);
  can1.enhanceFilter(MB6);
  can1.distribute();
  can1.mailboxStatus();

  
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
  //Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  //Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  //Serial.print(" TS: "); Serial.print(msg.timestamp);
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
  tft.println(i);//count received messages on can1
  tft.println(msg.id, HEX);
  //delay(1000);
}
void sendframe()
{
  CAN_message_t msg2;
  msg2.id = 0x102;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg2.buf[i] = i + 1;
  }
  
  msg2.buf[0] = 2;
  msg2.seq =1;
  CAN_message_t msg3;
  msg3.id = 0x103;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg3.buf[i] = i + 1;
  }
  
  msg3.buf[0] =3;
  msg3.seq =1;
 CAN_message_t msg5;
  msg5.id = 0x105;
  
  for ( uint8_t i = 0; i < 8; i++ ) {
    msg5.buf[i] = i + 1;
  }
  
  msg5.buf[0] =6;
  msg5.seq =1;
  can2.write( msg3); //send message from can2
  can2.write( msg2);//send message from can2
  can2.write( msg5);
  
 
  CAN_message_t msg1;
  msg1.id = 0x121;
  msg1.buf[0] = 1;
  msg1.seq =1;
  CAN_message_t msg4;
  msg4.id = 0x124;
  msg4.buf[0] = 4;
  msg4.seq =1;
  can1.write(msg1);//send message from can1
  can1.write( msg4);//send message from can1
}
  


void loop() {
  tft.fillScreen(ST7735_BLACK);
  can1.events();
  CAN_message_t msgcan2;

 /*  if(can1.read(msgcan1))
  {    check if we received a CAN frame 
      tft.fillScreen(ST7735_BLACK);
      Serial.println("Got can1");
      Serial.print("MB: "); Serial.print(msgcan1.mb);
      Serial.print("  OVERRUN: "); Serial.print(msgcan1.flags.overrun);
      Serial.print("  ID: 0x"); Serial.print(msgcan1.id, HEX );
      Serial.print("  EXT: "); Serial.print(msgcan1.flags.extended );
      Serial.print("  LEN: "); Serial.print(msgcan1.len);
      
      Serial.print(" DATA: ");
      for ( uint8_t i = 0; i <msgcan1.len ; i++ ) {
        Serial.print(msgcan1.buf[i]); Serial.print(" ");
      }
      Serial.print("  TS: "); Serial.println(msgcan1.timestamp);
      //tft.fillScreen(ST7735_BLACK); 
      tft.setFont(Arial_20);
      tft.setCursor(10,150);
      tft.println("CAN 1");
      i++ ; 
      tft.println(i);//count received messages on can1
     
    
  }
    */
  if(can2.read(msgcan2))
  {   /* check if we received a CAN frame */
      
      Serial.println("Got can2");
      Serial.print("MB: "); Serial.print(msgcan2.mb);
      //Serial.print("  OVERRUN: "); Serial.print(msgcan2.flags.overrun);
      Serial.print("  ID: 0x"); Serial.print(msgcan2.id, HEX );
      //Serial.print("  EXT: "); Serial.print(msgcan2.flags.extended );
      //erial.print("  LEN: "); Serial.print(msgcan2.len);
      
      Serial.print(" DATA: ");
      for ( uint8_t i = 0; i <msgcan2.len ; i++ ) {
        Serial.print(msgcan2.buf[i]); Serial.print(" ");
      }
      Serial.print("  TS: "); Serial.println(msgcan2.timestamp);
      
      tft.setCursor(120,10);
      tft.setFont(Arial_20);
      tft.println("CAN 2");
      y++ ; 
      tft.setCursor(120,30);
      tft.println(y); //count received messages on can1
      tft.setCursor(120,55);
      //tft.println(msgcan2.id, HEX);
      delay(50);
      
      
    
  }
  //tft.println(y);
  
 

}
