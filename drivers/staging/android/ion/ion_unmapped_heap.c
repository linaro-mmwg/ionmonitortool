/*
 * drivers/staging/android/ion/ion_unmapped_heap.c
 *
 * Copyright (C) 2016-2017 Linaro, Inc.
 * Copyright (C) Allwinner 2014
 * Author: <sunny@allwinnertech.com> for Allwinner.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * ION heap type for handling physical memory heap not mapped
 * in the linux-based OS.
 *
 * "unmapped heap" buffers are default not mapped but buffer owner
 * can explicitly request mapping for some specific purpose.
 *
 * Based on Allwinner work (allocation thru gen_pool) and
 * HiSilicon work (create ION heaps from DT nodes,
 * Author: Chen Feng <puck.chen@hisilicon.com>).
 */

#include <linux/err.h>
#include <linux/errno.h>
#include <linux/genalloc.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include "ion.h"

#define MAX_UNMAPPED_AREA 2

struct rmem_unmapped {
	phys_addr_t base;
	phys_addr_t size;

	char name[32];
};
static struct rmem_unmapped unmapped_data[MAX_UNMAPPED_AREA] = {0};
uint32_t unmapped_count = 0;

struct ion_unmapped_heap {
	struct ion_heap heap;
	struct gen_pool *pool;
	phys_addr_t base;
	size_t          size;
};

struct unmapped_buffer_priv {
	phys_addr_t base;
};

static phys_addr_t get_buffer_base(struct unmapped_buffer_priv *priv)
{
	return priv->base;
}

static struct device *heap2dev(struct ion_heap *heap)
{
	return heap->dev->dev.this_device;
}

#ifdef CONFIG_ION_MONITOR

/**
 * update_unmapped_heap_info - Update the debug info of the heap
 * @heap: ion heap
 */
static void update_unmapped_heap_info(struct ion_heap *heap)
{
	struct ion_unmapped_heap *umh =
		container_of(heap, struct ion_unmapped_heap, heap);
	struct gen_pool_meta *meta =
		container_of(umh->pool, struct gen_pool_meta, pool);
	
	size_t free_size, allocated_size;
	size_t heap_size, largest_free_buf_size;
	size_t allocated_peak;

	free_size = gen_pool_avail(&meta->pool);
	heap_size = get_meta(meta, HEAP_SIZE);
	allocated_peak = get_meta(meta, ALLOC_PEAK);
	allocated_size = heap_size - free_size;
	allocated_size > allocated_peak ? set_meta(meta, ALLOC_PEAK, allocated_size) : 1;
	largest_free_buf_size = gen_pool_largest_free_buf(&meta->pool);

	set_meta(meta, FREE_SIZE, free_size);
	set_meta(meta, ALLOCATED_SIZE, allocated_size);
	set_meta(meta, LARGEST_FREE_BUF, largest_free_buf_size);
}

/**
 *  get_unmapped_heap_meta - get a metadata value
 *  @heap: pointer to ion heap
 *  @name: name of the metadata to read
 *
 *  returns the requested metadata value
 */
static size_t get_unmapped_heap_meta(struct ion_heap *heap, int name)
{
	struct ion_unmapped_heap *unmapped_heap =
		container_of(heap, struct ion_unmapped_heap, heap);
	struct gen_pool_meta *meta =
		container_of(unmapped_heap->pool, struct gen_pool_meta, pool);
	return get_meta(meta, name);
}

#endif /* CONFIG_ION_MONITOR */

static phys_addr_t ion_unmapped_allocate(struct ion_heap *heap,
					   unsigned long size,
					   phys_addr_t *addr)
{
	struct ion_unmapped_heap *umh =
		container_of(heap, struct ion_unmapped_heap, heap);
	unsigned long offset = gen_pool_alloc(umh->pool, size);

	if (!offset) {
		dev_err(heap2dev(heap),
			"%s(%d) err: alloc 0x%08x bytes failed\n",
			__func__, __LINE__, (u32)size);
		return false;
	}

	*addr = offset;
	return true;
}

