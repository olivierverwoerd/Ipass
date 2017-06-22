#ifndef MORSE_HPP
#define MORSE_HPP

#include "..\hwlib\library\hwlib.hpp"\
/**
 * @class morse
 * @author Olivier
 * @date 20/06/2017
 * @file morse.hpp
 * @brief Listens for morse and transforms that information to an output.
 * the lenght of a dot is one unit
 * a dash is three usnis
 * the space between parts of the same letter is one untit
 * the space between letters is three units (With https://morsecode.scphillips.com/translator.html it is 6!)
 * the space between words are seven units. (With https://morsecode.scphillips.com/translator.html it is 5!)
 */
class morse {
private:
    hwlib::pin_in & digitalIn;
    int wpm;
	bool lastWasSpace;
	bool lastWasSlash;
public:
    /**
     * @brief Initializes the morse pin and set default configuration
     * @param digitalIn
	 * The digital pin you wish to receive morse with
     * @param wpm
	 * Words per Minute using PARIS as the standard word. 
	 * slow = 10
	 * normal = 15
	 * fast = 20
	 * this can be detected using calibrateWpm()
     * @param lastWasSpace
	 * space is in this case the morse space (the indication of seperation between letters of the same word)
	 * Used to let listen for morse wait for input instead of continuing
	 * 
	 * default = true so the script waits for intput.
     * @param lastWasSlash
	 * Used to let the output for space exist out of " / " with seperate char output because strings don't work
	 * 
	 * default = false because last was space is already true.
     */
    morse(hwlib::pin_in & digitalIn, int wpm = 15, bool lastWasSpace = true, bool lastWasSlash = false);
    /**
     * @brief Will listen for morse code and output . , _ , / as output
     */
    char listenForMorse();
/**
 * @brief This wil use the listenForMorse(); to get it's characters and output the character corresponding to the morse code
 * this is what you will most likely use for morse. The charactes are translated woth the morseDictionary();
 */
	char listenForMorseChar();
    /**
     * @brief If you have no idea what the wpm is use this tool
	 * it will not only tell you what the wpm is it will also warn you that the units are not the way they are meant to be
	 * like in my case. You may have to change the values in listenForMorse 
     */
    int calibrateWpm(); 
    /**
     * @brief If you callibrate wpm and the answer is NOT what you are sending. The microphone is not getting it's desired volume.
	 * Use this tool to get your morse working.
	 * Just send a bunch of eeeeeeeee's at 20 wpm and get the output as close to 0.
	 * than try calibrateWpm() again. 
     */
    void calibrateVolume(); 
	
	/**
 * @brief morseDictionary
 * hwlib stings somehow didn't work correctly
 * I used char arrays to solve this problem.
 * 
 *           dit .     *    dah -
 *                 /       \
 *             E               T
 *           /   \           /   \
 *         I       A       N       M
 *        / \     / \     / \     / \ 
 *       S   U   R   W   D   K   G   O
 *      / \ / \ / \ / \ / \ / \ / \ / \ 
 *      H V F * L * P J B X C Y Z Q * *
 * @param ma
 * ma is short voor Morse Array. The array should have a size of 7
 * (6 characters is morse max and a \0)
 * @return it will return the char corresponding to the code
 */
	const char morseToChar(char * morseArray);
};

#endif