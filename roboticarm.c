#include <18f46k22.h>
#device *=16
#device adc=10

#include <stdint.h>

#fuses INTRC_IO                  //use internal osc (yes !!) 
//#fuses PLLEN                     // Phase lock loop enable
#FUSES NOMCLR                    //Master Clear pin disabled,pin 1( RE3 available !) 
#FUSES NOWDT                     //No Watch Dog Timer 
#FUSES NOBROWNOUT                // brownout reset 
#FUSES NOPUT                     //No Power Up Timer 
#FUSES NODEBUG                   //No Debug mode for ICD 
#FUSES PROTECT                   //Code protected from reading 
#FUSES CPD                       //EE protection 
#FUSES WRT                       //Program memory write protected 
#FUSES NOWRTD                      //Data EEPROM write protected 
#FUSES NOWRTB                      //Boot block write protected 
#FUSES NOEBTR                      //Memory protected from table reads 
#FUSES NOEBTRB                     //Boot block protected from table reads 
#FUSES CPB                       //Boot Block code protection 
#FUSES WRTC                      //configuration not registers write protected 
#FUSES NOFCMEN                   //Fail-safe clock monitor enabled 
#FUSES NOSTVREN                  //Stack full/underflow will cause reset 
#FUSES NOIESO                    //Internal External Switch Over mode enabled 
#FUSES NOXINST                   //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#use delay(clock=64000000) 
 
#use fast_io(b) // Port yönlendirme komutlarý b portu için geçerli
#use fast_io(c) // Port yönlendirme komutlarý c portu için geçerli
#use fast_io(d) // Port yönlendirme komutlarý d portu için geçerli
#use fast_io(a) // Port yönlendirme komutlarý a portu için geçerli
// Taban Motor sürücü pini tanýmlamalarý
#define ENABLE_TABAN PIN_E1
#define DIR_TABAN PIN_A5
#define STEP_TABAN PIN_E0

// Nema Motor sürücü pini tanýmlamalarý
#define ENABLE_NEMA PIN_A6
#define DIR_NEMA PIN_E2
#define STEP_NEMA PIN_A7

// Dirsek Motor sürücü pini tanýmlamalarý
#define ENABLE_DIRSEK PIN_A3
#define DIR_DIRSEK PIN_A0
#define STEP_DIRSEK PIN_A2

// Bilek Motor sürücü pini tanýmlamalarý
#define ENABLE_BILEK PIN_B5
#define DIR_BILEK PIN_B2
#define STEP_BILEK PIN_B4

// El Motor sürücü pini tanýmlamalarý
#define ENABLE_EL PIN_B1
#define DIR_EL PIN_D7
#define STEP_EL PIN_B0


// Hareket komutlarý
#define LEFT 0
#define RIGHT 1


// Hareketleri kaydetmek için dizi
int rec_fl=0,i=0,bsk=0,ply_fl=0,clfl=0,home_fl=0,c7_fl=0,clfl_fl=0,reset_fl=0;
signed int32 x=0, y1=0, y2=0, y3=0, z=0, dif_x=0, a=0,b=0,dif_x_new=0,pulseX=0,pulseY1=0,dif_y1=0,dif_y1_new=0,pulseY2=0,dif_y2=0,dif_y2_new=0,pulseY3=0,dif_y3=0,dif_y3_new=0,pulseZ=0,dif_z=0,dif_z_new=0;
signed int32 hafiza[10][1][5]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
//sütun satýr içerik


void moveMotorTaban(int direction)
{
   if(direction==LEFT){
      output_high(DIR_TABAN);  // Yön ayarlamasý
      output_high(STEP_TABAN);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_TABAN); 
      x++;
      delay_us(300);
   }
   if(direction==RIGHT){
      output_low(DIR_TABAN);  // Yön ayarlamasý
      output_high(STEP_TABAN);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_TABAN); 
      x--;
      delay_us(300);
   }
}

void moveMotorNema(int direction)
{
   if(direction==LEFT){
      output_high(DIR_NEMA);  // Yön ayarlamasý
      output_high(STEP_NEMA);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_NEMA); 
      y1++;
      delay_us(300);
   }
   if(direction==RIGHT){
      output_low(DIR_NEMA);  // Yön ayarlamasý
      output_high(STEP_NEMA);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_NEMA); 
      y1--;
      delay_us(300);
   }
}

