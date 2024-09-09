/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2024 bmax121. All Rights Reserved.
 * Copyright (C) 2024 lzghzr. All Rights Reserved.
 */
#ifndef __XIIBA_UTILS_H
#define __XIIBA_UTILS_H

#include <hook.h>
#include <ksyms.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include <uapi/asm-generic/errno.h>

#define ERR_LOOKUP_FAILED  -21
#define ERR_FUNC_NULL      -22
#define ERR_HOOK_FAILED    -23

#define lookup_name(func)                                  \
  func = 0;                                                \
  func = (typeof(func))kallsyms_lookup_name(#func);        \
  pr_info("kernel function %s addr: %llx\n", #func, func); \
  if (!func)                                               \
  {                                                        \
    return ERR_LOOKUP_FAILED;                              \
  }

#define hook_func(func, argv, before, after, udata)                         \
  if (!func)                                                                \
  {                                                                         \
    return ERR_FUNC_NULL;                                                   \
  }                                                                         \
  hook_err_t hook_err_##func = hook_wrap(func, argv, before, after, udata); \
  if (hook_err_##func)                                                      \
  {                                                                         \
    func = 0;                                                               \
    pr_err("hook %s error: %d\n", #func, hook_err_##func);                  \
    return ERR_HOOK_FAILED;                                                 \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    pr_info("hook %s success\n", #func);                                    \
  }

#define unhook_func(func)            \
  if (func && !is_bad_address(func)) \
  {                                  \
    unhook(func);                    \
    func = 0;                        \
  }

#define is_bad_address(ptr) (!ptr || IS_ERR_VALUE(ptr))

#endif /* __XIIBA_UTILS_H */
