#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x35ec255d, "module_layout" },
	{ 0xbcc99e4f, "put_disk" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0xd4ce552f, "blk_cleanup_queue" },
	{ 0x9af95bb4, "del_gendisk" },
	{ 0xc0dad6e4, "add_disk" },
	{ 0xe914e41e, "strcpy" },
	{ 0x12e7193a, "blk_init_queue" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0xffb08a9f, "alloc_disk" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3530278B675B71F509C7BB4");
