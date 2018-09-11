#include "common/common.h"

void InsertCell( List **p_list, void *p_data )
{
    List *newCell = NULL;

    if( NULL == p_list)
    {
        return;
    }

    memalloc( &newCell, sizeof(List) );
    newCell->data = p_data;
  
    if( NULL == *p_list )
    {
        newCell->prev = NULL;
        newCell->next = NULL;
        *p_list = newCell;
    }
    else
    {
        newCell->prev = (*p_list)->prev;
        newCell->next = *p_list;

        (*p_list)->prev = newCell;
        *p_list = newCell;
    }
}

void InsertCellLast( List **p_list, void *p_data )
{
    List *newCell = NULL;

    if( NULL == p_list)
    {
        return;
    }

    memalloc( &newCell, sizeof(List) );
    newCell->data = p_data;
  
    if( NULL == *p_list ){
        newCell->prev = NULL;
        newCell->next = NULL;
        *p_list = newCell;
    }
    else
    {
        List *last;
        for(last = *p_list; last->next != NULL; last = last->next);
        
        newCell->prev = last;
        newCell->next = NULL;

        last->next = newCell;
    }

}

//We free List memory but not data memory
void RemoveCell( List **p_cell )
{
    List *prev; 
    List *next; 
    
    if( NULL == p_cell || NULL == *p_cell )
    {
        return;
    }

    prev = (*p_cell)->prev;
    next = (*p_cell)->next;

    if( NULL != prev )
    {
        prev->next = next;
    }
    else
    {
        //new first
        *p_cell = next;
    }

    if( NULL != next )
    {
      next->prev = prev;
    }

    memfree( p_cell );
}
