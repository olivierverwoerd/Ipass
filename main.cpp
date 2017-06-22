/*! \mainpage Ipass
 *
 * \section Ipass Ipass by Olivier Verwoerd
 * This is the documentation of the files I made for the Ipass of the university of applied sciances Utrecht.
 * it's the curiosity of useing analog and different signals that is not perfectly made to be understoud by the arduino.
 * Useing morse code was a perfect way to do this since morse is not perfecty timed. 
 * 
 * \section work What did I do
 * 1. First I made it posible to get morse form a digital signal. 
 * 2. Than i made an dictionary to get char form morse
 * 3. I wanted to make it faster and faster so I optimalized a few things
 * 4. Wanted even faster than sound and i found out morse is still used (sort of) to send bytes in IR remotes and made support for that.
 * 5. Everything above was digital so i wanted to do something analog. To start with volume measureing. 
 * 6. than frequency detection. 
 * 7. I've made some configuration tools in the code to debug and make the script more dynamic
 * 8. I wanted to make use of the frequency to filter and the volume detection the morse code to make it even better. 
 * This didn't work out.
 * Frequency detection only worked with no background noise because it couldn't detect multiple frequencys. It's not a good spectrometer. 
 * This just made the morse detection significant more unreliable.
 * Volume is more unreliable with a lower buffer size and therefore didn't work,
 * volume is too slow with a big buffer. This isn't inposible to implement. I never just got the configuration right and decided to let the hardware do it's work.
 * 9. Well. You are looking at my last thing i did. Doxygen documentation...

 * \section handy Good to know

 * NOTE: Since printing thousands of numbers in an array is simply too much to make any sense to me.
 * i've used this tool https://developer.mbed.org/users/borislav/notebook/serial-port-plotter/
 * A great tool for plotting the array. 
 * To let the tool work there needs to be an $ in front of the number followed by an ;
 * 
 * Because i'm inpactient to do wait for the slow terminal. I used baud rate of 19200 AND NOT 2400. 
 * Change this in makefile.due and in hwlib_ostream change BMPTK_BAUDRATE to 19200.
 * 
 * \section licence licence
 * Copyright Olivier Verwoerd 2017.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * \section dependency dependency
 * It needs this https://github.com/wovo/hwlib
 * and some love otherwise this will never work. 
 */

#include "hwlib.hpp"
#include "morse.hpp"
#include "ir.hpp"
#include "soundAnalyzer.hpp"

/**
 * @brief main
 * Quick demo of what I made and how to use it
 */
int main( void ){  
   WDT->WDT_MR = WDT_MR_WDDIS; // kill the watchdog
   hwlib::wait_ms(500); //prevent cout not ready at start
   
   auto digitalAudioIn = hwlib::target::pin_in( hwlib::target::pins::d2 );  
   auto analogAudioIn = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
   auto digitalIn = hwlib::target::pin_in( hwlib::target::pins::d3 ); //button
   auto digitalIRIn = hwlib::target::pin_in( hwlib::target::pins::d4 ); //IR receiver
   
   
   //defineing dictionay
   morse m(digitalAudioIn, 60);
   morse t(digitalIn, 20);
   ir i(digitalIRIn);
   soundAnalyzer a(analogAudioIn, 1000);
   
   //m.calibrateVolume();
   //m.calibrateWpm();
	for(;;){ //demo
		//i.debug();
		//a.debug();
		//hwlib::cout << a.frequency() << '\n';
		//hwlib::cout << a.volume() << '\n';
		//hwlib::cout << a.volume(1) << '\n';
		
		hwlib::cout << m.listenForMorse();
		//hwlib::cout << i.receive() << '\n';
	}
	
	int command; //demo of ir NEC 
	int counter;
	bool waitForCommand = true;
	for(;;){
		if (waitForCommand == true){
			command = i.receive();
		}
		waitForCommand = true;
		if (command == 65501){
			hwlib::cout << "\nButton left: ";
		}
		else if (command == 65341){
			hwlib::cout << "\nButton right: ";
		}
		else if (command == 65437){
			hwlib::cout << "\nButton up: ";
		}
		else if (command == 65367){
			hwlib::cout << "\nButton down: ";
		}
		else if (command == 65533){
			hwlib::cout << "\nButton fire: ";
		}
		else if (command == 0){
			hwlib::cout << "HOLD ";
			while(command == 0){
				command = i.receive();
				counter++;
				}
			waitForCommand = false;
			hwlib::cout << "for "<< counter << " times";
			counter = 0;
		}
	}
}
