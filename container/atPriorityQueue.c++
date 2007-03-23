
#include "atPriorityQueue.h++"


atPriorityQueue::atPriorityQueue()
{
   // Initialize the doubly-linked list
   queue_head = NULL;
   queue_tail = NULL;
   num_entries = 0;
}


atPriorityQueue::~atPriorityQueue()
{
   atPriorityQueueEntry *  current;

   // Go through the list and delete the items and entries
   while (queue_head)
   {
      // Bump the head of the list back one entry, keeping a pointer to the old
      // head so it may be deleted.
      current = queue_head;
      queue_head = queue_head->next;

      // Delete the item if it exists
      if (current->item != NULL)
         delete current->item;

      // Free the memory for the node itself.
      free(current);
   }
}


u_long atPriorityQueue::getNumEntries()
{
   return num_entries;
}


bool atPriorityQueue::insertEntry(atItem * item)
{
   atPriorityQueueEntry *   newEntry;
   atPriorityQueueEntry *   searchEntry;

   // Create a new entry for this item in the list
   newEntry = (atPriorityQueueEntry *) calloc(1, sizeof(atPriorityQueueEntry));
   if (newEntry == NULL)
   {
      // We failed to allocate the memory so tell user and return a failure
      notify(AT_WARN, "Unable to allocate memory in Priority Queue.\n");
      return false;
   }
   else
   {
      // Set the fields of the new entry.
      newEntry->item = item;
      newEntry->next = NULL;
      newEntry->previous = NULL;

      // Add the new entry to its appropriate location in the queue.
      if (queue_head == NULL)
      {
         // This new item is the only item in the queue.
         queue_head = newEntry;
         queue_tail = newEntry;
      }
      else if (queue_head->item->compare(newEntry->item) < 0)
      {
         // This node has a greater value than the current head of the queue.
         // Add it before the current head and update the head pointer to
         // indicate the new node.
         queue_head->previous = newEntry;
         newEntry->next = queue_head;
         queue_head = newEntry;
      }
      else if (queue_tail->item->compare(newEntry->item) > 0)
      {
         // This node has a lower value than the tail of the queue. Add it
         // after the current tail and update the tail pointer to indicate the
         // new node.
         queue_tail->next = newEntry;
         newEntry->previous = queue_tail;
         queue_tail = newEntry;
      }
      else
      {
         // Find the last node whose value is greater than that of the current
         // item. This node will be added next after that node.
         searchEntry = queue_head;
         while (searchEntry != NULL)
         {
            // It is already known that the new item is lesser in value than
            // the current node. If the new item is greater in value than the
            // next item, the new item should be inserted in between the
            // search node and the following node.
            if (searchEntry->next->item->compare(newEntry->item) < 0)
            {
               // Insert the new node between the current and the next nodes.
               newEntry->previous = searchEntry;
               newEntry->next = searchEntry->next;
               searchEntry->next->previous = newEntry;
               searchEntry->next = newEntry;

               // Halt the traversal.
               searchEntry = NULL;
            }
            else
            {
               // Move on to the next node.
               searchEntry = searchEntry->next;
            }
         }
      }

      // Increment the number of entries since we just added one
      num_entries++;

      // Return success
      return true;
   }
}


atItem * atPriorityQueue::removeEntry()
{
   atPriorityQueueEntry *   returnEntry;
   atItem *                 returnItem;

   // If we don't have any items, simply return NULL.
   if (queue_head == NULL)
   {
      return NULL;
   }
   else
   {
      // As this is a priority queue, the item to be removed will always be at
      // the head of the list. Remove this node from the list structurally.
      returnEntry = queue_head;

      // If there is only one item in the list, its removal method is special.
      if (queue_head == queue_tail)
      {
         queue_head = NULL;
         queue_tail = NULL;
      }
      else
      {
         queue_head = queue_head->next;
         queue_head->previous = NULL;
      }

      // Store a pointer to the item held by this node.
      returnItem = returnEntry->item;

      // Free up the memory used by the node.
      free(returnEntry);

      // Decrement the number of items held by this priority queue.
      num_entries--;

      // Return the item itself.
      return returnItem;
   }
}


void atPriorityQueue::removeAllEntries()
{
   // We'll do this by calling removeEntry() for as long as there are any items
   // remaining in the queue.
   while (queue_head != NULL)
   {
       removeEntry();
   }
}


