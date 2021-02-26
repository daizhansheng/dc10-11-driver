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
	{ 0x37a0cba, "kfree" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x2f35ab80, "cdev_del" },
	{ 0x84b6f4af, "class_destroy" },
	{ 0x6c5f5671, "device_destroy" },
	{ 0xc5c74531, "__mutex_init" },
	{ 0x60781a70, "device_create" },
	{ 0x32b91bd6, "__class_create" },
	{ 0xebd5feb1, "cdev_add" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xb2c831b6, "cdev_init" },
	{ 0x23e259c8, "cdev_alloc" },
	{ 0xb4471b80, "mutex_trylock" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x6339a8bc, "mutex_unlock" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "571F20BC1E0BD843547E381");
