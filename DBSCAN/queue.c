#include <malloc.h>

struct objects
{
    float x;
    float y;
    int cluster_id;
};

struct queue
{
    struct objects *o;
    struct queue *next;
};

void enqueue(struct objects *obj, struct queue **f, struct queue **r)
{
    struct queue *temp;
    temp = (struct queue *)malloc(sizeof(struct queue));
    temp->o = obj;
    temp->next = NULL;
    if (*f== NULL)
    {
        *f = temp;
        *r = temp;
    }
    else
    {
        (*r)->next = temp;
        *r = temp;
    }
}

struct objects *dequeue(struct queue **f)
{
    struct objects *obj;
    if (*f != NULL)
    {
        obj = (*f)->o;
        *f = (*f)->next;
    }
    return obj;
}

int is_empty(struct queue *f)
{
    if (f != NULL)
        return 0;
    return 1;
}

void show(struct queue *f)
{
    struct queue *temp = f;
    if(f == NULL)
    	printf("Empty queue\n");
    while (temp != NULL)
    {
        printf("%f %f %d\n", temp->o->x, temp->o->y, temp->o->cluster_id);
        temp = temp->next;
    }
}