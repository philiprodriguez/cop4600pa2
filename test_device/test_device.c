#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h> // Has copy to user function
#define DEVICE_NAME "testdev"
#define CLASS_NAME "test"
#define BUFFER_SIZE 1024

static int majorNumber;
struct class * testdeviceClass;
struct device * testdeviceDevice;

MODULE_LICENSE("GPL");

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release
};

// The buffer in which we will store characters!
char message[BUFFER_SIZE] = {0};
short size_of_message;

int init_module(void)
{
	printk(KERN_INFO "Initializing the test device...\n");

	// Assign a major number
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);

	// Did we succeed?
	if (majorNumber < 0)
	{
		// We failed.
		printk(KERN_ALERT "Failed to assign test device a major number!\n");
		return majorNumber;
	}
	// We succeeded!
	printk("Registered test device with major number %d.\n", majorNumber);

	// Register device class
	testdeviceClass = class_create(THIS_MODULE, CLASS_NAME);

	// Did we succeed?
	if (IS_ERR(testdeviceClass))
	{
		// Nope.
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create test device class!\n");
		return PTR_ERR(testdeviceClass);
	}
	// We succeeded!
	printk(KERN_INFO "Created test device class.\n");

	// Register the device driver
	testdeviceDevice = device_create(testdeviceClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);

	// Did we succeed?
	if (IS_ERR(testdeviceDevice))
	{
		// Nope.
		class_destroy(testdeviceClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the test device!\n");
		return PTR_ERR(testdeviceDevice);
	}
	// We succeeded!
	printk(KERN_INFO "Successfully created test device!\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up test device!\n");

	device_destroy(testdeviceClass, MKDEV(majorNumber, 0));
	class_unregister(testdeviceClass);
	class_destroy(testdeviceClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "Test device cleaned up!\n");
}

static int dev_open(struct inode * inodep, struct file * filep)
{
	printk(KERN_INFO "Test device opened.\n");
	return 0;
}

static int dev_release(struct inode * inodep, struct file * filep)
{
	printk(KERN_INFO "Test device closed.\n");
	return 0;
}

static ssize_t dev_read(struct file * filep, char * buffer, size_t len, loff_t * offset)
{
	int error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count == 0)
	{
		printk(KERN_INFO "Successfully sent characters to user program.\n");
		return 0;
	}
	else
	{
		printk(KERN_INFO "Failed to send characters to user program!\n");
		return -EFAULT;
	}
}

static ssize_t dev_write(struct file * filep, const char * buffer, size_t len, loff_t * offset)
{
	char rec[1024];
	if (len < BUFFER_SIZE) {
		sprintf(message, "%s", buffer);
	} else {
		snprintf(message, BUFFER_SIZE, "%s", buffer);
	}
	sprintf(rec, "%s(%zu letters)", message, len);
	size_of_message = strlen(message);
	printk(KERN_INFO "Test device received %zu characters: %s\n", len, rec);
	return len;
}
