using namespace std;
#include "Utils/MessageHandler.hh"
#include "Core/Color.hh"

#include <string.h>
#include <strings.h>
#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
namespace Marcel{

MessageHandler::MessageHandler() {};

MessageHandler::~MessageHandler() {};

int MessageHandler::getInteger(int position) {
	return receive_buffer[position + 0] * (0xFFFFFF + 1) + receive_buffer[position + 1] * (0xFFFF + 1) + receive_buffer[position + 2] * (0xFF + 1) + receive_buffer[position + 3];
}

int MessageHandler::getWord(int position) {
	return receive_buffer[position] * (0xFF + 1) + receive_buffer[position + 1];
}

int MessageHandler::getByte(int position) {
	return receive_buffer[position];
}

int MessageHandler::setInteger(int start_position, int value) {
	send_buffer[start_position + 0] = (value & MASK_1) >> 24;
	send_buffer[start_position + 1] = (value & MASK_2) >> 16;
	send_buffer[start_position + 2] = (value & MASK_3) >> 8;
	send_buffer[start_position + 3] = value & MASK_4;

	return 4;
}

int MessageHandler::setWord(int start_position, int value) {
	send_buffer[start_position + 0] = (value & MASK_3) >> 8;
	send_buffer[start_position + 1] = value & MASK_4;

	return 2;
}

int MessageHandler::setByte(int start_position, int value) {
	send_buffer[start_position + 0] = value & MASK_4;

	return 1;
}

int MessageHandler::setString(int start_position, unsigned char *value) {
	setWord(start_position, strlen((char *)value));
	memcpy(send_buffer + start_position + 2, value, strlen((char *)value));
	return 1;
}

int MessageHandler::setHash16(int start_position, unsigned char *value) {
	memcpy(send_buffer + start_position, value, 16);
	return 1;
}

int MessageHandler::setBlock(int start_position, int size, unsigned char *value) {
	setWord(start_position, size);
	memcpy(send_buffer + start_position + 2, value, size);
	return 1;
}


int MessageHandler::CreateMessage(const char *format, int message_type, ...) {
	//int    NbArguments = strlen(format) / 2;
	va_list ap;

	va_start(ap, message_type);
	int v = CreateMessage(NULL,format,message_type,ap);
	va_end(ap);

	return v;
}

int MessageHandler::CreateMessage(char **message, const char *format, int message_type, ...) {
	//int    NbArguments = strlen(format) / 2;
	va_list ap;

	va_start(ap, message_type);
	int v = CreateMessage(message,format,message_type,ap);
	va_end(ap);

	return v;
}

int MessageHandler::CreateMessage(char **message, const char *format, int message_type,  va_list ap) {
	int    IntValue;
	char   *StringValue;
	char * FormatPosition = (char *)format + 1;
	int    MessageSize = 0;
	int    NbArguments = strlen(format) / 2;

	setWord(0, message_type);

	int i = 0;
	while (i < NbArguments)
	{
		switch (FormatPosition[0])
		{
		case 'w':
			IntValue = va_arg(ap, int);
			setWord(MessageSize + 4, IntValue);
			MessageSize += 2;
			break;
		case 'i':
			IntValue = va_arg(ap, int);
			setInteger(MessageSize + 4, IntValue);
			MessageSize += 4;
			break;
		case 's':
			StringValue = va_arg(ap, char *);
			setString(MessageSize + 4, (unsigned char*)StringValue);
			MessageSize += 2 + strlen(StringValue);
			break;
		case 'b':
			IntValue = va_arg(ap, int);
			setByte(MessageSize + 4, IntValue);
			MessageSize += 1;
			break;
		case 'h':
			StringValue = va_arg(ap, char *);
			setHash16(MessageSize + 4, (unsigned char*)StringValue);
			MessageSize += 16;
			break;
		case 'B':
			StringValue = va_arg(ap, char *);
			//strcpy(StringValue,va_arg(ap,char *));
			setBlock(MessageSize + 4, 1024, (unsigned char*)StringValue);
			MessageSize += 2 + strlen(StringValue);
			break;
		}
		i = i + 1;
		FormatPosition = FormatPosition + 2;
	}

	setWord(2, MessageSize);

	if(message != NULL){
		memcpy(message, send_buffer, MessageSize + 4);
	}

	return MessageSize + 4;
}

int MessageHandler::ReadMessage(char *message, int size, const char *format, ...) {
	//int    NbArguments = strlen(format) / 2;
	va_list ap;

	va_start(ap, format);
	int v = ReadMessage(message,size,format,ap);
	va_end(ap);

	return v;
}

int MessageHandler::ReadMessage(char *message, int size, const char *format, va_list ap) {
	int  *  IntValue;
	char *  StringValue;
	int     StringSize;

	char * FormatPosition = (char *)format + 1;

	int    NbArguments = strlen(format) / 2;
	int    MessageSize = 0;

	memcpy(receive_buffer, (unsigned char*)message, 16384);

	int i = 0;
	while (i < NbArguments)
	{
		switch (FormatPosition[0])
		{
		case 'w':
			IntValue = va_arg(ap, int *);
			*(IntValue) = getWord(MessageSize + 4);
			MessageSize += 2;
			break;
		case 'i':
			IntValue = va_arg(ap, int *);
			*(IntValue) = getInteger(MessageSize + 4);
			MessageSize += 4;
			break;

		case 's':
			StringValue = va_arg(ap, char *);
			StringSize  = getWord(MessageSize + 4);
			memcpy(StringValue, (receive_buffer + 6 + MessageSize), StringSize);
			StringValue[StringSize] = 0;
			MessageSize += 2 + StringSize;
			break;

		case 'b':
			IntValue = va_arg(ap, int *);
			*(IntValue) = getByte(MessageSize + 4);
			MessageSize += 1;
			break;

		case 'B':
			StringValue = va_arg(ap, char *);
			memcpy(StringValue, (receive_buffer + 6 + MessageSize), 1024);
			MessageSize += 1024 + 2;
			break;
		}
		i = i + 1;
		FormatPosition = FormatPosition + 2;
	}

	return getWord(0);
}

void MessageHandler::setColor(int position, Color C) {
	//setWord((position * 6) + 4 + 2, (int)(C.getRed() * 255));
	//setWord((position * 6) + 6 + 2, (int)(C.getGreen() * 255));
	//setWord((position * 6) + 8 + 2, (int)(C.getBlue() * 255));

	setByte((position * 3) + 4 + 2, (int)(C.getRed() * 255));
	setByte((position * 3) + 5 + 2, (int)(C.getGreen() * 255));
	setByte((position * 3) + 6 + 2, (int)(C.getBlue() * 255));
}

Color MessageHandler::getColor(int position) {
	int r, g, b;
	//r = getWord((position * 6) + 4 + 2);
	//g = getWord((position * 6) + 6 + 2);
	//b = getWord((position * 6) + 8 + 2);

	r = getByte((position * 3) + 4 + 2);
	g = getByte((position * 3) + 5 + 2);
	b = getByte((position * 3) + 6 + 2);

	return Color((float)r / 255.0, (float)g / 255.0, (float)b / 255.0);
}

void MessageHandler::setBinaryBlock(int position, int size, unsigned char *message) {
	for (int i = 0; i < size; i++)
		setWord(position + 2 * i, message[i]);
}

void MessageHandler::getBinaryBlock(int position, int size, unsigned char **message) {
	unsigned char buffer[4096];

	for (int i = 0; i < size; i++)
		buffer[i] = getWord(position + 2 * i);
	memcpy(message, buffer, size);
}

void MessageHandler::copySent(char **message, int size, int position) {
	memcpy(message+position, send_buffer, size);
}

void MessageHandler::copyReceived(char **message, int size, int position) {
	memcpy(receive_buffer+position, message, size);
}

char * MessageHandler::getSendBuffer(){
	return (char *)send_buffer;
}

char * MessageHandler::getReceiveBuffer(){
	return (char *)receive_buffer;
}
}