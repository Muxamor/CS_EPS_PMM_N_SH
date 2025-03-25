#include  "Error_Handler.h"
#include  <stdio.h>



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
 */
void _Error_Handler(char *file, int line){

	printf("Wrong parameters value: file %s on line %d\r\n", file, line);

}
