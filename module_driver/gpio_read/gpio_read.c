/*
 * 此测试代码主要展示驱动对IO的操作以及C语言中对指针的运算注意事项
 * 1. __raw_readl是原始的操作IO的方法
 * 2. 驱动模块代码和内核代码是一个层级的东西，此处可以直接使用__raw_readl
 * 3. 需要注意gpioB是一个指针，对指针的加减操作要考虑到指针的数据类型
 */
#include <linux/module.h>
#include <mach/regs-gpio.h>
#include <asm/io.h>
#include <linux/gpio.h>

#if 1
struct s3c2410_gpio {
    unsigned int GPxCON;
    unsigned int GPxDAT;
    unsigned int reserve1;
    unsigned int reserve2;
};

static volatile struct s3c2410_gpio *gpioB;
static volatile struct s3c2410_gpio *gpioG;
#else
static volatile unsigned int *gpioB;
static volatile unsigned int *gpioG;
#endif

unsigned char IO_DATA[8];

static int __init gpio_init(void)
{
    int i;

    printk("gpio_init.\n");
    gpioB = ioremap(0x56000010, 16);
    gpioG = ioremap(0x56000060, 16);

    printk("gpioG = %p\n", gpioG);
    printk("gpioB = %p\n", gpioB);
    printk("&gpioG = %p\n", &gpioG);
    printk("&gpioB = %p\n", &gpioB);
    printk("&gpioG->GPxDAT = %p\n", &gpioG->GPxDAT);

    IO_DATA[0] = s3c2410_gpio_getpin(S3C2410_GPG(5));
    IO_DATA[1] = s3c2410_gpio_getpin(S3C2410_GPG(6));
    IO_DATA[2] = s3c2410_gpio_getpin(S3C2410_GPG(7));
    IO_DATA[3] = s3c2410_gpio_getpin(S3C2410_GPG(11));

    IO_DATA[4] = s3c2410_gpio_getpin(S3C2410_GPB(1));
    IO_DATA[5] = s3c2410_gpio_getpin(S3C2410_GPB(2));
    IO_DATA[6] = s3c2410_gpio_getpin(S3C2410_GPB(3));
    IO_DATA[7] = s3c2410_gpio_getpin(S3C2410_GPB(4));

    for (i = 0; i < 8; i++)
    {
        printk("IO_DATA[%d] = 0x%x\n", i, IO_DATA[i]);
    }
    printk("---------------\n");

    IO_DATA[0] = __raw_readl((unsigned char *)gpioG+0x4) & (1 << 5);
    IO_DATA[1] = __raw_readl((unsigned char *)gpioG+0x4) & (1 << 6);
    IO_DATA[2] = __raw_readl((unsigned char *)gpioG+0x4) & (1 << 7);
    IO_DATA[3] = __raw_readl((unsigned char *)gpioG+0x4) & (1 << 11);
    IO_DATA[4] = __raw_readl((unsigned char *)gpioB+0x4) & (1 << 1);
    IO_DATA[5] = __raw_readl((unsigned char *)gpioB+0x4) & (1 << 2);
    IO_DATA[6] = __raw_readl((unsigned char *)gpioB+0x4) & (1 << 3);
    IO_DATA[7] = __raw_readl((unsigned char *)gpioB+0x4) & (1 << 4);
    //or IO_DATA[7] = __raw_readl((unsigned int *)gpioB+0x1) & (1 << 4);
    printk("(insigned char *)gpioG+0x4 = 0x%x\n", (unsigned char *)gpioG+0x4);

    for (i = 0; i < 8; i++)
    {
        printk("IO_DATA[%d] = 0x%x\n", i, IO_DATA[i]);
    }
    printk("---------------\n");
    IO_DATA[0] = (gpioG->GPxDAT) & (1 << 5); 
    IO_DATA[1] = (gpioG->GPxDAT) & (1 << 6); 
    IO_DATA[2] = (gpioG->GPxDAT) & (1 << 7); 
    IO_DATA[3] = (gpioG->GPxDAT) & (1 << 11);

    IO_DATA[4] = (gpioB->GPxDAT) & (1 << 1); 
    IO_DATA[5] = (gpioB->GPxDAT) & (1 << 2); 
    IO_DATA[6] = (gpioB->GPxDAT) & (1 << 3); 
    IO_DATA[7] = (gpioB->GPxDAT) & (1 << 4); 

    for (i = 0; i < 8; i++)
    {
        printk("IO_DATA[%d] = 0x%x\n", i, IO_DATA[i]);
    }

    return 0;
}

static void __exit gpio_exit(void)
{
    iounmap(gpioB);
    iounmap(gpioG);
    return ;
}


module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("GPL");
