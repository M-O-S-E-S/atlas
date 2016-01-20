
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


#include <stdlib.h>
#include <stdio.h>
#include "atGlobals.h++"
#include "atString.h++"

#include "atMap.h++"

// ------------------------------------------------------------------------
// Constructor - Sets the tree to empty
// ------------------------------------------------------------------------
atMap::atMap()
{
    // Tree initially has not root node and no nodes
    treeRoot = NULL;
    treeSize = 0;
}

// ------------------------------------------------------------------------
// Destructor - Deletes the contents of the tree
// ------------------------------------------------------------------------
atMap::~atMap()
{
    // Delete all tree entries
    clear();
}

// ------------------------------------------------------------------------
// Adds a new mapping from key to value to the tree. Returns true if
// successful, or false if a mapping for that key already exists.
// ------------------------------------------------------------------------
bool atMap::addEntry(atItem * key, atItem * value)
{
    bool found;

    // Make sure that a node with the given key isn't already in
    // the tree.
    if (containsKey(key))
        return false;

    // Create the new node using the given key and value. New nodes
    // are initially colored red.
    atMapNode *newNode = new atMapNode;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->parent = NULL;
    newNode->color = AT_MAP_RED;
    newNode->nodeKey = key;
    newNode->nodeValue = value;
    
    // If the tree is empty, then the new node becomes the root.
    if (treeRoot == NULL)
    {
        // Set the root node, color it black (root nodes are always black),
        // increment the entry count, and return success.
        treeRoot = newNode;
        treeRoot->color = AT_MAP_BLACK;
        treeSize++;
        return true;
    }
    
    // The tree isn't empty. Do a binary search on the tree to determine
    // the correct location for the new node.
    atMapNode *nodeParent = treeRoot;
    found = false;
    while (!found)
    {
        // Branch left or right based on key comparison
        if (newNode->nodeKey->compare(nodeParent->nodeKey) < 0)
        {
            // Left subtree
            if (nodeParent->leftChild == NULL)
            {
                // Place the new node as the left child
                nodeParent->leftChild = newNode;
                newNode->parent = nodeParent;
                found = true;
            }
            else
            {
                // Move to the left child and keep searching
                nodeParent = nodeParent->leftChild;
            }
        }
        else
        {
            // Check for collision and print a loud warning if detected
            if (newNode->nodeKey->equals(nodeParent->nodeKey))
               notify(AT_WARN, "Key collision detected in map!\n");

            // Right subtree
            if (nodeParent->rightChild == NULL)
            {
                // Place the new node as the right child
                nodeParent->rightChild = newNode;
                newNode->parent = nodeParent;
                found = true;
            }
            else
            {
                // Move to the right child and keep searching
                nodeParent = nodeParent->rightChild;
            }
        }
    }
    
    // Clean up the tree after the insertion
    rebalanceInsert(newNode);
    treeRoot->color = AT_MAP_BLACK;

    // Increase entry count by one and return success
    treeSize++;
    return true;
}

// ------------------------------------------------------------------------
// Removes the mapping associated with the given key from the tree. Returns
// true if successful, or false if the key is not in the tree.
// ------------------------------------------------------------------------
bool atMap::deleteEntry(atItem * key)
{
    atMapNode *targetNode;
    
    // Find the node in the tree with the given key. Abort if there
    // is no such node.
    targetNode = findNode(treeRoot, key);
    if (targetNode == NULL)
        return false;

    // Call an internal function to do the actual deletion
    deleteNode(targetNode);

    // The last part of cleaning up the tree, which is the only part that
    // deleteNode() doesn't do by itself, is forcing the root node to be
    // black.
    if (treeRoot)
        treeRoot->color = AT_MAP_BLACK;

    // Decrease entry count by one and return success
    treeSize--;
    return true;
}

