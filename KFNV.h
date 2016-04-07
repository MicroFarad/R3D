/*
This is an implementation of the Fowler–Noll–Vo hash function and is not original work.
Credits go to Glenn Fowler, Landon Curt Noll, and Phong Vo.
*/

#define KFNV_INIT  0x811c9dc5ul
#define KFNV_PRIME 0x01000193ul

unsigned long KFNV_Hash(void *data,unsigned long count,unsigned long hash)
{
	unsigned char *dp;
	int n;
	dp=(unsigned char*)data;
	for(n=0;n<count;n++)
	{
		hash*=KFNV_PRIME;
		hash^=(unsigned long)*dp;
		dp++;
	}
	return hash;
}
