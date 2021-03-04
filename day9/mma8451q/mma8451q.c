#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#define WHO_AM_I 0x0d

int i2c_write_reg(struct i2c_client *client,char reg,char val)
{
	int ret;
	char wbuf[] = {reg,val};
	struct i2c_msg w_msg[1] = {
		[0] = {
			.addr  = client->addr,
			.flags = 0,
			.len   = 2,
			.buf   = wbuf,
		},			
	};
	
	ret = i2c_transfer(client->adapter, 
		w_msg, ARRAY_SIZE(w_msg));
	if(ret != ARRAY_SIZE(w_msg)){
		printk("send msg error\n");
		return -EAGAIN;
	}
	
	return 0;
}


int i2c_read_reg(struct i2c_client *client,char reg)
{	
	int ret;
	char val;
	char rbuf[] = {reg};
	struct i2c_msg r_msg[] = {
		[0] = {
			.addr  = client->addr,
			.flags = 0,
			.len   = 1,
			.buf   = rbuf,
		},
		[1] = {
			.addr  = client->addr,
			.flags = I2C_M_RD,
			.len   = 1,
			.buf   = &val,
		},
	};
	
	ret = i2c_transfer(client->adapter, 
			r_msg, ARRAY_SIZE(r_msg));
	if(ret != ARRAY_SIZE(r_msg)){
		printk("receive msg error\n");
		return -EAGAIN;
	}

	return val;
}

int mma8451q_probe(struct i2c_client *client, 
	const struct i2c_device_id *id)
{
	int ret;
	printk("%s:%s:%d\n",__FILE__,__func__,__LINE__);

	ret = i2c_read_reg(client,WHO_AM_I);

	printk("WHO_AM_I(0x1a) = %#x\n",ret);

	
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

