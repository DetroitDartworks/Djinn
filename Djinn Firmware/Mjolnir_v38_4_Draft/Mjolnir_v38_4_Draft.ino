#include <EEPROM.h>
#include <SSD1306AsciiWire.h>
#include <Wire.h>

#include "MenuHelpers.h"
#include "Logo.h"
#include "History.h"

#define VERSION 38
/* 10-12-23
  For the Nerf Comunity, open progect... RICHARD NICHOLSON from NEW ZEALAND

  Brushless | Solenoid Pusher | Singal Trigger | Mag Detetor Sw | 3 Pos Select Fire Sw | I2C OLED 128 x 64 display | Rotary Encoder with PB_Switch | Serial DeBUG 115200 Buad

  -------------------------------------------------------------------
    *** Mjolnir V1 Hardware Pin out. This is the Default setting
  -------------------------------------------------------------------
  *Nano PCB layout, USB pointing Up
 PIN |  Real would connection |  In/Out    | Register

  
  RHS
  D12     - Encoder SW             => Input        PB4           //  - Could use a PB = Single, Burst,Full Auto, Single, Burst,Full Auto...
  D11     - Mag Sensor             => Input        PB3   OC2A    //  - NO to D11 and COM to GND
  D10     - ESC stage 1 signal     => Output       PB2   OC1B    //  - 2 x ESC go on this signal 1st stage
  D9      - ESC stage 2 signal     => Output       PB1   OC1A    //  - 2nd Stage
  D8      - Selectfire Sw In1      => Input        PB0   *NEW    //  - Slide switch pin for Select fire
  D7      -                        => Input        PD7           //  -
  D6      -                        => Output       PD6           //  -  - OC0A
  D5      - Solenoide Pusher       => Output       PD5           //  - Mosfet Trigger
  D4      -                        => Output       PD4           //  -
  D3      - Input - Encoder DT     => Input        PD3   /Int1   //
  D2      - Input - Encoder CLK    => Input        PD2   /Int0   //

  LHS
  D13     - Tracer Darts Led       => Output       PB5  *NEW     //  - Use PCB led as indicator for testing.   *** Compile Time option
  A0/D14  - Rev Sw                 => Input        PC0  *NEW     //  - NO to A0/D14 and COM to GND             *** Compile Time option
  A1/D15  - Solenoid Stop          => Input        PC1           //  - Narfduino: Output - Solenoid Stop, left for compatablity with Narfduano PCB   *** Compile Time option
  A2/D16  - Trigger                => Input        PC2           //  - NO to A2/D16 and COM to GND.
  A3/D17  - SelectFire Sw In2      => Input        PC3  *NEW     //  - Slide switch pin for Select fire
  A4/D18  - OLED display  SDA      => I2C          PC4           //  -
  A5/D19  - OLED display  SCL      => I2C          PC5           //  -
  A6      - Analog Input Not used  => Input        ADC6          //  - ADC input only, no digital code, programing functions on this pin
  A7      - Analog Battery Reading => Input        ADC7          //  - Resistor devideder 47K/10K

  -------------------------------------------------------------------
            Mjolnir V2 Hardware Pin out. V2 pinout can be selected via compile time switch
  -------------------------------------------------------------------
  Nano PCB layout, USB pointing Up
  RHS PIN |  Real would connection |  In/Out    | Register

  D12     - Encoder SW             => Input        PB4           //  - Could use a PB Single, Burst,Full Auto, Single, Burst,Full Auto...
  D11     - Mag Sensor             => Input        PB3   OC2A    //  - NO to D11 and COM to GND
  D10     - ESC stage 1 signal     => Output       PB2   OC1B    //  - 2 x ESC go on this signal
  D9      - ESC stage 2 signal     => Output       PB1   OC1A    //  -
  D8      -                        => Output       PB0           //  -
  D7      - Encoder DT             => Input        PD7           //  -
  D6      -                        => Output       PD6   OC0A    //  -
  D5      - Solenoide Pusher       => Output       PD5           //  - Mosfet Trigger
  D4      -                        => Output       PD4           //  -
  D3      - SelectFire Sw In2      => Input        PD3   /Int1   //  *** Compile Time option
  D2      - Selectfire Sw In1      => Input        PD2   /Int0   //  *** Compile Time option

  LHS
  D13     - Tracer Darts Led       => Output       PB5           //  - Use PCB led as indicator for testing.   *** Compile Time option
  A0/D14  - Rev Sw                 => Input        PC0           //  - NO to A0/D14 and COM to GND             *** Compile Time option
  A1/D15  - Solenoid Stop          => Input        PC1           //  - Narfduino: Output - Solenoid Stop, left for compatablity with Narfduano PCB   *** Compile Time option
  A2/D16  - Trigger                => Input        PC2           //  - NO to A2/D16 and COM to GND.
  A3/D17  - Encoder CLK            => Input        PC3           //  -
  A4/D18  - OLED display  SDA      => I2C          PC4           //  -
  A5/D19  - OLED display  SCL      => I2C          PC5           //  -
  A6      - Analog Input Not used  => Input        ADC6          //  - ADC input only, no digital code, programing functions on this pin
  A7      - Analog Battery Reading => Input        ADC7          //  - Resistor devideder 47K/10K


  ---------------------------------------------------------------------------------------------------------------------------------
   Hardware
     - SSD1306 128 x 64 OLED Display NOTE: OLED_ADDR may need to be set correctly
     - Rotary Encoder with inbuilt PB Switch. DT to outside pin, CLK to oustide, GND to middel pin
     - Battery Resistor Devider on A7 47k/10K, with a bat, else code will say low bat
  ---------------------------------------------------------------------------------------------------------------------------------
READ.ME            **** V34 code and hardware changes ****
    
  This code suports a number of new features
    Optional Sliding Select Fire Switch
    Optional Rev switch, alwoing pre spin up to Ideal speed as set in the menu
    Optional Tracer Dart Output
    Simple Firmware configuration via editing compile time switchs, and default menu values, that are used in the Oled menu
    Default Menu values and compile time switchs located at start of file for easy editting
    
    Simpl Blaster configuration. Trigger and select fire switch only, set default values before firmware download
    to
    All Features:Oled, Encoder, Select Fire, Mag Sw, Rev Sw

    Idel Mode for flywheels, can set the ideal value in Menu. Turn on and off via Long trigger press (2 seconds) or use Rev Sw
    
  Hardware:
  Both V1 and V2 Mjolnir PCB wiring is suported via compile time code switchs. The Default is V1 hardware (V2 underdevelopment).
  
  New Hardware features, use unused pins from V1 plateform. You will need to solder wires to the Nano Pins on underside of Mjolnir V1 PCB
    - D8 Select Fire slide Sw input 1
    - A3 Select Fire Slide Sw Input 2
    - A0 Rev Sw Input
    - D13 Tracer dart output
    
  Optional Tracer Darts output via D13 (on Board Led). Follows Rev. Requires a driver suggest use mosfet
  
  Optional 3 position, Select Fire switch - Via compile time code switch. Encoder Select fire PB does not work with this enabled
  
  Optional Rev Switch, Via compile time code switch.  Ideal speed value set in config menu
  
  Mag Switch On/Off know done via compile time code switch (saves on memory)
  
  Logo can be turned off to save 3% of memory for code space Via compile time code switch
  Dipaly Fip can be done Via compile time code switch

  you can find the compile time switches just below, after the comments
  --------------------------------------------------------------------------------------------------
  
  INSTRUCTIONS

  Copy the Four files:

     Mjolnir.ino
     History.h
     Logo.h
     MenuHelpers.h

  Into one folder, and initiate the Adrouno IDE by clicking Mjolnir_vnn.ino
  Under Tools set the Board Type:Arduino Nano, Processor:ATmega328P (old Bootloader), and Com port

  Verify program, top left.
  It may error, requiring libarie files. These can be found under Tools/Manager Libaries..

  Once it Verifies OK...
  Upload to Arduino ( the next button)
  If updoad fails try changinging the Processor under Tools/Processos, to ATmega328P

  Set Serial Monitor, to 11520 baud, for debug output. Not required. Will Display version Number

  First time upload will load default values into EEprom and configuration screen
  -----------------------------------------------------------------------------------------

  ROTARY ENCODER
  Use Rotary Encoder Push Button (PB), Short click to change Select Fire Mode. Mode is on default display (Single / Burst: 2 / Full Auto)
  Use a Long Click to get into the Configuration screen, short click to get out when on the EXIT tag. Menu goes around in circle.

  OLED DISPLAY - Default home Screen
  Bat volts: 11.4v
  00:00:00 Game timer HH:MM:SS, set in minutes via Config Menu
  Select fire mode: Single, Burst: 2, Full Auto (Use short clicks to change Select Fire Mode
  ROF: MAX, Rate Of Fire, default is max Pusher speed, ie changeing value, via config screen will slow it down
  Pwr: 50%, Flywheels output, default is on 50%, you can change the power in the config menu, it will go faster or slower
  Ammo Counter: reset via mag switch. no trap on zero count.
  Use short clicks to change Select Fire Mode (Single / Burst: 2 / Full Auto)
  Long Click to get into the Configuration screen, short click to get out when on the EXIT tag, top or bottom of menu

  OLED DISPLAY - Configuration Screen. Long Click to enter.
  To change values, Click to get # rotate to change value, click to accept. Values are saved to EEPROM instantly

  EXIT           // Exit config back to default run screen
  GameTm:        // Count Down, Game Time Set in Minutes. Starts when exit config it not zero
  Power1:        // Flywheel Stage1 Power 30 - 100% Default: %50
  Power2:        // Flywheel Stage2 Power 30 - 100% Default %50 NOTE: can not be set below Stage 1 power, will auto correct
  A ROF          // Full Auto, Rate Of Fire 0(Max) - 150 Default: Max
  B ROF          // Burst, Rate Of Fire 0(Max) - 150 Default: Max
  Burst          // Burst Rate (number of darts to fire) 2-99 Default: 2
  MagS           // Mag Size capacity 0-99 Default: 18
  Ramp U         // Fly Wheel Start Ramp Up time 0 - 5000 Default: 0 (Fastest)
  Ramp D         // Fly Wheel Stop Ramp Down time 0 - 5000 Default: 4000
  Dwel U         // Ramp up Dwell time 0 - 5000 Default: 0 Use this to hold the rev
  Dwel D         // Ramp down Dwell time 0 - 5000 Default: 200 Use this to hold the rev
  SP Hi          // Solenoid Pulse High Time 0 - 1000 Default: 35
  SP Low         // Solenoid Pulse Low Time 0 - 1000 Default: 45
  SP Ret         // Solenoid Pulse Retract Time 0- 1000 Default: 45
  Bat Type       // 3S or 4S default 3S
  IdleSpd        // Battery Offset for calibration, increments of 0.1 displayed as interger
  ROF_Tm         // Mag switch detect enable on/off
  EXIT           // Exit config back to default run screen

  To load factory default settings, Hold trigger ON during power up or reset, let go when screen says too.

  ---------------------------------------------------------
  SOLENOID TUNING : to get best ROF you need to do this
  ---------------------------------------------------------
      In order to get a good solenoid calibration/tune, you need to have 3 things:
      - Push-out Time when LiPO is fully charged, ~12.5v or similar on 3S
      - Push-out Time when LiPO is depleted, storage voltage 11.5v or similar on 3S
      - and Retract Time.

     It is advisable to disconnect the ESC signal wire so that you don't have the motors draining power, and affecting your tune.

     We need to change these values in the Config Screen
       SP Hi, Solenoid PulseOnTimeHigh, default = 35;
       SP Low, Solenoid PulseOnTimeLow, default = 45;
       SP Ret, Solenoid PulseRetractTime = 45;

     Focus on the return rate first
     Via config menu:-
       Change A ROF to Max, it is by default
       Change SP Hi, Solenoid Pulse On Time High = 150
       Change SP Low, Solenoid Pulse On Time Low = 150
       Change SP Ret, Solenoid Pulse Retract Time = 100
     Exit Config

     Set select fire to Full Auto
     Start with SP Ret, at 100ms,to see if the solenoid is fully retracting between shots. Else it will be short strocking
     You might be able to use your finger to feel it. Increase or reduce this number until you have a consistent and reliable retraction.
     n.b. solenoids will retract slower with a fully loaded magazine, so once you have found the number, add a bit extra to it (like 10ms)
     To attchive the above may require multi edits

     Then focus on the fully charged pulse time. Fully charge your LiPO. Start reducing both SP Hi and SP Low
     (keep the numbers the same) until it's quick, but not so quick as it's short-stroking.
     Use your finger up the magwell to determine how hard the solenoid is hitting it. Keep reducing the time until it starts to feel weaker,
     then back off.
     Add a bit extra to it for safety margin. Write down this value.

     For the low battery discharge time, bring the LiPO to storage charge. Then do the same thing as above.

     Once you have both the SP Hi and SP Low values, key them in and test the solenoid over a range of battery voltages.
     Reducing the timing will make it faster, at the expense of reliability.

     Neutron: Thous 3S: SP HI = 47, SP Low = 47, SP Ret = 27
     Neutron: Rich 3S 25C: SP HI = 38, SP Low = 42, SP Ret = 27
     Neutron: Rich 3S 25C RD With 28v boost : SP HI = 28, SP Low = 32, SP Ret = 14

     Big 35mm: Rich values 4S RD: SP HI = 23, Sp low = 30 SP Ret = 23

  -------------------------------------------
  Suggested ESC setting, the ones to change
  -------------------------------------------
  Ramp Up Pwr: 150%
  Low Voltage Protection: Off    //leave Off, motoro draw down at startup can trigger this= unreliability
  Motor Timing: 20 Deg
  Min Trottel: 1040
  Max Trottel: 1960              // motoro dependant knormaly go for 80% of max capability
  Brake On Stop: Off             // This uses bat pwr and makes motors hot, can melt 3d prints if on
  Led control Off x 3
  Becon Delay: Off               // if not set to, off, ESC wil make a bleeping noise after no activity (downed dron). We might be hunting targets turn OFF
  PWM FRQ: 48Khz

  -----------------------------
  Logo Splach Screen - How To
  -----------------------------
  Generate 128x64 Pixel BMP or PNG file (MS Paint works), edit to suit (White background, black details)
  open browser on this https://javl.github.io/image2cpp/ (active link) use these settings:
  Browse and select your file
  Background color: Black
  Invert Image colors: off
  Brightness 128
  Out Put format: Arduino, Singal Bitmap
  Draw mode: Vertical-1 bit per pixel
  generate code & copy output
  goto Logo.h in Arduino IDE
  find bottom and past Ctrl-V
  edit into the abvoe block , ie delete the hex and past in the new hex

  ----------------------------------------------------------------------------------------------
*/

// =========================// USER SET ABLE //==================================================================
// COMPILE TIME SWITCHES & FIRMWARE DEFAULTS
// The end user can edit the following default values to tweak performance, before download
// Comment out using "//" if not required (Saves Moemory not having the option Enabled )
// ==============================================================================================================7

