#pragma once
#include "PktDef.h"
#include <fstream>

void writePostToFile(Post* post);
void writePacketRawDataToFile(char* buffer, int size, string status);