/* linux/drivers/media/video/samsung/fimg2d4x/fimg2d_helper.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *	http://www.samsung.com/
 *
 * Samsung Graphics 2D driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include "fimg2d.h"
#include "fimg2d_cache.h"
#include "fimg2d_helper.h"

void perf_print(struct fimg2d_context *ctx, int seq_no)
{
	int i;
	long time;
	struct fimg2d_perf *perf;

	for (i = 0; i < MAX_PERF_DESCS; i++) {
		perf = &ctx->perf[i];
		if (perf->valid != 0x11)
			continue;
		time = elapsed_usec(ctx, i);
		printk(KERN_INFO "[FIMG2D PERF (%8s)] ctx(0x%08x) seq(%d) "
				"%8ld   usec\n",
				perfname(i), (unsigned int)ctx, seq_no, time);
	}
	printk(KERN_INFO "[FIMG2D PERF **]\n");
}

void fimg2d_dump_command(struct fimg2d_bltcmd *cmd)
{
	int i;
	struct fimg2d_param *p = &cmd->param;
	struct fimg2d_image *img;
	struct fimg2d_rect *r;
	struct fimg2d_dma *c;

	printk(KERN_INFO " op: %d\n", cmd->op);
	printk(KERN_INFO " solid color: 0x%lx\n", p->solid_color);
	printk(KERN_INFO " g_alpha: 0x%x\n", p->g_alpha);
	printk(KERN_INFO " premultiplied: %d\n", p->premult);
	if (p->dither)
		printk(KERN_INFO " dither: %d\n", p->dither);
	if (p->rotate)
		printk(KERN_INFO " rotate: %d\n", p->rotate);
	if (p->repeat.mode) {
		printk(KERN_INFO " repeat: %d, pad color: 0x%lx\n",
				p->repeat.mode, p->repeat.pad_color);
	}
	if (p->bluscr.mode) {
		printk(KERN_INFO " bluescreen: %d, bs_color: 0x%lx "
				"bg_color: 0x%lx\n",
				p->bluscr.mode, p->bluscr.bs_color,
				p->bluscr.bg_color);
	}
	if (p->scaling.mode) {
		printk(KERN_INFO " scaling %d, s:%d,%d d:%d,%d\n",
				p->scaling.mode,
				p->scaling.src_w, p->scaling.src_h,
				p->scaling.dst_w, p->scaling.dst_h);
	}
	if (p->clipping.enable) {
		printk(KERN_INFO " clipping LT(%d,%d) RB(%d,%d) WH(%d,%d)\n",
				p->clipping.x1, p->clipping.y1,
				p->clipping.x2, p->clipping.y2,
				rect_w(&p->clipping), rect_h(&p->clipping));
	}

	for (i = 0; i < MAX_IMAGES; i++) {
		img = &cmd->image[i];
		if (!img->addr.type)
			continue;

		r = &img->rect;

		printk(KERN_INFO " %s type: %d addr: 0x%lx\n",
				imagename(i), img->addr.type, img->addr.start);
		if (img->plane2.type) {
			printk(KERN_INFO " %s type: %d plane2: 0x%lx\n",
					imagename(i), img->plane2.type,
					img->plane2.start);
		}
		printk(KERN_INFO " %s width: %d height: %d "
				"stride: %d order: %d format: %d\n",
				imagename(i), img->width, img->height,
				img->stride, img->order, img->fmt);
		printk(KERN_INFO " %s rect LT(%d,%d) RB(%d,%d) WH(%d,%d)\n",
				imagename(i), r->x1, r->y1, r->x2, r->y2,
				rect_w(r), rect_h(r));

		c = &cmd->dma[i].base;
		if (c->size) {
			printk(KERN_INFO " %s dma base addr: 0x%lx "
					"size: 0x%x cached: 0x%x\n",
					imagename(i), c->addr, c->size,
					c->cached);
		}

		c = &cmd->dma[i].plane2;
		if (c->size) {
			printk(KERN_INFO " %s dma plane2 addr: 0x%lx "
					"size: 0x%x cached: 0x%x\n",
					imagename(i), c->addr, c->size,
					c->cached);
		}
	}

	if (cmd->dma_all)
		printk(KERN_INFO " dma size all: 0x%x bytes\n", cmd->dma_all);

	printk(KERN_INFO " ctx: %p seq_no(%u) sync(%d)\n",
				cmd->ctx, cmd->seq_no, cmd->sync);
}
