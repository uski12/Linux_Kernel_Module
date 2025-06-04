#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

struct details {
    char *name;
    int day;
    int month;
    int year;
    struct list_head list;
};

static LIST_HEAD(details_list);

int simple_init(void)
{
    char *names[5] = {"Julius Caesar", "Jacob", "Susan", "Belle", "Marc Anthony"};

    for(int i = 0; i<5; i++) {
        u32 rand1, rand2;
        get_random_bytes(&rand1, sizeof(rand1));
        get_random_bytes(&rand2, sizeof(rand2)); //random bytes to generate random dates

        printk(KERN_INFO "OS_mp_v2: %u %u", rand1, rand2);
        struct details *person;
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        person->name = names[i];
        person->day = rand1%31 + 1;
        person->month = rand2%12 + 1;
        person->year = 1980 + rand1%15 + rand2%15;
        INIT_LIST_HEAD(&person->list);

        list_add_tail(&person->list, &details_list);
    }


    //List traversal
    struct details *ptr;

    list_for_each_entry(ptr, &details_list, list) {
        printk(KERN_INFO "OS_mp_v2: Name: %s\tDoB: %02d-%02d-%d\n", ptr->name, ptr->day, ptr->month, ptr->year);
    }

    return 0;
}

void simple_exit(void)
{
    struct details *ptr, *next;
    list_for_each_entry_safe(ptr, next, &details_list, list) {
        printk(KERN_INFO "OS_mp_v2: Removing list element (data: %s %d-%d-%d)", ptr->name, ptr->day, ptr->month, ptr->year);
        list_del(&ptr->list);
        kfree(ptr);
    }
    printk(KERN_INFO "Successfully removed OS_mp module\n");
}


module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module version 2");
MODULE_AUTHOR("uski12");