#define EN_ORG_HW                                 // Allow for V1 HW and Rotary Encoder on D2/D3. 
//#define EN_SEL_FIRE_SW                            // Enable Select Fire, 3 position swith on D8/A3/GND
//#define EN_MAG_SW                                 // Enable Mag Detector Switch
//#define EN_REV_SW                                 // Enable Rev Swicth, Idel Mode On/Off. Else Long Trigger =  Idel Mode On/Off
#define EN_LOGO                                   // Enable Splash Screeb Logo saves 3% program storage if not used
//#define DISPLAY_FLIP                              // Flips display upside down

// Config Menu, Default Profile values
#define GAMETIMESTART 0                           // Game Mode Run Time in minutes (60 = 1 hour)
#define MOTORSPEEDFULL 50                         // Power: Stage 1 flywheels   50%
#define MOTORSPEED2FULL 50                        // Power2: Stgae 2 flywheels  50%
#define ROFA 0                                    // ROF Full Auto              0 = Max
#define ROFB 0                                    // ROF Burst Max              0 = Max
#define BURSTSIZE 3                               // Burst shoot nummber        2
#define MAGSIZE 15                                // Mag 18
#define RAMP_UP 0                                 // Ramp Up 0 = fastest start up
#define RAMP_DOWN 1000                            // Ramp Down
#define DWELL_UP 100                              // Dwell Up keeps it reving
#define DWELL_DOWN 200                            // Dwell Down
#define SP_HI 35                                  // SP High (Solenoid Pulse)
#define SP_LOW 45                                 // SP Low
#define SP_RET 65                                 // SP Ret
#define BAT_3S                                    // **** Change to BAT_4S or BAT_3S
#define IDLE_SPEED 15                             // 0-100 If greater than 0, motors will idle at xxx
#define ROF_TM 0                                  // ROF Ramp time in full auto 0 = full rof
#define MagSwOFF 0
#define MagSwON 1
#define MagSwEnableON


#define BATTERY_3S_MIN 10.8                       // 3S_Min volts = absolute min 10.8, Nominal 11.1
#define BATTERY_3S_MAX 12.6                       // 3S_Max volts = 12.6 
#define BATTERY_4S_MIN 12.8                       // 4S_Min volts = 12.8 
#define BATTERY_4S_MAX 16.8                       // 4S_Max volts = 16.8

#define BATTERY_CALFACTOR 0.0                     // Battery Offset Adjustment for calibration

#define MOTOR_SPINUP_LAG 80                      // How long we give the motors before we know thay have spun up.

#define OLED_ADDR 0x3C                            // <----- OLED ADDRESS this may need changing, google the address

#define BAUDRATE 115200                           // Serial debug baud rate. Keep high for best program performance


//================================================================================================

//-------------------------------------------------------------------------------------------------------------------------
// BIOS Deffinitions
//-------------------------------------------------------------------------------------------------------------------------

#define PIN_PUSHER_RUN    5       //      Pusher              OP  D5      PD5 
#define PIN_PUSHER_STOP   15      //      Pusher              OP  D15     PC1     Not actualy used left for compatablity with Narfduano
#define PIN_ESC           10      //      ESC 1               OP  D10     PB2     OC1B (PWM)
#define PIN_ESC_2         9       //      ESC 2               OP  D9      PB1     OC1A (PWM)
#define PIN_TRACER        13      //      Trace Dart Led      OP  D13     PB5

#define PIN_INT0          2       //      Interupt 0          IP  D2      /INT0     Select Fire In1      *** Compile time option
#define PIN_INT1          3       //      Interupt 1          IP  D3      /INT1     Select Fire In2      *** Compile time option

#define PIN_REV_SW        14      //     Push button          IP  A0/D14  PC0     PCINT8  *** Compile time option

#ifdef EN_ORG_HW                  //      V1 wiring
  #define PIN_ENCODER_CLK   2     //      Encoder CLK,        IP D2      /INT0
  #define PIN_ENCODER_DT    3     //      Encoder DT          IP D3      /INT1
  #define PIN_SEL_FIRE_1    8     //      Select fire Sw      IP D8       PB0
  #define PIN_SEL_FIRE_2    A3    //      Select Fire Sw      IP A3       PC3

#else                             //      V2 wiring
  #define PIN_ENCODER_CLK   A3    //      Encoder CLK,        IP  A3/D17  PC3
  #define PIN_ENCODER_DT    7     //      Encoder DT          IP  D7      PD7
#endif

#define PIN_ENCODER_SW    12      //      Encoder SW          IP  D12     PB4     PCINT4 

#define PIN_MAG_S         11      //      Mag Sensor          IP  D11     PB3     PCINT3   Use NO and COM on Switch
#define PIN_TRIGGER       16      //      Trigger             IP  D16/A2  PC2     PCINT10  Use NO and COM on switch. 

#define PIN_SDA           A4      //      OLED display        IP  A4/D18  I2C interface  
#define PIN_SCL           A5      //      OLED display        IP  A5/D19  I2C Interface

#define PIN_BATT_MON      A7      //      Bat Volts           IP  A7      ADC7 Analog battery reading, Resistor devideder 47K/10K 


// ==============================================================================================================================
// Pin macros

// Read pin state (Inputs)
#define GET_FIRE_T ((PINC & 0b00000100) ? HIGH : LOW )       // Trigger          PC2   PCINT10   
#define GET_MAG_S ((PINB & 0b00001000) ? HIGH : LOW )        // Mag Switch       PB3   PCINT3   

#define GET_ENCODER_SW ((PINB & 0b00010000) ? HIGH : LOW )   // Encoder Switch   PB4   PCINT4

#ifdef EN_ORG_HW                                              //V1 wiring
  #define GET_ENCODER_CLK ((PIND & 0b00001000) ? HIGH : LOW ) // PD3 This is encoder Clk
  #define GET_ENCODER_DT ((PIND & 0b00000100) ? HIGH : LOW )  // PD2 This is encoder DT
  #define GET_SEL_FIRE_1 ((PINB & 0b00000001) ? HIGH : LOW )  // PB0
  #define GET_SEL_FIRE_2 ((PINC & 0b00001000) ? HIGH : LOW )  // PC3
#else                                                         //V2 Wiring
  #define GET_ENCODER_CLK ((PINC & 0b00001000) ? HIGH : LOW ) // Encoder Clk      PC3  
  #define GET_ENCODER_DT ((PIND & 0b10000000) ? HIGH : LOW )  // Encoder DT       PD7  
#endif

#define GET_INT1 ((PIND & 0b00001000) ? HIGH : LOW )        // SelectFire2      PD3   /Int1
#define GET_INT0 ((PIND & 0b00000100) ? HIGH : LOW )        // SelectFire1      PD2   /Int0

#define GET_REV_S ((PINC & 0b00000001) ? HIGH : LOW )       // Rev Switch       PC0   PCINT8

// Set Pin State (Outputs)
#define SET_P_RUN_FULL_ON (PORTD |= 0b00100000)             // Pusher Pulse ON  PD5
#define SET_P_RUN_FULL_OFF (PORTD &= 0b11011111)            // Pusher Pulse OFF PD5
#define SET_P_BRAKE_FULL_OFF (PORTC &= 0b11111101)          // For Nafduino compatability PC1
#define SET_TRACER_D_ON  (PORTB |= 0b00100000)               // Tracer Dart  ON  PB5
#define SET_TRACER_D_OFF (PORTB |= 0b11011111)              // Tracer Dart  OFF PB5

//----------------------------------------------------------------------------------------------------------

/*
#ifdef EN_MAG_SW
volatile bool MagSwEnable = true ;                        // <----- set true or false to Enable or disable Mag detector switch
#else
volatile bool MagSwEnable = false ;
#endif
*/

//----------------------------------------------------------------------------------------------------------

#ifdef DISPLAY_FLIP
volatile bool DisplayFlip = true ;                         // <----- set true or false to flip display
#else
volatile bool DisplayFlip = false ;                        // <----- set true or false to flip display
#endif

// OLED Display
SSD1306AsciiWire Oled;

//-----------------------------------------------------------------------------------------------------------

// Motor Controls
#define MOTOR_MAX_SPEED 2000                                //
int IdleMotorSpeed = 1200;                                   // Set to desired idle speed 
int MaxMotorSpeed = MOTOR_MAX_SPEED;                        //
int DecelerateTime = RAMP_DOWN;                             // Ramp D
int AccelerateTime = RAMP_UP;                               // Ramp U
long MotorRampUpPerMS = 0;                                  //
long MotorRampDownPerMS = 0;
int IdleSpeed = IDLE_SPEED;
int MinMotorSpeed = 1000;
int MinMotorSpeedIdle = 1000;
//int IdleMotorSpeed = 10;                                    //establish separate variable for idle motor speed
int CurrentMotorSpeed = MinMotorSpeed;
int TargetMotorSpeed = MinMotorSpeed;
byte SetMaxSpeed = 100;                                     // in percent.
unsigned long TimeLastMotorSpeedChanged = 0;
#define COMMAND_REV_NONE 0
#define COMMAND_REV_FULL 1
byte CommandRev = COMMAND_REV_NONE;
byte PrevCommandRev = COMMAND_REV_NONE;
byte MotorSpeedFull = MOTORSPEEDFULL;
byte MotorSpeed2Full = MOTORSPEED2FULL;
int MotorStartDwellTime = DWELL_UP;                         // Use this to hold the rev
int MotorStopDwellTime = DWELL_DOWN;                        // Use this to hold the rev


// Inputs
#define DebounceWindow 5                                    // Debounce Window = 5ms
#define RotaryDebounceWindow 100                            // Report window for rotary encoder changes
#define RepollInterval 250                                  // Just check the buttons ever xxx just in case we missed an interrupt.
volatile bool TriggerChanged = false;                       // ISR use
volatile bool EncoderSwChanged = false;                     // ISR use
volatile bool MagOutChanged = false;                        // ISR use
volatile bool SelFire1Changed = false;                      // ISR use
volatile bool SelFire2Changed = false;                      // ISR use
#define BTN_LOW 0
#define BTN_HIGH 1
#define BTN_ROSE 2
#define BTN_FELL 3
byte TriggerButtonState = BTN_HIGH;
byte MagOutButtonState = BTN_HIGH;
byte ConsoleRotaryAButtonState = BTN_LOW;                   //Encoder DT or CLK
byte ConsoleRotaryBButtonState = BTN_LOW;                   //ENCODER DT or CLK
byte ConsoleRotaryCButtonState = BTN_LOW;                   //ENcoder Sw
byte  RevSwButtonState = BTN_HIGH;
#define ROTARY_NONE 0
#define ROTARY_CW 1
#define ROTARY_CCW 2
byte RotaryTurned = ROTARY_NONE;
volatile byte LastPINB = 0;                                 // Preload with PINB Prior to events happening
volatile byte LastPINC = 0;                                 // Preload with PINC Prior to events happening
volatile byte LastPIND = 0;                                 // Preload with PIND Prior to events happening
#define BUTTON_PRESS_JUNK 5000                              // after 3s - encoder doesn't read value
#define BUTTON_PRESS_LONG 1500                              // 1.5s-3s encoder will read as long press
#define BUTTON_PRESS_SHORT 100
#define BTN_ACTION_NONE 0
#define BTN_ACTION_SHORT 1
#define BTN_ACTION_LONG 2
byte ConsoleRotaryButtonAction = BTN_ACTION_NONE;           //encoder rotary button
byte TriggerButtonAction = BTN_ACTION_NONE;
static unsigned long LastTriggerDownTime = 0;
static bool LastTriggerValue = false;
bool TChange = false;


// Pusher Controls - solenoid
volatile bool RequestStop = false;                          //
int PulseOnTime = 35;                                       // This get recalculated
int PulseOnTimeHigh = SP_HI;                                // SP High:
int PulseOnTimeLow = SP_LOW;                                // SP Low:
int PulseRetractTime = SP_RET;                              // SP Ret:
#define SOLENOID_CYCLE_IDLE 0
#define SOLENOID_CYCLE_PULSE 1
#define SOLENOID_CYCLE_RETRACT 2
#define SOLENOID_CYCLE_COOLDOWN 3
byte CurrentSolenoidCyclePosition = SOLENOID_CYCLE_IDLE;
unsigned long LastSolenoidCycleStarted = 0;
volatile bool PusherTickTock = false;                       // Flag indicating a solenoid fire cyclce has completed
byte ROFAdjustment = 0;                                     // Used to calculate the next adjustment into
byte ROFAdjustmentA = ROFA;                                 // Used to store the Full Auto adjustment
byte ROFAdjustmentB = ROFB;                                 // Used to store the Burst adjustment
byte TargetDPS = 0;                                         // Used to calculate the next adjustment into

//----------------------------------------------------------
byte Int0Changed = 0;                                       //
byte Int1Changed = 0;
byte Int0Tick = 0;
byte Int1Tick = 0;
byte SelFireAButtonState = 0;
byte SelFireBButtonState = 0;
byte SFRead = 5;                                            // Select fire read
byte ROF_RampTime = ROF_TM;
volatile bool IdleMode = false;                             // Ideal Mode Flag (low running motors)

//-----------------------------------------------------------

// Firing Controls
int DartsToFire = 0;
#define FIRE_MODE_SINGLE 0
#define FIRE_MODE_BURST 1
#define FIRE_MODE_AUTO 2
#define FIRE_MODE_SAFE 3
#define FIRE_MODE_IDLE 4
byte CurrentFireMode = FIRE_MODE_IDLE;
byte TargetFireMode = FIRE_MODE_SINGLE;                     //
bool RunFiringSequence = false;
bool StartNewFiringSequence = false;
bool ShutdownFiringSequence = false;
byte BurstSize = BURSTSIZE;
bool ExecuteFiring = false;                                 // Set to true when the Solenoid is supposed to move
int TimeBetweenShots = 0;                                   // Calculated to lower ROF


// Misc Controls
byte MagSize = MAGSIZE;
int DartsInMag = 0;
bool EnteringConfig = false;
bool ExitingConfig = false;
bool UpdateAmmoCounter = false;                             // Use this to signify when to update the ammo counter during a firing cycle.
static byte SelectFireClk = 1 ;


//bool JamDetected = false;                                   //


// System
#define SYSTEM_MODE_NORMAL 0
#define SYSTEM_MODE_MAGOUT 1
#define SYSTEM_MODE_CONFIG 2
#define SYSTEM_MODE_JAM 3
#define SYSTEM_MODE_LOWBATT 4
byte SystemMode = SYSTEM_MODE_NORMAL;


// Game Timer
#define ONESEC 1000                                       // one second divider
bool GameTimerRun = false;
bool DisplayInvert = false;
bool GameTimerStart = false;
byte GameTime = GAMETIMESTART;                            // user set Game mode time
int GameTimer = GAMETIMESTART;                            // Running game Timer
int sec = GameTimer;
int min = GameTimer;
int hrs = GameTimer;
byte TimerAlarm = 1;                                      // Time to flash display before timer finish. Set in Minutes
volatile unsigned long LastGTimerMillis = 0;              // 1 second timer trap

//ammo counter (mag switch code)

#ifdef MagSwEnableON
  byte MagSwEnable = MagSwON;
#else
  byte MagSwEnable = MagSwOFF;
#endif

//volatile bool MagSwEnable = (AmmoCounter == AmmoCtrON);

//debugging EEPROM Write Counts
//int eepromWriteCountAddress = 30;


