#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "defaults.h"
#include "parse.h"

void setTimeout(int milliseconds)
{
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    int end = milliseconds_since + milliseconds;

    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

int print_file(char* filename) {
	FILE* fp = NULL;
	char line[MAX_STRING_LENGTH] = "";

	fp = fopen(filename, "r");

	if(fp == NULL) {
		printf("%s : File %s couldn't be opened.\n", __func__, filename);
		return EXIT_FAILURE;
	}

	while(fgets(line, MAX_STRING_LENGTH, fp) != NULL) {
		printf("%s", line);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int get_info_value(char* heap_path, char* info_name, int word_count) {
	struct parse_info info;
	int ret = -1;

	init_parse_info(&info, " ");

	if(!simple_grep(&info, heap_path, info_name)) {
		printf("[WARNING] simple_grep has found nothing.\n");
	}
	for(int i = 0; i < info.nb_line; i++) {
		write_line_to_file("/data/grep_output", info.last_output[i]);
		
	}

	if(!simple_cut(&info, "/data/grep_output", word_count + 1)) {
		printf("[WARNING] simple_cut has found nothing.\n");
	}

	remove("/data/grep_output");

	ret = atoi(info.last_output[0]);
	return ret;
}

int perform_measure(char* heap_path, char* heap_data) {
	return get_info_value(heap_path, heap_data, 2);
}

int write_line_to_file(char* file_path, char* str) {
	FILE *fp = fopen(file_path, "w+");
    int ret = -1;

	if(fp == NULL) {
		printf("%s : File %s couldn't be opened.\n", __func__, file_path);
		return ret;
	} 
	ret = fprintf(fp, "%s", str);
	fclose(fp);
	return ret;
}

int append_line_to_file(char* file_path, char* str) {
	FILE *fp = fopen(file_path, "a");
    int ret = -1;

	if(fp == NULL) {
		printf("%s : File %s couldn't be opened.\n", __func__, file_path);
		return ret;
	} 
	ret = fprintf(fp, "%s", str);
	fclose(fp);
	return ret;
}

void export_info(char* heap_path) {
	char str[50] = "";

	int allocated_size;
	int free_size;
	int allocated_peak;
	int largest_free_buffer;
	int heap_fragmentation;

	time_t rawtime;
	struct tm* timestamp;
	time(&rawtime);
	timestamp = localtime(&rawtime);

	allocated_size = get_info_value(heap_path, "allocated size", 2);
	free_size = get_info_value(heap_path, "free size", 2);
	allocated_peak = get_info_value(heap_path, "allocated peak", 2);
	largest_free_buffer = get_info_value(heap_path, "largest free buffer", 3);
	heap_fragmentation = get_info_value(heap_path, "heap fragmentation", 2);

	sprintf(str, "%d:%d:%d %d\n", timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, allocated_size);
	append_line_to_file("allocated_size.log", str);

	sprintf(str, "%d:%d:%d %d\n", timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, free_size);
	append_line_to_file("free_size.log", str);

	sprintf(str, "%d:%d:%d %d\n", timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, allocated_peak);
	append_line_to_file("allocated_peak.log", str);

	sprintf(str, "%d:%d:%d %d\n", timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, largest_free_buffer);
	append_line_to_file("largest_free_buffer.log", str);

	sprintf(str, "%d:%d:%d %d\n", timestamp->tm_hour, timestamp->tm_min, timestamp->tm_sec, heap_fragmentation);
	append_line_to_file("heap_fragmentation.log", str);
}