// ------------------------------------------------------------------------
// Removes the entry specified by the key from the map, returning the
// value.  Yields ownership of both the key and value (neither are 
// deleted).  Returns NULL if there is no entry with the given key.
// ------------------------------------------------------------------------
atItem * atMap::removeEntry(atItem * key)
{
    atMapNode * targetNode;
    atItem * targetKey;
    atItem * targetValue;
    
    // Find the node in the tree with the given key. Abort if there
    // is no such node.
    targetNode = findNode(treeRoot, key);
    if (targetNode == NULL)
        return NULL;

    // Get the node's key and value entries before we remove the node
    // (we'll need them later)
    targetKey = targetNode->nodeKey;
    targetValue = targetNode->nodeValue;

    // Call an internal function to do the actual node removal
    removeNode(targetNode);

    // If the given key and the key previously stored in the tree are
    // different objects (i.e.: two different instances), we need to
    // delete the stored key to avoid a memory leak.  This should be
    // OK, as the user will still have the given key if needed
    if ((void *)key != (void *)targetKey)
       delete targetKey;

    // The last part of cleaning up the tree, which is the only part that
    // removeNode() doesn't do by itself, is forcing the root node to be
    // black.
    if (treeRoot)
        treeRoot->color = AT_MAP_BLACK;

    // Decrease entry count by one and return success
    treeSize--;
    return targetValue;
}

// ------------------------------------------------------------------------
// Returns the number of mappings contained in this tree
// ------------------------------------------------------------------------
u_long atMap::getNumEntries()
{
    return treeSize;
}

// ------------------------------------------------------------------------
// Checks if a mapping for the given key is present in the tree. Returns
// true if so, false if not.
// ------------------------------------------------------------------------
bool atMap::containsKey(atItem * key)
{
    // Call our helper function to find the node with the given key
    if (findNode(treeRoot, key) != NULL)
        return true;
    else
        return false;
}

// ------------------------------------------------------------------------
// Returns the value associated with the given key, or NULL if that key is
// not present within the tree.
// ------------------------------------------------------------------------
atItem * atMap::getValue(atItem * key)
{
    atMapNode *node;
    
    // Call our helper function to find the node with the given key
    node = findNode(treeRoot, key);
    if (node)
        return (node->nodeValue);

    // Return NULL if the target key wasn't found
    return NULL;
}

// ------------------------------------------------------------------------
// Attempts to change the value associated with the given key to newValue.
// Return the old value if successful, NULL if the given key is not present
// within the tree.
// ------------------------------------------------------------------------
atItem * atMap::changeValue(atItem * key, atItem * newValue)
{
    atMapNode *node;
    atItem *oldValue;
    
    // Call our helper function to find the node with the given key
    node = findNode(treeRoot, key);
    if (!node)
        return NULL;

    // If found, remove the current value, set the new value and return the 
    // old value
    oldValue = node->nodeValue;
    node->nodeValue = newValue;
    return oldValue;
}

// ------------------------------------------------------------------------
// Removes all mappings from the tree
// ------------------------------------------------------------------------
void atMap::clear()
{
    // No work to do if the tree is already empty
    if (treeRoot == NULL)
        return;

    // The deleteTree function does all of the actual work
    deleteTree(treeRoot);
    
    // Set the tree to empty
    treeRoot = NULL;
    treeSize = 0;
}

// ------------------------------------------------------------------------
// Fills the keyList and valueList with the keys and values from the tree,
// respectively. Each element of one list corresponds to the element with
// the same index from the other list. Corresponding element pairs are
// sorted in ascending key order.
// ------------------------------------------------------------------------
void atMap::getSortedList(atList * keyList, atList * valueList)
{
    // No work to do if the tree is empty
    if (treeSize == 0)
        return;

    // Call a helper function to copy the tree data to the arrays
    fillLists(treeRoot, keyList, valueList);

    // Error checking
    if (keyList != NULL)
    {
        // Make sure the key list makes sense
        if (keyList->getNumEntries() != treeSize)       
        {      
            notify(AT_ERROR, "atMap::getSortedList: Map Inconsistency:\n");   
            notify(AT_ERROR, "   Number of entries in map %d is not equal to "
                "the map's stated size %d\n", keyList->getNumEntries(),
                treeSize);      
        }
    }
    else if (valueList != NULL)
    {
        // Make sure the value list makes sense
        if (valueList->getNumEntries() != treeSize)       
        {      
            notify(AT_ERROR, "atMap::getSortedList: Map Inconsistency:\n");   
            notify(AT_ERROR, "   Number of entries in map %d is not equal to "
                "the map's stated size %d\n", valueList->getNumEntries(),
                treeSize);      
        }
    }
}