/*
#ifdef EN_MAG_SW
  volatile bool MagSwEnable = (AmmoCounter == AmmoCtrON); // Enable Mag detector switch if AmmoCounter is ON
#else
  volatile bool MagSwEnable = false; // MagSwEnable is always false if EN_MAG_SW is not defined
#endif
*/




// Battery Controls - this needs to be adjusted for optomal perforance
#define BATTERY_3S 3                                      // 3S Battery type
#define BATTERY_4S 4                                      // 4S Battery Type
int BatteryOffset = BATTERY_CALFACTOR;                    // asign to a floating point veriable so can be adjusted in config menu (work in progress)
float BatteryCurrentVoltage = 99.0;
#ifdef BAT_3S                                             // if Bat 3s set these values at compile time
byte BatteyType = BATTERY_3S;                             // Default Battery Type can be changed to BATTERY_4S
float BatteryMinVoltage = BATTERY_3S_MIN;
float BatteryMaxVoltage = BATTERY_3S_MAX;
int SolenoidLowVoltage = (int)(BATTERY_3S_MIN * 10.0);
int SolenoidHighVoltage = (int)(BATTERY_3S_MAX * 10.0);
#endif
#ifdef BAT_4S                                             // If Bat 4S set these values at compile time
byte BatteyType = BATTERY_4S;                             // Default Battery Type can be changed to BATTERY_4S
float BatteryMinVoltage = BATTERY_4S_MIN;
float BatteryMaxVoltage = BATTERY_4S_MAX;
int SolenoidLowVoltage = (int)(BATTERY_4S_MIN * 10.0);
int SolenoidHighVoltage = (int)(BATTERY_4S_MAX * 10.0);
#endif
bool BatteryFlat = false;
volatile bool ADCRead = false;
volatile unsigned long ADCValue = 0;


// Configuration menu stuff
#define MENU_ITEMS 19                                     // 0 -> 18 O and 18 are EXIT options and not actioned
MenuItem* MenuItems[ MENU_ITEMS ];


// Profile Storage... I really should transact out of these.. But it allows for more flexability in a remote control scenario
byte CurrentProfile = 0;
struct ProfileDef
{
  byte GameTime = GAMETIMESTART;                          // Game Mode count down Timer, minutes
  byte MotorSpeedFull = MOTORSPEEDFULL;                   // Power: Stage 1 flywheels 50%
  byte MotorSpeed2Full = MOTORSPEED2FULL;                 // Power2: Stgae 2 flywheels  50%
  byte ROFAdjustmentA = ROFA;                             // A ROF Max    Full Auto rate
  byte ROFAdjustmentB = ROFB;                             // B ROF Max    Burst rate
  byte BurstSize = BURSTSIZE;                             // Burst 2
  byte MagSize = MAGSIZE;                                 // Mag 18
  int AccelerateTime = RAMP_UP;                           // Ramp Up 0 = fastest start up
  int DecelerateTime = RAMP_DOWN;                         // Ramp Down
  int MotorStartDwellTime = DWELL_UP;                     // Dwell Up keeps it reving
  int MotorStopDwellTime = DWELL_DOWN;                    // Dwell Down
  int PulseOnTimeHigh = SP_HI;                            // SP High (Solenoid)
  int PulseOnTimeLow = SP_LOW;                            // SP Low
  int PulseRetractTime = SP_RET;                          // SP Ret
#ifdef BAT_3S                                           //
  byte BatteyType = BATTERY_3S;                           // Default is 3S battery. this Variable gets changed via the config menu
#else                                                   //
  byte BatteyType = BATTERY_4S;                           //
#endif                                                  //

  int IdleSpeed = IDLE_SPEED;                            // Idle speed Long trig press to turn on/off in singel shot mode IdleSpeed
  byte ROF_RampTime = ROF_TM;                             // ROF Ramp Time in seconds

#ifdef MagSwEnableON
  byte MagSwEnable = MagSwON;
#else
  byte MagSwEnable = MagSwOFF;
#endif


};


ProfileDef Profiles[1];

// EEPROM addresses -- critical alignment here
#define ADDR_GT 0                                 // 0 
#define ADDR_MSF 1                                // 1 1 byte  
#define ADDR_MSF2 2                               // 2
#define ADDR_ROFA 3                               // 3
#define ADDR_ROFB 4                               // 4
#define ADDR_BURST 5                              // 5
#define ADDR_MAGSIZE 6                            // 6
#define ADDR_ACCEL 7                              // 7/8      2 byte
#define ADDR_DECEL 9                              // 9/10     2 byte
#define ADDR_STARTD 11                            // 11/12    2 byte
#define ADDR_STOPD 13                             // 13/14    2 byte
#define ADDR_PULSE_HIGH 15                        // 15/16    2 byte
#define ADDR_PULSE_LOW 17                         // 17/18    2 byte
#define ADDR_PULSE_RETRACT 19                     // 19/20    2 byte
#define ADDR_BAT_TYPE 21                          // 21
#define ADDR_IDLE_SPEED 22                        // 22/23    2 byte
#define ADDR_ROF_TM 24                            // 24
#define ADDR_AMMOCTR 25                           // 25   
#define ADDR_PROA_BASE 0                          // 26
#define ADDR_PROB_BASE 27                         // 28
#define ADDR_SECTORL 26                           // 26


//MagSwitch Debugging:

byte readMagSwEnable() {
  byte MagSwEnableValue;
  EEPROM.get((CurrentProfile * ADDR_PROB_BASE) + ADDR_AMMOCTR, MagSwEnableValue);
  return MagSwEnableValue;
}





// ============================================= SETUP =============================================================================
void setup() {
  Serial.begin( BAUDRATE );                             // Used for Debugging only, to save code space
  Serial.println ();
  Serial.print( F("Booting|Ver: ") );
  Serial.println ( VERSION );                           // Displat Version on Boot

  unsigned long BootStart = millis();


  //Debugging EEPROM write count
  //int writeCount = EEPROM.read (eepromWriteCountAddress);

  // Increment the write count from EEPROM
  //writeCount++;
  //EEPROM.write(eepromWriteCountAddress, writeCount);

  //Print the current write count
  //Serial.print("EEPROM Write Count: ");
  //Serial.println(writeCount);



  // ----------------------------- Boot LCD ------------------------------------------------------
  //Serial.println( F("Initialising Display") );
  Wire.begin();
  Wire.setClock(400000L);                               // 400khz
  Oled.begin(&Adafruit128x64, OLED_ADDR);               // 0.96 Inch display
  // Oled.begin(&Adafruit128x32, OLED_ADDR);            // 0.91 inch display (code not formated for this display)

  Oled.displayRemap( DisplayFlip );                     // set true or false to flip screen
  Oled.clear();

#ifdef EN_LOGO                                          // SAVE CODE SPACE 3% prog memory
  // Display logo                                       // The pic lives in Logo.h.
  //Oled.clear();
  for ( byte Row = 0; Row < 8; Row ++ )
  {
    Wire.beginTransmission( OLED_ADDR );                // Set the memory page
    Wire.write( 0x00 );
    Wire.write( 0xB0 | Row & 7 );
    Wire.endTransmission();
    Wire.beginTransmission( OLED_ADDR );                // Dump data 16 bytes per i2c packet
    Wire.write( 0x40 );
    int Seg = 0;
    for ( byte Col = 0; Col < 128; Col++ )
    {
      Seg++;
      if ( Seg == 16 )
      {
        Wire.endTransmission();
        Wire.beginTransmission( OLED_ADDR );
        Wire.write( 0x40 );
        Seg = 0;
      }
      Wire.write( ~(pgm_read_byte_near(Logo + Row * 128 + Col )) );
    }
    Wire.endTransmission();
  }
#endif

  // --------------------- Config Interupt Vectors  ----------------------------------------------
  // PCINT Inputs
  // PB - PCINT Vector 0 ----PORT B --Encoder PB Switch & Mag Sensor
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);    // D12 PB4 PCINT4
  PCMSK0 |= _BV(PCINT4);
  pinMode(PIN_MAG_S, INPUT_PULLUP);         // D11 PB3 PCINT3
  PCMSK0 |= _BV(PCINT3);
  pinMode(PIN_SEL_FIRE_1, INPUT_PULLUP);    // D8 PB0 PCINT0
  PCMSK0 |= _BV(PCINT0);
  
  // PC - PCINT Vector 1 --- PORT C ---- Trigger
  pinMode(PIN_TRIGGER, INPUT_PULLUP);       // D16 PC2 PCINT10
  PCMSK1 |= _BV(PCINT10);
  pinMode(PIN_SEL_FIRE_2, INPUT_PULLUP);    // A3 PC3 PCINT11
  PCMSK1 |= _BV(PCINT11);

  LastPINB = PINB;                          // Ensure that we pre-load the LastPINB register to capture changes from NOW...
  LastPINC = PINC;                          // Ensure that we pre-load the LastPINC register to capture changes from NOW...
  PCICR |=  _BV(PCIE0) | _BV(PCIE1);        // Activates control register for PCINT vector 0 & 1

  // External Inerrupt Pin D2
  pinMode(PIN_INT0, INPUT_PULLUP);
  EICRA |= _BV(ISC00);                      // Triggers on edge
  EIMSK |= _BV(INT0);                       // activates the interrupt

  //External Interupt Pin D3
  pinMode(PIN_INT1, INPUT_PULLUP);
  EICRA |= _BV(ISC10);                      // Triggers on edge
  EIMSK |= _BV(INT1);                       // activates the interrupt

  // ----------------------------- Rotary Encoder A3-CLK & D7-DT inputs  -----------------------
  pinMode(PIN_ENCODER_CLK, INPUT_PULLUP);   // Enable the pin, if goes wrong way reverse D2/D3 wires
  pinMode(PIN_ENCODER_DT, INPUT_PULLUP);    // Enable the pin

  pinMode(PIN_REV_SW, INPUT_PULLUP);        // Enable the pin

  // --------------------------------------------------------------------------------------------
  // Setup Motor ESC Outputs Setup ESC/PWM/OCR: Pin 10 = OC1B (Stage 1) & Pin 9 = OC1A (Stage2
  // Cretical code do not fuck with it....
  //Serial.println( F("Config PWM Ports") );
  pinMode( PIN_ESC, OUTPUT );               //Stage one ESC Outoput Pin D10
  digitalWrite( PIN_ESC, LOW );
  pinMode( PIN_ESC_2, OUTPUT );             //Stage one ESC Output Pin D9
  digitalWrite( PIN_ESC_2, LOW );

  // Initialise the PWM register for OC1
  TCCR1A = 0;
  TCCR1A = (1 <<  WGM11) | (1 << COM1A1) | (1 << COM1B1);  // pin 9 is COM1A1 pin 10 is COM1B1
  TCCR1B = 0;
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  ICR1 = 40000;
  UpdatePWM( 1000, 1000 );                  // OC1B & COC1A, get uptadeed togeather NewSpeed and NewSpeed2 value gets updated by ProcessMainMotors

  // --------------------------------------------------------------------------------------
  // Setup Pusher Output
  //Serial.println( F("Configuring Pusher FET") );
  pinMode( PIN_PUSHER_RUN, OUTPUT );
  SET_P_RUN_FULL_OFF;                       // binary load of the port to turn push OP off

  pinMode( PIN_PUSHER_STOP, OUTPUT );       // binary load of the port to turn push Stop OP off, in case off nafduino PCB
  SET_P_BRAKE_FULL_OFF;

  pinMode( PIN_TRACER, OUTPUT );
  SET_TRACER_D_OFF;

  // ------------------------------------------------------------------------------------
  // Calculate the motor ramp rates
  CalculateRampRates();

  // --------------------------- Set the system mode to normal --------------------------
  SystemMode = SYSTEM_MODE_NORMAL;

  // ------------------------------------------------------------------------------------
  // Setup the ADC
  ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX1) | (1 << MUX2); // Use AVCC as the aref; Set MUX to ADC7
  ADCSRA = 0;
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // Turn on ADC, Interrupt enable, Prescalar to 128
  ADCSRB = 0;
  // SFIOR has ADTS0-2 already set to 0 for free-running mode.
  // Start ADC running
  ADCSRA |= (1 << ADSC);

  // Pre-charge the battery indicator
  for ( byte c = 0; c < 8; c++ )
  {
    ProcessBatteryMonitor();                // Check battery voltage
    delay( 550 );
  }

  // Wait for the sync - 2 seconds, but run the button debouncer to capture a trigger down state for EEPROM reset
  while ( millis() - BootStart < 5000 )
  {
    SelFire1Changed = true;       
    ProcessDebouncing();
    delay( 10 );
  }

  // --------------------- Load and apply Profile Configuration -------------------------------
  LoadEEPROM();
  UpdateProfile();

  // Now wait until the trigger is high, this forces a DEFAULT of the eeprom
  // Serial.println( F("Waiting for trigger safety") );

  Oled.clear();
  Oled.setFont(ZevvPeep8x16);
  Oled.setCursor(40, 2);
  Oled.print( F("Finger") );
  Oled.setCursor(35, 4);
  Oled.print( F("Off The") );
  Oled.setCursor(35, 6);
  Oled.print( F("Trigger!") );
  while ( (TriggerButtonState == BTN_LOW) || (TriggerButtonState == BTN_FELL) )
  {
    ProcessDebouncing();
    delay(10);
  }
  delay(10);

  // ---------------------------- Build the menu Display -----------------------------------
  BuildMenu();
}

