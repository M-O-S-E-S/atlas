
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


#ifndef AT_MAP_HPP
#define AT_MAP_HPP

#include "atGlobals.h++"
#include "atList.h++"
#include "atItem.h++"
#include "atNotifier.h++"
#include "atOSDefs.h++"

enum atMapColor
{
    AT_MAP_BLACK,
    AT_MAP_RED
};

enum atMapChildType
{
    AT_MAP_LEFTCHILD,
    AT_MAP_RIGHTCHILD,
    AT_MAP_ROOTNODE
};

struct atMapNode
{
    atMapNode *   leftChild;
    atMapNode *   rightChild;
    atMapNode *   parent;
    int           color;
    
    atItem *      nodeKey;
    atItem *      nodeValue;
};

class ATLAS_SYM atMap : public atNotifier
{
private:

    atMapNode *   treeRoot;
    u_long        treeSize;
    
    atMapNode *       findNode(atMapNode * node, atItem * key);
    void              rebalanceInsert(atMapNode * node);
    void              rebalanceDelete(atMapNode * parent,
                                      int deletedChildType);
    void              deleteNode(atMapNode * node);
    void              removeNode(atMapNode * node);
    atMapNode *       getInorderSuccessor(atMapNode * node);
    void              rotateLeft(atMapNode * node);
    void              rotateRight(atMapNode * node);
    void              deleteTree(atMapNode * node);
    int               getChildType(atMapNode * node);
    void              fillLists(atMapNode * node, atList * keyList,
                                atList * valueList);
    void              printTree(atMapNode * node, int indent);

public:

                atMap();
    virtual     ~atMap();

    bool        addEntry(atItem * key, atItem * value);
    bool        deleteEntry(atItem * key);
    atItem *    removeEntry(atItem * key);
    u_long      getNumEntries();
    
    bool        containsKey(atItem * key);
    atItem *    getValue(atItem * key);
    atItem *    changeValue(atItem * key, atItem * newValue);

    void        clear();

    void        getSortedList(atList * keyList,
                              atList * valueList);

    void        print();
};

#endif
