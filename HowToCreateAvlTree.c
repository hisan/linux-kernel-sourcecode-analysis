struct vm_area_struct {
	struct mm_struct * vm_mm;	/* VM area parameters */	//进程整个用户空间的抽象
	unsigned long vm_start;//start和end指定了一个虚存空间的内外边界
	unsigned long vm_end;

	/* linked list of VM areas per task, sorted by address */
	struct vm_area_struct *vm_next;//每个进程的虚存链表,根据地址排序
//属于同一进程的所有区间都要按虚存地址的高低次序连接在一起

	unsigned long vm_flags;//...

	/* AVL tree of VM areas per task, sorted by address */
	short vm_avl_height;
	struct vm_area_struct * vm_avl_left;
	struct vm_area_struct * vm_avl_right;

	/* For areas with an address space and backing store,
	 * one of the address_space->i_mmap{,shared} lists,
	 * for shm areas, the list of attaches, otherwise unused.
	 */
	struct vm_area_struct *vm_next_share;
	struct vm_area_struct **vm_pprev_share;

	struct vm_operations_struct * vm_ops;
	unsigned long vm_pgoff;		/* offset in PAGE_SIZE units, *not* PAGE_CACHE_SIZE */
	struct file * vm_file;
	unsigned long vm_raend;
	void * vm_private_data;		/* was vm_pte (shared mem) */
};

void avl_insert (struct vm_area_struct * new_node, struct vm_area_struct ** ptree)
{
	int key = new_node->vm_avl_key;
	struct vm_area_struct ** nodeplace = ptree;
	struct vm_area_struct ** stack[avl_maxheight];
	int stack_count = 0;
	struct vm_area_struct *** stack_ptr = &stack[0]; /* = &stack[stackcount] */
	for (;;) {
		struct vm_area_struct * node = *nodeplace;
		if (node == vm_avl_empty)
			break;
		*stack_ptr++ = nodeplace; stack_count++;
		if (key < node->vm_avl_key)
			nodeplace = &node->vm_avl_left;
		else
			nodeplace = &node->vm_avl_right;
	}
	new_node->vm_avl_left = vm_avl_empty;
	new_node->vm_avl_right = vm_avl_empty;
	new_node->vm_avl_height = 1;
	*nodeplace = new_node;
	avl_rebalance(stack_ptr,stack_count);
}


/*
 * Rebalance a tree.
 * After inserting or deleting a node of a tree we have a sequence of subtrees
 * nodes[0]..nodes[k-1] such that
 * nodes[0] is the root and nodes[i+1] = nodes[i]->{vm_avl_left|vm_avl_right}.
 */
void avl_rebalance (struct vm_area_struct *** nodeplaces_ptr, int count)//插入不牛，如何平衡才最牛？？？
{
	for ( ; count > 0 ; count--) {
		struct vm_area_struct ** nodeplace = *--nodeplaces_ptr;
		struct vm_area_struct * node = *nodeplace;
		struct vm_area_struct * nodeleft = node->vm_avl_left;
		struct vm_area_struct * noderight = node->vm_avl_right;
		int heightleft = heightof(nodeleft);
		int heightright = heightof(noderight);
		if (heightright + 1 < heightleft) {
			/*                                                      */
			/*                            *                         */
			/*                          /   \                       */
			/*                       n+2      n                     */
			/*                                                      */
			struct vm_area_struct * nodeleftleft = nodeleft->vm_avl_left;
			struct vm_area_struct * nodeleftright = nodeleft->vm_avl_right;
			int heightleftright = heightof(nodeleftright);
			if (heightof(nodeleftleft) >= heightleftright) {
				/*                                                        */
				/*                *                    n+2|n+3            */
				/*              /   \                  /    \             */
				/*           n+2      n      -->      /   n+1|n+2         */
				/*           / \                      |    /    \         */
				/*         n+1 n|n+1                 n+1  n|n+1  n        */
				/*                                                        */
				node->vm_avl_left = nodeleftright; nodeleft->vm_avl_right = node;
				nodeleft->vm_avl_height = 1 + (node->vm_avl_height = 1 + heightleftright);
				*nodeplace = nodeleft;
			} else {
				/*                                                        */
				/*                *                     n+2               */
				/*              /   \                 /     \             */
				/*           n+2      n      -->    n+1     n+1           */
				/*           / \                    / \     / \           */
				/*          n  n+1                 n   L   R   n          */
				/*             / \                                        */
				/*            L   R                                       */
				/*                                                        */
				nodeleft->vm_avl_right = nodeleftright->vm_avl_left;
				node->vm_avl_left = nodeleftright->vm_avl_right;
				nodeleftright->vm_avl_left = nodeleft;
				nodeleftright->vm_avl_right = node;
				nodeleft->vm_avl_height = node->vm_avl_height = heightleftright;
				nodeleftright->vm_avl_height = heightleft;
				*nodeplace = nodeleftright;
			}
		}
		else if (heightleft + 1 < heightright) {
			/* similar to the above, just interchange 'left' <--> 'right' */
			struct vm_area_struct * noderightright = noderight->vm_avl_right;
			struct vm_area_struct * noderightleft = noderight->vm_avl_left;
			int heightrightleft = heightof(noderightleft);
			if (heightof(noderightright) >= heightrightleft) {
				node->vm_avl_right = noderightleft; noderight->vm_avl_left = node;
				noderight->vm_avl_height = 1 + (node->vm_avl_height = 1 + heightrightleft);
				*nodeplace = noderight;
			} else {
				noderight->vm_avl_left = noderightleft->vm_avl_right;
				node->vm_avl_right = noderightleft->vm_avl_left;
				noderightleft->vm_avl_right = noderight;
				noderightleft->vm_avl_left = node;
				noderight->vm_avl_height = node->vm_avl_height = heightrightleft;
				noderightleft->vm_avl_height = heightright;
				*nodeplace = noderightleft;
			}
		}
		else {
			int height = (heightleft<heightright ? heightright : heightleft) + 1;
			if (height == node->vm_avl_height)
				break;
			node->vm_avl_height = height;
		}
	}
}



int main()
{
	struct vm_area_struct head,node1,node2;
	head.vm_avl_key = 2;
	node1.vm_avl_key = 5;
	
	struct vm_area_struct *ptree = NULL;
	avl_insert (&node1, &ptree);
}