//=================================================== LOAD EEPROM =============================================================
void LoadEEPROM()
{
  // Just dump the EEPROM into SRAM and worry about it later... Coz I'm a lazy fuck.
  byte* ProfileBaseAddress = &Profiles[0].GameTime;
  for ( byte c = 0; c < ADDR_SECTORL; c++ )
  {
    *(ProfileBaseAddress + c) = EEPROM.read( c + ADDR_PROA_BASE );
  }

  // Check for validity of data (range test)
  bool CorruptData = false;
  if ( (Profiles[0].GameTime < 0) || (Profiles[0].GameTime > 120) ) CorruptData = true;
  if ( (Profiles[0].MotorSpeedFull < 30) || (Profiles[0].MotorSpeedFull > 100) ) CorruptData = true;
  if ( (Profiles[0].MotorSpeed2Full < 30) || (Profiles[0].MotorSpeed2Full > 100) ) CorruptData = true;
  if ( (Profiles[0].ROFAdjustmentA < 0) || (Profiles[0].ROFAdjustmentA > 150) ) CorruptData = true;
  if ( (Profiles[0].ROFAdjustmentB < 0) || (Profiles[0].ROFAdjustmentB > 150) ) CorruptData = true;
  if ( (Profiles[0].BurstSize < 2) || (Profiles[0].BurstSize > 99) ) CorruptData = true;
  if ( (Profiles[0].MagSize < 0) || (Profiles[0].MagSize > 99) ) CorruptData = true;
  if ( (Profiles[0].AccelerateTime < 0) || (Profiles[0].AccelerateTime > 5000) ) CorruptData = true;
  if ( (Profiles[0].DecelerateTime < 0) || (Profiles[0].DecelerateTime > 5000) ) CorruptData = true;
  if ( (Profiles[0].MotorStartDwellTime < 0) || (Profiles[0].MotorStartDwellTime > 5000) ) CorruptData = true;
  if ( (Profiles[0].MotorStopDwellTime < 0) || (Profiles[0].MotorStopDwellTime > 5000) ) CorruptData = true;
  if ( (Profiles[0].PulseOnTimeHigh < 0) || (Profiles[0].PulseOnTimeHigh > 1000) ) CorruptData = true;
  if ( (Profiles[0].PulseOnTimeLow < 0) || (Profiles[0].PulseOnTimeLow > 1000) ) CorruptData = true;
  if ( (Profiles[0].PulseRetractTime < 0) || (Profiles[0].PulseRetractTime > 1000) ) CorruptData = true;
  if ( (Profiles[0].BatteyType < BATTERY_3S) || (Profiles[0].BatteyType > BATTERY_4S) ) CorruptData = true;
  if ( (Profiles[0].IdleSpeed < 0) || (Profiles[0].IdleSpeed > 255) ) CorruptData = true;
  if ( (Profiles[0].ROF_RampTime < 0) || (Profiles[0].ROF_RampTime > 255) ) CorruptData = true;
  if ( (Profiles[0].MagSwEnable < MagSwOFF) || (Profiles[0].MagSwEnable > MagSwON) ) CorruptData = true;

  // Data is not valid, or the trigger was held on power-on for a reset (load default values)
  if ( (TriggerButtonState == BTN_LOW) || CorruptData )
  {
    Serial.println( F("Loaded Default Values") );

    // Create a fresh profile to use as a template n(get default values)
    ProfileDef TemplateProfile;

    // Just dump it over the old ones. If I was less lazy, i'd use memcpy :p
    byte* ProfileBaseAddress = &Profiles[0].GameTime;
    byte* TemplateBaseAddress = &TemplateProfile.GameTime;
    for ( byte c = 0; c < ADDR_SECTORL; c++ )
    {
      *(ProfileBaseAddress + c) = *(TemplateBaseAddress + c);
    }
    ProfileBaseAddress = &Profiles[1].GameTime;
    for ( byte c = 0; c < ADDR_SECTORL; c++ )
    {
      *(ProfileBaseAddress + c) = *(TemplateBaseAddress + c);
    }

    // And flash it back to EEPROM
    ProfileBaseAddress = &Profiles[0].GameTime;
    for ( byte c = 0; c < ADDR_SECTORL; c++ )
    {
      EEPROM.write( c + ADDR_PROA_BASE, *(ProfileBaseAddress + c) );
    }
  }
}

// ================= Builds the config menu structure using my MenuHelpers, a tricky bit of code this is =======================
void BuildMenu()
{
  MenuItems[0] = new ExitMenuItem();
  strcpy( MenuItems[0]->Title, "EXIT" );

  MenuItems[1] = new GameTimeMenuItem();
  strcpy( MenuItems[1]->Title, "GameTm:" );

  MenuItems[2] = new MotorMenuItem();
  strcpy( MenuItems[2]->Title, "Power1:" );

  MenuItems[3] = new MotorMenuItem();
  strcpy( MenuItems[3]->Title, "Power2:" );

  MenuItems[4] = new ROFMenuItem();
  strcpy( MenuItems[4]->Title, "A ROF :" );

  MenuItems[5] = new ROFMenuItem();
  strcpy( MenuItems[5]->Title, "B ROF :" );

  MenuItems[6] = new BurstSizeMenuItem();
  strcpy( MenuItems[6]->Title, "Burst :" );

  MenuItems[7] = new MagSizeMenuItem();
  strcpy( MenuItems[7]->Title, "Mag S :" );

  MenuItems[8] = new TimingMenuItem();          
  strcpy( MenuItems[8]->Title, "Ramp U:" );

  MenuItems[9] = new TimingMenuItem();
  strcpy( MenuItems[9]->Title, "Ramp D:" );

  MenuItems[10] = new TimingMenuItemFine();                  //Changed/added TimingMenuFine - which has goes by incremements of 10 instead of 50
  strcpy( MenuItems[10]->Title, "Dwel U:" );

  MenuItems[11] = new TimingMenuItem();
  strcpy( MenuItems[11]->Title, "Dwel D:" );

  MenuItems[12] = new  SolenoidTimingMenuItem ();
  strcpy( MenuItems[12]->Title, "SP Hi :" );

  MenuItems[13] = new  SolenoidTimingMenuItem ();
  strcpy( MenuItems[13]->Title, "SP Low:" );

  MenuItems[14] = new  SolenoidTimingMenuItem ();
  strcpy( MenuItems[14]->Title, "SP Ret:" );

  MenuItems[15] = new  BatteryMenuItem ();
  strcpy( MenuItems[15]->Title, "BatType" );

  MenuItems[16] = new  IdleSpeedMenuItem ();
  strcpy( MenuItems[16]->Title, "IdleSpd" );

  MenuItems[17] = new  ROF_RampTimeMenuItem ();
  strcpy( MenuItems[17]->Title, "ROF_Tm " );

  MenuItems[18] = new  MagSwEnableMenuItem ();
  strcpy( MenuItems[18]->Title, "MagSwitch " );
  
}

//---------------------------------------------- Calculate Ramp Rates Subroutine ------------------------------------------------
// This is a boot time init sub to calcualte the Acceleration and deceleration ramp rates of the motors.
void CalculateRampRates()
{
  long SpeedRange = (long)(MaxMotorSpeed - MinMotorSpeed) * 1000; // Multiply by 1000 to give some resolution due to integer calculations
  if ( AccelerateTime == 0 )
  {
    MotorRampUpPerMS = SpeedRange;  // For instant acceleration
  }
  else
  {
    MotorRampUpPerMS = SpeedRange / AccelerateTime;  // Use when Accelerating
  }

  if ( DecelerateTime == 0 )
  {
    MotorRampDownPerMS = SpeedRange;  // For instant acceleration
  }
  else
  {
    MotorRampDownPerMS = SpeedRange / DecelerateTime;  // Use when Decelerating
  }
}

// --------------------------------- Updates the PWM Timer for stage One & Two ESC motors  --------------------------------------
void UpdatePWM( int NewSpeed, int NewSpeed2 )
{
  NewSpeed = (NewSpeed * 2) + 2;        // Adjust for the prescalar
  OCR1B = NewSpeed;                     // ESC 1 (Pin D10)
  //Serial.print ( "OCR1B D10 = ");
  //Serial.println (NewSpeed);

  NewSpeed2 = (NewSpeed2 * 2) + 2;      // Adjust for the prescalar
  OCR1A = NewSpeed2;                    // ESC 2 (Pin D9)
  //Serial.print ( "OCR1A D9 = ");
  //Serial.println (NewSpeed2);
}


//======================================== Interupt Servce Routines ==============================================+=================
ISR( INT0_vect )           // V2 D2 SelctFireIn1 or Pusher Front
{
  if ( (PIND & 0b00000100) != (LastPIND & 0b00000100) )    // PD2
  {
    Int0Changed = true;
  }
  LastPIND = PIND;
}

// -------------------------------------------------------------------------------------------------
ISR( INT1_vect )            // V2 D3 SelctFireIn2 or Pusher Rear
{
  if ( (PIND & 0b00001000) != (LastPIND & 0b00001000) )   // PD3
  {
    Int1Changed = true;
  }
  LastPIND = PIND;
}

// -------------------------------------------------------------------------------------------------
ISR( PCINT0_vect )                      // Rotary Encoder PB switch and MAG Sensor PORT B PCINT 3 & 4 SelFire 1
{
  // Selectfire 1 is PB0,
  if ( (PINB & 0b00000001) != (LastPINB & 0b00000001) )
  {
    SelFire1Changed = true;
  }

  
  // Encoder Sw is PB4,
  if ( (PINB & 0b00010000) != (LastPINB & 0b00010000) )
  {
    EncoderSwChanged = true;
  }

  // Mag Switch is PB3
  if ( (PINB & 0b00001000) != (LastPINB & 0b000001000) )
  {
    MagOutChanged = true;
  }

  LastPINB = PINB;
}

// -------------------------------------------------------------------------------------------------
ISR( PCINT1_vect )                      // Trigger - PORT C PCINT - PC2
{
  // Trigger is PC2
  if ( (PINC & 0b00000100) != (LastPINC & 0b00000100) )
  {
    TriggerChanged = true;
  }

   // Trigger is PC3
  if ( (PINC & 0b00001000) != (LastPINC & 0b00001000) )
  {
    SelFire2Changed = true;
  }

  LastPINC = PINC;
}

//--------------------------------------------------------------------------------------------------
ISR( ADC_vect )                       // Battery Monitor - ADC ISR
{
  ADCRead = true;
  ADCValue = ADCL | (ADCH << 8);
}

//===================================== ProcessDebpounceing ========================================================================
// Process the debouncing of the directly connected MCU inputs. Interupts set flags from ISR handlers to trigger bits in here
void ProcessDebouncing()
{
  // Single call to millis() for better performance
  unsigned long CurrentMillis = millis();


#ifdef EN_SEL_FIRE_SW               // *** compile time option
  //----- Select Fire via 3 pos switch on  A3 and INT1 D8
  static byte LastSFRead = 4;

  if ((SelFire1Changed == true) || ( SelFire2Changed == true ))              // trigged on failing edge so going low
  {
    //Serial.println ("SELECTFIRE");
    SelFire1Changed =false;
    SelFire2Changed =false;
    //Int0Changed = false;
    SFRead = 99;
    if ( GET_SEL_FIRE_1 )
    {
      if ( SelFireAButtonState != BTN_HIGH ) SelFireAButtonState = BTN_HIGH;
    }
    else
    {
      if ( SelFireAButtonState != BTN_LOW ) SelFireAButtonState = BTN_LOW;
    }
    if ( GET_SEL_FIRE_2 )
    {
      if ( SelFireBButtonState != BTN_HIGH ) SelFireBButtonState = BTN_HIGH;
    }
    else
    {
      if ( SelFireBButtonState != BTN_LOW ) SelFireBButtonState = BTN_LOW;
    }
  }
  
  // Determine the current Select Fire mode and set it          //MOVE ??????
  if ( SFRead != LastSFRead )
  {
    if ( SelFireAButtonState == BTN_LOW && SelFireBButtonState == BTN_LOW  )
      SFRead = 3;
    if ( SelFireAButtonState == BTN_LOW && SelFireBButtonState == BTN_HIGH  )
      SFRead = 2;
    if ( SelFireAButtonState == BTN_HIGH && SelFireBButtonState == BTN_LOW )
      SFRead = 1;
    if ( SelFireAButtonState == BTN_HIGH && SelFireBButtonState == BTN_HIGH )
      SFRead = 0;

    if (SFRead == 1) TargetFireMode = FIRE_MODE_SINGLE;

    if (SFRead == 0) TargetFireMode = FIRE_MODE_BURST;

    if (SFRead == 2) TargetFireMode = FIRE_MODE_AUTO;

    LastSFRead = SFRead;
  }
#endif


  /*
     ------- Trigger
  */
  static bool RunTriggerTimer = false;
  static unsigned long LastTriggerPress = 0;
  static byte TriggerDebounceState = 0;
  // Set up a repoll interval, just in case the interrupt is missed.
  if ( CurrentMillis - LastTriggerPress > RepollInterval ) TriggerChanged = true;
  // Move from edge to steady state
  if ( TriggerButtonState == BTN_ROSE ) TriggerButtonState = BTN_HIGH;
  if ( TriggerButtonState == BTN_FELL ) TriggerButtonState = BTN_LOW;
  if ( TriggerChanged )
  {
    TriggerChanged = false;
    if ( !RunTriggerTimer )
    {
      LastTriggerPress = CurrentMillis;

      TriggerDebounceState = 0b01010101; // We need to give the button time to settle. This will track.
      RunTriggerTimer = true;
    }
  }
  if ( RunTriggerTimer && ( CurrentMillis - LastTriggerPress > DebounceWindow  ) )
  {
    TriggerDebounceState = (TriggerDebounceState << 1) | ((PINC >> 2) & 0b00000001); // Shift the register pin to the left, Shift the pin result to the right most position, and tack it onto the debounce state. Ensure that only the last position can be a 1.

    if ( (TriggerDebounceState == 0) || (TriggerDebounceState == 255) ) // All 0's or all 1's. This means we have settled.
    {
      RunTriggerTimer = false;

      if ( GET_FIRE_T )
      {
        if ( TriggerButtonState != BTN_HIGH ) TriggerButtonState = BTN_ROSE;
        Serial.println ("ROSE");
      }
      else
      {
        if ( TriggerButtonState != BTN_LOW ) TriggerButtonState = BTN_FELL;
        Serial.println ("Fell");
      }
    }
  }

  /*
     --------- Rotary Encoder
  */
  // Encoder DT
  if ( ConsoleRotaryAButtonState == BTN_ROSE ) ConsoleRotaryAButtonState = BTN_HIGH;
  if ( ConsoleRotaryAButtonState == BTN_FELL ) ConsoleRotaryAButtonState = BTN_LOW;
  static bool LastConsoleRotaryAState = true;
  bool ConsoleRotaryAState = (GET_ENCODER_DT );
  if ( ConsoleRotaryAState != LastConsoleRotaryAState)
  {
    LastConsoleRotaryAState = ConsoleRotaryAState;
    if ( ConsoleRotaryAState )
    {
      ConsoleRotaryAButtonState = BTN_FELL;
    }
    else
    {
      ConsoleRotaryAButtonState = BTN_ROSE;
    }
  }

  // Encoder CLK
  if ( ConsoleRotaryBButtonState == BTN_ROSE ) ConsoleRotaryBButtonState = BTN_HIGH;
  if ( ConsoleRotaryBButtonState == BTN_FELL ) ConsoleRotaryBButtonState = BTN_LOW;
  static bool LastConsoleRotaryBState = true;
  bool ConsoleRotaryBState = ( GET_ENCODER_CLK );

  if ( ConsoleRotaryBState != LastConsoleRotaryBState)
  {
    LastConsoleRotaryBState = ConsoleRotaryBState;
    if ( ConsoleRotaryBState )
    {
      ConsoleRotaryBButtonState = BTN_FELL;
    }
    else
    {
      ConsoleRotaryBButtonState = BTN_ROSE;
    }
  }

  // -------------------------- Rotary Encoder Switch handling ------------
  /*
     Rotary Encoder PB
  */
  if ( ConsoleRotaryCButtonState == BTN_ROSE ) ConsoleRotaryCButtonState = BTN_HIGH;
  if ( ConsoleRotaryCButtonState == BTN_FELL ) ConsoleRotaryCButtonState = BTN_LOW;
  static bool LastConsoleRotaryCState = true;
  bool ConsoleRotaryCState = ( GET_ENCODER_SW );

  if ( ConsoleRotaryCState != LastConsoleRotaryCState)
  {
    EncoderSwChanged = false;
    LastConsoleRotaryCState = ConsoleRotaryCState;
    if ( ConsoleRotaryCState )
      ConsoleRotaryCButtonState = BTN_FELL;
    else
      ConsoleRotaryCButtonState = BTN_ROSE;
  }

  // --------- Rev Switch Input -----------------------------------------------
  static bool LastRevButtonState = true;
  
  bool RevButtonState = (GET_REV_S );             //Read Rev Switch inout

  if ( RevButtonState != LastRevButtonState)
  {
    LastRevButtonState = RevButtonState;
    if (  RevButtonState  )
    {
      if ( RevSwButtonState != BTN_HIGH ) RevSwButtonState = BTN_HIGH;      // set flag
      //Serial.println("Rose" );
    }
    else
    {
      if ( RevSwButtonState != BTN_LOW ) RevSwButtonState = BTN_LOW;       // set flag
      //Serial.println("fell" );
    }
    //if ( RevButtonState == false ) RevButtonState = BTN_LOW;              //
  }

  // -----------------------------------------------------------------
  /*
     MagSwitch
  */
  static bool RunMagSwitchTimer = false;
  static unsigned long LastMagSwitchPress = 0;
  static byte MagSwitchDebounceState = 0;
  // Set up a repoll interval, just in case the interrupt is missed.
  if ( CurrentMillis - LastMagSwitchPress > RepollInterval ) MagOutChanged = true;
  // Move from edge to steady state
  if ( MagOutButtonState == BTN_ROSE ) MagOutButtonState = BTN_HIGH;
  if ( MagOutButtonState == BTN_FELL ) MagOutButtonState = BTN_LOW;
  if ( MagOutChanged )
  {
    MagOutChanged = false;
    if ( !RunMagSwitchTimer )
    {
      LastMagSwitchPress = CurrentMillis;

      MagSwitchDebounceState = 0b01010101;             // We need to give the button time to settle. This will track.
      RunMagSwitchTimer = true;
    }
  }
  if ( RunMagSwitchTimer && ( CurrentMillis - LastMagSwitchPress > DebounceWindow  ) )
  {
    MagSwitchDebounceState = (MagSwitchDebounceState << 1) | ((PIND >> 7) & 0b00000001); // Shift the register pin to the left, Shift the pin result to the right most position, and tack it onto the debounce state. Ensure that only the last position can be a 1.

    if ( (MagSwitchDebounceState == 0) || (MagSwitchDebounceState == 255) )              // All 0's or all 1's. This means we have settled.
    {
      RunMagSwitchTimer = false;

      if ( GET_MAG_S )
      {
        if ( MagOutButtonState != BTN_HIGH ) MagOutButtonState = BTN_ROSE;
      }
      else
      {
        if ( MagOutButtonState != BTN_LOW ) MagOutButtonState = BTN_FELL;
      }
      if ( MagSwEnable == false ) MagOutButtonState = BTN_LOW;                            // If no Mag Dector switch, lock the output
    }
  }
}


