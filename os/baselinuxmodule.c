#include<linux/kernel.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include <linux/version.h>

#define procfs_name "tsu"
static struct proc_dir_entry *our_proc_file = NULL;

int init_module(void) {
pr_info("Hello User!\n");
return 0;
}
void cleanup_module(void) {
pr_info("Goodbye User!\n");
}
MODULE_LICENSE("GPL");
