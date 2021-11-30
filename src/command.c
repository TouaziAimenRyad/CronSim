#include "../include/includes.h"
#include "../include/command.h"

uint32_t convert_char_to_uint32(char *str) 
{
	uint32_t result = (str[0] << 24 | str[1] << 16 | str[2] << 8 | str[3]);
    return result;

}




uint32_t *convert_char_to_ARGV(char * str,uint32_t u, uint32_t *t)
{	
	u = convert_char_to_uint32(str);
	t = &u;
    return(t);
}

