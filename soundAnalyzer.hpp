#ifndef SOUNDANALYZER_HPP
#define SOUNDANALYZER_HPP

#include "hwlib.hpp"



/**
 * @class soundAnalyzer
 * @author Olivier
 * @date 20/06/2017
 * @file soundAnalyzer.hpp
 * @brief Uses the ADC to listen to sound and analyzes it's input
 * 
 */
class soundAnalyzer {
private:
    hwlib::target::pin_adc & analogIn;
	unsigned int sample_buffer_size; //131 hz minimum with 100;
public:
/**
 * @brief Init
 * @param analogIn
 * @param sample_buffer_size
 */
	soundAnalyzer(hwlib::target::pin_adc & analogIn, int sample_buffer_size = 100);
	
/**
 * @brief Returns the volume as interger
 * This is just a number to view differance between quiet and loud sounds
 * It's not linked to decibel.
 * @param method
 * Method 0 = Average volume (default)
 * Method 1 = Peak volume (faster but less accurate)
 */
	const int volume(int method = 0);
/**
 * @brief frequency
 * Will detect freqentcy based on the time between the top and the bottom of the sin.
 * 
 * Use a bigger sample buffer size to improve accuracy and
 */
	const int frequency();
/**
 * @brief Small tool to view the analog input 
 * The output has a $ and ; for the plotter 
 * https://developer.mbed.org/users/borislav/notebook/serial-port-plotter/
 */
	void debug();
};

#endif