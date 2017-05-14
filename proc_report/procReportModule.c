#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>

//Count of states.State evals to -1 unrunnable, 0 runnable, >0 stopped
int states[3] = {0};

void get_process_data(){
  struct task_struct *tasks;
/*
 for_each_process(tasks){
   char* processName;
   printk(KERN_INFO "Name=%s",tasks->comm);
   if(list_empty(&tasks->children)){
    printk(KERN_INFO "*No Children");
  } else {
    struct task_struct *childTask;
    childTask = list_first_entry(&tasks->children, struct task_struct, sibling);
    printk(KERN_INFO "First Child Name=%s",childTask->comm);
  } 
 }
*/
 //Loop through all processes from start to end
  for_each_process(tasks){
    char* processName;
    char* childName;   
    int processPid;
    int childPid;
    int numChildren;
    long state;
    numChildren = 0;

    processName = tasks->comm;
    processPid = tasks->pid;
    state = tasks->state;

    //Handle count of states
    if (state > 0){
      states[2] += 1;
    } else {
      states[state+1] += 1;
    }

    //Handle process children
    if(!list_empty(&tasks->children)){
      struct task_struct *childTask = list_first_entry(&tasks->children, struct task_struct, sibling);
      childPid = childTask->pid;
      childName = childTask->comm;

      //Count number of children
      struct list_head *cursor;
      list_for_each(cursor,&tasks->children){
        numChildren += 1;
      }
    }
    //All data collected
    if (numChildren == 0){
      printk(KERN_INFO "State= %d Process ID=%d Name=%s *No Children",states, processPid, processName);
    } else {
      printk(KERN_INFO "Process ID=%d Name=%s  number_of_childen: %d first_child_pid=%d first_child_name=%s",
      processPid,processName, numChildren, childPid, childName); 
    }
  }
  printk(KERN_INFO "Unrunnable=%d Runnable=%d Stopped=%d",states[0],states[1],states[2]);
}


int proc_init (void) {
  printk(KERN_INFO "procReportModule: kernel module initialized\n");
  
  get_process_data();
 
  return 0;
}

void proc_cleanup(void) {
  printk(KERN_INFO "procReportModule: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);

