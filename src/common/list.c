#include "common/common.h"

void InsertCell( List **p_list, void *p_data )
{
  List *newCell = NULL;

  if( NULL == p_list){
    return;
  }

  memalloc( &newCell, sizeof(List) );
  newCell->data = p_data;
  
  if( NULL == *p_list ){
    newCell->prev = NULL;
    newCell->next = NULL;
    *p_list = newCell;
  }
  else{
    newCell->prev = (*p_list)->prev;
    newCell->next = *p_list;

    (*p_list)->prev = newCell;
    *p_list = newCell;
  }

}

//We free List memory but not data memory
void RemoveCell( List *p_cell )
{
    List *prev = p_cell->prev;
    List *next = p_cell->next;
    
    if( NULL != prev ){
      prev->next = next;
    }

    if( NULL != next ){
      next->prev = prev;
    }

    memfree( p_cell );
}
