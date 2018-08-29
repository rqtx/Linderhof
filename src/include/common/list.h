
#ifndef LIST_H
  #define LIST_H

typedef struct ListData{
  void *data;
  struct ListData *next;
  struct ListData *prev;
}List;

#define LIST_FOREACH( p_list, aux_list ) for( aux_list = p_list; NULL != aux_list; aux_list = aux_list->next )

void InsertCell( List **p_list, void *p_data );
void RemoveCell( List *p_cell );

#endif
