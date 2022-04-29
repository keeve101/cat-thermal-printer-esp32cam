#include "esp_camera.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include "img_converters.h" // see https://github.com/espressif/esp32-camera/blob/master/conversions/include/img_converters.h
#include <Thermal_Printer.h>
#include <JPEGDEC.h>
#include "BluetoothSerial.h"
#include "Open_Sans_Bold_22.h"
#include "ightimma.h"
#include "pikachu.h"
#include "unilife.h"



/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int memecounter = 0;
uint8_t ucDither[576 * 16]; // buffer for the dithered pixel output
JPEGDEC jpg;
static int iWidth;



int JPEGDraw(JPEGDRAW *pDraw)
{
  int i, iCount;
  uint8_t *s = (uint8_t *)pDraw->pPixels;
  
  tpSetBackBuffer((uint8_t *)pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  // The output is inverted, so flip the bits
  iCount = (pDraw->iWidth * pDraw->iHeight)/8;
  for (i=0; i<iCount; i++)
     s[i] = ~s[i];
     
  tpPrintBuffer(); // Print this block of pixels
  return 1; // Continue decode
} /* JPEGDraw() */


// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// our call back to dump whatever we got in binary format
size_t jpgCallBack(void * arg, size_t index, const void* data, size_t len)
{
  uint8_t* basePtr = (uint8_t*) data;
  for (size_t i = 0; i < len; i++) {
    Serial.write(basePtr[i]);
  }
  return 0;
}

BluetoothSerial SerialBT;



void setup() {
  Serial.begin(115200);
  /* If no name is given, default 'ESP32' is applied */
  /* If you want to give your own name to ESP32 Bluetooth device, then */
  /* specify the name as an argument SerialBT.begin("myESP32Bluetooth"); */
  SerialBT.begin();
  Serial.println("Bluetooth Started! Ready to pair...");
    
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.fb_count = 2;   
    config.jpeg_quality = 10;
  }

  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  

  
}

void loop() {

  if (tpIsConnected() != 1) {
    if (tpScan()){
      tpConnect();
    }
  }
  else {
    
    if (SerialBT.available()){
      String str = SerialBT.readString();
      if (str == "TREE007"){
        camera_fb_t * fb = NULL;
        fb = esp_camera_fb_get();
        if (!fb) {
          Serial.println("Camera capture failed");
          return;
        }
          Serial.printf("\n\nWidth = %u, Height=%u\n", fb->width, fb->height);
            
        if (jpg.openRAM((uint8_t*)fb->buf, fb->len*4, JPEGDraw)) {
  
          jpg.setPixelType(ONE_BIT_DITHERED);
          jpg.decodeDither(ucDither, 0);
        } 
        tpFeed(80); // advance the paper 32 scan lines
        Serial.println("Finished printing!");
        esp_camera_fb_return(fb);
      }
      
      else if (str == "MEMESGEN202"){
        str = "";
        uint8_t *pImage;
        int iImageSize;
  
        //loop to cycle through images. use random to generate random memecounter or feel free to optimize this 
        if (memecounter % 5 == 0){
          pImage = (uint8_t *)unilife;
          iImageSize = (int)sizeof(unilife);
        }
        else if (memecounter % 5 == 1){
          pImage = (uint8_t *)pikachu;
          iImageSize = (int)sizeof(pikachu);
        }
        else if (memecounter % 5 == 2){
          pImage = (uint8_t *)ightimma;
          iImageSize = (int)sizeof(ightimma);
        }
        Serial.print(memecounter);
  
        
        Serial.print(iImageSize);
          if (jpg.openFLASH(pImage, iImageSize, JPEGDraw)) {
          jpg.setPixelType(ONE_BIT_DITHERED);
          jpg.decodeDither(ucDither, 0);
          
          } 
        memecounter ++;
  
       
      }
      
      String strs[42];
      String sentences[5];
      int StringCount = 0;
      int SentenceCount = 0;
      while (str.length() > 0){
     
        int index = str.indexOf(' ');
        if (index == -1) // No space found
        {
          strs[StringCount++] = str;
          break;
        }
        else
        {
          strs[StringCount++] = str.substring(0, index);
          str = str.substring(index+1);
        }
      }
      for (int i = 0; i < StringCount; i ++){
        String temp = strs[i] + " ";
        if ((sentences[SentenceCount] + temp).length() > 29){
          SentenceCount ++;
          sentences[SentenceCount] += temp;
        }
        else{
          sentences[SentenceCount] += temp;
        }
      }
      Serial.println((char *)"Connected!, printing graphics");
      Serial.println((char *)"Testing plain text printing");
    
      GFXfont *pFont;
      pFont = (GFXfont *)&Open_Sans_Bold_22;
      for (int i = 0; i <= SentenceCount; i++){
        tpPrintCustomText(pFont, 0, (char *) sentences[i].c_str());
      }
      tpFeed(80);
          
       
    }
  }
}