static void ion_unmapped_free(struct ion_heap *heap, phys_addr_t addr,
			    unsigned long size)
{
	struct ion_unmapped_heap *umh =
		container_of(heap, struct ion_unmapped_heap, heap);

	gen_pool_free(umh->pool, addr, size);
}

static struct sg_table *ion_unmapped_heap_map_dma(struct ion_heap *heap,
						struct ion_buffer *buffer)
{
	struct sg_table *table;
	int ret;

	table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table)
		return ERR_PTR(-ENOMEM);
	ret = sg_alloc_table(table, 1, GFP_KERNEL);
	if (ret) {
		kfree(table);
		return ERR_PTR(ret);
	}
	sg_set_page(table->sgl,
		    phys_to_page(get_buffer_base(buffer->priv_virt)),
		    buffer->size, 0);

	return table;
}

void ion_unmapped_heap_unmap_dma(struct ion_heap *heap,
				struct ion_buffer *buffer)
{
	sg_free_table(buffer->sg_table);
	kfree(buffer->sg_table);
}

static int ion_unmapped_heap_allocate(struct ion_heap *heap,
				    struct ion_buffer *buffer,
				    unsigned long size,
				    unsigned long flags)
{
	struct unmapped_buffer_priv *priv;
	phys_addr_t base;
	int rc = -EINVAL;

	if (!ion_unmapped_allocate(heap, size, &base))
		return -ENOMEM;

	priv = devm_kzalloc(heap2dev(heap), sizeof(*priv), GFP_KERNEL);
	if (IS_ERR_OR_NULL(priv)) {
		rc = -ENOMEM;
		goto err;
	}

	priv->base = base;
	buffer->size = roundup(size, PAGE_SIZE);
	buffer->priv_virt = priv;

	buffer->sg_table = ion_unmapped_heap_map_dma(heap, buffer);
	if (!buffer->sg_table) {
		rc = -ENOMEM;
		goto err;
	}

	sg_dma_address(buffer->sg_table->sgl) = priv->base;
	sg_dma_len(buffer->sg_table->sgl) = size;
	return 0;
err:
	ion_unmapped_free(heap, base, size);
	devm_kfree(heap2dev(heap), priv);
	buffer->priv_virt = NULL;
	return rc;
}

static void ion_unmapped_heap_free(struct ion_buffer *buffer)
{
	struct ion_heap *heap = buffer->heap;


	ion_unmapped_heap_unmap_dma(heap, buffer);
	ion_unmapped_free(heap, get_buffer_base(buffer->priv_virt),
			 buffer->size);
	devm_kfree(heap2dev(heap), buffer->priv_virt);
	buffer->priv_virt = NULL;
}

static int ion_unmapped_heap_map_user(struct ion_heap *heap,
				    struct ion_buffer *buffer,
				    struct vm_area_struct *vma)
{
	phys_addr_t pa = get_buffer_base(buffer->priv_virt);

	/*
	 * when user call ION_IOC_ALLOC not with ION_FLAG_CACHED, ion_mmap will
	 * change vma->vm_page_prot to pgprot_writecombine itself, so we do not
	 * need change to pgprot_writecombine here manually.
	 */
	return remap_pfn_range(vma, vma->vm_start,
				__phys_to_pfn(pa) + vma->vm_pgoff,
				vma->vm_end - vma->vm_start,
				vma->vm_page_prot);
}

static struct ion_heap_ops unmapped_heap_ops = {
	.allocate = ion_unmapped_heap_allocate,
	.free = ion_unmapped_heap_free,
	.map_user = ion_unmapped_heap_map_user,
	.map_kernel = ion_heap_map_kernel,
	.unmap_kernel = ion_heap_unmap_kernel,
};

