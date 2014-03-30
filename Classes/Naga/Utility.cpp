/*
    Copyright 2012 NAGA.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by NAGA or its suppliers or 
	licensors.  Title to the Material remains with NAGA or its suppliers and 
	licensors.  The Material is protected by worldwide copyright laws and 
	treaty provisions.  No part of the Material may be used, copied, reproduced, 
	modified, published, uploaded, posted, transmitted, distributed, or 
	disclosed in any way without NAGA's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by NAGA in
    writing.
*/
/*
	Author		:	Yu Li
	Description	:	Utility Functions
	History		:	2012, Initial implementation.
*/
#include "NagaLib.h"

NAMESPACE_NAGA_BEGIN

/// <description>
///  Print the message to specified target
/// </description>
void DebugPrintf(const char* fmt, ...)
{	
	va_list arg;
	va_start(arg,fmt);

	char buffer[512];
	vsnprintf(buffer, sizeof(buffer) / sizeof(char), fmt, arg);
    printf("%s", buffer);

	va_end(arg);	
}	

/// <description>
///  Print the message to specified target
/// </description>
void DebugPrintf(const UTxString& info) {
	DebugPrintf(info.c_str());
}

/// <description>
///  Print the message to specified target
/// </description>
void Warning(const char*fmt, ...)
{
	va_list arg;
	va_start(arg,fmt);
	char buffer[512];
	vsnprintf(buffer, sizeof(buffer) / sizeof(char), fmt, arg);
	va_end(arg);	

	DebugPrintf(buffer);
}

/// <description>
///  Print the message to specified target
/// </description>
void NAGAAPI FatalError(const char*fmt, ...)
{
	va_list arg;
	va_start(arg,fmt);
	char buffer[512];
	vsnprintf(buffer, sizeof(buffer) / sizeof(char), fmt, arg);
	va_end(arg);

	DebugPrintf(buffer);
}

void NAGAAPI FormatString(UTxString& str, const char*fmt, ...)
{
	va_list arg;
	va_start(arg,fmt);
	char buffer[512];
	vsnprintf(buffer, sizeof(buffer) / sizeof(char), fmt, arg);
	va_end(arg);

	str.assign(buffer);
}

void _Assert(const char* _Message, const char* _File, unsigned _Line)
{
	DebugPrintf("%s : %s %d\n", _Message, _File, _Line);
}
/// <description>
/// used to check the standard return value 
/// </description>
void NAGAAPI Verify(long hr, const char* filename, long lineno)
{
	DebugPrintf("Verify fails @%s(%d)\n",filename,lineno);
}

#define NAGA_MAX_PATH 1024 // used in Linux and OS X platform wide char to ANSCII char conversion
#define NAGA_MAX_MODE 20

NAMESPACE_NAGA_END
