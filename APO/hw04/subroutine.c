/* Implementation of the subroutine subroutine_fnc */

#include "subroutine.h"

int subroutine_fnc(int f, int val)
{
  int counter = 0;
    char c = 0;
    int r = read(val, &c, 0x1);
    if (r == 1) {
        r = write(f, &c, 0x1);
        if (r < 0) {
            if (r >= 0) {
                return counter;
            } else {
                return -1;
            }
        } else {
            counter++;
        }
    } 
    if (r >= 0) {
        return counter;
    } else {
        return -1;
    }
}
