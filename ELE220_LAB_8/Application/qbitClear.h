/*
 * qbitClear.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#ifndef QBITCLEAR_H_
#define QBITCLEAR_H_

/*
#define MY_CLEARING_Q_FLAG_MACRO do {					\
	int32_t my_apsr = 0;								\
	asm volatile(										\
			"mrs %0, apsr\n\t"							\
			"bic %0, %0, #(1 << 27)\n\t"				\
			"msr apsr_nzcvq, %0" : "+r" (my_apsr));
} while (0)
*/

#endif /* QBITCLEAR_H_ */
