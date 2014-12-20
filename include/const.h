#ifndef _CONST_H
#define _CONST_H

#include <sys/stat.h>

#define O_PERM_CREAT	(O_RDWR | O_CREAT | O_TRUNC | O_EXCL)

#define S_DEFAULT	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#endif
