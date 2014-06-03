/* NI01, 02.06.2014 */

#include <stdio.h>

#include <windows.h>


int main ( int argc, char *argv[] )
{
  MessageBox(NULL, "Hello worLd\nplease, do not press any key\n", "Test", MB_OK);
  MessageBox(NULL, "Why did you press any key?\n", "!!!", MB_OK);
  return 0;
}