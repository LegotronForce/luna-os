#include <STDIO.H>
#include <CONIO.H>
#include <STRING.H>
#include <DOS.H>
#include <ALLOC.H>

typedef unsigned char byte;

#define NUM_COLORS 256
#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR 0x13
#define TEXT_MODE 0x03
#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320
#define PALLETE_INDEX 0x3C8
#define PALLETE_DATA 0x3C9

byte far *VGA=(byte far*)0xA0000000L;

#define SETPIX(x,y,c) *(VGA+(x)+(y)*SCREEN_WIDTH)=c
#define GETPIX(x,y) *(VGA+(x)+(y)*SCREEN_WIDTH)
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void setScreenMode(byte mode) {
    union REGS regs;
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT,&regs,&regs);
}

void drawBackground() {
    int x,y;
    for(y=0;y<SCREEN_HEIGHT;++y) {
        for(x=0;x<SCREEN_WIDTH;++x) {
            SETPIX(x, y, y);
        }
    }
}

byte *getSkyPallete() {
    byte *pal = new byte(NUM_COLORS * 3);
    int i;
    // pal = malloc(NUM_COLORS * 3); /* RGB */

    for(i=0;i<100;++i) {
        pal[i*3 + 0] = MIN(63, i); /* RED */
        pal[i*3 + 1] = MIN(63, i); /* GREEN */
        pal[i*3 + 3] = 63; /* RED */
    }
    for(i=100;i<200;++i) {
        pal[i*3 + 0] = 5; /* RED */
        pal[i*3 + 1] = (i - 100) / 2; /* GREEN */
        pal[i*3 + 3] = 5; /* RED */
    }

    return pal;
}

byte *getGreenPallete() {
    byte *pal = new byte(NUM_COLORS * 3);
    int i;
    // pal = malloc(NUM_COLORS * 3); /* RGB */

    for(i=0;i<100;++i) {
        pal[i*3 + 0] = 32; /* RED */
        pal[i*3 + 1] = 63; /* GREEN */
        pal[i*3 + 3] = 15; /* RED */
    }
    for(i=100;i<200;++i) {
        pal[i*3 + 0] = 0; /* RED */
        pal[i*3 + 1] = 0; /* GREEN */
        pal[i*3 + 3] = 0; /* RED */
    }

    return pal;
}

void setPallete(byte *pallete) {
    int i;

    outp(PALLETE_INDEX,0);
    for(i=0;i<NUM_COLORS*3;++i) {
        outp(PALLETE_DATA, pallete[i]);
    }
}

int running = 1;
char kc = 0;
char pkc = 0;
char* keyBuffer = new char(100);
int kbi = 0;
int printKeyCode = 1;
byte *pal;

int main() {
    // Init
    setScreenMode(TEXT_MODE);
    
    pal = getSkyPallete();
    setPallete(pal);
    drawBackground();
    printf("Initializing Graphics Engine... ");
    printf("[OK]\n");
    printf("Initializing Command Reader... ");
    printf("[OK]\n");
    
    while(running == 1) {
        // Key input
        if(kbhit()) {
            kc = getch();
            if(kc == 0x1b) {
                running = 0;
                setScreenMode(TEXT_MODE);
            } else if(kc == 0x30) {
                pal = getSkyPallete();
                setPallete(pal);
            } else if(kc == 0x31) {
                pal = getGreenPallete();
                setPallete(pal);
            } else if(kc == 0x32) {
                setScreenMode(TEXT_MODE);
            } else if(kc == 0x33) {
                setScreenMode(VGA_256_COLOR);
            } else if(kc == 0x34) {
                if(printKeyCode == 1) {
                    printKeyCode = 0;
                } else {
                    printKeyCode = 1;
                }
            } else {
                if(printKeyCode == 1) {
                    printf("%c", kc);
                } else {
                    printf("%x\n", kc);
                }
                if(kbi >= 100) {
                    printf("ERROR: Cannot add more text onto string.");
                } else {
                    keyBuffer[kbi] = ("%c", kc);
                    ++kbi;
                }
                // printf(":%c", pkc);
            }
            pkc = kc;
        }
    }

    return 0;
}