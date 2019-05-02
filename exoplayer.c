#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "exoplayer.h"
#include "stream_defs.h"

void start_exoplayer() {
	char command[200] = "cmd activity start-activity ";
	strcat(command, COMP);
	system(command);
}

void start_stream(char* url) {
	char command[200] = "am start -a com.google.android.exoplayer.demo.action.VIEW -d ";
	strcat(command, url);
	system(command);
}

void stop_stream() {
	system("input keyevent 4");
}

void stop_exoplayer() {
	char command[200] = "am force-stop ";
	strcat(command, PKG);
	system(command);
}

void unlock_screen() {
	system("input touchscreen swipe 930 880 930 380");
}

char* get_stream_name(stream stream_type, char* ret) {
	switch(stream_type) {
		case WIDEVINE_DASH_H264_CLEAR_SD :
			strcpy(ret, "WIDEVINE_DASH_H264_CLEAR_SD");
			break;
		case WIDEVINE_DASH_H264_CLEAR_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_CLEAR_HD");
			break;
		case WIDEVINE_DASH_H264_CLEAR_UHD :
			strcpy(ret, "WIDEVINE_DASH_H264_CLEAR_UHD");
			break;
		case WIDEVINE_DASH_H264_CLEAR_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_CLEAR_SD_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_SD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CENC_SD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CENC_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_UHD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CENC_UHD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CENC_SD_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_SD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBC1_SD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBC1_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_UHD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBC1_UHD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_SD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBCS_SD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBCS_HD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_UHD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBCS_UHD");
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD");
			break;
		case WIDEVINE_DASH_H265_CLEAR_SD :
			strcpy(ret, "WIDEVINE_DASH_H265_CLEAR_SD");
			break;
		case WIDEVINE_DASH_H265_CLEAR_HD :
			strcpy(ret, "WIDEVINE_DASH_H265_CLEAR_HD");
			break;
		case WIDEVINE_DASH_H265_CLEAR_UHD :
			strcpy(ret, "WIDEVINE_DASH_H265_CLEAR_UHD");
			break;
		case WIDEVINE_DASH_H265_CLEAR_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H265_CLEAR_SD_HD");
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_SD :
			strcpy(ret, "WIDEVINE_DASH_H265_SECURE_CENC_SD");
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_HD :
			strcpy(ret, "WIDEVINE_DASH_H265_SECURE_CENC_HD");
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_UHD :
			strcpy(ret, "WIDEVINE_DASH_H265_SECURE_CENC_UHD");
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_SD_HD :
			strcpy(ret, "WIDEVINE_DASH_H265_SECURE_CENC_SD_HD");
			break;
		default :
			break;
	}
	return ret;
}

char* get_stream_url(stream stream_type, char* url) {
	switch(stream_type) {
		case WIDEVINE_DASH_H264_CLEAR_SD :
			strcpy(url, WIDEVINE_DASH_H264_CLEAR_SD_URL);
			break;
		case WIDEVINE_DASH_H264_CLEAR_HD :
			strcpy(url, WIDEVINE_DASH_H264_CLEAR_HD_URL);
			break;
		case WIDEVINE_DASH_H264_CLEAR_UHD :
			strcpy(url, WIDEVINE_DASH_H264_CLEAR_UHD_URL);
			break;
		case WIDEVINE_DASH_H264_CLEAR_SD_HD :
			strcpy(url, WIDEVINE_DASH_H264_CLEAR_SD_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_SD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CENC_SD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CENC_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_UHD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CENC_UHD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CENC_SD_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CENC_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_SD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBC1_SD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBC1_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_UHD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBC1_UHD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_SD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBCS_SD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBCS_HD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_UHD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBCS_UHD_URL);
			break;
		case WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD :
			strcpy(url, WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD_URL);
			break;
		case WIDEVINE_DASH_H265_CLEAR_SD :
			strcpy(url, WIDEVINE_DASH_H265_CLEAR_SD_URL);
			break;
		case WIDEVINE_DASH_H265_CLEAR_HD :
			strcpy(url, WIDEVINE_DASH_H265_CLEAR_HD_URL);
			break;
		case WIDEVINE_DASH_H265_CLEAR_UHD :
			strcpy(url, WIDEVINE_DASH_H265_CLEAR_UHD_URL);
			break;
		case WIDEVINE_DASH_H265_CLEAR_SD_HD :
			strcpy(url, WIDEVINE_DASH_H265_CLEAR_SD_HD_URL);
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_SD :
			strcpy(url, WIDEVINE_DASH_H265_SECURE_CENC_SD_URL);
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_HD :
			strcpy(url, WIDEVINE_DASH_H265_SECURE_CENC_HD_URL);
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_UHD :
			strcpy(url, WIDEVINE_DASH_H265_SECURE_CENC_UHD_URL);
			break;
		case WIDEVINE_DASH_H265_SECURE_CENC_SD_HD :
			strcpy(url, WIDEVINE_DASH_H265_SECURE_CENC_SD_HD_URL);
			break;
		default :
			break;
	}
	return url;
}

void get_stream_list(stream* stream_list, int clear, int secure, int uhd) {
	int n = 0;
	if(clear) {
		for(int i = WIDEVINE_DASH_H264_CLEAR_SD; i <= WIDEVINE_DASH_H265_CLEAR_SD_HD; i++) {
			stream_list[i] = i;
			n++;
		}
	}
	if(secure) {
		for(int i = WIDEVINE_DASH_H264_SECURE_CENC_SD; i <= WIDEVINE_DASH_H265_SECURE_CENC_SD_HD; i++) {
			stream_list[i] = i;
			n++;
		}
	}
	if(clear && uhd) {
		for(int i = WIDEVINE_DASH_H264_CLEAR_UHD; i <= WIDEVINE_DASH_H265_CLEAR_UHD; i++) {
			stream_list[i] = i;
			n++;
		}
	}
	if(secure && uhd) {
		for(int i = WIDEVINE_DASH_H264_SECURE_CENC_UHD; i <= WIDEVINE_DASH_H265_SECURE_CENC_UHD; i++) {
			stream_list[i] = i;
			n++;
		}
	}
	stream_list[n] = last;
}