// ------------------------------------------------------------------------
// Private function
// Searches the subtree rooted at 'node' for a node with the given key.
// Returns that node, or NULL if it can't find it.
// ------------------------------------------------------------------------
atMapNode *atMap::findNode(atMapNode * node, atItem * key)
{
    // If the target node is NULL, then this definitely isn't the
    // node we're looking for
    if (node == NULL)
        return NULL;

    // If the keys match, return the target node
    if (node->nodeKey->equals(key))
        return node;

    // Otherwise, search a child for the key; which child to search is
    // determined by comparing key values
    if (key->compare(node->nodeKey) > 0)
        return findNode(node->rightChild, key);
    else
        return findNode(node->leftChild, key);
}

// ------------------------------------------------------------------------
// Private function
// Rebalances the tree after an insertion operation. After inserting, since
// new nodes are colored red, only check for red-red rule violations; the
// black-balance rule can't have been violated.
// ------------------------------------------------------------------------
void atMap::rebalanceInsert(atMapNode * node)
{
    atMapNode *parent, *grandparent, *uncle;
    int nodeChildType, parentChildType;

    // If this node is black, there's no work to do
    if (node->color == AT_MAP_BLACK)
        return;

    // If the parent is black (or nonexistant), there's no work to do
    parent = node->parent;
    if (parent == NULL)
        return;
    if (parent->color == AT_MAP_BLACK)
        return;

    // If there's no grandparent node, then there's no work to do here.
    // Both this node and its parent are red, which should be a violation,
    // but if there's no grandparent then the parent must be the tree's
    // root node, and the root is automatically set to black as the last
    // step of insertion cleanup.
    grandparent = parent->parent;
    if (grandparent == NULL)
        return;

    // If this node's 'uncle' is red, then balance can be restored by
    // simply 'splitting' the grandparent's black value; parent and
    // uncle become black, and grandparent becomes red, which fixes
    // the red-red violation without affecting the black-balance. However,
    // this can cause a red-red violation at grandparent if it is changed
    // to red, so the rebalancing process must iterate again up the tree.
    parentChildType = getChildType(parent);
    if (parentChildType == AT_MAP_LEFTCHILD)
        uncle = grandparent->rightChild;
    else
        uncle = grandparent->leftChild;
    if (uncle && (uncle->color == AT_MAP_RED))
    {
        grandparent->color = AT_MAP_RED;
        parent->color = AT_MAP_BLACK;
        uncle->color = AT_MAP_BLACK;
        rebalanceInsert(grandparent);
        return;
    }

    // At this point, a rotation or two and some strategic node recoloring
    // should fix the problem.
    nodeChildType = getChildType(node);
    if (parentChildType == AT_MAP_LEFTCHILD)
    {
        // Force node to be a left-child, if it isn't already
        if (nodeChildType == AT_MAP_RIGHTCHILD)
        {
            rotateLeft(parent);
            node = parent;
            parent = node->parent;
        }

        // A right rotation at grandparent and a color swap should fix
        // the red-red problem without introducing any other problems
        rotateRight(grandparent);
        parent->color = AT_MAP_BLACK;
        grandparent->color = AT_MAP_RED;
    }
    else
    {
        // Force node to be a right-child, if it isn't already
        if (nodeChildType == AT_MAP_LEFTCHILD)
        {
            rotateRight(parent);
            node = parent;
            parent = node->parent;
        }

        // A left rotation at grandparent and a color swap should fix
        // the red-red problem without introducing any other problems
        rotateLeft(grandparent);
        parent->color = AT_MAP_BLACK;
        grandparent->color = AT_MAP_RED;
    }
}

