#ifndef DEBUG_H
# define DEBUG_H

# include <stdio.h>
# include <stdlib.h>

#define NO_DEBUG_ALLOC


void	*debug_malloc(size_t size, const char *file, int line);
void	debug_free(void *ptr, const char *file, int line);

#ifndef NO_DEBUG_ALLOC
# define malloc(x) debug_malloc(x, __FILE__, __LINE__)
# define free(x) debug_free(x, __FILE__, __LINE__)
#endif


#endif
