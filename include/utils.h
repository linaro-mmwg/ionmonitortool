#ifndef UTILS_H
#define UTILS_H

void setTimeout(int milliseconds);
int print_file(char* filename);
int get_info_value(char* heap_path, char* info_name, int word_count);
int perform_measure(char* heap_path, char* heap_data);
int write_line_to_file(char* file_path, char* str);
int append_line_to_file(char* file_path, char* str);
void export_info(char* heap_path);

#endif