// ------------------------------------------------------------------------
// Private function
// Rebalance the tree after a deletion operation. Deletion operations can
// violate both the red-red rule and the black-balance rule, but since this
// function is only called after a black node was deleted then concentrate
// on restoring the black-balance and any red-red violations will get
// cleaned up at the same time.
// ------------------------------------------------------------------------
void atMap::rebalanceDelete(atMapNode * parent, int deletedChildType)
{
    atMapNode *child, *sibling;
    
    // If we deleted the root node, there's no rebalancing work to do
    if (deletedChildType == AT_MAP_ROOTNODE)
        return;

    // If the child that took the place of the deleted node exists (isn't
    // NULL) and is red, then changing it to black will restore the 
    // black-balance without doing any other damage.
    if (deletedChildType == AT_MAP_LEFTCHILD)
        child = parent->leftChild;
    else
        child = parent->rightChild;
    if (child && (child->color == AT_MAP_RED))
    {
        child->color = AT_MAP_BLACK;
        return;
    }
    
    // If we got this far, then we have to do it the hard way. Obtain
    // the 'sibling' (parent's other child) of the deleted node and
    // manipulate that in order to restore the black-balance. This sibling
    // node _must_ exist (can't be NULL) if a black node was deleted,
    // because otherwise the tree wouldn't have been black-balanced before
    // the deletion.
    if (deletedChildType == AT_MAP_LEFTCHILD)
    {
        // Get the sibling
        sibling = parent->rightChild;

        // If it isn't already, force the sibling to be black by rotatng
        // the subtree and swapping colors around.
        if (sibling->color == AT_MAP_RED)
        {
            rotateLeft(parent);
            parent->color = AT_MAP_RED;
            sibling->color = AT_MAP_BLACK;
            sibling = parent->rightChild;
        }
        
        // Case 1: Sibling's children are both black

        // If both of the children of the sibling node are black (or
        // nonexistant), then we can color the sibling red. However,
        // this effectively chases the problem farther up the tree,
        // so rebalance there.
        if ( ((sibling->leftChild == NULL) ||
              (sibling->leftChild->color == AT_MAP_BLACK)) &&
             ((sibling->rightChild == NULL) ||
              (sibling->rightChild->color == AT_MAP_BLACK)) )
        {
            sibling->color = AT_MAP_RED;
            rebalanceDelete(parent->parent, getChildType(parent));
            return;
        }
        
        // Case 2: At least one of sibling's children is red

        // If sibling's left child is red, then manipulate the
        // tree so that only the right child is red. This can
        // temporarily create a red-red violation, but the next block
        // of code will fix that.
        if ((sibling->leftChild) &&
            (sibling->leftChild->color == AT_MAP_RED))
        {
            sibling->leftChild->color = AT_MAP_BLACK;
            sibling->color = AT_MAP_RED;
            rotateRight(sibling);
            sibling = parent->rightChild;
        }
        
        // Sibling's right child must be red; the imbalance can be
        // repaired here by a rotation and some color swapping.
        rotateLeft(parent);
        sibling->color = parent->color;
        parent->color = AT_MAP_BLACK;
        sibling->rightChild->color = AT_MAP_BLACK;
    }
    else
    {
        // Get the sibling
        sibling = parent->leftChild;

        // If it isn't already, force the sibling to be black by rotatng
        // the subtree and swapping colors around.
        if (sibling->color == AT_MAP_RED)
        {
            rotateRight(parent);
            parent->color = AT_MAP_RED;
            sibling->color = AT_MAP_BLACK;
            sibling = parent->leftChild;
        }
        
        // Case 1: Sibling's children are both black

        // If both of the children of the sibling node are black (or
        // nonexistant), then we can color the sibling red. However,
        // this effectively chases the problem farther up the tree,
        // so rebalance there.
        if ( ((sibling->leftChild == NULL) ||
              (sibling->leftChild->color == AT_MAP_BLACK)) &&
             ((sibling->rightChild == NULL) ||
              (sibling->rightChild->color == AT_MAP_BLACK)) )
        {
            sibling->color = AT_MAP_RED;
            rebalanceDelete(parent->parent, getChildType(parent));
            return;
        }
        
        // Case 2: At least one of sibling's children is red

        // If sibling's right child is red, then manipulate the
        // tree so that only the left child is red. This can
        // temporarily create a red-red violation, but the next block
        // of code will fix that.
        if ((sibling->rightChild) &&
            (sibling->rightChild->color == AT_MAP_RED))
        {
            sibling->rightChild->color = AT_MAP_BLACK;
            sibling->color = AT_MAP_RED;
            rotateLeft(sibling);
            sibling = parent->leftChild;
        }
        
        // Sibling's left child must be red; the imbalance can be
        // repaired here by a rotation and some color swapping.
        rotateRight(parent);
        sibling->color = parent->color;
        parent->color = AT_MAP_BLACK;
        sibling->leftChild->color = AT_MAP_BLACK;
    }
}

