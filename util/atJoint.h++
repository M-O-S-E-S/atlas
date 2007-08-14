                                                                                
#ifndef AT_JOINT_H
#define AT_JOINT_H
                                                                                
                                                                                
// INCLUDES
#include <sys/types.h>
#include "atItem.h++"
#include "atString.h++"
#include "atVector.h++"
#include "atQuat.h++"


class atJoint : public atItem
{
   protected:
      atString   joint_name;
      atVector   joint_position;
      atQuat     joint_rotation;

   public:
      atJoint(atString name, atVector position, atQuat rotation);
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

