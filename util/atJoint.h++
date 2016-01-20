
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

                                                                                
#ifndef AT_JOINT_H
#define AT_JOINT_H
                                                                                
                                                                                
// INCLUDES
#include <sys/types.h>
#include "atItem.h++"
#include "atOSDefs.h++"
#include "atString.h++"
#include "atVector.h++"
#include "atQuat.h++"


class ATLAS_SYM atJoint : public atItem
{
   protected:
      atString   joint_name;
      atVector   joint_position;
      atQuat     joint_rotation;

   public:
      atJoint(atString name, atVector position, atQuat rotation);
      atJoint();
      virtual ~atJoint();

      virtual atString   getName();
      virtual atVector   getPosition();
      virtual atQuat     getRotation();

      virtual void     setName(atString name);
      virtual void     setPosition(atVector position);
      virtual void     setRotation(atQuat rotation);
      virtual void     set(atString name, atVector position, atQuat rotation);

      virtual bool     equals(atString name);
      virtual bool     equals(atJoint joint);
};


#endif

