#ifndef _UACCESS_H_
#define _UACCESS_H_

/* This is a limited set of the features from linux uaccess, only the
   ones we need for now */

extern unsigned long __copy_user(void  *to,
				 const void  *from, unsigned long n);

static inline unsigned long
copy_to_user(void *to, const void *from, unsigned long n)
{
	return __copy_user(to, from, n);
}

static inline unsigned long
copy_from_user(void *to, const void *from, unsigned long n)
{
	return __copy_user(to, from, n);
}

#endif /* _UACCESS_H_ */
