/* SPDX-License-Identifier: GPL-2.0-or-later */
/* 
 * Copyright (C) 2023 bmax121. All Rights Reserved.
 */

#include <compiler.h>
#include <kpmodule.h>
#include <linux/printk.h>
#include <linux/mount.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>

KPM_NAME("kpm-termux-unmount");
KPM_VERSION("1.0.0");
KPM_LICENSE("GPL v2");
KPM_AUTHOR("bmax121");
KPM_DESCRIPTION("KernelPatch Module to Unmount Termux Mount Points");

static const char *termux_mount_path = "/data/data/com.termux/files";  // Termux 默认的挂载路径

static int unmount_termux_fs(void)
{
    struct path path;
    int err;

    pr_info("kpm-termux-unmount: Trying to unmount Termux at %s\n", termux_mount_path);

    // 根据路径查找挂载点
    err = kern_path(termux_mount_path, LOOKUP_FOLLOW, &path);
    if (err) {
        pr_err("kpm-termux-unmount: Failed to find path %s, error: %d\n", termux_mount_path, err);
        return err;
    }

    // 尝试卸载挂载点
    err = vfs_umount(&path, MNT_FORCE);
    if (err) {
        pr_err("kpm-termux-unmount: Failed to unmount %s, error: %d\n", termux_mount_path, err);
    } else {
        pr_info("kpm-termux-unmount: Successfully unmounted %s\n", termux_mount_path);
    }

    path_put(&path);  // 释放路径
    return err;
}

static long kpm_termux_unmount_init(const char *args, const char *event, void *__user reserved)
{
    pr_info("kpm-termux-unmount: Initializing...\n");

    // 尝试卸载Termux挂载点
    return unmount_termux_fs();
}

static long kpm_termux_unmount_exit(void *__user reserved)
{
    pr_info("kpm-termux-unmount: Exiting...\n");
    return 0;
}

KPM_INIT(kpm_termux_unmount_init);
KPM_EXIT(kpm_termux_unmount_exit);
