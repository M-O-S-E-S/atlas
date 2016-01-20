
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

                                                                                
#include <string.h>
#include "atJoint.h++"


atJoint::atJoint(atString name, atVector position, atQuat rotation)
{
   // Initialize the entity type to the passed in components
   joint_name = name;
   joint_position = position;
   joint_rotation = rotation;
}


atJoint::atJoint()
{
}


atJoint::~atJoint()
{
   // Nothing to do
}


atString atJoint::getName()
{
   // Return the joint name
   return joint_name;
}


atVector atJoint::getPosition()
{
   // Return the joint position
   return joint_position;
}


atQuat atJoint::getRotation()
{
   // Return the joint rotation
   return joint_rotation;
}


void atJoint::setName(atString name)
{
   // Set this joint's name to the passed in one
   joint_name = name;
}


void atJoint::setPosition(atVector position)
{
   // Set this joint's position to the passed in one
   joint_position = position;
}


void atJoint::setRotation(atQuat rotation)
{
   // Set this joint's rotation to the passed in one
   joint_rotation = rotation;
}


void atJoint::set(atString name, atVector position, atQuat rotation)
{
   // Set this joint's components to the passed in ones
   joint_name = name;
   joint_position = position;
   joint_rotation = rotation;
}


bool atJoint::equals(atString name)
{
   // Return whether or not the passed in joint's name equals this one
   return (joint_name.equals(&name));
}


bool atJoint::equals(atJoint joint)
{
   // Return whether or not the passed in joint equals this one
   return ( (joint_name.equals(&joint.joint_name)) &&
            (joint_position == joint.joint_position) &&
            (joint_rotation == joint.joint_rotation) );
}

