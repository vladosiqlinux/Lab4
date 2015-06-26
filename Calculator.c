#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>

#define POSITIVE 1
#define NEGATIVE -1
#define BUFF_SIZE 255

int first, second;
char operation;

ssize_t calc_write(struct file *filp, const char *buf, size_t length, loff_t *offp)
{
	int i, tmp = 0, sign = POSITIVE, start_index = 0;
	bool next_number = false;
	for (i = 0 ; i < length - 1; ++i) {
		if (buf[i] >= '0' && buf[i] <= '9') {
			tmp = tmp * 10 + (buf[i]-'0');
		} else {
			if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/') {
				if(i == start_index && buf[i] == '-')
					sign = NEGATIVE;
				else {
					if(!next_number) {
						operation = buf[i];
						next_number = true;
						start_index = i + 1;
						first = tmp * sign;
						tmp = 0;
						sign = POSITIVE;
					} else {
						printk(KERN_INFO "Wrong input.\n");
						return length;
					}
				}
			} else if (buf[i] != ' ') {
				printk(KERN_INFO "Wrong input. %ca\n", buf[i]);
				return length;
			} else
				++start_index;
		}
	}
	second = tmp * sign;
	return length;
}

ssize_t calc_show_result(struct file *filp, char *buf, size_t length, loff_t *offp) 
{
	char str[BUFF_SIZE];
	int len = BUFF_SIZE, res = 0;
	static int finished = 0;
	

	switch (operation) {
	case '+':
		res = first + second;
		break;
	case '-':
		res = first - second;
		break;
	case '*':
		res = first * second;
		break;
	case '/':
		if (second == 0) {
			printk(KERN_INFO "Division by zero.\n");
			return 0;
		}
		res = first / second;
		break;	
	default:
		break;
	}
	
	len = sprintf(str, "%d\n", res);	
	if (finished) {
		finished = 0;
		return 0;
	}
	
	finished = 1;
	strcpy(buf, str);
	return len;
}

static const struct file_operations calc_result_fops = {
	.owner = THIS_MODULE,
	.read = calc_show_result
};

static const struct file_operations calc_write_fops = {
	.owner = THIS_MODULE,
	.write = calc_write
};
	

static int __init proc_init(void)
{
	proc_create("calc_write", 0666, NULL, &calc_write_fops);
	proc_create("calc_result", 0, NULL, &calc_result_fops);

	return 0;
}

static void __exit proc_exit(void)
{
	remove_proc_entry("calc_result", NULL);
	remove_proc_entry("calc_write", NULL);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
