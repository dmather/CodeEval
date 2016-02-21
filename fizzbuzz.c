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
        exit(1);
    }

    if(line == NULL) {
        exit(1);
    }
    
    // Get ourselves a single character
    do
    {
        ic = fgetc(file);
        if(ic == EOF)
            return -1;

        if(char_count == LINE_LENGTH)
        {
            fprintf(stderr, "line too long\n");
            exit(1);
        }
        else
        {
            line[char_count] = ic;
            char_count++;
        }
    }
    while (ic != EOF && ic != '\n');
}

int parse_line(int *fd, int *sd, int *count, char *line)
{
    char buf[3];
    int i = 0;
    int buf_iter = 0;
    char single_char;
    int space_count = 0;
    if(line == NULL)
        return 1;

    //printf("%s", line);

    do
    {
        //printf("%c\n", line[i]);
        single_char = line[i];
        if(single_char == ' ')
        {
            //printf("Hello 1\n");
            //printf("%s\n", buf);
            if(space_count == 0)
            {
                *fd = atoi(buf);
                //printf("%d\n", *fd);
            }
            else if(space_count == 1)
            {
                *sd = atoi(buf);
                //printf("%d\n", *sd);
            }
            else if(space_count == 2)
            {
                *count = atoi(buf);
                //printf("%d\n", *count);
            }
            for(buf_iter = 0; buf_iter < 3; buf_iter++)
            {
                buf[buf_iter] = '\0';
            }
            buf_iter = 0;
            space_count++;
        }
        else
        {
            //printf("Hello 2\n");
            buf[buf_iter] = single_char;
            buf_iter++;
            //printf("Buffer: %s\n", buf);
        }
        i++;
    }
    while(single_char != '\0');
    
    if(space_count == 2) {
        *count = atoi(buf);
        //printf("%d\n", *count);
    }
    return 0;
}

void play_game(int fd, int sd, int count)
{
    char out_line[2048];
    int char_iter = 0;
    int i;
    //printf("First divisor: %d, second divisor: %d, count: %d\n",
    //        fd, sd, count);
    for(i = 1; i <= count; i++)
    {
        //printf("%d\n", i);
        if(i % fd == 0) {
            out_line[char_iter] = 'F';
            char_iter++;
            out_line[char_iter] = ' ';
            char_iter++;
        } else if(i % sd == 0) {
            out_line[char_iter] = 'B';
            char_iter++;
            out_line[char_iter] = ' ';
            char_iter++;
        } else if( (i % fd == 0) && (i % sd == 0) ) {
            out_line[char_iter] = 'F';
            char_iter++;
            out_line[char_iter] = 'B';
            char_iter++;
        }
        else {
            char num_var[3];
            sprintf(num_var, "%d", i);
            //printf("Number: %s\n", num_var);
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

    printf("%s\n", out_line);
}

int main(int argc, char **argv)
{
    char file_name[1024];
    FILE *game_file;

    if(argc >= 1) {
        //strncpy(file_name, argv[1], sizeof(file_name));
        game_file = fopen(argv[1], "r"); 

        //printf("Opened file\n");
        // Create our buffer and initialize it with empties
        char *line_buf = NULL;    
        line_buf = (char*)calloc(LINE_LENGTH, sizeof(char));

        int dv1;
        int dv2;
        int count;
        
        int get_line_success = 0;

        do
        {
            get_line_success = get_line(game_file, line_buf);
            if(get_line_success == -1)
                break;

            //printf("%s", line_buf);
            parse_line(&dv1, &dv2, &count, line_buf);
            play_game(dv1, dv2, count);
        }
        while(1);

        if(game_file != NULL) {
            fclose(game_file);
        }

        free(line_buf);
    }

    return 0;
}