//==================================================================================================================================
//                      Main loop. Run the blaster here.
//==================================================================================================================================
void loop() {
  
  
  ProcessDebouncing();              // Process the pin input, and handle any debouncing
  ProcessButtons();                 // Where interperation is necessary, handle it here Trigger is being done here
  ProcessBatteryMonitor();          // Check battery voltage occasionally
  ProcessMagRelease();              // Handle when a magazing is dropped out
  ProcessSystemMode();              // Find out what the system should be doing
  ProcessConsoleButtonManager();    // Work out what to do based on the console button input
  ProcessDisplay();                 // Update the Oled display

  if ( PrevCommandRev != CommandRev )     // Detected a change to the command. Reset the last speed change timer
  {
    TimeLastMotorSpeedChanged = millis();
    PrevCommandRev = CommandRev;
  }
  ProcessSpeedControl();            // Process speed control to resolve a % target
  ProcessMotorSpeed();              // Calculate the new motor speed in terms of a PWM signal
  ProcessMainMotors();              // Send the speed to the ESC

  ProcessFiringControl();           // Handle the cycle control here

  ProcessPusherControl();           // Handle the pusher management here
  ProcessGameTimer();               // update game timer

  //Debugging EEPROM write count
  //int writeCount = EEPROM.read (eepromWriteCountAddress);
  
/*
  Serial.println("AmmoCounter State");          //readout serial within main loop
  Serial.println(AmmoCounter);
  Serial.println("SystemMode");
  Serial.println(SystemMode);
  Serial.println("MagSwEnable");
  Serial.println(MagSwEnable);

*/
  
  if (Serial.available() > 0) {
    // Check for a command or trigger to read and print AmmoCounter value
    char command = Serial.read();
    if (command == 'R' || command == 'r') {
      byte savedMagSwEnable = readMagSwEnable();
      Serial.print("MagSwEnable Read: ");
      Serial.println(savedMagSwEnable);
      //Serial.print("MagSwEnable: ");
      //Serial.println(MagSwEnable);
      //Serial.print("EEPROM Write Count: ");
      //Serial.println(writeCount);
    }
  }


}
// ====================================================== End Main Loop ===========================================================


//---------------------------------------------------------------------------------------------------------------------------------
// this process butons after debounce, ie long and short clicks, and encoder value
void ProcessConsoleButtonManager()
{

  // Decode the rotary
  static unsigned long LastRotaryChange = 0;
  RotaryTurned = ROTARY_NONE;

  bool ConsoleRotaryAValue = (ConsoleRotaryAButtonState == BTN_ROSE) || (ConsoleRotaryAButtonState == BTN_HIGH);
  bool ConsoleRotaryBValue = (ConsoleRotaryBButtonState == BTN_ROSE) || (ConsoleRotaryBButtonState == BTN_HIGH);
  static bool LCAV = false;    //Last Console A Value
  static bool LCBV = false;   //Last Console B Value
  bool Change = false;

  if ( ConsoleRotaryAValue != LCAV )
  {
    Change = true;
  }

  if ( ConsoleRotaryBValue != LCBV )
  {
    Change = true;
  }

  if ( Change )
  {
    // RotaryTurned =, is the output of this bit
    if ( !ConsoleRotaryAValue && !ConsoleRotaryBValue )
    {
      if ( !LCAV && LCBV )
      {
        RotaryTurned = ROTARY_CW;
      }
      if ( LCAV && !LCBV )
      {
        RotaryTurned = ROTARY_CCW;
      }
    }

    LCAV = ConsoleRotaryAValue;
    LCBV = ConsoleRotaryBValue;
  }

  // ------- Handel Encoder speed control
  if  ((SystemMode == SYSTEM_MODE_NORMAL) && ( RotaryTurned == ROTARY_CCW ))
  {
   MotorSpeedFull += 5;
   if( MotorSpeedFull > 100 ) MotorSpeedFull = 100;
   //Serial.println ( MotorSpeedFull ); 
  }
  if  ((SystemMode == SYSTEM_MODE_NORMAL) && ( RotaryTurned == ROTARY_CW ))
  {
   MotorSpeedFull -= 5;
   if( MotorSpeedFull < 30 ) MotorSpeedFull = 30;
   //Serial.println ( MotorSpeedFull ); 
  }


  // ------------ Handle the Rotary Encoder button --------------------------------------
  static unsigned long LastConsoleRotaryDownTime = 0;
  static bool LastConsoleRotaryValue = false;
  Change = false;
  ConsoleRotaryButtonAction = BTN_ACTION_NONE;
  bool ConsoleRotaryValue = (ConsoleRotaryCButtonState == BTN_ROSE) || (ConsoleRotaryCButtonState == BTN_HIGH);
  if ( LastConsoleRotaryValue != ConsoleRotaryValue )
  {
    Change = true;
    LastConsoleRotaryValue = ConsoleRotaryValue;
  }
  if ( Change )
  {
    if ( !ConsoleRotaryValue )
    {
      if ( millis() - LastConsoleRotaryDownTime > BUTTON_PRESS_JUNK )
      {
        // Junk press.. Ignore
      }
      else if ( millis() - LastConsoleRotaryDownTime > BUTTON_PRESS_LONG )
      {
        ConsoleRotaryButtonAction = BTN_ACTION_LONG;
      }
      else if ( millis() - LastConsoleRotaryDownTime > BUTTON_PRESS_SHORT )
      {
        ConsoleRotaryButtonAction = BTN_ACTION_SHORT;
      }
    }
    if ( ConsoleRotaryValue )
    {
      LastConsoleRotaryDownTime = millis();
    }
  }

#ifndef EN_SEL_FIRE_SW                        //if we not defined selecfire then SF via Encoder PB
  // Execute Select Fire                                        
  if ( SystemMode == SYSTEM_MODE_NORMAL )
  {
    if ( ConsoleRotaryButtonAction == BTN_ACTION_SHORT )
    {
      SelectFireClk += 1;
      if ( SelectFireClk == 4)  SelectFireClk = 1;           //trap clicks 1-3 range

      if (SelectFireClk == 1) TargetFireMode = FIRE_MODE_SINGLE;

      if (SelectFireClk == 2) TargetFireMode = FIRE_MODE_BURST;

      if (SelectFireClk == 3) TargetFireMode = FIRE_MODE_AUTO;
    }
  }
#endif

  // ------------- Detect Long Trigger Press, Turns ideal mode on -------------
  static unsigned long LastTriggerDownTime = 0;
  static bool LastTriggerValue = false;
  bool TChange = false;
  TChange = false;
  TriggerButtonAction = BTN_ACTION_NONE;                      //reset the output
  bool TriggerValue = (TriggerButtonState == BTN_FELL) || (TriggerButtonState == BTN_LOW);
  if ( LastTriggerValue != TriggerValue )
  {
    TChange = true;
    LastTriggerValue = TriggerValue;
  }
  if ( TChange )
  {
    if ( !TriggerValue )
    {
      if ( millis() - LastTriggerDownTime > BUTTON_PRESS_JUNK )
      {
        // Junk press.. Ignore
      }
      else if ( millis() - LastTriggerDownTime > BUTTON_PRESS_LONG )
      {
        TriggerButtonAction = BTN_ACTION_LONG;
      }
      else if ( millis() - LastTriggerDownTime > BUTTON_PRESS_SHORT )
      {
        TriggerButtonAction = BTN_ACTION_SHORT;
      }
    }
    if ( TriggerValue )
    {
      LastTriggerDownTime = millis();
    }
  }
  
#ifndef EN_REV_SW                                // If EN_REV_SW is NOT defined - use long trigger press
  if ((TriggerButtonAction == BTN_ACTION_LONG) && (TargetFireMode == FIRE_MODE_SINGLE) && (IdleSpeed > 0))
  {
    if (IdleMode == false)                        // if off turn on
    {
      IdleMode = true;                            // set flag turn Idle on
      Serial.println ("Rev On");

      //IdleMotorSpeed = (IdleSpeed * 10) + 1000;
      //TargetMotorSpeed = IdleMotorSpeed;           // set new target speed
      MinMotorSpeed = (IdleSpeed * 10) + 1000;
      //MinMotorSpeedIdle = (IdleSpeed * 10) + 1000;
      TargetMotorSpeed = MinMotorSpeed;           // set new target speed
    

      //Serial.println ("IdleMotorSpeed");
      //Serial.println (IdleMotorSpeed);


    }
    else if (IdleMode == true)
    {
      IdleMode = false;                           // set flag idle off
      Serial.println ("Rev Off");
      MinMotorSpeed = 1000;
      TargetMotorSpeed = MinMotorSpeed;           // set new target speed
    }
    TriggerButtonAction = BTN_ACTION_NONE;
  }
#else
  //---- Rev switch to turn on/off ideal mode
  if ((RevSwButtonState == BTN_LOW ) &&  (IdleSpeed > 0) )      //turn idel on
  {
    if (IdleMode == false)                         // ifs off so turn on
    {
      IdleMode = true;                           // set flag turn Idle on
      MinMotorSpeed = (IdleSpeed * 10) + 1000;
      //MinMotorSpeedIdle = (IdleSpeed * 10) + 1000;
      TargetMotorSpeed = MinMotorSpeed;          // set new target speed
      SET_TRACER_D_ON;
    }
  }
  if ((RevSwButtonState == BTN_HIGH )  &&  (IdleMode == true))    //turn idel off
  {
    IdleMode = false;                           // set flag idle off
    MinMotorSpeed = 1000;
    TargetMotorSpeed = MinMotorSpeed;           // set new target speed
    digitalWrite( PIN_TRACER, LOW );
  }
#endif
}


//---------------------------------------- PROCESS SYTEM MODE ----------------------------------------------------------------------
// Process the main system state.
void ProcessSystemMode()
{
  bool MagReleasePressed = ((MagOutButtonState == BTN_LOW ) || (MagOutButtonState == BTN_FELL) );

  if ( ((SystemMode == SYSTEM_MODE_NORMAL) || (SystemMode == SYSTEM_MODE_MAGOUT) || (SystemMode == SYSTEM_MODE_LOWBATT)) && ( ConsoleRotaryButtonAction == BTN_ACTION_LONG ) && !ExitingConfig )
  {
    EnteringConfig = true;
    SystemMode = SYSTEM_MODE_CONFIG;
  }
  //else if( JamDetected )
  //{
  //  SystemMode = SYSTEM_MODE_JAM;
  //  ExitingConfig = false;
  //}
  else if ( BatteryFlat )
  {
    if ( (SystemMode != SYSTEM_MODE_CONFIG ) || ExitingConfig )
    {
      SystemMode = SYSTEM_MODE_LOWBATT;
      ExitingConfig = false;
    }
  }
  else if ( !MagReleasePressed )
  {
    if ( (SystemMode != SYSTEM_MODE_CONFIG ) || ExitingConfig )
    {
      SystemMode = SYSTEM_MODE_MAGOUT;
      ExitingConfig = false;
    }
  }
  else if ( ( SystemMode != SYSTEM_MODE_CONFIG ) || ExitingConfig )
  {
    SystemMode = SYSTEM_MODE_NORMAL;
    ExitingConfig = false;
  }
}


