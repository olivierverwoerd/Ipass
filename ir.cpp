#ifndef IR_HPP
#include "ir.hpp"

	ir::ir(hwlib::pin_in & digitalIn):
	digitalIn(digitalIn)
	{}
	
	const int ir::receive(int protocol){
		int array[1000] = {};
		int startItem = 0;
		int counter0 = 0;
		int counter1 = 0;
		unsigned int receive = 0;
		while ( digitalIn.get() != 0){};
		for(int item = 0; item < 1000; item++){
		array[item] = digitalIn.get();
		hwlib::wait_ns(75000);
		}
		if (protocol == 0){
			protocol = 1;
			for(int item = 0; item < 100; item++){
				if (array[item] == 1){
					protocol = 2;
					break;
				}
			}
		}
		if (protocol == 1){
			for(int item = 0; item < 1000; item++){
				if (array[item] == 1 && array[item+1] == 0){
					startItem = item;
					break;
				}
				
			}
			for(int item = startItem; item < 1000; item++){
				if (array[item] == 0){ // led is ON 
					counter1++;
					if (counter0 != 0){
						receive = receive << 1;
						if(counter0*7 < startItem && startItem > counter0*9){
							receive += 1;
						}
						else{
						}
					}
					counter0 = 0;
				}
				else {
					counter0++;
					if (counter0 > 50){
						break;
					}
					counter1 = 0;
				}
			}
			int receive2;
			int checksum = receive >> 8; ///calculates the checksum from the NEC protocol. If incorrect ir will return 0
			checksum = checksum & receive;
			for (int bit = 0; bit < 8; bit++){
				if ((checksum & 0x01) == 1){
					return 0;
				}
				checksum = checksum >> 1;
			}
			
			checksum = checksum >> 8;
			for (int bit = 0; bit < 8; bit++){
				if ((checksum & 0x01) == 1){
					return 0;
				}
				checksum = checksum >> 1;
			}
			receive = receive >> 8;  /// Will get rid of the inverse in the checksum and output 16 bit 
			receive2 = receive >> 8;
			receive = receive & 255; //00ff
			receive2 = receive2 & 65280;//ff00
			receive = (receive | receive2);
		}
		else if (protocol == 2){
			for(int item = 0; item < 1000; item++){
				if (array[item] == 1){
					startItem = item;
					break;
				}
				
			}
			for(int item = startItem; item < 1000; item++){
				if (array[item] == 0){ // led is ON 
					counter1++;
					counter0 = 0;
				}
				else {
					counter0++;
					if (counter1 != 0){
						receive = receive << 1;
						if(counter1 > startItem*0.4 && startItem*0.6 > counter1){
							receive += 1;
						}
					}
					if (counter0 > 50){
						break;
					}
					counter1 = 0;
				}
			}
		}
	return receive;
	}
	
	const void ir::debug(){
	int array[1500] = {};
	while ( digitalIn.get() != 0){};
	for(int item = 0; item < 1000; item++){
		array[item] = digitalIn.get();
		hwlib::wait_ns(75000);
	}
	for(int item = 0; item < 1000; item++){
		hwlib::cout << '$' << array[item] <<  ';' << hwlib::flush;
	}
	}
#endif

