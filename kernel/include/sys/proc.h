//定义进程表数据结构
#include "type.h"
#include "protect.h"
#ifndef _DIYOS_PROC_H
#define _DIYOS_PROC_H
//#define MAX_PROCESS_COUNT	32	//最多32个进程
#define TASKS_COUNT		1	//系统进程个数
#define PROCS_COUNT		4	//用户进程数量
//消息广播
#define ANY	(TASKS_COUNT + PROCS_COUNT + 10)
//
#define NO_TASK	(TASKS_COUNT + PROCS_COUNT + 20)
#define HARD_INT		1
//中断类型消息
#define INTERRUPT		-10
//任务状态
#define SENDING			0x02
#define RECEIVING		0x04
//这里暂时将PROCS_COUNT写死，所以MAX_PROCESS_COUNT就为tasks + procs
#define MAX_PROCESS_COUNT	(TASKS_COUNT + PROCS_COUNT)	

//RPL LEVEL
#define RPL_TASK	1
#define RPL_USER	3


//stacks of tasks
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TOTAL	(STACK_SIZE_TESTA)
//进程表的栈，用来保存进程寄存器的值
struct stackframe{
	//中断开始时，由我们的中断处理函数进行压栈
	u32 gs;
	u32 fs;
	u32 es;
	u32 ds;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 kernel_esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	//
	u32 retaddr;
	//中断开始时，cpu会自动压栈
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 esp;
	u32 ss;
};

//保存进程信息
struct process{
	struct stackframe regs; //进程寄存器的值
	
	u16 ldt_sel;		//ldt在gdt中的选择子
	struct descriptor ldts[MAX_LDT_ITEMS]; //local descriptor table
	int ticks;
	int priority;	//进程优先级
	
	u32 pid;	//进程id
	char name[16];	//进程名
	int p_flags;		//进程标志 SENDING正在发消息状态阻塞 RECEIVING正在接收消息状态阻塞  0分配时间片正在运行或准备运行
	struct message *p_msg;	//存储进程接受或发送的消息
	int p_recvfrom;		//消息来源的pid
	int p_sendto;		//消息发送的目的pid
	int has_int_msg;	//如果非0，说明正在处理中断
	struct process *q_sending;	//向该进程发送消息的队列
	struct process *next_sending;	//队列头
	int tty_idx;	//tty表索引
};

typedef void (*proc_entry_point)();
//任务
struct task{
	proc_entry_point	entry;
	int stacksize;
	char name[32];	
};


#define proc2pid(x)     (x-proc_table)
extern void schedule();
extern void* va2la(int pid, void* va);
extern void reset_msg(struct message *p_msg);
extern void block(struct process *p_proc);
extern void unblock(struct process *p_proc);
extern msg_send(struct process *current, int dest,struct message *m);
extern msg_receive(struct process *current, int src, struct message *m);
#endif
