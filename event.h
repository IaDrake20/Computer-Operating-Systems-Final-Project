#ifndef __EVENT_H__
#define __EVENT_H__

typedef struct elf_event
{
    int event_id;
    void *data;
} elf_event_t;

void init_event(elf_event_t *tmp, void *input_data);

void *get_data(elf_event_t *tmp);

#endif /* __EVENT_H__ */
