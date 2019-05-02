#ifndef PARSE_H
#define PARSE_H

#include "defaults.h"

struct parse_info {
	char delim[2];
	char last_output[MAX_OUTPUT][MAX_STRING_LENGTH];
	int nb_line;
};

void init_parse_info(struct parse_info* info, char* delim);

void last_output_add_line(struct parse_info* info, char* line, int pos);
void last_output_reset_line(struct parse_info* info);

int write_line_to_file(char* file_path, char* str);

int simple_grep(struct parse_info* info, char* file_path, char* needle);
int simple_cut(struct parse_info* info, char* file_path, int pos);

#endif