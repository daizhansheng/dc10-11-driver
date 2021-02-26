

#define wait_event_interruptible(wq, condition)				
({									
	int __ret = 0;	
	//定义了返回值的变量
	
	if (!(condition))						
		__wait_event_interruptible(wq, condition, __ret);	
	//如果数据没有准备好就会执行
	//__wait_event_interruptible这个函数，如果
	//硬件的数据已经准备好了，这个函数
	//就会被跳过。

	__ret;	
	//返回值，成功返回0，失败返回错误码							\
})


#define __wait_event_interruptible(wq, condition, ret)			
	do {									
		DEFINE_WAIT(__wait);						
		/*
		wait_queue_t __wait = {						
			.private	= current,	
			//current就是获取当前进程的task_struct结构体
			.func		= function, 	
			//唤醒时用到的唤醒函数
			.task_list	= LIST_HEAD_INIT((name).task_list), 
			//内核的链表
		}
		*/
		//定义等待队列项，并初始化等待队列项

		
		for (;;) {							
			prepare_to_wait(&wq, &__wait, TASK_INTERRUPTIBLE);	
			//将等待队列项放入到等待队列头后
			//并标识进程的状态为可中断的等待态

			if (condition)	//如果数据准备好了，就					
				break;		//退出循环				

			if (!signal_pending(current)) { 	
			//判断是否有信号挂起了
			//如果有信号到来了signal_pending返回真
			//如果有信号到了表示数据没有准备好
			//给它返回一个错误码
			//如果当前没有信号到了，这个if就为真
			//调用schedule完成休眠。
				schedule(); 			
			//schedule这个是主动放弃cpu的函数
				continue;					
			}							
			ret = -ERESTARTSYS; 				
			break;	

			
		}							
		finish_wait(&wq, &__wait);	
		//将进程的状态重新修改为运行的状态
		//并将等待队列项从等待队列头中删除
	} while (0)




	

