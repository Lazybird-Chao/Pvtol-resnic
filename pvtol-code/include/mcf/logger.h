#ifndef LOGGER_H_
#define LOGGER_H_

#ifdef _PVTOL_CELL_LOGGER
    #define PROGRESS( which )                  \
     {                                         \
        printf( "L%d %s\n", __LINE__, which ); \
     }
#else
     #define PROGRESS( which );
#endif

#endif /*LOGGER_H_*/

