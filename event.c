#include "event.h"

void init_event(elf_event_t *tmp, void *input_data)
{
    tmp->event_id = 0;
    tmp->data = input_data;
}

void *get_data(elf_event_t *tmp)
{
    return tmp->data;
}
