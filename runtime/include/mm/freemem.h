#ifdef USE_FREEMEM

#ifndef __FREEMEM_H__
#define __FREEMEM_H__

#define NEXT_PAGE(page) *((uintptr_t*)page)
#define LIST_EMPTY(list) ((list).count == 0 || (list).head == 0)
#define LIST_INIT(list) { (list).count = 0; (list).head = 0; (list).tail = 0; }

struct pg_list
{
	uintptr_t head;
	uintptr_t tail;
	unsigned int count;
};

void spa_init(uintptr_t base, size_t size);
uintptr_t spa_get(void);
uintptr_t spa_get_zero(void);
void spa_put(uintptr_t page);
unsigned int spa_available();
#endif
#endif