//------------------------------------- PROCESS FIRING CONTROL ---------------------------------------------------------------------
void ProcessFiringControl()     // Handle the high-level firing logic
{
#define FS_STAGE_IDLE 0
#define FS_STAGE_REV 1
#define FS_STAGE_FIRING 2
#define FS_STAGE_DECEL 3
  static byte FiringSequenceStage = FS_STAGE_IDLE;
  static byte LastSystemMode = 99;
  static bool PreviousRunFiringSequence = true;
  static unsigned long RevStart = 0;
  static unsigned long DecelStart = 0;

  int ROFToLoad = 00; // Calculate the ROF to load into the system, this is TargetDPS in solenoide code

  int StartLag = MOTOR_SPINUP_LAG + AccelerateTime; // Calculate the lag for the motor start

  //if(IdleMode == false)
  //{MinMotorSpeed = MinMotorSpeedIdle;
  //}


  if ( SystemMode != SYSTEM_MODE_NORMAL )
  {
    if ( StartNewFiringSequence )
      StartNewFiringSequence = false;               // Abort a new cycle
    CommandRev = COMMAND_REV_NONE;
    if ( RunFiringSequence )
    {
      switch ( FiringSequenceStage )
      {
        case FS_STAGE_REV:
          // Bypass the firing sequence and start decelerating
          FiringSequenceStage = FS_STAGE_DECEL;
          DecelStart = 0;
          break;
        case FS_STAGE_FIRING:
          // Set one dart into the queue if there is more than one
          if ( DartsToFire > 1 ) DartsToFire = 1;
          break;
        case FS_STAGE_DECEL:
          // Already decelerating
          break;
        default:
          break; // Idle
      }
    }
  }

  if ( StartNewFiringSequence )
  {
    StartNewFiringSequence = false;
    if ( (SystemMode != SYSTEM_MODE_NORMAL) )
    {
      return; // Don't start a new firing sequence if out of the right mode.
    }
    if ( RunFiringSequence )
    {
      switch ( FiringSequenceStage )
      {
        case FS_STAGE_REV:
          // Carry on
          break;
        case FS_STAGE_FIRING:
          // Add more darts to the queue
          switch ( CurrentFireMode )
          {
            case FIRE_MODE_SINGLE:    // Add one to the queue
              DartsToFire ++;
              break;
            case FIRE_MODE_BURST:
              DartsToFire = BurstSize;
              break;
            case FIRE_MODE_AUTO:
              DartsToFire = 99;
              break;
            default: // Idle and Safe Mode
              break;
          }
          break;
        case FS_STAGE_DECEL:
          // Start again from scratch, but factor in the current motor speed
          if ( TargetFireMode != FIRE_MODE_SAFE )
          {
            switch ( CurrentFireMode )
            {
              case FIRE_MODE_SINGLE: // Add one to the queue
                ROFToLoad = 0;
                break;
              case FIRE_MODE_BURST:
                ROFToLoad = ROFAdjustmentB;
                break;
              case FIRE_MODE_AUTO:
                ROFToLoad = ROFAdjustmentA;
                break;
              default: // Idle and Safe Mode
                break;
            }
            FiringSequenceStage = FS_STAGE_REV;
            RequestStop = false;
            ROFAdjustment = ROFToLoad;
            CommandRev = COMMAND_REV_FULL;                              // this is ESC kick off
            //RevStart = millis() + MotorStartDwellTime + (StartLag / 2);                    // Basic ignore the user dwell time, and have half of the startup latency
            RevStart = millis() + ((MotorStartDwellTime + StartLag) / 2);                    // Changed to include MotorStartDwellTime + StartLag, then divided by 2, this is likely closer to reality
            CurrentFireMode = TargetFireMode;
            //Serial.println ("REV_FULL-1");
          }
          // Add one in single shot mode
          // Reset to Burst in burst mode
          // Set to 99 in Auto mode
          switch ( CurrentFireMode )
          {
            case FIRE_MODE_SINGLE: // Add one to the queue
              DartsToFire = 1;
              break;
            case FIRE_MODE_BURST:
              DartsToFire = BurstSize;
              break;
            case FIRE_MODE_AUTO:
              DartsToFire = 99;
              break;
            default: // Idle and Safe Mode
              break;
          }
          break;
        default:
          // We shouldn't be here, but we were probably idle.
          break;
      }
    }
    else
    {
      if ( TargetFireMode != FIRE_MODE_SAFE )
      {
        RunFiringSequence = true;
        FiringSequenceStage = FS_STAGE_REV;
        RevStart = 0;
        CurrentFireMode = TargetFireMode;
      }
    }
  }

  if ( ShutdownFiringSequence )
  {
    // Only need for auto, reset dart counter to 1 and let it fly out naturally
    ShutdownFiringSequence = false;
    if ( CurrentFireMode == FIRE_MODE_AUTO )
    {
      DartsToFire = 1;
    }
  }

  if ( RunFiringSequence )
  {
    if ( FiringSequenceStage == FS_STAGE_REV )
    {
      if ( RevStart == 0 )
      {
        // Init
        switch ( CurrentFireMode )
        {
          case FIRE_MODE_SINGLE: // Add one to the queue
            ROFToLoad = 0;
            break;
          case FIRE_MODE_BURST:
            ROFToLoad = ROFAdjustmentB;
            break;
          case FIRE_MODE_AUTO:
            ROFToLoad = ROFAdjustmentA;
            break;
          default: // Idle and Safe Mode
            break;
        }
        RequestStop = false;
        ROFAdjustment = ROFToLoad;
        CommandRev = COMMAND_REV_FULL;                //ESC trigger, kicks off void ProcessSpeedControl
        //Serial.println ("REV_FULL-2");
        RevStart = millis();
        switch ( CurrentFireMode )
        {
          case FIRE_MODE_SINGLE: // Add one to the queue
            DartsToFire = 1;
            break;
          case FIRE_MODE_BURST:
            DartsToFire = BurstSize;
            break;
          case FIRE_MODE_AUTO:
            DartsToFire = 99;
            break;
          default: // Idle and Safe Mode
            break;
        }
      }
      else if ( millis() - RevStart > (StartLag + MotorStartDwellTime) )  // We have waited long enough for the motor to ramp
      {
        //Serial.println ("waited2longGo");                               // start pusher firing sequence
        RevStart = 0;
        FiringSequenceStage = FS_STAGE_FIRING;
        LastSolenoidCycleStarted = millis();
        //Serial.println ("SOLENOID STARTED");
        PulseOnTime = map( (int)(BatteryCurrentVoltage * 10), SolenoidHighVoltage, SolenoidLowVoltage, PulseOnTimeLow, PulseOnTimeHigh );
        if ( PulseOnTime < 0 )
          PulseOnTime = 0;
        ExecuteFiring = true;
      }
    }
    else if ( FiringSequenceStage == FS_STAGE_FIRING )
    {
      if ( RequestStop ) // Want to stop - move from firing state to stop state
      {
        FiringSequenceStage = FS_STAGE_DECEL;
        DecelStart = 0;
      }
      if ( CommandRev == COMMAND_REV_NONE ) // Somehow the motors aren't spinning
      {
        RequestStop = true;
        FiringSequenceStage = FS_STAGE_DECEL;
        DecelStart = 0;
      }
    }
    else if ( FiringSequenceStage == FS_STAGE_DECEL )
    {
      if ( DecelStart == 0 ) // Initiate shutdown
      {
        DecelStart = millis();
      }
      else if ( ((millis() - DecelStart > MotorStopDwellTime) || (millis() - DecelStart > 5000)) && !(CommandRev == COMMAND_REV_NONE) )
      {
        CommandRev = COMMAND_REV_NONE;
      }
      else if ( CurrentMotorSpeed == MinMotorSpeed ) // Shutdown complete
      {
        FiringSequenceStage = FS_STAGE_IDLE;
        RunFiringSequence = false;
        DecelStart = 0;
        CurrentFireMode = FIRE_MODE_IDLE;
      }
    }
  }
  else
  {
    CommandRev = COMMAND_REV_NONE;
  }
  

}
//------------------------------------ END Process Firing Control  --  -------------------------------------------------------------


//------------------------------------ Process Pusher Control ----------------------------------------------------------------------
void ProcessPusherControl()
{
  static bool PreviousPusherTickTock = true;    // Flag indicating a solenoid firecycle has been completed

  if ( !ExecuteFiring )                         // Just skip if there is no firing to execute
  {
    return;
  }

  // this does Darts and Mag Counter, its trigger via the flag which is set after a solenoid fire cycle, has been completed
  if ( PusherTickTock != PreviousPusherTickTock ) // Pusher has changed positions, a shot has been fired
  {
    PreviousPusherTickTock = PusherTickTock;      //  Recoder flag status
    PusherTickTock = false ;                      //  Reset fired flag
    if ( PreviousPusherTickTock )                 //  Pusher is out, we have fired
    {
      DartsToFire --;
      if ( MagSize == 0 )
      {
        if ( DartsInMag < 255 ) DartsInMag ++;    // Handle the magazine status
      }
      else
      {
        if ( DartsInMag >= 1 ) DartsInMag --;     // Handle the magazine status
      }
    }
    else
    {
      UpdateAmmoCounter = true;                   // We're in the home position, so spend the time to update the OLED.
    }
  }                                              //------- End if PusherTickTock

  // cheek some stuff
  //-------------------------------------------------------
  // Initiate Pusher Shutdown, if run out of darts
  if ( DartsToFire < 1 )
  {
    //RequestStop = true;                               //FIX later, triger shut down. not effecting anything
    //Serial.println( "PC_RequestStop" );
    
    CurrentFireMode = FIRE_MODE_IDLE;
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_IDLE;
    SET_P_RUN_FULL_OFF;
    ExecuteFiring = false;
    RequestStop = true;
    //Serial.println( "PC_Finished" );                          // this stops it
    return;
  }

  // -------------------------------------------------------
  // This is an error condition. Just kind pretend it never happens, but allow for debugging code to catch it.
  if ( DartsToFire < 0 )
  {
    //Serial.println( "%%%% WTF %%%%" );
    DartsToFire = 0;
  }

  // -------------------------------------------------------
  // Calculate duty cycle whenever the target changes (this is ROF)
  static byte PrevTargetDPS = 0;
  static bool ResetPulseTimer = true;

  TargetDPS = ROFAdjustment;              //link old and new code TargetDPS can be replaced with ROFAdjustment
  if ( PrevTargetDPS != TargetDPS )
  {
    PrevTargetDPS = TargetDPS;
    if ( TargetDPS == 99 ) // Full rate
    {
      TimeBetweenShots = 0;
    }
    else
    {
      int PulseOverhead = PulseOnTime + PulseRetractTime;
      int TotalPulseOverhead = PulseOverhead * TargetDPS;
      int FreeMS = TotalPulseOverhead - 1000;
      TimeBetweenShots = FreeMS / TargetDPS ;
      if ( FreeMS <= 0 )
      {
        TimeBetweenShots = 0; // Pusher won't achieve this rate
      }
      else
      {
        TimeBetweenShots = FreeMS / TargetDPS;
      }
    }
  }

  // We actually have nothing to do.
  if ( CurrentFireMode == FIRE_MODE_IDLE )
  {
    //Serial.println( "PC_FireMode IDEL" );
    return;                             // Solenoid is idling.
  }

  // We are apparently supposed to fire 0 darts... Typically for end-of-firing scenarios. This is the exit point
  if ( (DartsToFire == 0) && (CurrentFireMode != FIRE_MODE_IDLE) )
  {
    CurrentFireMode = FIRE_MODE_IDLE;
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_IDLE;
    SET_P_RUN_FULL_OFF;
    ExecuteFiring = false;
    RequestStop = true;
    //Serial.println( "PC_Finished" );
    return;
  }

  // Last check to ensure the motors are running before we send a dart into them
  if ( (CommandRev == COMMAND_REV_NONE) && (SystemMode == SYSTEM_MODE_NORMAL) )
  {
    CurrentFireMode = FIRE_MODE_IDLE;
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_IDLE;
    SET_P_RUN_FULL_OFF;
    ExecuteFiring = false;
    RequestStop = true;
    //Serial.println( "PC_Rev None" );
    return;
  }

  // Pulse solenoid on high
  if ( (millis() - LastSolenoidCycleStarted) < PulseOnTime )
  {
    //Serial.print ( "PulseOnTime ");
    //Serial.println (PulseOnTime);       //Calculated number handy to see
    //Serial.println( "PULSE" );
    if ( CurrentSolenoidCyclePosition != SOLENOID_CYCLE_PULSE )
    {
      if ( (SystemMode != SYSTEM_MODE_NORMAL) ) // Don't fire unless the system mode is normal
      {
        DartsToFire = 0;
        return;
      }
    }
    if ( ResetPulseTimer )
    {
      ResetPulseTimer = false;
      LastSolenoidCycleStarted = millis();
    }
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_PULSE;
    SET_P_RUN_FULL_ON;
    return;
  }

  // Release solenoid for retraction
  if ( (millis() - LastSolenoidCycleStarted) < (PulseOnTime + PulseRetractTime) )
  {
    //Serial.println( "RETRACT" );
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_RETRACT;
    SET_P_RUN_FULL_OFF;
    return;
  }

  // Wait for the Global Cool Down... i.e. ROF adjustment
  if ((millis() - LastSolenoidCycleStarted) < (PulseOnTime + PulseRetractTime + TimeBetweenShots))
  {
    //Serial.println( "COOL" );
    CurrentSolenoidCyclePosition = SOLENOID_CYCLE_COOLDOWN;
    SET_P_RUN_FULL_OFF;
    return;
  }

  // We have completed a single solenoid cycle
  CurrentSolenoidCyclePosition = SOLENOID_CYCLE_IDLE;   // Return to idle, ready for the next shot
  LastSolenoidCycleStarted = millis();
  ResetPulseTimer = true ;
  PusherTickTock = true ;                               // Flag Shot fired, for DartsToFire/Amo Counter update, //DartsToFire -= 1;
  PreviousPusherTickTock = false ;
  Serial.println( "Bang!" );
  
 
  Serial.println("MinMotorSpeed");
  Serial.println(MinMotorSpeed);
  Serial.println("TargetMotorSpeed");
  Serial.println(TargetMotorSpeed);
  Serial.println("MinMotorSpeedIdle");
  Serial.println(MinMotorSpeedIdle);
  Serial.println("SetMaxSpeed");
  Serial.println(SetMaxSpeed);
  

}
// ------------------------------------ End Process Pusher Control  -----------------------------------------------------------------


//----------------------------- Process Motor Speed Subroutine --------------------------------------------------------------------
// Calculate the desired motor speed based on where we are, and where we need to be. This takes into account the ramp rates of the motor.
void ProcessMotorSpeed()
{

  //Serial.println("Process Motor Speed Subroutine TargetMotorSpeed");
  //Serial.println(TargetMotorSpeed);

  // Don't do anything if the motor is already running at the desired speed.
  if ( CurrentMotorSpeed == TargetMotorSpeed )
  {
    return;
  }

  unsigned long CurrentTime = millis(); // Need a base time to calcualte from
  unsigned long MSElapsed = CurrentTime - TimeLastMotorSpeedChanged;
  if ( MSElapsed == 0 ) // No meaningful time has elapsed, so speed will not change
  {
    return;
  }
  if ( CurrentMotorSpeed < TargetMotorSpeed )
  {
    long SpeedDelta = (MSElapsed * MotorRampUpPerMS / 1000);
    if ( SpeedDelta < 1 ) return; // Not enough cycles have passed to make an appreciable difference to speed.
    int NewMotorSpeed = CurrentMotorSpeed + SpeedDelta; // Calclate the new motor speed..

    // If it's within 1% (which is 10) of target, then just set it
    if ( NewMotorSpeed + 10 >= TargetMotorSpeed )
    {
      NewMotorSpeed = TargetMotorSpeed;
    }

    TimeLastMotorSpeedChanged = CurrentTime;
    CurrentMotorSpeed = NewMotorSpeed;
  }
  if ( CurrentMotorSpeed > TargetMotorSpeed )
  {
    long SpeedDelta = (MSElapsed * MotorRampDownPerMS / 1000);
    if ( SpeedDelta < 1 ) return;                              // Not enough cycles have passed to make an appreciable difference to speed.
    int NewMotorSpeed = CurrentMotorSpeed - SpeedDelta;       // Calclate the new motor speed..

    // If it's within 1% (which is 10) of target, then just set it
    if ( NewMotorSpeed - 10 <= TargetMotorSpeed )
    {
      NewMotorSpeed = TargetMotorSpeed;
    }

    TimeLastMotorSpeedChanged = CurrentTime;
    CurrentMotorSpeed = NewMotorSpeed;
  }
}


