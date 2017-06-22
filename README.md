#Ipass by Olivier Verwoerd

These are the files I made for the Ipass of the university of applied sciances Utrecht. 
it's the curiosity of useing analog and different signals that is not perfectly made to be understoud by the arduino. Useing morse code was a perfect way to do this since morse is not perfecty timed.

#What did I do

First I made it posible to get morse form a digital signal.
Than i made an dictionary to get char form morse
I wanted to make it faster and faster so I optimalized a few things
Wanted even faster than sound and i found out morse is still used (sort of) to send bytes in IR remotes and made support for that.
Everything above was digital so i wanted to do something analog. To start with volume measureing.
than frequency detection.
I've made some configuration tools in the code to debug and make the script more dynamic
I wanted to make use of the frequency to filter and the volume detection the morse code to make it even better. This didn't work out. Frequency detection only worked with no background noise because it couldn't detect multiple frequencys. It's not a good spectrometer. This just made the morse detection significant more unreliable. Volume is more unreliable with a lower buffer size and therefore didn't work, volume is too slow with a big buffer. This isn't inposible to implement. I never just got the configuration right and decided to let the hardware do it's work.
Well. You are looking at my last thing i did. Doxygen documentation...
Good to know

NOTE: Since printing thousands of numbers in an array is simply too much to make any sense to me. i've used this tool https://developer.mbed.org/users/borislav/notebook/serial-port-plotter/ A great tool for plotting the array. To let the tool work there needs to be an $ in front of the number followed by an ;

Because i'm inpactient to do wait for the slow terminal. I used baud rate of 19200 AND NOT 2400. Change this in makefile.due and in hwlib_ostream change BMPTK_BAUDRATE to 19200.

#licence

Copyright Olivier Verwoerd 2017. Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#dependency

It needs this https://github.com/wovo/hwlib and some love otherwise this will never work.