void moveMotorDirsek(int direction)
{
   if(direction==LEFT){
      output_high(DIR_DIRSEK);  // Yön ayarlamasý
      output_high(STEP_DIRSEK);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_DIRSEK); 
      y2++;
      delay_us(300);
   }
   if(direction==RIGHT){
      output_low(DIR_DIRSEK);  // Yön ayarlamasý
      output_high(STEP_DIRSEK);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_DIRSEK); 
      y2--;
      delay_us(300);
   }
}

void moveMotorBilek(int direction)
{
   if(direction==LEFT){
      output_high(DIR_BILEK);  // Yön ayarlamasý
      output_high(STEP_BILEK);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_BILEK); 
      y3++;
      delay_us(300);
   }
   if(direction==RIGHT){
      output_low(DIR_BILEK);  // Yön ayarlamasý
      output_high(STEP_BILEK);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_BILEK); 
      y3--;
      delay_us(300);
   }
}

void moveMotorEl(int direction)
{
   if(direction==LEFT){
      output_high(DIR_EL);  // Yön ayarlamasý
      output_high(STEP_EL);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_EL); 
      z++;
      delay_us(300);
   }
   if(direction==RIGHT){
      output_low(DIR_EL);  // Yön ayarlamasý
      output_high(STEP_EL);           // Adým sinyalini yüksek seviyeye ayarla
      delay_us(300);                    // Adým sinyalini 10ms boyunca yüksek seviyede tut
      output_low(STEP_EL); 
      z--;
      delay_us(300);
   }
}


void resetMoves()
{
      pulseX=0-x;
      pulseY1=0-y1;
      pulseY2=0-y2;
      pulseY3=0-y3;
      pulseZ=0-z;
      
      if(pulseX>0){
         moveMotorTaban(LEFT);
      }
      if(pulseX<0){
         moveMotorTaban(RIGHT);
      }
      if(pulseX==0){
         x=0;
      }
      if(x==0){
         if(pulseY1>0){
            moveMotorNema(LEFT);
         }
         if(pulseY1<0){
            moveMotorNema(RIGHT);
         }
         if(pulseY1==0){
            y1=0;
         }
      }
      if(x==0 && y1==0){
         if(pulseY2>0){
            moveMotorDirsek(LEFT);
         }
         if(pulseY2<0){
            moveMotorDirsek(RIGHT);
         }
         if(pulseY2==0){
            y2=0;
         }
      }
      
      if(x==0 && y1==0 && y2==0){
         if(pulseY3>0){
            moveMotorBilek(LEFT);
         }
         if(pulseY3<0){
            moveMotorBilek(RIGHT);
         }
         if(pulseY3==0){
            y3=0;
         }
      }
      if(x==0 && y1==0 && y2==0 && y3==0){
         if(pulseZ>0){
            moveMotorEl(LEFT);
         }
         if(pulseZ<0){
            moveMotorEl(RIGHT);
         }
         if(pulseZ==0){
            home_fl=0;
            z=0;
            reset_fl=1;
         }
      }
}

