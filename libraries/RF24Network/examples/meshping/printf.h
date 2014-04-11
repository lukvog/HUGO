#include <stdarg.h>

char buf[128]; 

/*
  * like printf() but output goes to "Serial.print".
  * If FREE_RTOS is defined, a mutex is used to protect from reentrancy.
  * TODO: Success/fail checks for xSemaphoreCreateMutex() and xSemaphoreTake() timeout.
  * Returns number of chars that did not fit into I/O port device buffer or 0.
  * Note:  Serial.print on the USB returns 0 in error, causing a flawed return value here.
*/
int kprintf(char *format, ...)  {
  int n;
#ifdef _FREE_RTOS_
  static xSemaphoreHandle kprintfLock = 0; // mutual exclusion for non-reentrant va_start
  
  if (kprintfLock == 0)  { 
    // first time only
    kprintfLock = xSemaphoreCreateMutex();
    xSemaphoreGive(kprintfLock);
  }
  
  if ((xSemaphoreTake(kprintfLock, configTICK_RATE_HZ / 4)) == pdPASS)  // TODO if pdFAIL, do more than just discard the print data.
#endif

  {
    va_list args;
    va_start (args, format);
    vsniprintf(buf, sizeof(buf), format, args); // does not overrun sizeof(buf) including null terminator
    va_end (args);
    // the below assumes that the new data will fit into the I/O buffer. If not, Serial may drop it.
    //   if Serial had a get free buffer count, we could delay and retry. Such does exist at the device class level, but not at this level.
    n = strlen(buf) - Serial.print(buf); // move chars to I/O buffer, freeing up local buf
#ifdef _FREE_RTOS_
    xSemaphoreGive(kprintfLock);
#endif

  }
  return n; // number of chars unable to fit in device I/O buffer (see bug notice above)
}
