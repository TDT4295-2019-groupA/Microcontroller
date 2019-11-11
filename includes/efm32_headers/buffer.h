#ifndef HEADERS_BUFFER_H_
#define HEADERS_BUFFER_H_

#include <stdbool.h>

#define READBUFFER_SIZE 32 //must be a power of 2

typedef struct {
	unsigned char *buf;
	unsigned int head;
	unsigned int tail;
	bool full;
} cbuf;

bool isFull(cbuf *buffer);

bool isEmpty(cbuf *buffer);

void advanceHead(cbuf *buffer);

void advanceTail(cbuf *buffer);


#endif /* INCLUDES_EFM32_HEADERS_BUFFER_H_ */