//!// Kaydedilen hareketleri tekrar etme fonksiyonu
void playMoves(){
   if(bsk==0){
      if(clfl==0 && clfl_fl==0){
         dif_x = 0-hafiza[0][0][0];
         dif_x_new = dif_x;
         
         dif_y1 = 0-hafiza[0][0][1];
         dif_y1_new = dif_y1;
         
         dif_y2 = 0-hafiza[0][0][2];
         dif_y2_new = dif_y2;
         
         dif_y3 = 0-hafiza[0][0][3];
         dif_y3_new = dif_y3;
         
         dif_z = 0-hafiza[0][0][4];
         dif_z_new = dif_z;
         clfl=1;
         
      }
      
      if(dif_x_new < 0){
         moveMotorTaban(LEFT);
         dif_x_new++;
      }
      if(dif_x_new > 0){
         moveMotorTaban(RIGHT);
         dif_x_new--;
      }

      if(dif_x_new==0){
         if(dif_y1_new < 0){
            moveMotorNema(LEFT);
            dif_y1_new++;
         }
         if(dif_y1_new > 0){
            moveMotorNema(RIGHT);
            dif_y1_new--;
         }
//!         if(dif_y1_new == 0){
//!            bsk++;
//!            clfl_fl = 1;
//!         }
      }
      
      if(dif_x_new==0 && dif_y1_new==0){
         if(dif_y2_new < 0){
            moveMotorDirsek(LEFT);
            dif_y2_new++;
         }
         if(dif_y2_new > 0){
            moveMotorDirsek(RIGHT);
            dif_y2_new--;
         }
//!         if(dif_y2_new == 0){
//!            bsk++;
//!            clfl_fl = 1;
//!         }
      }
      if(dif_x_new==0 && dif_y1_new==0 && dif_y2_new==0){
         if(dif_y3_new < 0){
            moveMotorBilek(LEFT);
            dif_y3_new++;
         }
         if(dif_y3_new > 0){
            moveMotorBilek(RIGHT);
            dif_y3_new--;
         }
         if(dif_y3_new == 0){
            clfl_fl = 1;
         }
      }
      if(dif_x_new==0 && dif_y1_new==0 && dif_y2_new==0 && dif_y3_new==0){
         if(dif_z_new < 0){
            moveMotorEl(LEFT);
            dif_z_new++;
         }
         if(dif_z_new > 0){
            moveMotorEl(RIGHT);
            dif_z_new--;
         }
         if(dif_z_new == 0){
            bsk++;
            clfl_fl = 1;
         }
      }
   }
//!   
   if(bsk==1){
      if(clfl==1){
         dif_x = hafiza[0][0][0]-hafiza[1][0][0];
         dif_x_new = dif_x;
         
         dif_y1 = hafiza[0][0][1]-hafiza[1][0][1];
         dif_y1_new = dif_y1;
         
         dif_y2 = hafiza[0][0][2]-hafiza[1][0][2];
         dif_y2_new = dif_y2;
         
         dif_y3 = hafiza[0][0][3]-hafiza[1][0][3];
         dif_y3_new = dif_y3;
         
         dif_z = hafiza[0][0][4]-hafiza[1][0][4];
         dif_z_new = dif_z;
         clfl=2;
      }
      
      if(dif_x_new < 0){
         moveMotorTaban(LEFT);
         dif_x_new++;
      }
      if(dif_x_new > 0){
         moveMotorTaban(RIGHT);
         dif_x_new--;
      }
      if(dif_x_new == 0){
      
         if(dif_y1_new < 0){
            moveMotorNema(LEFT);
            dif_y1_new++;
         }
         if(dif_y1_new > 0){
            moveMotorNema(RIGHT);
            dif_y1_new--;
         }
      
      
      if(dif_x_new == 0 && dif_y1_new == 0){
      
         if(dif_y2_new < 0){
            moveMotorDirsek(LEFT);
            dif_y2_new++;
         }
         if(dif_y2_new > 0){
            moveMotorDirsek(RIGHT);
            dif_y2_new--;
         }
      
      if(dif_x_new == 0 && dif_y1_new == 0 && dif_y2_new == 0){
      
         if(dif_y3_new < 0){
            moveMotorBilek(LEFT);
            dif_y3_new++;
         }
         if(dif_y3_new > 0){
            moveMotorBilek(RIGHT);
            dif_y3_new--;
         }
//!         if(dif_y3_new == 0){
//!            bsk=0;
//!            clfl_fl = 0;
//!            clfl = 0;
//!            ply_fl=0;
//!            reset_fl=0;
//!         }
      if(dif_x_new == 0 && dif_y1_new == 0 && dif_y2_new == 0 && dif_y3_new == 0){
      
         if(dif_z_new < 0){
            moveMotorEl(LEFT);
            dif_z_new++;
         }
         if(dif_z_new > 0){
            moveMotorEl(RIGHT);
            dif_z_new--;
         }
         if(dif_z_new == 0){
            bsk=0;
            clfl_fl = 0;
            clfl = 0;
            ply_fl=0;
            reset_fl=0;
         }
      }
      }
      }
      }
      
   }
}


