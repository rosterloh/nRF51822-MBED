#ifndef _H_LOGGER_H
#define _H_LOGGER_H

#define NEED_CONSOLE_OUTPUT 1 /* Set this if you need debug messages on the console;
                               * it will have an impact on code-size and power consumption. */

#if NEED_CONSOLE_OUTPUT
//Serial  pc(USBTX, USBRX);
//pc.baud(9600);
//pc.set_flow_control(SerialBase::RTSCTS, p8, p10);
#define DEBUG(...) { printf(__VA_ARGS__); }
#else
#define DEBUG(...) /* nothing */
#endif /* #if NEED_CONSOLE_OUTPUT */

#endif //_H_LOGGER_H