/*struct ion_heap *ion_unmapped_heap_create(struct ion_platform_heap *pheap)
{
	struct ion_unmapped_heap *umh;

	if (pheap->type != ION_HEAP_TYPE_UNMAPPED)
		return NULL;

	umh = kzalloc(sizeof(struct ion_unmapped_heap), GFP_KERNEL);
	if (!umh)
		return ERR_PTR(-ENOMEM);

	umh->pool = gen_pool_create(PAGE_SHIFT, -1);
	if (!umh->pool) {
		kfree(umh);
		return ERR_PTR(-ENOMEM);
	}
	umh->base = pheap->base;
	umh->size = pheap->size;

	gen_pool_add(umh->pool, umh->base, pheap->size, -1);
	umh->heap.ops = &unmapped_heap_ops;
	umh->heap.type = ION_HEAP_TYPE_UNMAPPED;

	return &umh->heap;
}*/

static int ion_unmapped_heap_debug_show(struct ion_heap *heap, struct seq_file *s, void *unused)
{
	#ifdef CONFIG_ION_MONITOR

	if(!heap->debug_state) {
		seq_puts(s, "\n ION monitor tool is disabled.\n");
		return 0;
	}

	seq_puts(s, "\n----- ION UNMAPPED HEAP DEBUG -----\n");

	size_t heap_size = 0;
	size_t heap_free_size = 0;
	size_t heap_allocated_size = 0;
	size_t heap_peak = 0;
	size_t heap_largest_free_buf = 0;
	size_t heap_frag = 0;
	
	switch(heap->type) {
		case ION_HEAP_TYPE_UNMAPPED:
			update_unmapped_heap_info(heap);
			heap_size = get_unmapped_heap_meta(heap, HEAP_SIZE);
			heap_free_size = get_unmapped_heap_meta(heap, FREE_SIZE);
			heap_allocated_size = get_unmapped_heap_meta(heap, ALLOCATED_SIZE);
			heap_peak = get_unmapped_heap_meta(heap, ALLOC_PEAK);
			heap_largest_free_buf = get_unmapped_heap_meta(heap, LARGEST_FREE_BUF);
			heap_frag = ((heap_free_size - heap_largest_free_buf) * 100) / heap_free_size;
			seq_printf(s, "%19s %19zu\n", "heap size", heap_size);
			seq_printf(s, "%19s %19zu\n", "free size", heap_free_size);
			seq_printf(s, "%19s %19zu\n", "allocated size", heap_allocated_size);
			seq_printf(s, "%19s %19zu\n", "allocated peak", heap_peak);
			seq_printf(s, "%19s %19zu\n", "largest free buffer", heap_largest_free_buf);
			seq_printf(s, "%19s %19zu\n\n", "heap fragmentation", heap_frag);
			break;
		default:
			pr_err("%s: Invalid heap type for debug: %d\n", __func__, heap->type);
	}
	#endif /* CONFIG_ION_MONITOR */
	return 0;
}

struct ion_heap *ion_unmapped_heap_create(struct rmem_unmapped *heap_data)
{
	struct ion_unmapped_heap *unmapped_heap;
	int ret;

	struct page *page;
	size_t size;

	page = pfn_to_page(PFN_DOWN(heap_data->base));
	size = heap_data->size;

	ret = ion_heap_pages_zero(page, size, pgprot_writecombine(PAGE_KERNEL));
	if (ret)
		return ERR_PTR(ret);

	unmapped_heap = kzalloc(sizeof(*unmapped_heap), GFP_KERNEL);
	if (!unmapped_heap)
		return ERR_PTR(-ENOMEM);

	// ensure memory address align to 64K which can meet VPU requirement.
	#ifdef CONFIG_ION_MONITOR
	unmapped_heap->pool = gen_pool_meta_create(PAGE_SHIFT+4, -1);
	#else
	unmapped_heap->pool = gen_pool_create(PAGE_SHIFT+4, -1);
	#endif /* CONFIG_ION_MONITOR */