void main()
{
      set_tris_a(0x00);
      set_tris_b(0x00);
      set_tris_c(0b11111100);
      set_tris_d(0b01111111);
      // Motor sürücü pinlerini çýkýþ olarak ayarla
      output_low(ENABLE_TABAN);
      output_low(DIR_TABAN);
      output_low(STEP_TABAN);
      
      // Motor sürücü pinlerini çýkýþ olarak ayarla
      output_low(ENABLE_NEMA);
      output_low(DIR_NEMA);
      output_low(STEP_NEMA);
      
      // Motor sürücü pinlerini çýkýþ olarak ayarla
      output_low(ENABLE_DIRSEK);
      output_low(DIR_DIRSEK);
      output_low(STEP_DIRSEK);
      
      // Motor sürücü pinlerini çýkýþ olarak ayarla
      output_low(ENABLE_BILEK);
      output_low(DIR_BILEK);
      output_low(STEP_BILEK);
      
      // Motor sürücü pinlerini çýkýþ olarak ayarla
      output_low(ENABLE_EL);
      output_low(DIR_EL);
      output_low(STEP_EL);

 
while(TRUE) // sonsuz döngü
{
      output_high(STEP_NEMA);
      
      //ADC Portlarýn ayarlanmasý.
      setup_adc_ports(sAN20|sAN15|sAN23|sAN22|sAN14);
      setup_adc(ADC_CLOCK_INTERNAL);
      
      
      set_adc_channel(20);
      delay_us(10); // Kanal geçiþi için bir bekleme süresi
      int16_t joystick_taban = read_adc();
      delay_us(10);
      
      set_adc_channel(15); // AN15 pinine geç
      delay_us(10); // Kanal geçiþi için bir bekleme süresi
      int16_t joystick_nema = read_adc(); // AN1 pininin analog deðerini oku
      delay_us(10);
      
      set_adc_channel(23); // AN15 pinine geç
      delay_us(10); // Kanal geçiþi için bir bekleme süresi
      int16_t joystick_dirsek = read_adc(); // AN1 pininin analog deðerini oku
      delay_us(10);
      
      set_adc_channel(22); // AN15 pinine geç
      delay_us(10); // Kanal geçiþi için bir bekleme süresi
      int16_t joystick_bilek = read_adc(); // AN1 pininin analog deðerini oku
      delay_us(10);
      
      set_adc_channel(14); // AN15 pinine geç
      delay_us(10); // Kanal geçiþi için bir bekleme süresi
      int16_t joystick_el = read_adc(); // AN1 pininin analog deðerini oku
      
      // TABAN HAREKET
      if(joystick_taban > 700)
      {
         moveMotorTaban(LEFT);
         
      }
      // TABAN HAREKET
      else if(joystick_taban < 250)
      {
          moveMotorTaban(RIGHT);
         
      }
      
      // NEMA HAREKET
      if(joystick_nema > 700)
      {
         moveMotorNema(LEFT);
         
      }
      // NEMA HAREKET
      else if(joystick_nema < 250)
      {
          moveMotorNema(RIGHT);
         
      }
      
      // DÝRSEK
      if(joystick_dirsek > 700)
      {
         moveMotorDirsek(LEFT);
         
      }
      // DÝRSEK
      else if(joystick_dirsek < 250)
      {
          moveMotorDirsek(RIGHT);
         
      }
      // BÝLEK
      if(joystick_bilek > 700)
      {
         moveMotorBilek(LEFT);
         
      }
      // BÝLEK
      else if(joystick_bilek < 250)
      {
          moveMotorBilek(RIGHT);
         
      }
      
      // EL
      if(joystick_el > 700)
      {
         moveMotorEl(LEFT);
         
      }
      // EL
      else if(joystick_el < 250)
      {
          moveMotorEl(RIGHT);
         
      }
      
      
      // C7 pinine baðlý düðme kontrolü
      if(input(PIN_C7) && rec_fl==0)
      {
         rec_fl=1;
         delay_ms(500);
         
      }
      if(input(PIN_C7)){
         c7_fl = 1;
         delay_ms(200);
      }
      
      if(c7_fl==1 && rec_fl==1)
      {
         resetMoves();
         if(x==0 && y1==0 && y2==0 && y3==0 && z==0){
            c7_fl=0;
            rec_fl=0;
            i=0;
         }
         
      }
      if(rec_fl==1){
         if(input(PIN_D4) && i==0){
            hafiza[i][0][0]=x;
            hafiza[i][0][1]=y1;
            hafiza[i][0][2]=y2;
            hafiza[i][0][3]=y3;
            hafiza[i][0][4]=z;
            i=1;
            delay_ms(200);
         }
         
         if(input(PIN_D4) && i==1){
            hafiza[i][0][0]=x;
            hafiza[i][0][1]=y1;
            hafiza[i][0][2]=y2;
            hafiza[i][0][3]=y3;
            hafiza[i][0][4]=z;
            i=2;
            delay_ms(200);
         }
      }
      
   if(input(PIN_D5) && ply_fl==0)
   {
      ply_fl=1;
      delay_ms(200);
   }
   if(input(PIN_D5) && ply_fl==1)
   {
      clfl_fl = 0;
      clfl = 0;
      ply_fl=0;
      reset_fl=0;
      delay_ms(200);
   }
   if(ply_fl==1)
   {
      if(reset_fl==0){
         resetMoves();
      }
      if(reset_fl==1){
         playMoves();
      }
   }
//......... HOME BUTON
   if(input(PIN_D6) && home_fl==0)
   {
      home_fl=1;
      delay_ms(200);
   }
   if(input(PIN_D6) && home_fl==1)
   {
      home_fl=0;
      delay_ms(200);
   }
   if(home_fl==1)
   {
      resetMoves();
   }
   
//......... HOME BUTON
}
}
