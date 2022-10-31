#include<sys.h>
#include<malloc.h>

#define PAGE_SIZE 4096

// bucket descriptor,16 bytes
struct bucket_desc
{
    void *page;
    struct bucket_desc *next;
    void *freeptr;
    unsigned short refcnt;
    unsigned short bucket_size;

};

//struct of bucket directory
struct bucket_dir
{
   int size;
   struct bucket_desc *chain;
};

//bucket directory
struct bucket_dir bucket_dir[]=
{
   {16,(struct bucket_desc *) 0},
   {32,(struct bucket_desc *) 0},
   {64,(struct bucket_desc *) 0},
   {128,(struct bucket_desc *) 0},
   {256,(struct bucket_desc *) 0},
   {512,(struct bucket_desc *) 0},
   {1024,(struct bucket_desc *) 0},
   {2048,(struct bucket_desc *) 0},
   {4096,(struct bucket_desc *) 0},
   {0,(struct bucket_desc *) 0}
};

//a link of free bucket descriptor blocks
struct bucket_desc * free_bucket_desc = (struct bucket_desc *) 0;

//initialize link of free bucket descriptors
int init_bucket_desc()
{
   struct bucket_desc * bdesc;
   int i;
   unsigned int addr;
  
   free_bucket_desc=(struct bucket_desc *)brk();
   bdesc=free_bucket_desc;
   if(!bdesc)
   return 0;
   for(i=PAGE_SIZE/sizeof(struct bucket_desc);i>1;i--)
   {
        bdesc->next=bdesc+1;
        bdesc++;   
   }
   bdesc->next=(struct bucket_desc *)0;
   return 1;
}

//malloc
void * malloc(unsigned int len)
{
    struct bucket_dir * bdir;
    struct bucket_desc *bdesc;
    void * retval;
    for(bdir=bucket_dir;bdir->size;bdir++)
    {
         if(bdir->size>=len)
            break;  
    }
    if(!bdir->size)
    {
         printf("%d bytes:malloc called with larger than 4096 bytes!\n",len);
         return;
    }
    
    for(bdesc=bdir->chain;bdesc;bdesc=bdesc->next)
    {   
        if(bdesc->freeptr)
           break;
    }

    //If we haven't found one, than we should establish a new bucket_desctiptor
    if(!bdesc) 
    {    
          void * cp;  
          int i;
          //If free_bucket_desc is NULL, than establish the link of free bucket
          if(!free_bucket_desc)
          {
				if(!init_bucket_desc())
                {     
                    printf("No enough memory for bucket descriptors!\n");
                    return;  
                }
                
          } 

          
          //take out one free bucket
          bdesc=free_bucket_desc;
          free_bucket_desc=bdesc->next;
          
          //initialize the bucket descriptor
          bdesc->page=(void *)brk();
          bdesc->next=bdir->chain;
          bdesc->freeptr=bdesc->page;
          bdesc->refcnt=0;
          bdesc->bucket_size=bdir->size;
          bdir->chain=bdesc;
          cp=bdesc->page;
          if(!bdesc->page)
          {
                printf("No enough memory for allocation!\n");
                return;
          }
      
          for(i=PAGE_SIZE/bdir->size;i>1;i--)
          {
                *((char**)cp)=cp+bdir->size;
                cp=cp+bdir->size;
          }
                *((char**)cp)=0;
     }
    
     //allocate memory block
     retval=bdesc->freeptr;
     bdesc->freeptr=*((void **)bdesc->freeptr);
     bdesc->refcnt++;
  
     return(retval);  
}

int free(void * obj )
{
    void * page;
    struct bucket_dir *bdir;
    struct bucket_desc *bdesc;
    
    page=(unsigned long)obj&0xfffff000;
    for(bdir=bucket_dir;bdir->size!=0;bdir++)
        for(bdesc=bdir->chain;bdesc!=(struct bucket_desc *)0;bdesc=bdesc->next)
            if(bdesc->page==page)
              goto found;
    return 0;
found:
    *((void**)obj)=bdesc->freeptr;
    bdesc->freeptr=obj;
    bdesc->refcnt--;
    return 1;
          
}