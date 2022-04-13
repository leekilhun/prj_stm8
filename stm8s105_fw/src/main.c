#include "main.h"
#include "stdint.h"





int main(void) 
{
  hwInit();
  apInit();

  apMain();
  
  return 0;
}
