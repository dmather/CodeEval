#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 64

int get_line(FILE *file, char *line)
{
    memset(line, '\0', sizeof(char) * LINE_LENGTH);
    char ic;
    int char_count = 0;
    
    if(file == NULL) {
        free(line);
        exit(1);
    }

    if(line == NULL) {
        free(line);
        exit(1);
    }
    
    // Get ourselves a single character and check it
    do
    {
        ic = fgetc(file);
        // If we're at the end of the file then return
        if(ic == EOF)
            return -1;

        if(char_count == LINE_LENGTH)
        {
            fprintf(stderr, "line too long\n");
            free(line);
            exit(1);
        }
        else
        {
            line[char_count] = ic;
            char_count++;
        }
    }
    while (ic != '\n');
}

int parse_line(int *fd, int *sd, int *count, char *line)
{
    char buf[6];
    int i = 0;
    int buf_iter = 0;
    char single_char;
    int space_count = 0;
    if(line == NULL)
        return 1;

    do
    {
        single_char = line[i];
        if(single_char == ' ')
        {
            if(space_count == 0)
            {
                buf[buf_iter] = '\0';
                *fd = atoi(buf);
            }
            else if(space_count == 1)
            {
                buf[buf_iter] = '\0';
                *sd = atoi(buf);
            }
            else if(space_count == 2)
            {
                buf[buf_iter] = '\0';
                *count = atoi(buf);
            }
            for(buf_iter = 0; buf_iter < 6; buf_iter++)
            {
                buf[buf_iter] = '\0';
            }
            buf_iter = 0;
            space_count++;
        }
        else
        {
            buf[buf_iter] = single_char;
            buf_iter++;
        }
        i++;
    }
    while(single_char != '\0');
    
    if(space_count == 2)
        *count = atoi(buf);

    return 0;
}

void play_game(int *fd, int *sd, int *count)
{
    char out_line[2048];
    int char_iter = 0;
    int i;
    for(i = 1; i <= *count; i++)
    {
        if(i % *fd == 0)
        {
            out_line[char_iter] = 'F';
            char_iter++;
            if(i % *sd == 0)
            {
                out_line[char_iter] = 'B';
                char_iter++;
                out_line[char_iter] = ' ';
                char_iter++;
            }
            else
            {
                out_line[char_iter] = ' ';
                char_iter++;
            }
        }
        else if(i % *sd == 0)
        {
            out_line[char_iter] = 'B';
            char_iter++;
            out_line[char_iter] = ' ';
            char_iter++;
        }
        else
        {
            char num_var[5];
            sprintf(num_var, "%d", i);
            int j = 0;
            do
            {
                out_line[char_iter] = num_var[j];
                char_iter++;
                j++;
            } while(num_var[j] != '\0');
            out_line[char_iter] = ' ';
            char_iter++;
        }
    }
    out_line[char_iter] = '\0';

    printf("%s\n", out_line);
}

int main(int argc, char **argv)
{
    char file_name[1024];
    FILE *game_file;

    if(argc >= 1) {
        game_file = fopen(argv[1], "r"); 

        // Create our buffer and initialize it with empties
        char *line_buf = NULL;    
        line_buf = (char*)calloc(LINE_LENGTH, sizeof(char));

        int dv1 = 0;
        int dv2 = 0;
        int count = 0;
        
        int get_line_success = 0;

        do
        {
            get_line_success = get_line(game_file, line_buf);
            if(get_line_success == -1)
                break;

            parse_line(&dv1, &dv2, &count, line_buf);
            play_game(&dv1, &dv2, &count);
        }
        while(1);

        if(game_file != NULL) {
            fclose(game_file);
        }

        free(line_buf);
    }

    return 0;
}
