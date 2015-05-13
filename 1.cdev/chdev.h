#ifndef CHDEV_H_
#define CHDEV_H_

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/aio.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>

#define CHDEV_MAJOR 0 /* Dynamic major default */
#define CHDEV_MINOR 1 /* Device minor number */
#define CHDEV_IOC_MAGIC 'k'/* "k" Useing it as system number */

/* Ioctls */
#define CHDEV_IOGET _IOR( CHDEV_IOC_MAGIC, 0, int )
#define CHDEV_IOSET _IOW( CHDEV_IOC_MAGIC, 1, int )

#define CHDEV_IOC_MAXNR 2

MODULE_LICENSE("Dual BSD/GPL");

int major = CHDEV_MAJOR;
int minor = CHDEV_MINOR;

module_param(major, int, 0);
module_param(minor, int, 0);
MODULE_AUTHOR("Roman Peresipkyn");

typedef struct my_dev
{
	char name[10];
	int num;

	struct cdev chardev; /* chardev file operations */
	struct mutex mutex; /* Mutual exclusion */
	struct completion my_completion;

} my_dev, *pmy_dev;


#endif /* CHDEV_H_ */
