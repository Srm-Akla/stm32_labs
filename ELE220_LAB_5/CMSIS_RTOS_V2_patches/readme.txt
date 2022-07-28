Last edited 2020-01-24

This folder contains patches needed for CMSIS-RTOS v2 support since the files generated with STM32CubeMX
is missing some components.

File cmsis_os2.c are missing CMSIS-RTOS v2 functions for MessagePool. Implementation of this functions are 
found at https://github.com/ARM-software/CMSIS-FreeRTOS/ and included in file cmsis_os2_patches.c. Needed 
types and defines in file freertos_mpool.h downloaded from  https://github.com/ARM-software/CMSIS-FreeRTOS/.

The above patch includes a call to uxSemaphoreGetCountFromISR() not found anywhere and a patch for that
added in file semphr_patches.h.