//------------------------------------------ PROCESS SPEED CONTROL -----------------------------------------------------------------
// We need to set the Target Motor Speed here, as a percentage. Sense check it to ensure it's not too slow, or too fast.
void ProcessSpeedControl()
{
  static byte LastSetMaxSpeed = 100;

  if ( CommandRev == COMMAND_REV_FULL ) SetMaxSpeed = MotorSpeedFull;
  if ( CommandRev == COMMAND_REV_NONE ) SetMaxSpeed = 0;

/*
  if ( CommandRev == COMMAND_REV_FULL ) {
    SetMaxSpeed = MotorSpeedFull;
  }
  else if ( CommandRev == COMMAND_REV_NONE ) {
    SetMaxSpeed = 0;
  } 
  else {
    SetMaxSpeed = IdleMode ? IdleSpeed : MotorSpeedFull;
  }
*/

  if( IdleMode == true)
  {
    if ( CommandRev == COMMAND_REV_NONE ) SetMaxSpeed = IdleSpeed;
  }

  if ( LastSetMaxSpeed == SetMaxSpeed ) return; // Speed hasn't changed

  if ( CommandRev > COMMAND_REV_NONE )
  {
    SetMaxSpeed = constrain( SetMaxSpeed, 30, 100 ); // Constrain between 30% and 100%
  }

  TargetMotorSpeed = map( SetMaxSpeed, 0, 100, 1000, 2000 );  // Find out our new target speed.

 // if (IdleMode == false) //if idle is ON
 // { 
  //  TargetMotorSpeed = map(SetMaxSpeed, 0, 100, 1000, 2000);
 // }
 // else
  //{
 //   TargetMotorSpeed = map(SetMaxSpeed, 0, 100, 1000, 2000);
  //}
  LastSetMaxSpeed = SetMaxSpeed;

  //Serial.println(" (Process Speed Control) SetMaxSpeed");
  //Serial.println(SetMaxSpeed);


}


//----------------------------------------------- PROCESS MAIN MOTORS --------------------------------------------------------------
// Update the motors with the new speed
void ProcessMainMotors()
{
  static int PreviousMotorSpeed = MinMotorSpeed;
  int CurrentMotorSpeed2 = CurrentMotorSpeed;
  int MaxMotorSpeed2 = MaxMotorSpeed;
  int Amount2Add2;

  // --- calculate Stage2 speed based on percentage diferance
  float PercentDif = (MotorSpeed2Full - MotorSpeedFull);        // calculate differanc between Stage 1 And Stage 2 power settings in config
  PercentDif = PercentDif * 0.01 ;                              // convert to a percentage, easier to work in floatingpoint, else stac overflow
  float Amount2Add = CurrentMotorSpeed * PercentDif ;           // Calculate the percentage to add to stage2 speed
  Amount2Add2 = int (Amount2Add);                               // convert to interga
  CurrentMotorSpeed2 = CurrentMotorSpeed + Amount2Add2;         // <==== add value to stage2

  //calculate Stage2 max speed based on percentage differance
  Amount2Add = MaxMotorSpeed * PercentDif ;
  Amount2Add2 = int (Amount2Add);                               // convert to interga
  MaxMotorSpeed2 = MaxMotorSpeed + Amount2Add2;                 // <==== add value to stage2

  if ( PreviousMotorSpeed != CurrentMotorSpeed )
  {
    if ( CurrentMotorSpeed > MaxMotorSpeed )
    {
      UpdatePWM( MaxMotorSpeed, MaxMotorSpeed2 );
    }
    else
    {
      if ( CurrentMotorSpeed == 1000) {                         //trap 1000 = 0 zero speed so set Stage2 the same
        CurrentMotorSpeed2 = 1000;
      }
      UpdatePWM( CurrentMotorSpeed, CurrentMotorSpeed2 );
      //Serial.print("CurrentMotorSpeed = " );
      //Serial.println(CurrentMotorSpeed);
      //Serial.print("CurrentMotorSpeed2 = " );
      //Serial.println(CurrentMotorSpeed2);
    }
    PreviousMotorSpeed = CurrentMotorSpeed;
  }
}


//----------------------------------------------- PROCESS BUTTON Trigger ------------------------------------------------------------------
void ProcessButtons()
{
  // Button Controls
  //--- Trigger ----
  if ( TriggerButtonState == BTN_FELL )     //= on
  {
    StartNewFiringSequence = true;
    SET_TRACER_D_ON;
  }
  else if ( TriggerButtonState == BTN_ROSE ) // = off
  {
    if ( RunFiringSequence )
    {
      ShutdownFiringSequence = true;
    }
    digitalWrite( PIN_TRACER, LOW );
  }

  // ---- Rev_SW -----
#ifdef EN_REV_SW
  if ( RevSwButtonState == BTN_LOW )     //= on
  {
    SET_TRACER_D_ON;
  }
  else if ( RevSwButtonState == BTN_HIGH && ShutdownFiringSequence == true) // = off
  {
    digitalWrite( PIN_TRACER, LOW );
  }
#endif
}


// ------------------------------------------------ PROCESS MAG RELEAASE -----------------------------------------------------------
void ProcessMagRelease()
{
  //volatile bool MagSwEnable = (AmmoCounter == AmmoCtrON); //To process magswenable based on AmmoCounter and AmmoCtrON in the magrelease loop so it updates before it checks
  static bool LastMagReleasePressed = false;
  bool MagReleasePressed = ((MagOutButtonState == BTN_LOW ) || (MagOutButtonState == BTN_FELL) );
  if ( LastMagReleasePressed != MagReleasePressed ) // Detect a change in the status quo
  {
    if ( MagReleasePressed )  // A mag has been inserted
    {
      DartsInMag = MagSize;
    }
    else                      // Mag has been dropped
    {
      DartsInMag = 0;
      //JamDetected = false;
    }
  }
  LastMagReleasePressed = MagReleasePressed;

  if ( MagSwEnable == false ) (DartsInMag = 0);       // set ammo counter zero if no Mag dect switch

}


// -----------------------------------------------PROCESS DISPLAY ------------------------------------------------------------------
void ProcessDisplay()
{
  static int LastSystemMode = 99;
  bool ClearScreen = false;

  if ( LastSystemMode != SystemMode )
  {
    ClearScreen = true;
    LastSystemMode = SystemMode;
  }

  Display_ScreenHeader( ClearScreen );
  switch ( SystemMode )
  {
    case SYSTEM_MODE_MAGOUT:
      Display_MagOut( ClearScreen );
      break;
    case SYSTEM_MODE_CONFIG:
      Display_Config( ClearScreen );
      break;
    case SYSTEM_MODE_LOWBATT:
      Display_LowBatt( ClearScreen );
      break;
    //case SYSTEM_MODE_JAM:
    //  Display_Jam( ClearScreen );
    //  break;
    default:
      Display_Normal( ClearScreen );
      break;
  }
}


// -------------------------------------------- DISPLAY MAG OUT SCREEN -------------------------------------------------------------
void Display_MagOut( bool ClearScreen )
{
  if ( ClearScreen )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 2);
    Oled.print( F("################\n") );
    Oled.print( F("# MAG DROPPED! #\n") );
    Oled.print( F("################") );
  }
}

//---------------------------------------------- DISPALY DEFUALT SCREEN ------------------------------------------------------------
void Display_Normal( bool ClearScreen )
{
  char Buffer[4];
  static int LastDartsInMag = 99;
  static byte LastTargetFireMode = 99;
  static byte LastDisplayDPS = 199;
  static byte LastMotorSpeedFull = 199;
  static byte LastBurstSize = 99;
  static unsigned long LastRefresh = 0;
  if ( ClearScreen || (TargetFireMode != LastTargetFireMode) || (BurstSize != LastBurstSize) )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 2);
    if ( TargetFireMode == FIRE_MODE_SINGLE )
    {
      Oled.print( F("Single    ") );
    }
    else if ( TargetFireMode == FIRE_MODE_AUTO )
    {
      Oled.print( F("Full Auto ") );
    }
    else if ( TargetFireMode == FIRE_MODE_SAFE )
    {
      Oled.print( F("** SAFE **") );
    }
    else
    {
      sprintf( Buffer, "%2d", BurstSize );
      Oled.print( F("Burst: ") );
      Oled.print( Buffer );
    }
  }

  byte DisplayDPS = 0;
  switch ( TargetFireMode )
  {
    case FIRE_MODE_BURST:
      DisplayDPS = ROFAdjustmentB;
      break;
    case FIRE_MODE_AUTO:
      DisplayDPS = ROFAdjustmentA;
      break;
    default:
      DisplayDPS = 0;   // Single should show MAX
  }

  if ( ClearScreen || (DisplayDPS != LastDisplayDPS) )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 4);
    Oled.print( F("ROF: ") );
    if ( DisplayDPS == 0 )
      Oled.print( F("MAX") );
    else
    {
      sprintf( Buffer, "%2d ", DisplayDPS );
      Oled.print( Buffer );
    }
  }

  if ( ClearScreen || (MotorSpeedFull != LastMotorSpeedFull) )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 6);
    Oled.print( F("Pwr:") );
    sprintf( Buffer, "%3d", MotorSpeedFull );
    Oled.print( Buffer );
    Oled.print( F("%") );
  }

  if ( ClearScreen || (DartsInMag != LastDartsInMag) )
  {
    //Don't the screen if it's firing and the pusher is in the return stroke, unless the screen is refreshed.
    if ( ((UpdateAmmoCounter) || (!RunFiringSequence)) || (ClearScreen) )
    {
      LastRefresh = millis();
      Oled.setFont(ZevvPeep8x16);
      Oled.setCursor(90, 3);
      Oled.set2X();
      sprintf( Buffer, "%2d", DartsInMag );
      Oled.print( Buffer );
      Oled.set1X();
      LastDartsInMag = DartsInMag;
      UpdateAmmoCounter = false;
    }
  }

  LastBurstSize = BurstSize;
  LastDisplayDPS = DisplayDPS;
  LastMotorSpeedFull = MotorSpeedFull;
  LastTargetFireMode = TargetFireMode;
}


// ------------------------------------------------------DISPLAY SCREEN HEADER -----------------------------------------------------
void Display_ScreenHeader( bool ClearScreen )
{
  static float LastBatteryVoltage = 99.0;
  static byte LastSec = 99;
  char Buffer[21];

  if ( ClearScreen )
  {
    Oled.clear();
  }
  // Do not clear screen, voltage has changed
  if ( ClearScreen || ( (int)(LastBatteryVoltage * 10) != (int)(BatteryCurrentVoltage * 10) ) )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 0);
    sprintf( Buffer, "%3d", (int)(BatteryCurrentVoltage * 10) );
    Buffer[4] = 0;
    Buffer[3] = Buffer[2];
    Buffer[2] = '.';
    Oled.print( Buffer );
    Oled.print( F("v ") );
  }

  if ( (SystemMode != SYSTEM_MODE_CONFIG) && (ClearScreen || (LastSec != sec)  ))
  {
    Oled.setCursor( 65, 0 );
    Oled.setFont(ZevvPeep8x16);
    sprintf(Buffer, "%02d:%02d:%02d", hrs, min, sec);
    Oled.print( Buffer );
  }
  LastBatteryVoltage = BatteryCurrentVoltage;
  LastSec = sec;
}

//---------------------------------------------- DISPLAY LOW BATTERY ---------------------------------------------------------------
void Display_LowBatt( bool ClearScreen )
{
  if ( ClearScreen )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 2);
    Oled.print( F("################\n") );
    Oled.print( F("# LOW BATTERY! #\n") );
    Oled.print( F("################") );
  }
}

//----------------------------------------------- DISPLAY JAM SCREEN ---------------------------------------------------------------
/*
  void Display_Jam( bool ClearScreen )
  {
  if( ClearScreen )
  {
    Oled.setFont(ZevvPeep8x16);
    Oled.setCursor(0, 2);
    Oled.print( F("################\n") );
    Oled.print( F("# JAM DETECTED #\n") );
    Oled.print( F("################") );
  }
  }
*/
//------------------------------------ Keep tabs on the battery --------------------------------------------------------------------
void ProcessBatteryMonitor()
{

#define BM_STATUS_IDLE 0
#define BM_STATUS_READING 1
#define BM_STATUS_READY 2

  static byte CurrentStatus = 0;
  static unsigned long LastCheck = 0;

  // Every 500ms, start a background ADC read
  if ( CurrentStatus == BM_STATUS_IDLE )
  {
    if ( millis() - LastCheck < 500 )
    {
      return;
    }

    LastCheck = millis();
    CurrentStatus = BM_STATUS_READING;
    ADCSRA |= (1 << ADSC); // Run the ADC
    return;
  }

  // When the ADC has finished it's conversion, proceed to the processing step
  if ( CurrentStatus == BM_STATUS_READING )
  {
    if ( !ADCRead )
    {
      return; // Nothing to see here
    }
    ADCRead = false;
    CurrentStatus = BM_STATUS_READY;
  }


  if ( CurrentStatus != BM_STATUS_READY )
    return;

#define NUM_SAMPLES 6
  static byte CollectedSamples = 0;
  static float SampleAverage = 0;
  int BatteryOffsetfloat = 0;

  if ( CollectedSamples < NUM_SAMPLES )
  {
    CollectedSamples ++;
    SampleAverage += (float)ADCValue;
  }
  else
  {
    float BatOffsetFloat = BatteryOffset * 0.1;
    BatteryCurrentVoltage = (((float)SampleAverage / (float)CollectedSamples * 5.0)  / 1024.0 * (float)((47.0 + 10.0) / 10.0)) + BatOffsetFloat;  // Voltage dividor - 47k and 10k
    if ( BatteryCurrentVoltage < BatteryMinVoltage )
    {
      if ( BatteryCurrentVoltage > 1.6 )       // If the current voltage is 0, we are probably debugging
      {
        BatteryFlat = true;
      }
      else
      {
        BatteryFlat = false;
      }
    }
    else
    {
      BatteryFlat = false;
    }
    CollectedSamples = 0;
    SampleAverage = 0;
  }

  // Reset back to the default position.
  LastCheck = millis();
  CurrentStatus = BM_STATUS_IDLE;
}

//---------------------------------- Game Mode Timer - Count Down Timer------------------------------------------------------------
void ProcessGameTimer()
{
  if ((GameTime > 0) && (GameTimerRun == false) && (GameTimerStart == true))      // not running so start GameTimer, has come out of config menu, via EXIT
  {
    GameTimerRun = true;                                                          // Start Game Timer
    GameTimerStart = false;                                                       // Reset Game Timer Start flag
    GameTimer = (GameTime * 60);                                                  // load game time into timer , convert to seconds
  }
  if ( (SystemMode != SYSTEM_MODE_CONFIG) && ((millis() - LastGTimerMillis) > ONESEC) && (GameTimerRun == true))      // 1 second timer trap
  {
    GameTimer --;                                                               // Dec game timer 1 second

    if (GameTimer < (TimerAlarm * 60))                                          // less than 1 minute flash display at 1 sec interval
      if (DisplayInvert == false)
      {
        DisplayInvert = true;
        Oled.invertDisplay(!(2 % 2));                                           // invert display (White)
      }
      else
      {
        DisplayInvert = false;
        Oled.invertDisplay(!(1 % 2));                                           //  display normal (Black)
      }

    if ((GameTimer == 0) || (GameTime == 0))
    {
      Oled.invertDisplay(!(1 % 2));                                             // display normal, (Black)
      GameTimerRun = false ;                                                    // Stop timer
      GameTimer = 0;
    }

    // convert GameTimer to diplay format HH:MM:SS
    sec = GameTimer % 60;                                                       // Get Seconds out
    min = GameTimer / 60;                                                       // Get Minutes out
    hrs = ((GameTimer / 60) / 60);                                              // Get Hours out
    min = min % 60;                                                             // Tuncate
    hrs = hrs % 24;                                                             // Tuncate haha complex shit....

    LastGTimerMillis = millis();                                                // save the interval trap time
  }
}

