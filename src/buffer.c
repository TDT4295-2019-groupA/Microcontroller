#include "buffer.h";
#include "usbhost.h";

bool isFull(cbuf *buffer) {
	return (buffer->full);
}

bool isEmpty(cbuf *buffer) {
	return (!buffer->full && (buffer->head == buffer->tail));
}

void advanceHead(cbuf *buffer) {
	buffer->head += (USB_OUTPUT_SIZE & (READBUFFER_SIZE-1));
	buffer->full = (buffer->head == buffer->tail);
}

void advanceTail(cbuf *buffer) {
	memset(buffer->buf, buffer->tail, USB_OUTPUT_SIZE);
	buffer->tail += (USB_OUTPUT_SIZE & (READBUFFER_SIZE-1));
	buffer->full = false;
}

