/*
 * print_server.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#ifndef PRINT_SERVER_H_
#define PRINT_SERVER_H_

#if 1
#define PRINTF(...) PrintServerPrintf(__VA_ARGS__)
#else
#define PRINTF(...) do{} while(0)
#endif

void PrintServerPrintf(const char *fmt, ...);

#endif /* PRINT_SERVER_H_ */