// ------------------------------------------------------------------------
// Private function
// Deletes the specified node from the tree, calling the function to
// restore the tree balance afterwards if needed.
// ------------------------------------------------------------------------
void atMap::deleteNode(atMapNode * node)
{
    int childType = getChildType(node);
    atMapNode *parent = node->parent;
    atMapNode *child;
    atItem * tempItem;

    // Switch based on the number of children the node has
    if ((node->leftChild == NULL) && (node->rightChild == NULL))
    {
        // Case 1: node to delete has no children
        // Remove the node and rebalance

        // Remove the node
        if (childType == AT_MAP_LEFTCHILD)
            parent->leftChild = NULL;
        else if (childType == AT_MAP_RIGHTCHILD)
            parent->rightChild = NULL;
        else
            treeRoot = NULL;
        
        // Rebalance the tree if needed
        if (node->color == AT_MAP_BLACK)
            rebalanceDelete(parent, childType);
        
        // Delete the removed node
        if (node->nodeKey != NULL)
            delete node->nodeKey;
        if (node->nodeValue != NULL)
            delete node->nodeValue;
        delete node;
    }
    else if ((node->leftChild == NULL) || (node->rightChild == NULL))
    {
        // Case 2: node to delete has one child
        // Move the child node into the location that the node to
        // be deleted is in, and rebalance

        // Get the child node
        if (node->leftChild)
            child = node->leftChild;
        else
            child = node->rightChild;

        // Reparent the child node
        child->parent = parent;

        // Rechild the parent node
        if (childType == AT_MAP_LEFTCHILD)
            parent->leftChild = child;
        else if (childType == AT_MAP_RIGHTCHILD)
            parent->rightChild = child;
        else
            treeRoot = child;
        
        // Rebalance the tree if needed
        if (node->color == AT_MAP_BLACK)
            rebalanceDelete(parent, childType);

        // Delete the removed node
        if (node->nodeKey != NULL)
            delete node->nodeKey;
        if (node->nodeValue != NULL)
            delete node->nodeValue;
        delete node;
    }
    else
    {
        // Case 3: node to delete has two children
        // Rather than deleting the node, instead find the node with
        // the next-higher key value, transplant that value into the
        // node that would have been deleted, and delete that other node.

        // Find the node with the 'next' value
        child = getInorderSuccessor(node);
        
        // Swap the keys
        tempItem = node->nodeKey;
        node->nodeKey = child->nodeKey;
        child->nodeKey = tempItem;

        // Swap the values
        tempItem = node->nodeValue;
        node->nodeValue = child->nodeValue;
        child->nodeValue = tempItem;
        
        // Delete the 'next' node instead
        deleteNode(child);
    }
}

// ------------------------------------------------------------------------
// Private function
// Removes the specified node from the tree, calling the function to
// restore the tree balance afterwards if needed.  Deletes the node
// structure, but unlike deleteNode(), it does not delete the contents of
// the node.
// ------------------------------------------------------------------------
void atMap::removeNode(atMapNode * node)
{
    int childType = getChildType(node);
    atMapNode *parent = node->parent;
    atMapNode *child;
    atItem * tempItem;

    // Switch based on the number of children the node has
    if ((node->leftChild == NULL) && (node->rightChild == NULL))
    {
        // Case 1: node to delete has no children
        // Remove the node and rebalance

        // Remove the node
        if (childType == AT_MAP_LEFTCHILD)
            parent->leftChild = NULL;
        else if (childType == AT_MAP_RIGHTCHILD)
            parent->rightChild = NULL;
        else
            treeRoot = NULL;
        
        // Rebalance the tree if needed
        if (node->color == AT_MAP_BLACK)
            rebalanceDelete(parent, childType);
        
        // Delete the detached node
        delete node;
    }
    else if ((node->leftChild == NULL) || (node->rightChild == NULL))
    {
        // Case 2: node to delete has one child
        // Move the child node into the location that the node to
        // be deleted is in, and rebalance

        // Get the child node
        if (node->leftChild)
            child = node->leftChild;
        else
            child = node->rightChild;

        // Reparent the child node
        child->parent = parent;

        // Rechild the parent node
        if (childType == AT_MAP_LEFTCHILD)
            parent->leftChild = child;
        else if (childType == AT_MAP_RIGHTCHILD)
            parent->rightChild = child;
        else
            treeRoot = child;
        
        // Rebalance the tree if needed
        if (node->color == AT_MAP_BLACK)
            rebalanceDelete(parent, childType);

        // Delete the detached node
        delete node;
    }
    else
    {
        // Case 3: node to delete has two children
        // Rather than deleting the node, instead find the node with
        // the next-higher key value, transplant that value into the
        // node that would have been deleted, and delete that other node.

        // Find the node with the 'next' value
        child = getInorderSuccessor(node);
        
        // Swap the keys
        tempItem = node->nodeKey;
        node->nodeKey = child->nodeKey;
        child->nodeKey = tempItem;

        // Swap the values
        tempItem = node->nodeValue;
        node->nodeValue = child->nodeValue;
        child->nodeValue = tempItem;
        
        // Remove the 'next' node instead
        removeNode(child);
    }
}

