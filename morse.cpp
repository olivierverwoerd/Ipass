#ifndef MORSE_HPP
#include "morse.hpp" 
  
    morse::morse(hwlib::pin_in & digitalIn, int wpm, bool lastWasSpace, bool lastWasSlash):
    digitalIn(digitalIn), wpm(wpm), lastWasSpace(lastWasSpace), lastWasSlash(lastWasSlash)
    {}
    
    
    char morse::listenForMorse(){
    int unit = (600)/wpm;  ///If you change the device the clock is different and the constant devided by wpm has to be canged. 
	int unit1min = unit *0.8; ///the unit is given a min and a max that has a 20% tollerance. This is stable at different wpm.
	int unit1max = unit *1.2;
	
	for (int counter = wpm; counter > 20; counter = counter - 10){
		unit1min = unit1min *0.9;
		unit1max = unit1max *1.1;
		}
	int unit3min = unit1min * 3; /// If the unit differs form the normal. like in my case change the 
	int unit3max = unit1max * 3;
	int unitspacemin = unit1min * 6; ///Change unitspace and unitslash if the calibrate wpm notice these are wrong (Change only the number afther the * )
	int unitspacemax = unit1max * 6; ///This is NOT a bug it's called Farnsworth Speed and it differs. This is normal
	int unitslashmin = unit1min * 14; //not in use the morse code program had a longer time for space (14 units). Higher than unit max is used instead
	//there is no need for a slash max since it would be an / anyway.

	
	
	if (lastWasSlash == true){
		if (lastWasSpace == false){
			lastWasSpace = true;
			return '/';
			}
		else {
			lastWasSlash = false;
			return ' ';
		}
	}
	while(true){
		if (lastWasSpace == true){
		while (digitalIn.get() == 0){} /// The program waits for signal if the last char was a space to prevent doubble spaces
		}
		
		int counter;
		int counterHigh = 0;
		int counterLow = 0;
		while(true){
			counter = 0;
			hwlib::wait_ms(1);
			for(int x=0; x < 20; x++){
				counter += digitalIn.get();
			}
			
			if (counter > 1){
				counterHigh++;
				if (lastWasSpace == false){
					if (counterLow >= unitslashmin){
						lastWasSlash = true;
						return ' ';
					}
					if (counterLow >= unitspacemin && counterLow <= unitspacemax){ /// the program i used for morse https://morsecode.scphillips.com/translator.html uses a 5 unit space instead of the normal 3 unit. 
						lastWasSpace = true;
						lastWasSlash = false;
						return ' ';
						}
				}
				counterLow = 0;
				}
			else {
				counterLow++;
				if (counterLow > unit1min/2){ // small error prevention
					if (counterHigh >= unit3min && counterHigh <= unit3max){
						lastWasSpace = false;
						lastWasSlash = false;
						return '-';
					}
					
					if (counterHigh >= unit1min && counterHigh <= unit1max){
						lastWasSpace = false;
						lastWasSlash = false;
						return  '.';
					}
				counterHigh = 0;
				}
				if (counterLow > unitslashmin*2){ ///If theres nothing at the end this will enter a space (" / ")
					if (lastWasSpace == false){
						lastWasSpace = false;
						lastWasSlash = true;
						return ' ';
						}
					else{
						break;
						}
				}
			}
		}
	}
	return '?';
}


char morse::listenForMorseChar(){
	char array[7] = {0,0,0,0,0,0,'\0'};
	for (int item = 0; item < 7; item++){
		array[item] = listenForMorse();
		if (array[item] == ' '){
			array[item] = 0;
			break;
		if (array[item] == '/'){
			return ' ';
		}
	}
	}
	return morseToChar(array);
}

int morse::calibrateWpm(){
	const int timersToWpm = 550;
	
	bool noise = false;

	int n1 = 0; ///timers for the 
	int n2 = 0;
	int n3 = 0;
	int slash = 0;
	int t = 0;
	int space = 0;
	int e = 0;
	hwlib::cout << "Calibration started. Make sure there is NO backgound noise.\nIf the result is different from what you send\nPlease adjust volume until correct\n\nPlease wait...\n";
	for (int time = 0; time < 1000; time++){
		hwlib::wait_ms(5);
		if (digitalIn.get() == 1){
			noise = true;
			break;
		}
	}
	if (noise == true){
		hwlib::cout << "ERROR CALLIBRATING: TOO NOISEY\n\nCalibration returned 15\n";
	}
	else {
		hwlib::cout << "Please send me the following: \"N TE\" (-. / - .)\n";
		int counter;
		int errorFilter;
		int checkIndex = 0;
		while (digitalIn.get() == 0){}
		while (checkIndex < 7){
			hwlib::wait_ms(1);
			counter = 0;
			for(int x=0; x < 20; x++){
					counter += digitalIn.get();
				}
			if (counter > 1){
				errorFilter = 0;
				if (checkIndex == 0){n1++;}
				else if (checkIndex == 1){checkIndex++;}
				else if (checkIndex == 2){n3++;}
				else if (checkIndex == 3){checkIndex++;}
				else if (checkIndex == 4){t++;}
				else if (checkIndex == 5){checkIndex++;}
				else if (checkIndex == 6){e++;}
			}
			else if (counter == 0){
				errorFilter++;
				if (errorFilter > 10){
					if (checkIndex == 0){checkIndex++;}
					else if (checkIndex == 1){n2++;}
					else if (checkIndex == 2){checkIndex++;}
					else if (checkIndex == 3){slash++;}
					else if (checkIndex == 4){checkIndex++;}
					else if (checkIndex == 5){space++;}
					else if (checkIndex == 6){checkIndex++;}
				}
			}
		}
		if (n3 < space*2.7 || n3 > space*3.3){
			hwlib::cout << "WARNING: SPACE IS NOT 3 UNITS!\n";
			hwlib::cout << "debug numbers: \n1unit:\t" << n3 << '\t' << e <<
			"\n3units:\t" << n1 << '\t' << t <<
			"\nslash:\t" << slash <<
			"\nSpace:\t" << space <<  '\n';
		}
		if (n3 < e*0.9 || n3 > e*1.1 || n1 < t*0.9 || n1 > t*1.1){
			hwlib::cout << "ERROR: TOO MUCH difference between long and short\nMost likely this means your volume needs calibration\n\nCalibration returned: "<< timersToWpm/n3 << '\n';
			hwlib::cout << "debug numbers: \n1unit:\t" << n3 << '\t' << e <<
			"\n3units:\t" << n1 << '\t' << t <<
			"\nslash:\t" << slash <<
			"\nSpace:\t" << space <<  '\n';
			wpm = timersToWpm/n3;
			return timersToWpm/n3;
		}
		else{
			hwlib::cout 
			<< " . =\t1 Unit\n"
			<< " - =\t" << n1/n3 << "units\n"
			<< "Space =\t" << space/n3 << "units\n"
			<< " / =\t" << slash/n3 << "units\n";
			hwlib::cout << "Callibration succesful. Returned: " << timersToWpm/n3 << " WPM\n\n";
			wpm = timersToWpm/n3;
			return timersToWpm/n3;
		}
	}
	return 15;
}

