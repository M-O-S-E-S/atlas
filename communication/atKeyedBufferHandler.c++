
#include <stdlib.h>
#include <string.h>
#include "atKeyedBufferHandler.h++"


atKeyedBufferHandler::atKeyedBufferHandler(u_long bufferType, 
   char * bufferTarget, u_long classID, u_long bufferKey, 
   u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_type = bufferType;
   buffer_target = bufferTarget;
   buffer_class = classID;
   buffer_key = bufferKey;
   buffer_text = buffer;
   buffer_len = bufferLen;
}


atKeyedBufferHandler::~atKeyedBufferHandler()
{
}


void atKeyedBufferHandler::retrieveBuffer(u_long * bufferType, 
   char ** bufferTarget, u_long * classID, u_long * bufferKey,
   u_char ** buffer, u_long * bufferLen)
{
   // Save the data of this buffer
   *bufferType = buffer_type;
   *bufferTarget = buffer_target;
   *classID = buffer_class;
   *bufferKey = buffer_key;
   *buffer = buffer_text;
   *bufferLen = buffer_len;
}


void atKeyedBufferHandler::setBuffer(u_long bufferType, char * bufferTarget,
   u_long classID, u_long bufferKey, u_char * buffer, u_long bufferLen)
{
   // Save the data of this buffer
   buffer_type = bufferType;
   buffer_target = bufferTarget;
   buffer_class = classID;
   buffer_key = bufferKey;
   buffer_text = buffer;
   buffer_len = bufferLen;
}


void atKeyedBufferHandler::print()
{
   // Call notify with the buffer (right now this is at an INFO level,
   // but should probably be lowered once lower levels are implemented)
   notify(AT_INFO, "Buffer Handler has buffer of \"%s\".\n", buffer_text);
}

