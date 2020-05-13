// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Copyright © 2014 Broadcom
 */

#include <linux/seq_file.h>
#include <linux/circ_buf.h>
#include <linux/ctype.h>
#include <linux/debugfs.h>

#include "vc4_drv.h"
#include "vc4_regs.h"

/*
 * Called at drm_dev_register() time on each of the minors registered
 * by the DRM device, to attach the debugfs files.
 */
void
vc4_debugfs_init(struct drm_minor *minor)
{
	struct vc4_dev *vc4 = to_vc4_dev(minor->dev);

	debugfs_create_bool("hvs_load_tracker", S_IRUGO | S_IWUSR,
			    minor->debugfs_root, &vc4->load_tracker_enabled);
}

static int vc4_debugfs_regset32(struct seq_file *m, void *unused)
{
	struct drm_simple_info_entry *entry = m->private;
	struct debugfs_regset32 *regset = entry->file.data;
	struct drm_printer p = drm_seq_file_printer(m);

	drm_print_regset32(&p, regset);

	return 0;
}

/*
 * Registers a debugfs file with a callback function for a vc4 component.
 *
 * Various VC4 functions will register their debugfs files during the
 * component bind process using drm_debugfs_add_file().
 * These requests are tracked and delayed until their called on each
 * minor during drm_dev_register().
 */
void vc4_debugfs_add_file(struct drm_device *dev,
			  const char *name,
			  int (*show)(struct seq_file*, void*),
			  void *data)
{
	drm_debugfs_add_file(dev, name, show, data);
}

void vc4_debugfs_add_regset32(struct drm_device *drm,
			      const char *name,
			      struct debugfs_regset32 *regset)
{
	vc4_debugfs_add_file(drm, name, vc4_debugfs_regset32, regset);
}