// ------------------------------------------------------------------------
// Private function
// Searches the tree for the node with the next-higher key than the given
// node's key. Returns NULL if no such node exists.
// ------------------------------------------------------------------------
atMapNode *atMap::getInorderSuccessor(atMapNode * node)
{
    // If there is no node with a greater key, abort.
    if (node->rightChild == NULL)
        return NULL;

    // The node with the next highest key must be the node with the
    // smallest key in the original node's right subtree. 
    atMapNode *result = node->rightChild;
    while (result->leftChild)
        result = result->leftChild;

    // Return the node
    return result;
}

// ------------------------------------------------------------------------
// Private function
// Performs a left rotation at the subtree rooted at the given node. A
// left rotation rearranges nodes in this pattern:
//
//   parent                   parent
//     |                         |
//    left(= node)             right
//   /    \         ->        /     \
//  *      right          left       *
//        /     \        /    \
//   child       *      *      child
//
// 'parent' and 'child' may be NULL, 'left' and 'right' must not be.
// ------------------------------------------------------------------------
void atMap::rotateLeft(atMapNode * node)
{
    atMapNode *left, *right, *child, *parent;
    int childType;
    
    // 'right' must not be NULL
    if (node->rightChild == NULL)
    {
        notify(AT_ERROR, "atMap::rotateLeft: Can't rotate left on a node "
            "with no right child\n");
        return;
    }
    
    // Assign temporary pointers
    left = node;
    right = left->rightChild;
    child = right->leftChild;
    parent = left->parent;
    
    // Determine what kind of child the target node is
    childType = getChildType(node);
    
    // Perform the rotation
    left->rightChild = child;
    left->parent = right;
    right->leftChild = left;
    right->parent = parent;
    if (child)
        child->parent = left;
    
    // Correct which node the parent points to
    if (childType == AT_MAP_LEFTCHILD)
        parent->leftChild = right;
    else if (childType == AT_MAP_RIGHTCHILD)
        parent->rightChild = right;
    else
        treeRoot = right;
}

// ------------------------------------------------------------------------
// Private function
// Performs a right rotation at the subtree rooted at the given node. A
// right rotation rearranges nodes in this pattern:
//
//        parent               parent
//           |                   |
//         right(= node)        left
//        /     \         ->   /    \
//    left       *            *      right
//   /    \                         /     \
//  *      child               child       *
//
// 'parent' and 'child' may be NULL, 'right' and 'left' must not be.
// ------------------------------------------------------------------------
void atMap::rotateRight(atMapNode * node)
{
    atMapNode *left, *right, *child, *parent;
    int childType;
    
    // 'left' must not be NULL
    if (node->leftChild == NULL)
    {
        notify(AT_ERROR, "atMap::rotateRight: Can't rotate right on a node "
            "with no left child\n");
        return;
    }
    
    // Assign temporary pointers
    right = node;
    left = right->leftChild;
    child = left->rightChild;
    parent = right->parent;
    
    // Determine what kind of child the target node is
    childType = getChildType(node);

    // Perform the rotation
    right->leftChild = child;
    right->parent = left;
    left->rightChild = right;
    left->parent = parent;
    if (child)
        child->parent = right;
    
    // Correct which node the parent points to
    if (childType == AT_MAP_LEFTCHILD)
        parent->leftChild = left;
    else if (childType == AT_MAP_RIGHTCHILD)
        parent->rightChild = left;
    else
        treeRoot = left;
}

// ------------------------------------------------------------------------
// Private function
// Destroys the subtree rooted at the given node, without any cleaning-up
// of the tree afterwards. Called by the clear() method.
// ------------------------------------------------------------------------
void atMap::deleteTree(atMapNode * node)
{
    // No tree, no work
    if (node == NULL)
        return;

    // Recurse on the node's children
    deleteTree(node->leftChild);
    deleteTree(node->rightChild);
    
    // Destroy this node along with the items it contains
    if (node->nodeKey != NULL)
        delete node->nodeKey;
    if (node->nodeValue != NULL)
        delete node->nodeValue;
    delete node;
}

