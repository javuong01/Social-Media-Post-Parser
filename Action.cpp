#include "Action.h"

Action::Action(std::string core_arg_host_url, std::string core_arg_owner_vsID,                 // takes in information from the json as string arguments
	       std::string core_arg_class_id, std::string core_arg_object_id, std::string arg_name)
    : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  this->name = arg_name;
}

Json::Value * Action::dumpJ(void)
{
  int i;

  Json::Value *result_ptr = new Json::Value();      // this part creates a singular pointer for the new json object.

  (*result_ptr)["class_id"] = this->class_id;   //line 16 to 20 fills in back to the json format
  (*result_ptr)["host_url"] = this->host_url;
  (*result_ptr)["name"] = this->name;
  (*result_ptr)["object_id"] = this->object_id;
  (*result_ptr)["owner_vsID"] = this->owner_vsID;

#ifdef _ECS36B_DEBUG_
  std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
  return result_ptr;                         //returns the pointer for the new json object
}