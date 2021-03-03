#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

int mma8451q_probe(struct i2c_client *client, 
	const struct i2c_device_id *id)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
int mma8451q_remove(struct i2c_client *client)
{
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	return 0;
}
const struct i2c_device_id mma8451q_idtable[] = {
	{"mma8451q",10},
	{/*end*/}
};

struct i2c_driver mma8451q = {
	.probe = mma8451q_probe,
	.remove = mma8451q_remove,
	.driver = {
		.name = "hello",
	},
	.id_table = mma8451q_idtable,
	
};

module_i2c_driver(mma8451q);
MODULE_LICENSE("GPL");

