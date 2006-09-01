
#include <stdlib.h>
#include <string.h>
#include "atBufferHandler.h++"


atBufferHandler::atBufferHandler(u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_text = buffer;
   buffer_len = bufferLen;
}


atBufferHandler::~atBufferHandler()
{
}


void atBufferHandler::retrieveBuffer(u_char ** buffer, u_long * bufferLen)
{
   // Save the data of this buffer
   *buffer = buffer_text;
   *bufferLen = buffer_len;
}


void atBufferHandler::setBuffer(u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_text = buffer;
   buffer_len = bufferLen;
}


void atBufferHandler::print()
{
   // Call notify with the buffer (right now this is at an INFO level,
   // but should probably be lowered once lower levels are implemented)
   notify(AT_INFO, "Buffer Handler has buffer of \"%s\".\n", buffer_text);
}

