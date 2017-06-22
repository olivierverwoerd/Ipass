#ifndef IR_HPP
#define IR_HPP

#include "..\hwlib\library\hwlib.hpp"



/**
 * @class ir
 * @author Olivier
 * @date 20/06/2017
 * @file ir.hpp
 * @brief IR
 * This will translate an IR signal to its information as an int.
 * When the checksum is incorrect or the code doesn't make sense the output will be 0 if NEC is beeing received.
 * For sony the value will just repeat
 * Cause of the way the program catches IR the checksum will almost certain be false when holding down a button.
 * You can make use of this like the example of the main
 */
class ir {
private:
    hwlib::pin_in & digitalIn;
public:
/**
 * @brief Initializes the pin.
 * @param digitalIn
 * The pin that you wish to receive IR.
 */
	ir(hwlib::pin_in & digitalIn);
/**
 * @brief receive
 * Waits to receive an IR command and returns this as a int value
 * This supports Sony and NEC protocol. I didn't have more remotes to work with.
 * @param protocol
 * 0 = auto detect Sony and NEC
 * 1 = NEC explained here: http://www.sbprojects.com/knowledge/ir/nec.php
 * 2 = SONY explained here: http://www.sbprojects.com/knowledge/ir/sirc.php 
 */
	const int receive(int protocol = 0);

/**
 * @brief Small tool to view what IR receives.
 * The output has a $ and ; for the plotter 
 * https://developer.mbed.org/users/borislav/notebook/serial-port-plotter/
 */
	const void debug();
};

#endif