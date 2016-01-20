
// ATLAS: Adaptable Tool Library for Advanced Simulation
//
// Copyright 2015 University of Central Florida
//
//
// This library provides many fundamental capabilities used in creating
// virtual environment simulations.  It includes elements such as vectors,
// matrices, quaternions, containers, communication schemes (UDP, TCP, DIS,
// HLA, Bluetooth), and XML processing.  It also includes some extensions
// to allow similar code to work in Linux and in Windows.  Note that support
// for iOS and Android development is also included.
//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "atPriorityQueue.h++"

#include <math.h>


atPriorityQueue::atPriorityQueue()
{
   // Initialize the array properties to their defaults.
   max_capacity = AT_PRIORITY_QUEUE_DEFAULT_CAPACITY;
   num_entries = 0;

   // Initialize the array itself.
   heap_array = (atPriorityQueueEntry *)
      malloc(max_capacity * sizeof(atPriorityQueueEntry)); 
}


atPriorityQueue::atPriorityQueue(u_long initialCapacity)
{
   // Initialize the array properties to their defaults.
   max_capacity = initialCapacity;
   if (max_capacity < 1)
      max_capacity = 1;
   num_entries = 0;

   // Initialize the array itself.
   heap_array = (atPriorityQueueEntry *)
      malloc(max_capacity * sizeof(atPriorityQueueEntry));
}


atPriorityQueue::~atPriorityQueue()
{
   // Free the memory that was allocated for the heap
   free(heap_array);
}


u_long atPriorityQueue::getNumEntries()
{
   return num_entries;
}


bool atPriorityQueue::addEntry(atItem * item)
{
   int                      newCapacity;
   atPriorityQueueEntry *   newHeap;

   // If this new item exceeds the current capacity, more space must be
   // allocated.
   if (num_entries == max_capacity)
   {
      // Increase the heap depth by 1, just more than doubling the capacity of
      // the priority queue.
      newCapacity = (max_capacity * 2) + 1;

      // Allocate a new heap.
      newHeap = (atPriorityQueueEntry *)
         realloc(heap_array, newCapacity * sizeof(atPriorityQueueEntry));

      // Make sure the allocation was successful.
      if (newHeap == NULL)
      {
         // We failed to allocate memory so notify user and indicate failure.
         notify(AT_WARN, "Unable to allocate memory for Priority Queue!\n");
         return false;
      }
      else
      {
         // The original array may have been moved, so use the new pointer.
         heap_array = newHeap;
         
         // Set the new max capacity for the heap since allocation was success.
         max_capacity = newCapacity;
      }
   }

   // Store the new item in the array and increment the entry counter.
   heap_array[num_entries].item = item;
   num_entries++;

   // Now bubble the newly-added node up to maintain heap conditions.
   bubbleUp(num_entries - 1);

   // Indicate that the addition was successful.
   return true;
}


atItem * atPriorityQueue::peekEntry()
{
   // If we don't have any items, simply return NULL.
   if (num_entries == 0)
   {
      return NULL;
   }
   else
   {
      // As this is a priority queue, the item to be removed will always be at
      // the head of the list. Return the item associated with this node.
      return heap_array[0].item;
   }
}


atItem * atPriorityQueue::removeEntry()
{
   atItem *   removeItem;

   // If we don't have any items, simply return NULL.
   if (num_entries == 0)
   {
      return NULL;
   }
   else
   {
      // Store the item to be removed so we can resort the heap without losing
      // it.
      removeItem = heap_array[0].item;

      // Move the last item in the heap up to the first position and indicate
      // that the item has been removed.
      heap_array[0].item = heap_array[num_entries - 1].item;
      num_entries--;

      // Now cause the new root to trickle downward to reestablish the heap
      // condition.
      trickleDown(0);

      // Return the item itself.
      return removeItem;
   }
}


void atPriorityQueue::removeAllEntries()
{
   // Simply set the number of valid entries back down to 0.
   num_entries = 0;
}


void atPriorityQueue::bubbleUp(int index)
{
   int        parentIndex;
   atItem *   swapItem;

   // If the object is at the root, it cannot be bubbled up any further.
   if (index == 0)
      return;

   // Calculate the index of the parent node.
   parentIndex = (int)floor((index - 1) / 2.0);

   // If the current item has higher priority than its parent, they need to be
   // swapped.
   if (heap_array[index].item->compare(heap_array[parentIndex].item) > 0)
   {
      // Swap the two items.
      swapItem = heap_array[index].item;
      heap_array[index].item = heap_array[parentIndex].item;
      heap_array[parentIndex].item = swapItem;

      // Recurse on the parent location, which now contains the new item.
      bubbleUp(parentIndex);
   }
}


void atPriorityQueue::trickleDown(int index)
{
   u_long     leftChildIndex;
   u_long     rightChildIndex;
   bool       leftChildHigher;
   bool       rightChildHigher;
   atItem *   swapItem;

   // Calculate the indices of the children of the current node.
   leftChildIndex = (index * 2);
   rightChildIndex = leftChildIndex + 1;

   // Determine whether the left child has a higher priority than this node.
   if ((leftChildIndex < num_entries) &&
      (heap_array[index].item->compare(heap_array[leftChildIndex].item) < 0))
   {
      leftChildHigher = true;
   }
   else
   {
      leftChildHigher = false;
   }

   // Determine whether the right child has a higher priority than this node.
   if ((rightChildIndex < num_entries) &&
      (heap_array[index].item->compare(heap_array[rightChildIndex].item) < 0))
   {
      rightChildHigher = true;
   }
   else
   {
      rightChildHigher = false;
   }

   // Now determine whether this node needs to be swapped.
   if (leftChildHigher && rightChildHigher)
   {
      // Store the item to be traded.
      swapItem = heap_array[index].item;

      // Both children have a higher priority than the current node. The
      // current node must be swapped with the child node with the higher
      // priority.
      if (heap_array[leftChildIndex].item->
         compare(heap_array[rightChildIndex].item) > 0)
      {
         // The left child has higher priority than the right child. The
         // current node should be swapped with the left child.
         heap_array[index].item = heap_array[leftChildIndex].item;
         heap_array[leftChildIndex].item = swapItem;

         // The heap condition must be reestablished on the new left child.
         trickleDown(leftChildIndex);
      }
      else
      {
         // The right child has higher priority than the left child. The
         // current node should be swapped with the right child.
         heap_array[index].item = heap_array[rightChildIndex].item;
         heap_array[rightChildIndex].item = swapItem;

         // The heap condition must be reestablished on the new right child.
         trickleDown(rightChildIndex);
      }
   }
   else if (leftChildHigher)
   {
      // The left child has higher priority than the current item.
      swapItem = heap_array[index].item;
      heap_array[index].item = heap_array[leftChildIndex].item;
      heap_array[leftChildIndex].item = swapItem;

      // The heap condition must be reestablished on the new left child.
      trickleDown(leftChildIndex);
   }
   else if (rightChildHigher)
   {
      // The right child has higher priority than the current item.
      swapItem = heap_array[index].item;
      heap_array[index].item = heap_array[rightChildIndex].item;
      heap_array[rightChildIndex].item = swapItem;

      // The heap condition must be reestablished on the new right child.
      trickleDown(rightChildIndex);
   }
}


