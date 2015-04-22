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

MODULE_LICENSE("Dual BSD/GPL");

int major = 0; /* Dynamic major default */
int minor = 1; /* Device minor number */
char data[50];

module_param(major, int, 0);
module_param(minor, int, 0);
MODULE_AUTHOR("Roman Peresipkyn");

typedef struct my_dev
{
	char name[10];
	int num;

	struct cdev chardev;
} my_dev, *pmy_dev;

pmy_dev mycdev;

int my_dev_open (struct inode *inode, struct file *filp)
{
	pmy_dev dv;
	dv = container_of(inode->i_cdev, struct my_dev, chardev );

	printk(KERN_NOTICE "*** my_dev: OPEN %s %d\n", dv->name, dv->num );

	filp->private_data = dv;
	return 0;
}

int my_dev_release (struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "*** my_dev: RELEASE\n");
	return 0;
}

int c = 3;

ssize_t my_dev_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_NOTICE "*** my_dev: WRITE\n");

    int a = 1;
    int *b = kmalloc( sizeof(int), GFP_KERNEL);
    *b = 2;

     printk(KERN_NOTICE "+++%d %p\n+++%d %p\n+++%d %p\n+++buff %p\n",
		a, &a, *b, b, c, &c, buf);




    int retval = 0;
    memset( data, 0, sizeof( data ) );

    if (copy_from_user(data, buf, count)) 
    {
        retval = -EFAULT;
        goto out;
    }

	return count;

out:
    return retval;
}

int flag = 0;

ssize_t my_dev_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{

	int retval = 0;

	printk(KERN_NOTICE "*** my_dev: READ\n");

	if( 0 == flag )
	{
		count = 50;

		if (copy_to_user (buf, data, count))
		{
			retval = -EFAULT;
			goto nothing;
		}

		flag = 1;

		return count;
	}
	else
	{
		flag = 0;
		return 0;
	}

nothing:
	return retval;

}


struct file_operations my_fops =
{
		.owner =     THIS_MODULE,
		.open =	     my_dev_open,
		.read =	     my_dev_read,
		.write =     my_dev_write,
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
	/* Fail gracefully if need be */

	printk(KERN_NOTICE "*** setup_cdev: devno-%d\n", devno );

	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

static int my_dev_init(void)
{
    int res;
    dev_t dev = MKDEV(major, 0);

    printk(KERN_ALERT "*** my_dev_init: init\n");

    if( major ) {
    	res = register_chrdev_region(dev, minor, "my_dev");
    }
    else {
    	res = alloc_chrdev_region(&dev, 0, minor, "my_dev");
    	major = MAJOR(dev);
    }
	if (res < 0)
		return res;

	strcpy( data, "Hello from driver to user\n" );
	printk(KERN_ALERT "*** my_dev_init: dev-%d major-%d\n", dev, major );

	mycdev = kmalloc( sizeof (my_dev), GFP_KERNEL);
	if( !mycdev )
	{
		res = -ENOMEM;
		goto fail_malloc;
	}

	memset( mycdev, 0, sizeof( my_dev ) );
	setup_cdev( mycdev, 0 );
	printk(KERN_ALERT "*** my_dev_init: init is finished\n");

    return 0;

fail_malloc:
	unregister_chrdev_region(dev, minor);

	return res;
}




static void my_dev_exit(void)
{
    printk(KERN_ALERT "*** my_dev_init: deinit\n");

    cdev_del(&mycdev->chardev);
    kfree(mycdev);
}

module_init(my_dev_init);
module_exit(my_dev_exit);
