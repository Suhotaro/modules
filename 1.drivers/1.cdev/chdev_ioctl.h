#ifndef CHDEV_IOCTL_H_
#define CHDEV_IOCTL_H_

#include <linux/types.h>
#include <asm/ioctl.h>

#define CHDEV_IOC_MAGIC 'k'/* "k" Useing it as system number */

/* Ioctls */
#define CHDEV_IOGET _IOR( CHDEV_IOC_MAGIC, 0, int )
#define CHDEV_IOSET _IOW( CHDEV_IOC_MAGIC, 1, int )

#define CHDEV_IOC_MAXNR 2

#endif /* chdev_ioctl.h */
