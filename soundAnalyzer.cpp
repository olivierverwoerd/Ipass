#ifndef IR_HPP
#include "soundAnalyzer.hpp"

	soundAnalyzer::soundAnalyzer(hwlib::target::pin_adc & analogIn, int sample_buffer_size):
	analogIn(analogIn), sample_buffer_size(sample_buffer_size)
	{}
	
	const int soundAnalyzer::volume(int method){
		unsigned int array[sample_buffer_size];
		unsigned int max = 0;
		unsigned int min = 4095; ///ADC Max value
		for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
			hwlib::wait_ms(1);
			array[counter] = analogIn.get();
		}
		
		if (method == 0){ //average
			unsigned int average = 0; /// with max ADC of 4095 sample_buffer_size has to be less than 5244164
			min = 0;
			for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
				average += array[counter];
			}
			average = average / sample_buffer_size;
			
			for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
				if (array[counter] > average){
					max += (array[counter] - average);
				}
				if (array[counter] < average){
					min += (average - array[counter]);
				}
			}
			return (min + max )/2;
		}
		if (method == 1){ //Peak
			
			for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
				if (array[counter] > max){
					max = array[counter];
				}
				if (array[counter] < min){
					min = array[counter];
				}
			}
			return max - min;
		}
	return 0;
	}
	
	const int soundAnalyzer::frequency(){
		const unsigned int minamp = 60; //minimal aplitude
		unsigned int average = 0; /// with max ADC of 4095 sample_buffer_size has to be less than 5244164
		unsigned int array[sample_buffer_size];
		int firstIsTop = -1;
		unsigned int tab[sample_buffer_size]; ///tops and bottoms of each wave
		unsigned int tabIndex = 0;
		
		for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
			array[counter] = analogIn.get();
		}
		for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
			average += array[counter];
		}
		average = average / sample_buffer_size;
		for(unsigned int counter = 2; counter < sample_buffer_size; counter++){
			if (firstIsTop != 1){
				if (array[counter-2] < array[counter-1] && 
				array[counter-1] < array[counter] && 
				array[counter] > array[counter+1] && 
				array[counter+1] > array[counter+2] &&
				array[counter] > average + minamp){
					tab[tabIndex] = counter;
					tabIndex++;
					firstIsTop =1;
				}
			}
			if (firstIsTop != 0 ){
				if (array[counter-2] > array[counter-1] && 
				array[counter-1] > array[counter] && 
				array[counter] < array[counter+1] && 
				array[counter+1] < array[counter+2] &&
				array[counter] < average - minamp){
					tab[tabIndex] = counter;
					tabIndex++;
					firstIsTop = 0;
				}
			}
		}
		average = 0;
		for(unsigned int counter = 1; counter < tabIndex; counter++){
			average += (tab[counter] - tab[counter-1] );
		}
	return average / tabIndex;
	}
	

	
	void soundAnalyzer::debug(){
		int array[sample_buffer_size];
		for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
			array[counter] = analogIn.get();
		}
		hwlib::cout << '$' << 0 <<  ';' << '\n'<< hwlib::flush; /// this debug tool helps to view what the beginning and the end is when plotting by beginning with a zero
		for(unsigned int counter = 0; counter < sample_buffer_size; counter++){
			hwlib::cout << '$' << array[counter] << ';' << '\n'<< hwlib::flush;
		}
	}
#endif
