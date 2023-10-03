#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>

#define PN532_IRQ   (2)
#define PN532_RESET (3)

#define NEO_PIN   6
#define NEO_COUNT 144

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
Adafruit_NeoPixel pixels(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel jewel(12, 7, NEO_GRB + NEO_KHZ800);


//Tag numbers
//round
//0x02 0x22 0xBB 0x89 ox87 0xE0 0x00
//0x02 0x22 0xC8 0xEA 0x87 0xE0 0x00
//0x02 0x22 0xC9 0xCD 0x14 0x00 0x00
//0x02 0x22 0xC9 0x0D 0x94 0x00 0x00
//0x02 0x22 0xD5 0xB8 0x14 0x00 0x00
//inlay
//0x04 0x1B 0xB3 0x12 0x8D 0x11 0x90

//commands 
// 0x00 = White bottle
// 0x01 = Black bottle
// 0x02 = Red bottle
// 0x03 = Blue bottle
// 0x04 = Yellow bottle
// 0x05 = Wand

uint8_t tagArray[6][8]={  {0x02, 0x22, 0xBB, 0x89, 0x87, 0xE0, 0x00, 0x01},
                      {0x02, 0x22, 0xC8, 0xEA, 0x87, 0xE0, 0x00, 0x02},
                      {0x02, 0x22, 0xC9, 0xCD, 0x14, 0x00, 0x00, 0x03},
                      {0x02, 0x22, 0xC9, 0x0D, 0x94, 0x00, 0x00, 0x04},
                      {0x02, 0x22, 0xD5, 0xB8, 0x14, 0x00, 0x00, 0x05},
                      {0x04, 0x1B, 0xB3, 0x12, 0x8D, 0x11, 0x90, 0x06}};


int colourArray[10]={0,0,0,0,0,0,0,0,0,0};
int colourPos=0;
int colourMix[3]={0,0,0};

int colour = 0x00;

int colourArrayGame[3]={0,0,0};
int colourPosGame=0;

int neoArray[11]={0,14,28,43,57,71,85,100,114,128,144};
int neoArrayGame[4]={0,48,96,144};

int neoBright = 50;

bool mode = false; //False is normal, true game mode

int gameStart = 0;
int gameArray[3]={0,0,0};


void displayColours(int disMode){
  //pixels.clear();
  if (disMode==0){

    colourMix[0]=0;
    colourMix[1]=0;
    colourMix[2]=0;
    for(int i=0; i<10; i++){
      for (int j=neoArray[i]; j<neoArray[i+1]; j++){
        switch (colourArray[i]) {
          case 0:
            //Serial.println("None");
            pixels.setPixelColor(j, pixels.Color(0,0,0));
            break;
          case 1:
            //Serial.println("Tag 0");
            pixels.setPixelColor(j, pixels.Color(255,0,0));
            break;
          case 2:
            //Serial.println("Tag 1");
            pixels.setPixelColor(j, pixels.Color(0,255,0));
            break;
          case 3:
            //Serial.println("Tag 2");
            pixels.setPixelColor(j, pixels.Color(0,0,255));
          default:
            //Serial.println("Not a colour tag");
            break;
        }
      
      }
      switch(colourArray[i]){
        case 1:
          colourMix[0]=colourMix[0]+25;
          break;
        case 2:
          colourMix[1]=colourMix[1]+25;
          break;
        case 3:
          colourMix[2]=colourMix[2]+25;
          break;
        default:
        break;
      }
    }
    pixels.show();
    Serial.print("Colour Mix: R = ");
    Serial.print(colourMix[0]);
    Serial.print(" G = ");
    Serial.print(colourMix[1]);
    Serial.print(" B = ");
    Serial.println(colourMix[2]);
  }else{
    colourMix[0]=0;
    colourMix[1]=0;
    colourMix[2]=0;
    for(int i=0; i<3; i++){
      for (int j=neoArrayGame[i]; j<neoArrayGame[i+1]; j++){
        switch (colourArrayGame[i]) {
          case 0:
            //Serial.println("None");
            pixels.setPixelColor(j, pixels.Color(0,0,0));
            break;
          case 1:
            //Serial.println("Tag 0");
            pixels.setPixelColor(j, pixels.Color(255,0,0));
            break;
          case 2:
            //Serial.println("Tag 1");
            pixels.setPixelColor(j, pixels.Color(0,255,0));
            break;
          case 3:
            //Serial.println("Tag 2");
            pixels.setPixelColor(j, pixels.Color(0,0,255));
          default:
            //Serial.println("Not a colour tag");
            break;
        }
      }
      switch(colourArrayGame[i]){
        case 1:
          colourMix[0]=colourMix[0]+85;
          break;
        case 2:
          colourMix[1]=colourMix[1]+85;
          break;
        case 3:
          colourMix[2]=colourMix[2]+85;
          break;
        default:
        break;
      }
    }
    pixels.show();
    Serial.print("Colour Mix: R = ");
    Serial.print(colourMix[0]);
    Serial.print(" G = ");
    Serial.print(colourMix[1]);
    Serial.print(" B = ");
    Serial.println(colourMix[2]);
  }  
}
void flash_cauldron(int win){
  if (win==1){
    for (int i=0; i<5; i++){
      for (int j=0;j<144; j++){
        pixels.setPixelColor(j, pixels.Color(255,255,255));
      }
      pixels.show();
      delay(500);
      for (int j=0;j<144; j++){
        pixels.setPixelColor(j, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(500);
    }
  }else{
    for (int i=0; i<5; i++){
      for (int j=0;j<144; j++){
        pixels.setPixelColor(j, pixels.Color(255,0,0));
      }
      pixels.show();
      delay(500);
      for (int j=0;j<144; j++){
        pixels.setPixelColor(j, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(500);
    }
  }
}

void wand(int wand_mode){
  Serial.println("Wand");
  if (wand_mode == 0){
    for (int p=0; p<NEO_COUNT; p++){
      pixels.setPixelColor(p, pixels.Color(colourMix[0],colourMix[1],colourMix[2]));
    }
    for (int i=0; i<10;i++){
      colourArray[i]=0;
    }
  pixels.show();
  colourPos=0;
  }else{
    for (int p=0; p<NEO_COUNT; p++){
      pixels.setPixelColor(p, pixels.Color(colourMix[0],colourMix[1],colourMix[2]));
    }
    pixels.show();
    int tempScore = 0;
    for (int i=0; i<3;i++){
      Serial.println(gameArray[i]);
      Serial.println(colourMix[i]);
      if (gameArray[i]==colourMix[i]){
        tempScore=tempScore+1;
        Serial.println("Match");
      }  
    }
    delay(2000);
    if (tempScore == 3){
      Serial.println("WIN!");
      flash_cauldron(1);
      gameStart = 0;
    }else{
      Serial.println("Try Again");
      flash_cauldron(0);
    }
    for (int i=0; i<3;i++){
      colourArrayGame[i]=0;
    }
  pixels.show();
  colourPos=0;
  }
}

void setup() {
  Serial.begin(115200);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");

  pixels.begin();
  pixels.setBrightness(neoBright);
  pixels.clear();
  jewel.begin();
  jewel.setBrightness(neoBright);
  jewel.clear();
  displayColours(0);

  pinMode(5,INPUT_PULLUP);

  for (int p=0; p<12; p++){
    jewel.setPixelColor(p, pixels.Color(0,0,0));
  }
  jewel.show();

  randomSeed(analogRead(0));

}

void normal_mode() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    //Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    delay(1000);
    Serial.println("Starting to look for tag");
    int match = 0;
      if (uidLength == 7) {
        while (match == 0){
          for (int x=0; x<6; x++){
            if (uid[0] == tagArray[x][0]){
              if (uid[1] == tagArray[x][1]){
                if (uid[2] == tagArray[x][2]){
                  if (uid[3] == tagArray[x][3]){
                    if (uid[4] == tagArray[x][4]){
                      if (uid[5] == tagArray[x][5]){
                        if (uid[6] == tagArray[x][6]){
                          match = 1;
                          colour = tagArray[x][7];
                          Serial.print ("Item scanned = ");
                          Serial.println(colour);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

    if (colour <4){
      colourArray[colourPos]= colour;
      Serial.print("Colour Pos: ");
      Serial.println(colourPos);
      if (colourPos == 0){
        colourMix[0]=0;
        colourMix[1]=0;
        colourMix[2]=0;
      }
      if (colourPos < 9){
        colourPos++;
      }else{
        colourPos = 0;
      }
    
      Serial.print("Colour Array: ");
      for (int i=0; i<10; i++){
        Serial.print(colourArray[i]);
      }
      Serial.println(".");

      displayColours(0);
    }else if (colour == 4){
      Serial.println("Brighter");
      if (neoBright<=90){
        neoBright=neoBright+10;
      }
      Serial.println(neoBright);
      pixels.setBrightness(neoBright);
      pixels.show();
    }else if (colour == 5){
      Serial.println("Darker");
      if (neoBright>=20){
        neoBright=neoBright-10;
      }
      Serial.println(neoBright);
      pixels.setBrightness(neoBright);
      pixels.show();
    }else if (colour == 6){
      wand(0);
    }
    delay(1000);
  }
}

void game_mode() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    //Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    delay(1000);
    Serial.println("Starting to look for tag");
    int match = 0;
      if (uidLength == 7) {
        while (match == 0){
          for (int x=0; x<6; x++){
            if (uid[0] == tagArray[x][0]){
              if (uid[1] == tagArray[x][1]){
                if (uid[2] == tagArray[x][2]){
                  if (uid[3] == tagArray[x][3]){
                    if (uid[4] == tagArray[x][4]){
                      if (uid[5] == tagArray[x][5]){
                        if (uid[6] == tagArray[x][6]){
                          match = 1;
                          colour = tagArray[x][7];
                          Serial.print ("Item scanned = ");
                          Serial.println(colour);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

    if (colour <4){
      colourArrayGame[colourPosGame]= colour;
      Serial.print("Colour Pos: ");
      Serial.println(colourPos);
      if (colourPosGame == 0){
        colourMix[0]=0;
        colourMix[1]=0;
        colourMix[2]=0;
      }
      if (colourPosGame < 2){
        colourPosGame++;
      }else{
        colourPosGame = 0;
      }
    
      Serial.print("Colour Array Game: ");
      for (int i=0; i<3; i++){
        Serial.print(colourArrayGame[i]);
      }
      Serial.println(".");

      displayColours(1);
    }else if (colour == 4){
      Serial.println("Brighter");
      if (neoBright<=90){
        neoBright=neoBright+10;
      }
      Serial.println(neoBright);
      pixels.setBrightness(neoBright);
      pixels.show();
    }else if (colour == 5){
      Serial.println("Darker");
      if (neoBright>=20){
        neoBright=neoBright-10;
      }
      Serial.println(neoBright);
      pixels.setBrightness(neoBright);
      pixels.show();
    }else if (colour == 6){
      wand(1);
    }
    delay(1000);
  }
}

void check_mode(){
  if (digitalRead(5)== LOW){
    mode = true;
    Serial.println("game mode");
  }else{
    mode = false;
    Serial.println("normal mode");
  }
}

void loop(){

  check_mode();
  
  if (mode == false){
    for (int p=0; p<12; p++){
      jewel.setPixelColor(p, pixels.Color(255,255,255));
    }
    jewel.show();
    normal_mode();
  }else if (mode == true){
    if (gameStart == 0){
      for (int i=0; i<3; i++){
        gameArray[i]=0;
      }
      for (int i=0; i<3; i++){
        int tempCol = random(0,3);
        Serial.println(tempCol);
          switch(tempCol){
            case 0:
              Serial.println("Random: Red");
              gameArray[0]= gameArray[0]+85;
              break;
            case 1:
              Serial.println("Random: Green");
              gameArray[1]= gameArray[1]+85;
              break;
            case 2:
              Serial.println("Random: Blue");
              gameArray[2]= gameArray[2]+85;
              break;
            default:
              break;
          }
   
        }
      for (int p=0; p<12; p++){
        jewel.setPixelColor(p, pixels.Color(gameArray[0],gameArray[1],gameArray[2]));
        
      }
      jewel.show();
      gameStart = 1;
    }
    game_mode();
  }
}
