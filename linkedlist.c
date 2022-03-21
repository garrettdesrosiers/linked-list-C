#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 4096
#define TRUE 1
#define FALSE 0
#define READ_STATE 2
#define WRITE_STATE 1
#define DONE_STATE 0

typedef struct node {
   int fd;
   char request[BUFSIZE];
   char response[BUFSIZE];
   int reqoffset;
   int respoffset;
   int state;
   struct node *next;
} node_t;

static node_t *head = NULL;

/* Linked List Functions */

node_t* get_node(node_t *head, int fd)
{
   node_t *current = head;
   if(current == NULL)
      return NULL;
   while(current->next != NULL)
   {
      if(current->fd == fd)
         return current;
      current = current->next;
   }
   if (current->fd == fd)
      return current;
   return NULL;
}

int in_list(node_t *head, int fd)
{
   node_t *current = head;
   if(current == NULL)
      return FALSE;
   while(current->next != NULL)
   {
      if(current->fd == fd)
         return TRUE;
      current = current->next;
   }
   if (current->fd == fd)
      return TRUE;
   return FALSE;
}

void add_node(node_t **head, int fd)
{
   node_t *node = (node_t*)malloc(sizeof(node_t));
   node->fd = fd;
   memset(&(node->request), 0, sizeof(node->request));
   memset(&(node->response), 0, sizeof(node->response));
   node->reqoffset = 0;
   node->respoffset = 0;
   node->state = READ_STATE;
   node->next = *head;
   *head = node;
}

void remove_node(node_t **head, int fd)
{
   node_t *current = *head;
   node_t *prev = NULL;
   if(current == NULL)
      return;
 /*  if(current->next == NULL)
   {
      if(current->fd == fd)
      {
         *head = NULL;
         free(current);
         return;
      }
      return;
   }*/
   if(current->fd == fd)
   {
      *head = current->next;
      free(current);
      return;
   }
   prev = current;
   current = current->next;
   while(current->next != NULL)
   {
      if(current->fd == fd)
      {
         prev->next = current->next;
         free(current);
         return;
      }
      prev = current;
      current = current->next;
   }
   if (current->fd == fd)
   {
      prev->next = NULL;
      free(current);
      return;
   }
}

void free_list(node_t *head)
{
   node_t *current = head;
   if(current == NULL)
      return;
   node_t *next = head->next;
   while(next != NULL)
   {
      free(current);
      current = next;
      next = current->next;
   }
   free(current);
}

int main(int argc, char* argv[])
{
   add_node(&head, 1);
   remove_node(&head, 1);
   add_node(&head, 2);
   add_node(&head, 3);
   remove_node(&head, 3);
   add_node(&head, 4);
   add_node(&head, 5);
   printf("1 %d\n", in_list(head, 1));
   printf("2 %d\n", in_list(head, 2));
   printf("3 %d\n", in_list(head, 3));
   printf("4 %d\n", in_list(head, 4));
   printf("5 %d\n", in_list(head, 5));
   printf("%d\n", get_node(head, 2)->state);
   printf("%d\n", get_node(head, 4)->state);
   printf("%d\n", get_node(head, 5)->state);
   get_node(head, 2)->state = WRITE_STATE;
   get_node(head, 4)->state = DONE_STATE;
   printf("%d\n", get_node(head, 2)->state);
   printf("%d\n", get_node(head, 4)->state);
   printf("%d\n", get_node(head, 5)->state);
   free_list(head);
   exit(EXIT_SUCCESS);
}