//----------------------------------- DIPSPLAY Config Screen and write values to EEPROM -------------------------------------------
void Display_Config( bool ClearScreen )
{
  static byte CurrentMenuItem = 0;
  static byte LastMenuItem = 99;
  static byte DisplayedProfile = 3;
  static bool InConfig = false;
  char Cursor;
  if ( EnteringConfig )
  {
    CurrentMenuItem = 0;
    LastMenuItem = 99;
    UpdateMenuItems();
    InConfig = false;
  }
  if ( DisplayedProfile != CurrentProfile )
  {
    DisplayedProfile = CurrentProfile;
    LastMenuItem = 99;
  }
  if ( ClearScreen || (CurrentMenuItem != LastMenuItem) )  //key
  {
    MenuItem* DisplayList[3];
    byte CursorItem = 0;
    if ( CurrentMenuItem == 0 )
    {
      DisplayList[0] = MenuItems[0];
      DisplayList[1] = MenuItems[1];
      DisplayList[2] = MenuItems[2];
      CursorItem = 0;
    }
    else if ( CurrentMenuItem == (MENU_ITEMS - 1) )
    {
      DisplayList[0] = MenuItems[MENU_ITEMS - 3];
      DisplayList[1] = MenuItems[MENU_ITEMS - 2];
      DisplayList[2] = MenuItems[MENU_ITEMS - 1];
      CursorItem = 2;
    }
    else
    {
      DisplayList[0] = MenuItems[CurrentMenuItem - 1];
      DisplayList[1] = MenuItems[CurrentMenuItem];
      DisplayList[2] = MenuItems[CurrentMenuItem + 1];
      CursorItem = 1;
    }

    Oled.setFont(ZevvPeep8x16);

    if ( InConfig )
      Cursor = '#';
    else
      Cursor = '>';

    Oled.setCursor(0, 2);
    DisplayList[0]->PrepareOutput();
    if ( CursorItem == 0 )
      Oled.print( Cursor );
    else
      Oled.print( ' ' );
    Oled.print( DisplayList[0]->Title );
    Oled.print( ' ' );
    Oled.print( DisplayList[0]->Output );
    Oled.clearToEOL();

    Oled.setCursor(0, 4);
    DisplayList[1]->PrepareOutput();
    if ( CursorItem == 1 )
      Oled.print( Cursor );
    else
      Oled.print( ' ' );
    Oled.print( DisplayList[1]->Title );
    Oled.print( ' ' );
    Oled.print( DisplayList[1]->Output );
    Oled.clearToEOL();

    Oled.setCursor(0, 6);
    DisplayList[2]->PrepareOutput();
    if ( CursorItem == 2 )
      Oled.print( Cursor );
    else
      Oled.print( ' ' );
    Oled.print( DisplayList[2]->Title );
    Oled.print( ' ' );
    Oled.print( DisplayList[2]->Output );
    Oled.clearToEOL();

    LastMenuItem = CurrentMenuItem;
  }
  // --------------------------------------------
  // Rotary here
  if ( RotaryTurned == ROTARY_CW )
  {
    if ( InConfig )
    {
      MenuItems[CurrentMenuItem]->Increase();
      LastMenuItem = 99;
    }
    else
    {
      if ( CurrentMenuItem < (MENU_ITEMS - 1) )
      {
        CurrentMenuItem ++;
      }
      else                                      // go back to beginig of menu
      {
        CurrentMenuItem = 0;
      }
    }
  }
  if ( RotaryTurned == ROTARY_CCW )
  {
    if ( InConfig )
    {
      MenuItems[CurrentMenuItem]->Decrease();
      LastMenuItem = 99;
    }
    else
    {
      if ( CurrentMenuItem > 0  )
      {
        CurrentMenuItem --;
      }
      else                                  // go to bottom of menu
      {
        CurrentMenuItem = (MENU_ITEMS - 1);
      }
    }
  }

  if ( (ConsoleRotaryButtonAction == BTN_ACTION_SHORT) && !EnteringConfig )
  {
    switch ( CurrentMenuItem )
    {
      case 0:                                      //exit request at begining of menu options
        SystemMode == SYSTEM_MODE_NORMAL;
        ExitingConfig = true;
        EnteringConfig = false;
        if (GameTime > 0)                          // set game time start flag on exit
        {
          GameTimerStart = true;
          GameTimer = GameTime;
          GameTimerRun == false;
        }
        break;
      case 19:                                   //exit request at end of menu options
        SystemMode == SYSTEM_MODE_NORMAL;
        ExitingConfig = true;
        EnteringConfig = false;
        if (GameTime > 0)                          // set game time start flag on exit
        {
          GameTimerStart = true;
          GameTimer = GameTime;
          GameTimerRun == false;
        }
        break;
      default:
        // Handle config stuff here
        InConfig = !InConfig;
        if ( !InConfig )
        {
          switch ( CurrentMenuItem )
          {
            case 1:
              GameTime = MenuItems[1]->CurrentValue;
              Profiles[CurrentProfile].GameTime = GameTime;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_GT, GameTime );
              break;
            case 2:
              MotorSpeedFull = MenuItems[2]->CurrentValue;
              Motor2RangeTest();
              Profiles[CurrentProfile].MotorSpeedFull = MotorSpeedFull;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_MSF, MotorSpeedFull );
              break;
            case 3:
              MotorSpeed2Full = MenuItems[3]->CurrentValue;
              Motor2RangeTest();
              Profiles[CurrentProfile].MotorSpeed2Full = MotorSpeed2Full;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_MSF2, MotorSpeed2Full );
              break;
            case 4:
              ROFAdjustmentA = MenuItems[4]->CurrentValue;
              Profiles[CurrentProfile].ROFAdjustmentA = ROFAdjustmentA;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_ROFA, ROFAdjustmentA );
              break;
            case 5:
              ROFAdjustmentB = MenuItems[5]->CurrentValue;
              Profiles[CurrentProfile].ROFAdjustmentB = ROFAdjustmentB;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_ROFB, ROFAdjustmentB );
              break;
            case 6:
              BurstSize = MenuItems[6]->CurrentValue;
              Profiles[CurrentProfile].BurstSize = BurstSize;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_BURST, BurstSize );
              break;
            case 7:
              MagSize = MenuItems[7]->CurrentValue;
              Profiles[CurrentProfile].MagSize = MagSize;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_MAGSIZE, MagSize );
              break;
            case 8:
              AccelerateTime = MenuItems[8]->CurrentValue;
              Profiles[CurrentProfile].AccelerateTime = AccelerateTime;
              EEPROM.put( (CurrentProfile * ADDR_PROB_BASE) + ADDR_ACCEL, AccelerateTime );
              CalculateRampRates(); // Recalculate the ramp rates
              break;
            case 9:
              DecelerateTime = MenuItems[9]->CurrentValue;
              Profiles[CurrentProfile].DecelerateTime = DecelerateTime;
              EEPROM.put( (CurrentProfile * ADDR_PROB_BASE) + ADDR_DECEL, DecelerateTime );
              CalculateRampRates(); // Recalculate the ramp rates
              break;
            case 10:
              MotorStartDwellTime = MenuItems[10]->CurrentValue;
              Profiles[CurrentProfile].MotorStartDwellTime = MotorStartDwellTime;
              EEPROM.put( (CurrentProfile * ADDR_PROB_BASE) + ADDR_STARTD, MotorStartDwellTime );
              break;
            case 11:
              MotorStopDwellTime = MenuItems[11]->CurrentValue;
              Profiles[CurrentProfile].MotorStopDwellTime = MotorStopDwellTime;
              EEPROM.put( (CurrentProfile * ADDR_PROB_BASE) + ADDR_STOPD, MotorStopDwellTime );
              break;
            case 12:
              PulseOnTimeHigh = MenuItems[12]->CurrentValue;
              Profiles[CurrentProfile].PulseOnTimeHigh = PulseOnTimeHigh;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_PULSE_HIGH, PulseOnTimeHigh );
              break;
            case 13:
              PulseOnTimeLow = MenuItems[13]->CurrentValue;
              Profiles[CurrentProfile].PulseOnTimeLow = PulseOnTimeLow;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_PULSE_LOW, PulseOnTimeLow );
              break;
            case 14:
              PulseRetractTime = MenuItems[14]->CurrentValue;
              Profiles[CurrentProfile].PulseRetractTime = PulseRetractTime;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_PULSE_RETRACT, PulseRetractTime );
              break;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_PULSE_RETRACT, PulseRetractTime );
              break;
            case 15:
              BatteyType = MenuItems[15]->CurrentValue;
              UpdateBatteryType();
              Profiles[CurrentProfile].BatteyType = BatteyType;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_BAT_TYPE, BatteyType );
              break;
            case 16:
              IdleSpeed = MenuItems[16]->CurrentValue;
              Profiles[CurrentProfile].IdleSpeed = IdleSpeed;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_IDLE_SPEED, IdleSpeed );
              break;
            case 17:
              ROF_RampTime = MenuItems[17]->CurrentValue;
              Profiles[CurrentProfile].ROF_RampTime = ROF_RampTime;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_ROF_TM, ROF_RampTime );
              break;
            case 18:
              MagSwEnable = MenuItems[18]->CurrentValue;
              Profiles[CurrentProfile].MagSwEnable = MagSwEnable;
              EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_AMMOCTR, MagSwEnable );
              Serial.println("MagSwEnable Saved");
              break;
          }
        }
        LastMenuItem = 99;
        break;
    }
  }
  if ( EnteringConfig )
    EnteringConfig = false;
}


// -------------------------------- Stage 2 Flywheeels Range Test result value from menu -------------------------------------------
void Motor2RangeTest()
{
  if ( MotorSpeed2Full < MotorSpeedFull )         //if less than Stage 1 then equals stage 1
  {
    MotorSpeed2Full = MotorSpeedFull ;
    UpdateMenuItems();
    Profiles[CurrentProfile].MotorSpeedFull = MotorSpeedFull;
    EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_MSF, MotorSpeedFull );
    Profiles[CurrentProfile].MotorSpeed2Full = MotorSpeed2Full;
    EEPROM.write( (CurrentProfile * ADDR_PROB_BASE) + ADDR_MSF2, MotorSpeed2Full );
  }
}

// -------------------------------- If battery Type is changed these constantes need to change -------------------------------------
void UpdateBatteryType()
{

  if ( BatteyType == BATTERY_3S)
  {
    BatteryMinVoltage = BATTERY_3S_MIN;
    BatteryMaxVoltage = BATTERY_3S_MAX;
    SolenoidLowVoltage = (int)(BATTERY_3S_MIN * 10.0);
    SolenoidHighVoltage = (int)(BATTERY_3S_MAX * 10.0);
  }
  if (BatteyType == BATTERY_4S)
  {
    BatteryMinVoltage = BATTERY_4S_MIN;
    BatteryMaxVoltage = BATTERY_4S_MAX;
    SolenoidLowVoltage = (int)(BATTERY_4S_MIN * 10.0);
    SolenoidHighVoltage = (int)(BATTERY_4S_MAX * 10.0);
  }
  /*
    Serial.println ( BatteyType );
    Serial.println ( BatteryMinVoltage );
    Serial.println ( BatteryMaxVoltage );
    Serial.println ( SolenoidLowVoltage );
    Serial.println ( SolenoidHighVoltage );
  */
}

// ---------------------------------------------------------------------------------------------------------------------------------
void UpdateMenuItems()
{
  // MenuItems [0] is EXIT - Ignore
  MenuItems[1]->CurrentValue = GameTime;
  MenuItems[2]->CurrentValue = MotorSpeedFull;
  MenuItems[3]->CurrentValue = MotorSpeed2Full;
  MenuItems[4]->CurrentValue = ROFAdjustmentA;
  MenuItems[5]->CurrentValue = ROFAdjustmentB;
  MenuItems[6]->CurrentValue = BurstSize;
  MenuItems[7]->CurrentValue = MagSize;
  MenuItems[8]->CurrentValue = AccelerateTime;         // Ramp U
  MenuItems[9]->CurrentValue = DecelerateTime;         // Ramp D
  MenuItems[10]->CurrentValue = MotorStartDwellTime;    // Dwel U
  MenuItems[11]->CurrentValue = MotorStopDwellTime;    // Dwel D
  MenuItems[12]->CurrentValue = PulseOnTimeHigh;
  MenuItems[13]->CurrentValue = PulseOnTimeLow;
  MenuItems[14]->CurrentValue = PulseRetractTime;
  MenuItems[15]->CurrentValue = BatteyType;
  MenuItems[16]->CurrentValue = IdleSpeed;
  MenuItems[17]->CurrentValue = ROF_RampTime;
  MenuItems[18]->CurrentValue = MagSwEnable;
}

//--------------------------------------------------------------- only called at start up ------------------------------------------
void UpdateProfile()
{
  GameTime = Profiles[CurrentProfile].GameTime;
  MotorSpeedFull = Profiles[CurrentProfile].MotorSpeedFull;
  MotorSpeed2Full = Profiles[CurrentProfile].MotorSpeed2Full;
  ROFAdjustmentA = Profiles[CurrentProfile].ROFAdjustmentA;
  ROFAdjustmentB = Profiles[CurrentProfile].ROFAdjustmentB;
  BurstSize = Profiles[CurrentProfile].BurstSize;
  MagSize = Profiles[CurrentProfile].MagSize;
  AccelerateTime = Profiles[CurrentProfile].AccelerateTime;
  DecelerateTime = Profiles[CurrentProfile].DecelerateTime;
  MotorStartDwellTime = Profiles[CurrentProfile].MotorStartDwellTime;
  MotorStopDwellTime = Profiles[CurrentProfile].MotorStopDwellTime;
  PulseOnTimeHigh = Profiles[CurrentProfile].PulseOnTimeHigh;
  PulseOnTimeLow = Profiles[CurrentProfile].PulseOnTimeLow;
  PulseRetractTime = Profiles[CurrentProfile].PulseRetractTime;
  BatteyType = Profiles[CurrentProfile].BatteyType;
  IdleSpeed = Profiles[CurrentProfile].IdleSpeed;
  ROF_RampTime = Profiles[CurrentProfile].ROF_RampTime;
  MagSwEnable = Profiles[CurrentProfile].MagSwEnable;

  CalculateRampRates();                         // Recalculate the ramp rates
  UpdateBatteryType();                          // Not need but why not
  GameTimer = GameTime;
}
