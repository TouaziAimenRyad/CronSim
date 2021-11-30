#ifndef COMMAND_H
#define COMMAND_H


struct command_line {
    uint32_t ARGC;
    uint32_t *ARGV;
    
};

// char * * getwords(char * str);
uint32_t convert_char_to_uint32(char *str);
uint32_t *convert_char_to_ARGV(char * str,uint32_t u, uint32_t *t);

#endif /* COMMAND_H*/
