/*************************************************************************
Title:    Milight / LimitlessLED LED Strip Network Library for Arduino
Authors:  Nathan Holmes <maverick@drgw.net>, Colorado, USA
          Michael Petersen <railfan@drgw.net>, Colorado, USA
		  Koen van der kruk <koenvdkruk@gmail.com>, Utrecht, Netherlands
File:     Milight.h
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2015 Nathan Holmes
    
    The latest source can be obtained from ISE's Github repository here:
	Arduino: https://github.com/IowaScaledEngineering/Milight
	ESP8266: https://github.com/koen-github/MiLight-Controller-ESP8266

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License along
    with this program. If not, see http://www.gnu.org/licenses/
    
*************************************************************************/

#ifndef _MILIGHT_H_
#define _MILIGHT_H_

#include <SPI.h>
#include <WiFiUDP.h>


#define MILIGHT_QUEUE_DEPTH 64
#define MILIGHT_COLOR_WHITE 0x00

#define MILIGHT_COLOR_BLUE    0x01
#define MILIGHT_COLOR_AQUA    0x20
#define MILIGHT_COLOR_CYAN    0x40
#define MILIGHT_COLOR_MINT    0x50
#define MILIGHT_COLOR_GREEN   0x60
#define MILIGHT_COLOR_LIME    0x7E
#define MILIGHT_COLOR_YELLOW  0x90
#define MILIGHT_COLOR_ORANGE  0xA0
#define MILIGHT_COLOR_RED     0xAE
#define MILIGHT_COLOR_PINK    0xC0
#define MILIGHT_COLOR_FUCHSIA 0xD0
#define MILIGHT_COLOR_PURPLE  0xF0

//#define WHITE_ALLOFF				0x39
//#define WHITE_ALLON					0x35
//#define WHITE_BRIGHTNESS_UP			0x3C
//#define WHITE_BRIGHTNESS_DOWN		0x34
//#define WHITE_WARM_WHITE_INCREASE	0x3E
//#define WHITE_COOL_WHITE_INCREASE	0x3F
//#define WHITE_GROUP_1_ALL_ON		0x38
//#define WHITE_GROUP_1_ALL_OFF		0x3B
//#define WHITE_GROUP_2_ALL_ON		0x3D
//#define WHITE_GROUP_2_ALL_OFF		0x33
//#define WHITE_GROUP_3_ALL_ON		0x37
//#define WHITE_GROUP_3_ALL_OFF		0x3A
//#define WHITE_GROUP_4_ALL_ON		0x32
//#define WHITE_GROUP_4_ALL_OFF		0x36
//#define WHITE_NIGHT_MODE_ALL						0x39  //100ms followed by: 0xB9
//#define WHITE_NIGHT_MODE_ALL_PRESSAndHOLD			0xBB
//#define WHITE_NIGHT_SAVER_MODE_GROUP_1 				0x3B //100ms followed by: 0xBB
//#define WHITE_NIGHT_SAVER_MODE_GROUP_1_PRESSAndHOLD 0xBB
//#define WHITE_NIGHT_SAVER_MODE_GROUP_2 				0x33 //100ms followed by: 0xB3
//#define WHITE_NIGHT_SAVER_MODE_GROUP_2_PRESSAndHOLD 0xB3
//#define WHITE_NIGHT_SAVER_MODE_GROUP_3 				0x3A //100ms followed by: 0xBA
//#define WHITE_NIGHT_SAVER_MODE_GROUP_3_PRESSAndHOLD 0xBA
//#define WHITE_NIGHT_SAVER_MODE_GROUP_4 				0x36 //100ms followed by: 0xB6
//#define WHITE_NIGHT_SAVER_MODE_GROUP_4_PRESSAndHOLD 0xB6
//#define WHITE_FULL_BRIGHTNESS_ALL 					0x35 //100ms followed by: 0xB5
//#define WHITE_FULL_BRIGHTNESS_ALL_PRESSAndHOLD 		0xB5
//#define WHITE_FULL_BRIGHTNESS_GROUP_1 				0x38  //100ms followed by: 0xB8
//#define WHITE_FULL_BRIGHTNESS_GROUP_1_PRESSAndHOLD 	0xB8
//#define WHITE_FULL_BRIGHTNESS_GROUP_2 				0x3D  //100ms followed by: 0xBD
//#define WHITE_FULL_BRIGHTNESS_GROUP_2_PRESSAndHOLD 	0xBD
//#define WHITE_FULL_BRIGHTNESS_GROUP_3 				0x37  //100ms followed by: 0xB7
//#define WHITE_FULL_BRIGHTNESS_GROUP_3_PRESSAndHOLD 	0xB7
//#define WHITE_FULL_BRIGHTNESS_GROUP_4 				0x32  //100ms followed by: 0xB2
//#define WHITE_FULL_BRIGHTNESS_GROUP_4_PRESSAndHOLD 	0xB2

#define MILIGHT_CH_ALL        0
#define MILIGHT_CH_1          1
#define MILIGHT_CH_2          2
#define MILIGHT_CH_3          3
#define MILIGHT_CH_4          4

typedef struct
{
  uint8_t data[2];
  uint8_t repeats;
} MilightCmd;

typedef struct
{
  uint8_t headIdx;
  uint8_t tailIdx;
  bool full;
  MilightCmd pktData[MILIGHT_QUEUE_DEPTH];
} MilightCmdQueue;


class Milight
{
  public:
	uint8_t white_on[5] = {0x35, 0x38, 0x3D, 0x37, 0x32};
	uint8_t white_off[5] = {0x39, 0x3B, 0x33, 0x3A, 0x36};

	uint8_t ww_increase = 0x3E;
	uint8_t cw_increase = 0x3F;
	uint8_t white_brightness_up = 0x3C;
	uint8_t white_brightness_down = 0x34;
	
	uint8_t white_night_mode[5] = {0x39, 0x3B, 0x33, 0x3A, 0x36};
	uint8_t white_night_mode_pressandhold[5] = {0xBB, 0xBB, 0xB3, 0xBA, 0xB6};
	
	uint8_t white_brightness_full[5] = {0x35, 0x38, 0x3D, 0x37, 0x32};
	uint8_t white_brightness_full_pressandhold[5] = {0xB5, 0xB8, 0xBD, 0xB7, 0xB2};
	
    Milight();
    void begin(WiFiUDP * udp, IPAddress * addr, uint16_t port);
    void on(uint8_t channel, uint8_t color, uint8_t intensity);
    void off(uint8_t channel);
    void workQueue();
    uint8_t packetsQueued();
	
	void onWWCW(uint8_t channel);
	void offWWCW(uint8_t channel);
	void brightnessUpWWCW(uint8_t channel);
	void brightnessDownWWCW(uint8_t channel);
    void warmerWWCW(uint8_t channel);
	void coolerWWCW(uint8_t channel);
	void brightnessFullWWCW(uint8_t channel);
	void nightModeWWCW(uint8_t channel);
	
  private:
    IPAddress * destAddr;
    WiFiUDP * udp;
    bool useShortPkt;
    uint16_t port;
    void send(uint8_t cmd1, uint8_t cmd);
    uint32_t lastUpdateMillis;
    MilightCmdQueue queue;
    uint8_t pktRepeats;
    
};

#endif

