                                                                                
#include <string.h>
#include "atJoint.h++"


atJoint::atJoint(atString name, atVector position, atQuat rotation)
{
   // Initialize the entity type to the passed in components
   joint_name = name;
   joint_position = position;
   joint_rotation = rotation;
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