void morse::calibrateVolume(){
	hwlib::cout << "Calibration Volume started\nMake sure to ONLY send the letter e \nLike this: \"eeeeeeeeeeeeeeeeee\" for as long as needed\n\nUSE 20 WPM";
	const int targetCounter = 27; 
	int volume = 0;
	int counter;
	int errorFilter;
	while(digitalIn.get() == 0){}//wait for input
	while(true){
		hwlib::wait_ms(1);
		counter = 0;
		for(int x=0; x < 20; x++){
				counter += digitalIn.get();
			}
		if (counter > 1){
			if (volume > targetCounter*1*5){
				hwlib::cout << volume-targetCounter << " TOO LOUD!\n";
				volume = 0;
				errorFilter = 0;
				while(digitalIn.get() == 0){}
				}
			volume++;
			errorFilter = 0;
		}
		else {
			if (errorFilter > 10){
				hwlib::cout << volume-targetCounter << '\n';
				volume = 0;
				errorFilter = 0;
				while(digitalIn.get() == 0){}
				//break;
			}
			errorFilter++;
		}
	}
	
}

const char morse::morseToChar(char * ma){
	if (ma[0] == '.' && ma[1] == '-' && ma[2] == 0){
		return 'A';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '.' && ma[3] == '.' && ma[4] == 0){
		return 'B';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '-' && ma[3] == '.' && ma[4] == 0){
		return 'C';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '.' && ma[3] == 0){
		return 'D';
	}
	else if (ma[0] == '.' && ma[1] == 0){
		return 'E';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '-' && ma[3] == '.' && ma[4] == 0){
		return 'F';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '.' && ma[3] == 0){
		return 'G';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == '.' && ma[4] == 0){
		return 'H';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == 0){
		return 'I';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '-' && ma[3] == '-' && ma[4] == 0){
		return 'J';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '-' && ma[3] == 0){
		return 'K';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '.' && ma[3] == '.' && ma[4] == 0){
		return 'L';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == 0){
		return 'M';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == 0){
		return 'N';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '-' && ma[3] == 0){
		return 'O';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '-' && ma[3] == '.' && ma[4] == 0){
		return 'P';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '.' && ma[3] == '-' && ma[4] == 0){
		return 'Q';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '.' && ma[3] == 0){
		return 'R';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == 0){
		return 'S';
	}
	else if (ma[0] == '-' && ma[1] == 0){
		return 'T';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '-' && ma[3] == 0){
		return 'U';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == '-' && ma[4] == 0){
		return 'V';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '-' && ma[3] == 0){
		return 'W';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '.' && ma[3] == '-' && ma[4] == 0){
		return 'X';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '-' && ma[3] == '-' && ma[4] == 0){
		return 'Y';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '.' && ma[3] == '.' && ma[4] == 0){
		return 'Z';
	}
	else if (ma[0] == '.' && ma[1] == '-' && ma[2] == '-' && ma[3] == '-' && ma[4] == '-' && ma[5] == 0){
		return '1';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '-' && ma[3] == '-' && ma[4] == '-' && ma[5] == 0){
		return '2';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == '-' && ma[4] == '-' && ma[5] == 0){
		return '3';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == '.' && ma[4] == '-' && ma[5] == 0){
		return '4';
	}
	else if (ma[0] == '.' && ma[1] == '.' && ma[2] == '.' && ma[3] == '.' && ma[4] == '.' && ma[5] == 0){
		return '5';
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '.' && ma[3] == '.' && ma[4] == '.' && ma[5] == 0){
		return '6';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '.' && ma[3] == '.' && ma[4] == '.' && ma[5] == 0){
		return '7';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '-' && ma[3] == '.' && ma[4] == '.' && ma[5] == 0){
		return '8';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '-' && ma[3] == '-' && ma[4] == '.' && ma[5] == 0){
		return '9';
	}
	else if (ma[0] == '-' && ma[1] == '-' && ma[2] == '-' && ma[3] == '-' && ma[4] == '-' && ma[5] == 0){
		return '0';
	
	}
	else if (ma[0] == '-' && ma[1] == '.' && ma[2] == '-' && ma[3] == '.' && ma[4] == '-' && ma[5] == 0){
		return '.';
	
	}
	else if (ma[0] == '/' && ma[1] == 0){
		return ' '; //SPATIE
	}
	return '?';
	}
#endif