	if (!unmapped_heap->pool) {
		kfree(unmapped_heap);
		return ERR_PTR(-ENOMEM);
	}
	unmapped_heap->base = heap_data->base;
	unmapped_heap->size = size;
	gen_pool_add(unmapped_heap->pool, unmapped_heap->base, heap_data->size,
		     -1);
	
	#ifdef CONFIG_ION_MONITOR
	struct gen_pool_meta *meta = 
		container_of(unmapped_heap->pool, struct gen_pool_meta, pool);
	set_meta(meta, HEAP_SIZE, heap_data->size);
	set_meta(meta, LARGEST_FREE_BUF, heap_data->size);
	set_meta(meta, FREE_SIZE, heap_data->size);
	set_meta(meta, ALLOCATED_SIZE, 0);
	set_meta(meta, ALLOC_PEAK, 0);

	unmapped_heap->heap.debug_state = 1;
	#endif /* CONFIG_ION_MONITOR */

	unmapped_heap->heap.ops = &unmapped_heap_ops;
	unmapped_heap->heap.type = ION_HEAP_TYPE_UNMAPPED;
	unmapped_heap->heap.flags = ION_HEAP_FLAG_DEFER_FREE;
	unmapped_heap->heap.name = heap_data->name;
	unmapped_heap->heap.debug_show = ion_unmapped_heap_debug_show;
	
	return &unmapped_heap->heap;
}

static int ion_add_unmapped_heap(void)
{
	struct ion_heap *heap;
	uint32_t i;

	for (i=0;i<unmapped_count;i++)
	{
		if (unmapped_data[i].base == 0 || unmapped_data[i].size == 0)
			return -EINVAL;

		heap = ion_unmapped_heap_create(&unmapped_data[i]);
		if (IS_ERR(heap))
			return PTR_ERR(heap);

		ion_device_add_heap(heap);
	}
	return 0;
}

void ion_unmapped_heap_destroy(struct ion_heap *heap)
{
	struct ion_unmapped_heap *umh =
	    container_of(heap, struct  ion_unmapped_heap, heap);
	#ifdef CONFIG_ION_MONITOR
	struct gen_pool_meta *meta =
		container_of(umh->pool, struct gen_pool_meta, pool);
	gen_pool_meta_destroy(meta);
	#else     
	gen_pool_destroy(umh->pool);
	#endif /* CONFIG_ION_MONITOR */
	kfree(umh);
	umh = NULL;
}

static int rmem_unmapped_device_init(struct reserved_mem *rmem,
					 struct device *dev)
{
	dev_set_drvdata(dev, rmem);
	return 0;
}

static void rmem_unmapped_device_release(struct reserved_mem *rmem,
					 struct device *dev)
{
	dev_set_drvdata(dev, NULL);
}

static const struct reserved_mem_ops rmem_dma_ops = {
	.device_init    = rmem_unmapped_device_init,
	.device_release = rmem_unmapped_device_release,
};

static int __init rmem_unmapped_setup(struct reserved_mem *rmem)
{
	if (unmapped_count < MAX_UNMAPPED_AREA)
	{
		unmapped_data[unmapped_count].base = rmem->base;
		unmapped_data[unmapped_count].size = rmem->size;
		memcpy(unmapped_data[unmapped_count].name,rmem->name,32);
		rmem->ops = &rmem_dma_ops;
		pr_info("Reserved memory: ION unmapped pool %s at %pa, size %ld MiB\n",
				rmem->name,&rmem->base, (unsigned long)rmem->size / SZ_1M);

		unmapped_count++;
		return 0;
	} else {
		return -EINVAL;
	}
}

RESERVEDMEM_OF_DECLARE(unmapped, "imx-secure-ion-pool", rmem_unmapped_setup);

device_initcall(ion_add_unmapped_heap);
