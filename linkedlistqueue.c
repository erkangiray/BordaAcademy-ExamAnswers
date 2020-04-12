#include <stdio.h> 
#include <stdlib.h> 

//item struct holds a pointer to an int and an integer
typedef struct A {
	int* p_data;
	int p_priority;
}item_t;

//node struct so that a linked list may be created
struct Node{
	item_t* item;
	struct Node* next;//demek ki neymiþ typedef struct yapacaksan onun içinde ayný sturct'ý kullanmayacaksýn þayet öyle yaparsan compiler hata verir ve geri döner her yere struct Node* yazarsýn Teþekkürler Türkiye
};

//input parameters:gets reference to head pointer, and reference to item to be queued
//
//queues items according to precedence AND priority
//head is the head of the linked list forming the queue
void push(struct Node** r_head , item_t *r_item)
{
	struct Node* newnode = (struct Node*)(malloc(sizeof(struct Node)));//allocate space for newnode
	newnode->next = NULL;//next null
	newnode->item = r_item;
	/**((newnode->item)->p_data) = *(r_item->p_data);//newnode's item's p_data's pointed value equals r_item's p_data's pointed value
	newnode->item->p_priority = r_item->p_priority;*/
	
	if (*r_head == NULL) {//if list is empty make head pointer the new node
		*r_head = newnode;
		return;
	}
	//if same or lower priority is encountered in the first element of the queue, set immediately
	//head points to newnode and function exited immediately
	//this ensures that higher priority items come first in the list, 
	//AND items with the same priority are listed according to which came last (first in first out)
	if (((*r_head)->item->p_priority) <= (newnode->item->p_priority)) {
		newnode->next = *r_head;
		*r_head = newnode;
		return;
	}

	//if there is more than one element in the queue, this loop starts to run 
	struct Node* temp; temp = *r_head;
	while (temp->next != NULL) {
		//if next elements priority is smaller or equal to newnode's priority, put the newnode in between elements
		//and exit function
		if ((temp->next->item->p_priority) <= (newnode->item->p_priority)) {//checking temp->next->priority before temp=temp->next
			newnode->next = temp->next;										//makes sure that we never try to access NULL
			temp->next = newnode;
			return;
		}
		temp = temp->next;//advance a node
	}
	//if loop runs until temp's next points to NULL, it means that no element with same or lower priority has been encountered
	//so the last element of the queue will become the one with the lowest priority, which in this case is newnode
	temp->next = newnode;
	return;
}

//input parameters:head pointer of the queue
//removes the top item of the queue,since list is according to precedence and priority, removing from the top ensures the integrity of the priority queue
//
//return pointer to highest priority item
item_t* pop(struct Node** r_head) {
	
	if (*r_head == NULL) //nothing to pop
		return NULL;

	else {
		struct Node* temp;
		temp = *r_head; *r_head = (*r_head)->next;//highest priority is always at the beginning of the queue
		return  temp->item;//returns pointer to highest priority item
	}
}

//input parameters:head ptr of the list
//prints the queue
void printQ(struct Node**r_head){
	struct Node* temp;
	temp = *r_head;
	if (temp == NULL) {
		printf("List is empty.\n");
		return;
	}
	while (temp->next != NULL) {
		printf("Priority:%d Data:%d\n", temp->item->p_priority, *(temp->item->p_data));
		temp = temp->next;
	}
	printf("Priority:%d Data:%d\n", temp->item->p_priority, *(temp->item->p_data));
}

//this function is used in conjuncture with the pop() function
//takes as parameter the pointer returned by pop()
//prints out the item that has been deqeued. It is not to print items
void pop_print(item_t* item) {
	if (item == NULL)
		printf("List is empty. Nothing to dequeue.\n");
	else 
		printf("Dequeued item priority:%d data:%d\n", item->p_priority, *(item->p_data));
}

int main()
{
	struct Node* head = NULL;
	item_t item1; item1.p_data = malloc(sizeof(item_t));*(item1.p_data) = 15; item1.p_priority = 1;
	item_t item2; item2.p_data = malloc(sizeof(item_t)); *(item2.p_data) = -15; item2.p_priority = 0;
	item_t item3; item3.p_data = malloc(sizeof(item_t)); *(item3.p_data) = 12; item3.p_priority = 1;
	item_t item4; item4.p_data = malloc(sizeof(item_t)); *(item4.p_data) = -28; item4.p_priority = 0;
	item_t item5; item5.p_data = malloc(sizeof(item_t)); *(item5.p_data) = 5; item5.p_priority = 1;
	item_t item6; item6.p_data = malloc(sizeof(item_t)); *(item6.p_data) = 155; item6.p_priority = 2;
	item_t item7; item7.p_data = malloc(sizeof(item_t)); *(item7.p_data) = 286; item7.p_priority = 2;
	item_t item8; item8.p_data = malloc(sizeof(item_t)); *(item8.p_data) = 375; item8.p_priority = 2;
	item_t item9; item9.p_data = malloc(sizeof(item_t)); *(item9.p_data) = 3; item9.p_priority = 1;
	item_t item10; item10.p_data = malloc(sizeof(item_t)); *(item10.p_data) = -45; item10.p_priority = 0;
	item_t item11; item11.p_data = malloc(sizeof(item_t)); *(item11.p_data) = -83; item11.p_priority = 0;
	item_t item12; item12.p_data = malloc(sizeof(item_t)); *(item12.p_data) = -157; item12.p_priority = 0;
	push(&head, &item1);
	push(&head, &item2);
	push(&head, &item3);
	push(&head, &item4);
	push(&head, &item5);
	push(&head, &item6);
	push(&head, &item7);
	push(&head, &item8);
	push(&head, &item9);
	push(&head, &item10);
	push(&head, &item11);
	push(&head, &item12);
	printQ(&head);

	pop_print(pop(&head));
	pop_print(pop(&head));
	pop_print(pop(&head));
	pop_print(pop(&head));

	printQ(&head);


}
