#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "parse.h"
#include "stream_defs.h"
#include "defaults.h"
#include "exoplayer.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	int opt;
	int repeat = DEFAULT_REPEAT;
	int play_clear = 0;
	int play_secure = 0;
	int play_uhd = 0;
	char heap_path[MAX_STRING_LENGTH] = DEFAULT_HEAP_PATH;
	char heap_data[MAX_STRING_LENGTH] = DEFAULT_HEAP_INFO;

	setvbuf(stdout, NULL, _IONBF, 0);
	
	printf("-------- Welcome in ION Monitor Tool -------- \n\n");

	while((opt = getopt(argc, argv, "f:r:csuh")) != -1) {
		switch(opt) {
			case 'f':
				printf("Heap to monitor : %s \n", optarg);
				strcpy(heap_path, optarg);
				break;	
			case 'r':
				printf("Number of repeat : %s \n", optarg);
				repeat = atoi(optarg);
				break;
			case 'c':
				printf("Clear content will be tested.\n");
				play_clear = 1;
				break;
			case 's':
				printf("Secure content will be tested.\n");
				play_secure = 1;
				break;
			case 'u':
				printf("UHD content will be tested.\n");
				play_uhd = 1;
				break;
			case 'h':
			    printf("Options : \n");
			    printf("  -f : specify path to the heap you want to monitor. (DEFAULT: %s)\n", DEFAULT_HEAP_PATH);
			    printf("  -r : specify number of time you want to repeat the playback. (DEFAULT: %d)\n", DEFAULT_REPEAT);
			    printf("  -c : test clear content.\n");
			    printf("  -s : test secure content.\n");
			    printf("  -u : test UHD content.\n");
			    printf("  -h : display this help message.\n");
			    return EXIT_SUCCESS;
			    break;
			case '?':
			    if(optopt == 'f') {
			    	return EXIT_FAILURE;
			    }
			    else {
			    	printf("Unknown option %c \n", optopt);
			    }
			    break;
		}
	}

	if( !play_clear && !play_secure && !play_uhd) {
		printf("[ERROR] You didn't select test to run. Use -h for help. \n");
		return EXIT_FAILURE;
	}

	remove("/data/grep_output");

	stream stream_list[STREAM_NUMBER+1];
	get_stream_list(stream_list, play_clear, play_secure, play_uhd);

	unlock_screen();

	for(int i = 0; stream_list[i] != last; i++) {
		char testcase[MAX_STRING_LENGTH];
		char url[MAX_STRING_LENGTH];
		int start_measure = 0;
		int end_measure = 0;
		int warning = 0;

		get_stream_name(stream_list[i], testcase);
		get_stream_url(stream_list[i], url);

		printf("-------- TESTING %s -------- \n", testcase);

		start_exoplayer();
		for(int i = 0; i < repeat; i++) {
			setTimeout(DEFAULT_WAIT);
			printf("-------- MEASURE %d -------- \n", i + 1);
			start_measure = perform_measure(heap_path, heap_data);
			printf("[INFO] Value before playback: %d \n", start_measure);
			start_stream(url);
			setTimeout(DEFAULT_PLAYTIME);
			stop_stream();
			setTimeout(DEFAULT_WAIT);
			end_measure = perform_measure(heap_path, heap_data);
			printf("[INFO] Value after playback: %d \n", end_measure);
			if( (end_measure - start_measure) > 0 ) {
				warning++;
				printf("[WARNING] Leak behavior detected. \n");
			}
			else if ( (end_measure - start_measure) < 0) {
				warning--;
				printf("[INFO] Memory has been tardily released. \n");
			}
			printf("-------- DIFFERENCE MEASURED %d -------- \n", end_measure - start_measure);
		}
		stop_exoplayer();
		if(warning >= LEAK_THRESHOLD*(double)repeat) {
			printf("[RESULT] %s FAIL\n\n", testcase);
		}
		else {
			printf("[RESULT] %s PASS\n\n", testcase);
		}
	}
	return 0;
}