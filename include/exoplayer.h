#ifndef EXOPLAYER_H
#define EXOPLAYER_H

#include "stream_defs.h"

#define PKG "com.google.android.exoplayer2.demo"
#define COMP "com.google.android.exoplayer2.demo/.SampleChooserActivity"

void start_exoplayer();
void start_stream(char* url);
void stop_stream();
void stop_exoplayer();
void unlock_screen();
char* get_stream_name(stream stream_type, char* ret);
char* get_stream_url(stream stream_type, char* url);
void get_stream_list(stream* stream_list, int clear, int secure, int uhd);

#endif