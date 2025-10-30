//////////////////////////////////////////////////////////
//                                                      //
//                                                      //
//                  fuckUNIX.H                          //
//          This is the core header file used by        //
//            everything that needs kernel APIs         //
//                      and stuff                       //
//                                                      //         
//                                                      //
//////////////////////////////////////////////////////////

// define the kernel strings
#define KERNEL_INF "[INFO]"
#define KERNEL_WARNING "[WARN]"
#define KERNEL_ERR "[ERROR]"

// define the VGA colors
#define VGA_RED 0xC
#define VGA_GREEN 0x2
#define VGA_YELLOW 0xE
#define VGA_WHITE 0xF

// define the kernel API
void execute_command(char *input); // executes a command
//void kpoweroff(); // powers off the PC
//void khalt(); // halts the CPU