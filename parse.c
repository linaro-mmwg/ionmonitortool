#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "include/parse.h"

void init_parse_info(struct parse_info* info, char* delim) {
	strcpy(info->delim, delim);
	info->nb_line = 0;
}

void last_output_add_line(struct parse_info* info, char* line, int pos) {
	strcpy(info->last_output[pos], line);
	info->nb_line++;
}

void last_output_reset_line(struct parse_info* info) {
	info->nb_line = 0;
}

int simple_grep(struct parse_info* info, char* file_path, char* needle) {
	FILE* fp = NULL;
	char line[MAX_STRING_LENGTH] = "";
	char* ret = NULL;
	int line_added = 0;

	fp = fopen(file_path, "r");

	if(fp == NULL) {
		printf("%s : File %s couldn't be opened.\n", __func__, file_path);
		return -1;
	}

	last_output_reset_line(info);

	while(fgets(line, MAX_STRING_LENGTH, fp) != NULL) {
		ret = strstr(line, needle);
		if(ret != NULL) {
			last_output_add_line(info, line, info->nb_line);
			line_added++;		
		}
	}
	fclose(fp);
	return line_added;
}

int simple_cut(struct parse_info* info, char* file_path, int pos) {
	FILE* fp = NULL;
	char line[MAX_STRING_LENGTH] = "";
	char* ret = NULL;
	int count = 0;
	int line_added = 0;
	fp = fopen(file_path, "r");

	if(fp == NULL) {
		printf("%s : File %s couldn't be opened.\n", __func__, file_path);
		return -1;
	}

	last_output_reset_line(info);

	while(fgets(line, MAX_STRING_LENGTH, fp) != NULL) {
		ret = strtok(line, info->delim);
		while(ret != NULL) {
			count++;
			if(count == pos) {
				last_output_add_line(info, ret, info->nb_line);
				count = 0;
				line_added++;
			}
			ret = strtok(NULL, info->delim);
		}
	}
	fclose(fp);
	return line_added;
}
