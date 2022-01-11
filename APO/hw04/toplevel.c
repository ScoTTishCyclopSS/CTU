/* Implementation of the subroutine toplevel_fnc */

#include "subroutine.h"

int toplevel_fnc(void)
{
  int f = open("filename.txt", 0x242, 0x180); 
    if(f >= 0)
     return subroutine_fnc(f, 0);
    else
        return f;
  /*
   * The code calls function subroutine_fnc
   * and processes its return value.
   * 
   * Then it resturn final result.
   */
}
