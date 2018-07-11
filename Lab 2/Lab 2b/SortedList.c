/*                                                                                                                                                                                                          
Sparsh Arora                                                                                                                                                                                                
UID: 804653078                                                                                                                                                                                              
sparsharora@g.ucla.edu                                                                                                                                                                                      
*/
#include "SortedList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>


void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
  // fprintf(stderr,"Insert\n");
  if(element==NULL || list==NULL)
    return;

  if(list->next==NULL && list->prev==NULL)
    {
      if (opt_yield & INSERT_YIELD)
	sched_yield();
      list->next=element;
      element->prev=list;
      element->next=NULL;
      return;
    }
  
  SortedListElement_t *ptr=list->next;
  SortedListElement_t *ptr_t;
  
  while(ptr!=NULL)
    {
      if(strcmp(element->key, ptr->key)<=0)
	break;

      ptr_t=ptr;
      ptr=ptr->next;
    }

  if (opt_yield & INSERT_YIELD)
    sched_yield();

  if(ptr==NULL)
    {
      element->next=NULL;
      element->prev=ptr_t;
      ptr_t->next=element;
    }
  else
    {
      element->next=ptr;
      element->prev=ptr->prev;
      ptr->prev->next=element;
      ptr->prev=element;
    }
  //fprintf(stderr,"Insert\n");
}


int SortedList_delete( SortedListElement_t *element)
{
  if(element==NULL)
    return 1;

  if(element->next==NULL)
    {
      if(element->prev->next==element)
	{
	  if (opt_yield & DELETE_YIELD)
	    sched_yield();
	  element->prev->next=NULL;
	  //free(element);
	  return 0;
	}
      else
	return 1;
    }
  else 
    {
      if(element->prev->next==element && element->next->prev==element)
	{
	  if (opt_yield & DELETE_YIELD)
	    sched_yield();
	  element->prev->next=element->next;
	  element->next->prev=element->prev;
	  //free(element);
	  return 0;
	}
      else
	return 1;
    }


}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
  //fprintf(stderr,"look\n");
  if(list==NULL|| key==NULL || list->next==NULL)
    return NULL;

  
  SortedListElement_t *ptr=list->next;

  while(ptr!=NULL)
    {
      if(strcmp(ptr->key,key)==0)
	return ptr;
      
      if (opt_yield & LOOKUP_YIELD)
        sched_yield();

      ptr=ptr->next;
    }

  return NULL;
}


int SortedList_length(SortedList_t *list)
{

  if(list==NULL)
    return -1;

  int ctr=0;
  
  SortedListElement_t *ptr=list->next;
  while(ptr!=NULL)
    {
      if(ptr->next==NULL && ptr->prev->next!=ptr)
	return -1;
      else if(ptr->next!=NULL && (ptr->next->prev!=ptr || ptr->prev->next!=ptr))
	return -1;
      else
	{
	  ctr++;

	  if (opt_yield & LOOKUP_YIELD)
	    sched_yield();
	  
	  ptr=ptr->next;
	}
    }

  return ctr;
}
