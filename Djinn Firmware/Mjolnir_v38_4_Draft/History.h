 // Credits & History
/* 
 * ------------------------------------------------------------------------ 
 * HISTORY
 * Originals files from:-
 * ------------------------------------------------------------------------ 
 * (c) 2019 Michael Ireland / Ireland Software / Airzone
 * 
 * Creative Commons: Attribution, Non-Commercial
 * Free for Non-Commercial use
 * Files: Nafduino FDL.ini and Caliburn 3.04.ini
 * ------------------------------------------------------------------------
 * v1.0 - Initial release
 * v1.1 - Adjusted the ADC for better throughput, and the screen refresh timing
 * Vx.x Richard Nicholson, from New Zealand. started to play with intention to stitch in Solenoid operation from Caliburn code
 * v2 Org code uses a HW shift reg console for encoder PBs. Encoder and switch working on nano HW, but primitive
 * v3 failed
 * v4 Encoder Working and Encoder Sw, Tidy up and document
 * v5 Another go at Encoder CLK/DT, working better with no INT, Process Econder in ProcessDebounce, yippy
 * v6 MAG Sensor IP working
 * v7 Trigger IP Working
 * v8 Start of Solenoid pusher changes (hardwork)
 * v9 Lot of stuff disableed, try anouther cut in
 * v10 cut in but not quite going
 * v11 life, its firing be it slow. probably due to debug code
 * v12 Working, tidy up and remove old code, add comments
 * V14 PulseOnTime fixed, Burst mode error count fixed
 * V15 Use Long press to get into Config, Short press to get out. Selectfire mode change via short click
 * V16 fix ROF issue, remove debug lines
 * v17 More Tidy Ups and documentation. Flip Screen, cos Thor got it up the wrong way :-), but easy fix and good for later reffferance
 * v18 Configuration menu tidy up and addition of solenoide tuning variables in config menu
 * v19 Tidy up code and comments
 * v20 Battery type select and Battery Offset Calibration added to config menu
 * v22 2nd stage pwm output development.BIOS level working, with dif speeds on ether ESC
 * V23 ESC stage2 control via oled etc, software config flag to flip screen, & mag switch disable
 * V24 Get V23 working properly... tidy ups
 * v25 Get SW Mag Dectetor switch disabled working
 * v26 Add Pusher Brake off, initilization in case narfduino pcb. Bug fix 4s/3s low bat, not going into config, Mag-Sw_Enable in config menu
 * V27 Game Timer dev
 * v28 Game Timer. Set in minutes Count Down. Starts when exit config and if > zero
 * v29 
 * V30
 * v32
 *   -  Define constants and compile time switches - Done
 *   - Encoder PB first hit issue, not switching mode -Fixed
 *   - Bat max min 3s 4s, get working better - Fixed
 *   - Menu go around in circile - Fixed
 *  
 *   SelectFire:-
 *    - Use Push Button Switch across Encoder SW input, 123,123... 
 *    - 3 Position Switch, slide or toggel sw. D2, D3, GND, = 11,01,10  **** Compile time option
 *    - D2 = SelectFire In1   - leave open
 *    - D3 = SelectFire In2   - leave open 
 *    
 *    - Low Flywheel idle speed. Ie 0 rpm = 30rpm = low running. Long trigger to turn on/off in single fire mode
 *    - Tracer dart output when wheels running.  
 *    
 * v33 Compile time swicth for original wiring. Update notes. Fork point v32 used for HolyGrail v34 (Seed XIAO MCU)
 *     - Rev SW connected to Ideal Mode
 * v34 make fully compatable with V1 hardware
 * 
 * v35 Found bug in Pusher Control, allowing run away.fixed
 * v36 Fly wheel speed control via encoder when in normal/run mode
 * 
 * v37 Logo options loaded
 * 
 *   To Do:-
 *    - Ramp Up ROF when trigger held, when in full auto if Menu: ROF Ramp time>0, else normal full auto

 *v38 - OFD/dpairsoft revisions 
 -fixed response delay in single fire b/c PINC was on the wrong pin
 -reduced motor dwell timer to 80ms - can add time in settings menu
 -increased window of pressing encoder to get into the menu to make it easier
 -TBD: 
 -idle mode fix (in idle mode it would increase target speed)
 -Being able to turn the mag switch on/off from settings
 -Being able to change the default encoder functionality on the main screen from fps to rof or mag size or off completely so you don’t accidentally change anything
 -Binary trigger mode

 *   
 */
