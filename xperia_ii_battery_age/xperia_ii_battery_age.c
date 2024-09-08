#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <linux/dcache.h>
#include <linux/sched.h>
#include <linux/mnt_namespace.h>
#include <linux/proc_fs.h>
#include <linux/namei.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Kernel module to unmount Termux mount point");

static struct vfsmount *get_mount_by_path(const char *path)
{
    struct path p;
    struct vfsmount *mnt = NULL;
    int ret;

    ret = kern_path(path, LOOKUP_FOLLOW, &p);
    if (ret) {
        printk(KERN_ERR "Failed to resolve path: %s\n", path);
        return NULL;
    }

    mnt = p.mnt;
    path_put(&p);

    return mnt;
}

static int unmount_path(const char *path)
{
    struct vfsmount *mnt;

    mnt = get_mount_by_path(path);
    if (!mnt) {
        printk(KERN_ERR "Failed to get mount for path: %s\n", path);
        return -EINVAL;
    }

    printk(KERN_INFO "Attempting to unmount: %s\n", path);
    if (mnt) {
        int ret = do_umount(mnt, MNT_DETACH);
        if (ret == 0) {
            printk(KERN_INFO "Successfully unmounted: %s\n", path);
        } else {
            printk(KERN_ERR "Failed to unmount: %s\n", path);
        }
        return ret;
    }

    return -ENOENT;
}

static int __init termux_unmount_init(void)
{
    printk(KERN_INFO "Termux unmount module loaded\n");

    // 卸载 Termux 的挂载点
    unmount_path("/data/data/com.termux");

    return 0;
}

static void __exit termux_unmount_exit(void)
{
    printk(KERN_INFO "Termux unmount module unloaded\n");
}

module_init(termux_unmount_init);
module_exit(termux_unmount_exit);