// ------------------------------------------------------------------------
// Private function
// Determines the child type of the given node. A child's type indicates
// whether it is the left or right child of its parent, or doesn't have
// a parent at all (and is the root of the tree).
// ------------------------------------------------------------------------
int atMap::getChildType(atMapNode * node)
{
    atMapNode *parent = node->parent;
    
    // Figure out which child of its parent the node is
    if (parent == NULL)
        return AT_MAP_ROOTNODE;
    if (parent->leftChild == node)
        return AT_MAP_LEFTCHILD;
    if (parent->rightChild == node)
        return AT_MAP_RIGHTCHILD;

    // Error checking
    notify(AT_ERROR, "atMap::getChildType: Map Inconsistency: 'node' is not "
        "a child of its own parent!\n");
    return -1;
}

// ------------------------------------------------------------------------
// Private function
// Traverses the tree rooted at the given node, copying map entries into
// the given two lists
// ------------------------------------------------------------------------
void atMap::fillLists(atMapNode * node, atList * keyList, atList * valueList)
{
    // No work to do if there's no tree
    if (!node)
        return;

    // Inorder - traverse left child
    fillLists(node->leftChild, keyList, valueList);
    
    // Add the key and value to their respective lists
    if (keyList != NULL)
        keyList->addEntry(node->nodeKey);
    if (valueList != NULL)
        valueList->addEntry(node->nodeValue);
    
    // Inorder - traverse right child
    fillLists(node->rightChild, keyList, valueList);
}

void atMap::print()
{
    // Print a header for the map tree first
    printf("atMap %p (%lu entries):\n", this, treeSize);

    // Call the recursive printTree() method with the root of the red-black
    // tree and a zero indent
    if (treeRoot != NULL)
        printTree(treeRoot, 0);
}

void atMap::printTree(atMapNode *node, int indent)
{
    int i;
    atString * itemStr;

    // Print this node's information
    // Start with an opening brace
    for (i = 0; i < indent; i++)
        printf(" ");
    printf("{\n");

    // Print the node's address
    for (i = 0; i < indent+2; i++)
        printf(" ");
    printf("Node         %p\n", node);

    // Print the node's color
    for (i = 0; i < indent+2; i++)
        printf(" ");
    switch(node->color)
    {
        case AT_MAP_BLACK:
            printf("Color        %s\n", "BLACK");
            break;
        case AT_MAP_RED:
            printf("Color        %s\n", "RED");
            break;
    };

    // Print the node's key item pointer
    for (i = 0; i < indent+2; i++)
        printf(" ");
    printf("Key          %p", node->nodeKey);

    // If the node key is an atString, append the string to the printout
    // for additional information
    if (itemStr = dynamic_cast<atString *>(node->nodeKey))
        printf("  \"%s\"\n", itemStr->getString());
    else
        printf("\n");

    // Print the node's value item pointer
    for (i = 0; i < indent+2; i++)
        printf(" ");
    printf("Value        %p", node->nodeValue);

    // If the node value is an atString, append the string to the printout
    // for additional information
    if (itemStr = dynamic_cast<atString *>(node->nodeValue))
        printf("  \"%s\"\n", itemStr->getString());
    else
        printf("\n");

    // Leave a blank line before printing the linkage information
    printf("\n");

    // Print the node's parent node pointer
    for (i = 0; i < indent+2; i++)
        printf(" ");
    printf("Parent       %p\n", node->parent);

    // Now, traverse and print the subtrees
    // First, the left child
    for (i = 0; i < indent+2; i++)
        printf(" ");
    if (node->leftChild != NULL)
    {
        printf("Left Child:  %p\n", node->leftChild);
        printTree(node->leftChild, indent + 2);
    }
    else
        printf("Left Child:  (none)\n");

    // Then, the right child
    for (i = 0; i < indent+2; i++)
        printf(" ");
    if (node->rightChild != NULL)
    {
        printf("Right Child: %p\n", node->rightChild);
        printTree(node->rightChild, indent + 2);
    }
    else
        printf("Right Child: (none)\n");

    // Finish with a closing brace
    for (i = 0; i < indent; i++)
        printf(" ");
    printf("}\n");
}

