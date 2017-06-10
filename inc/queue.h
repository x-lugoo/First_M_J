#ifndef QUEUE_H
#define QUEUE_H

void *queue_create(int data_size);
int queue_free(void *queue);
int queue_add_data(void *queue,void *data);
void *queue_read_data(void *queue);
int queue_del_head(void *queue);
int queue_get_data(void *queue,void *data);
int queue_is_empty(void *queue);

#endif
