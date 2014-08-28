#ifndef ANSIUTILS_H
#define ANSIUTILS_H

typedef enum {BLACK,RED,GREEN,YELLOW,BLUE,MAGENTA,CYAN,WHITE,unknown,RESET,
              END_COLOURS}
              COLOURS;



int ansi_fprintf(FILE *file, char *fmt, ...);

#endif
