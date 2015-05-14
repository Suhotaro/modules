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

#include "chdev.h"
#include "chdev_ioctl.h"


pmy_dev mycdev; /* main structure */
char data[50]; /* buffer for comunicating with user-spcae read/write */

int my_dev_open (struct inode *inode, struct file *filp)
{
	pmy_dev dv;
	dv = container_of(inode->i_cdev, struct my_dev, chardev );

	printk(KERN_NOTICE "mychdev: OPEN %s\n", dv->name );

	filp->private_data = dv;
	return 0;
}
/*------------------------------------------------------------------------------------------*/

int my_dev_release (struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "mychdev: RELEASE\n");
	return 0;
}
/*------------------------------------------------------------------------------------------*/

ssize_t my_dev_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_NOTICE "mycdev: WRITE\n");

	pmy_dev mycdev = NULL;
	int retval = 0;

	mycdev = filp->private_data;
    memset( data, 0, sizeof( data ) );

    mutex_lock(&mycdev->mutex);

    if (copy_from_user(data, buf, count)) 
    {
        retval = -EFAULT;
        mutex_unlock(&mycdev->mutex);

        return retval;
    }

    mutex_unlock(&mycdev->mutex);

	return count;
}
/*------------------------------------------------------------------------------------------*/

ssize_t my_dev_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk(KERN_NOTICE "mychdev: READ\n");

	pmy_dev mycdev = NULL;
	int retval = 0;

    mycdev = filp->private_data;
    mutex_lock(&mycdev->mutex);

	if (copy_to_user (buf, data, count))
	{
		retval = -EFAULT;
		mutex_unlock(&mycdev->mutex);

		return retval;
	}

	mutex_unlock(&mycdev->mutex);

	return count;
}
/*------------------------------------------------------------------------------------------*/

long my_dev_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_NOTICE "mycdev: IOCTL\n");

	int err = 0, retval = 0, num = 0;

	/* don't even decode wrong cmds: better returning  ENOTTY than EFAULT */
	if (_IOC_TYPE(cmd) != CHDEV_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > CHDEV_IOC_MAXNR) return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err) return -EFAULT;

	switch(cmd)
	{
		case CHDEV_IOGET:
			num = 5;
			retval = __put_user( num, (int __user *)arg);

			printk(KERN_NOTICE "mycdev: IOCTL seted val retval=%d num=%d\n", retval, num);
			break;

		case CHDEV_IOSET:
			retval = __get_user( num, (int __user *)arg);

			printk(KERN_NOTICE "mycdev: IOCTL geted val retval=%d num=%d\n", retval, num);
			break;

		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}

	return retval;
}

struct file_operations my_fops =
{
		.owner =     THIS_MODULE,
		.open =	     my_dev_open,
		.read =	     my_dev_read,
		.write =     my_dev_write,
		.unlocked_ioctl =  my_dev_ioctl,
		.release =   my_dev_release,
};

static void setup_cdev( pmy_dev mycdev, int index )
{
	int err, devno = MKDEV( major, 0 );

	strcpy( mycdev->name, "MYDEV" );
	mycdev->num = 123;

	cdev_init( &mycdev->chardev, &my_fops );
	mycdev->chardev.owner = THIS_MODULE;
	mycdev->chardev.ops = &my_fops;

	err = cdev_add (&mycdev->chardev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

static int my_dev_init(void)
{
    int res;
    dev_t dev = MKDEV(major, 0);

    printk(KERN_ALERT "mychdev: INIT\n");

    if( major )
    {
    	res = register_chrdev_region(dev, minor, "my_dev");
    }
    else
    {
    	res = alloc_chrdev_region(&dev, 0, minor, "my_dev");
    	major = MAJOR(dev);
    }

	if (res < 0)
		return res;

	strcpy( data, "Hello from driver to user\n" );
	printk(KERN_NOTICE "mychdev: dev: %d major: %d\n", dev, major );

	mycdev = kmalloc( sizeof (my_dev), GFP_KERNEL);
	if( !mycdev )
	{
		res = -ENOMEM;
		goto fail_malloc;
	}

	memset(mycdev, 0, sizeof(my_dev));

	mutex_init(&mycdev->mutex);
	init_completion(&mycdev->my_completion); /* temporary it is not used */

	setup_cdev( mycdev, 0 );

	printk(KERN_ALERT "mychdev: INIT END\n");

    return 0;

fail_malloc:
	unregister_chrdev_region(dev, minor);

	return res;
}

static void my_dev_exit(void)
{
    printk(KERN_ALERT "mychdev: DEINIT\n");

    cdev_del(&mycdev->chardev);
    kfree(mycdev);
}

module_init(my_dev_init);
module_exit(my_dev_exit);

