//redacted

// JSON_Post_redacted.cpp
// #define _ECS36B_DEBUG_

#include <arpa/inet.h>
#include "Post.h"
//#include <string.h>

JvTime *
getNowJvTime
(void)
{
  time_t ticks;
  ticks = time(NULL);
  struct std::tm *my_tm_ptr = gmtime(&ticks);
  JvTime * jvT_ptr = new JvTime();
  int rc = jvT_ptr->setStdTM(my_tm_ptr);
  if (rc != 0)
    {
      std::cout << "error: failed to set time!" << std::endl;
      delete jvT_ptr;
      return NULL;
    }
  return jvT_ptr;
}

std::map<std::string, OKey *> All_Keys;

std::vector<Post *> *
getPostbyKey
(std::string key)
{
  if (All_Keys.find(key) == All_Keys.end())
    return NULL;

  OKey *okey_ptr = All_Keys[key];
  return okey_ptr->posts;
}

OKey *
getOKey
(std::string key)
{
  if (All_Keys.find(key) != All_Keys.end())
    return All_Keys[key];

  OKey *new_okey_ptr = new OKey(key);
  new_okey_ptr->posts = new std::vector<Post *>();
  All_Keys[key] = new_okey_ptr;
  return new_okey_ptr;
}

std::string
Json_2_PostID
(Json::Value myv)
{
  return ((myv["id"]).asString());
}

Transaction *
Json_Transaction_Common
(Json::Value myv)
{
  Transaction *transaction_ptr = NULL;
  if (((myv["avatar_name"]).isNull() != true ) &&
  ((myv["avatar_name"]).isString() == true) &&
  (((myv["avatar_name"]).asString()).size() > 0) &&
  ((myv["vsID"]).isNull() != true) &&
  ((myv["vsID"]).isString() == true) &&
  (((myv["vsID"]).asString()).size() > 0))
  {
    transaction_ptr = new Transaction(0);
    transaction_ptr->status = 0;

    Person *agent_ptr = new Person ((myv ["avatar_name"]).asString(), (myv["vsID"].asString()));
    transaction_ptr->agent = agent_ptr;

    if(((myv["location"]).isNull() != true) &&
      ((myv["location"]["latitude"]).isNull() != true) &&
      ((myv["location"]["longitude"]).isNull() != true) &&
      ((myv["location"]["latitude"]).isDouble() == true) &&
      ((myv["location"]["longitude"]).isDouble() == true))
      {
        GPS_DD * gps_dd_ptr = new GPS_DD((myv["location"]["latitude"]).isDouble(), (myv["location"]["longitude"]).isDouble());
        transaction_ptr->where = gps_dd_ptr;
      }  
    
    if (((myv["network"]).isNull() != true) &&
      ((myv["network"]["IPv4"]).isNull() != true) &&
      ((myv["network"]["IPv4"]).isString() == true) &&
      (((myv["network"]["IPv4"]).asString()) .size() > 0))
      {
        IP_Address * ip_ptr = new IP_Address(inet_addr(((myv["network"]["IPv4"]).asString()).c_str()));
        transaction_ptr->srcIP = ip_ptr;
      }
    if (((myv["created_time"]).isNull() != true) &&
      ((myv["created_time"]).isString() == true) &&
      (((myv["created_time"]).asString()).size() > 0))
      {
        JvTime * cr_time_ptr = new JvTime (((myv["created_time"]).asString()).c_str());
        transaction_ptr->when = cr_time_ptr;
      }
  }
  return transaction_ptr;
}

// here it is
int
Json_Post_Common
(Json::Value myv, Post * my_post_ptr)
{
  int i, j;

  // under Json_Post_Common, we assume that the ID already matched
  // for both Post and myv

  // "actions"
  my_post_ptr->actions = new vector<Action *>();
    if ((myv["actions"]["count"]).asInt() > 0)
  // if  (((myv["actions"]).isNull() != true) &&        //not really sure if these checks worked properly
  //   ((myv["actions"]["count"]).asInt() > 0))
  {
      for (i = 0; i < (myv["actions"]["count"]).asInt(); i++)
	{
	  Json::Value l_jv = myv["actions"]["data"][i];
	  Action *action_ptr = new Action((l_jv["host_url"]).asString(),
					  (l_jv["owner_vsID"]).asString(), (l_jv["class_id"]).asString(),
					  (l_jv["object_id"]).asString(), (l_jv["name"]).asString());
 
	  (*(my_post_ptr->actions)).push_back(action_ptr);

	}

  }
  // "comments"
  Comment * comment_ptr;
  my_post_ptr->comments = new vector<Comment *>();

  vector<Reaction *> * vec_c_reactions_ptr;
  vector<Tag      *> * vec_tag_ptr = new vector<Tag*> ();
  if ((myv["comments"]["count"]).asInt() > 0)
// if  (((myv["comments"]).isNull() != true) &&                  //these checks might not be working properly
//     ((myv["comments"]["count"]).asInt() > 0))
  {
    for (i = 0; i < (myv["comments"]["count"]).asInt(); i++)
	  {
	    Json::Value l_jv = myv["comments"]["data"][i];

      JvTime *comment_cr_time_ptr = 
      new JvTime(((l_jv["created_time"]).asString()).c_str());

      Person *comment_p_ptr = new Person((l_jv["from"]["avatar_name"]).asString(),
		  (l_jv["from"]["vsID"]).asString()); 

      Message  *message_ptr = new Message(l_jv["message"].asString());

       //breaking up id parts
      std::string profile_id;
      std::string post_id;
      std::string comment_id;
      
      profile_id = ((l_jv["id"]).asString()).substr(0,39);

      post_id = ((l_jv["id"]).asString()).substr(40,15);

      comment_id = ((l_jv["id"]).asString()).substr(56,15);

	    // adding the reactions part
      vec_c_reactions_ptr = new vector<Reaction*> ();
	    for (j = 0; j < (l_jv["reactions"]["count"]).asInt(); j++)
	    {
            // Json::Value l_jv_react = l_jv["reactions"]["data"][j];
            // if (((l_jv["avatar_name"]).isNull() != true) &&          //unsure of this if statement. 
            // ((l_jv["vsID"]).isNull() != true) &&                     //just following the motions
            // ((l_jv["type"]).isNull() != true) &&
            // ((l_jv["created_time"]).isNull() != true))
            //the lines above were supposed to check for the fields, but unsure what to do with them        
	    Json::Value l_jv_react = l_jv["reactions"]["data"][j];
	    Person *react_p_ptr = new Person((l_jv_react["avatar_name"]).asString(),
		  (l_jv_react["vsID"]).asString()); 
	    JvTime *react_cr_time_ptr = 
            new JvTime(((l_jv_react["created_time"]).asString()).c_str());
	    Reaction * react_ptr = new Reaction((l_jv_react["type"]).asString(),
	            react_p_ptr, react_cr_time_ptr);
	    vec_c_reactions_ptr->push_back(react_ptr);
	    }
      

	    
      // adding the tags part
      if (((l_jv["tags"]).isNull() != true) && ((l_jv["tags"]["count"]).asInt() > 0))
  { 
    if (comment_ptr->tags == NULL)
      vec_tag_ptr = new vector<Tag*> ();
      for (j = 0; j < (l_jv["tags"]["count"]).asInt(); j++)
      {
          Json::Value l_jv_tag = l_jv["tags"]["data"][j];
          Tag * tag_ptr = new Tag();
          tag_ptr->index = (l_jv_tag["index"]).asInt();
          tag_ptr->avatar_name = (l_jv_tag["avatar_name"]).asString();
          tag_ptr->comment = (l_jv_tag["comment"]).asString();
          tag_ptr->vsID = (l_jv_tag["vsID"]).asString();
          vec_tag_ptr->push_back(tag_ptr);
      }
  }

	    comment_ptr = new Comment(profile_id, post_id, comment_id, comment_p_ptr, message_ptr, comment_cr_time_ptr);
      comment_ptr->reactions = vec_c_reactions_ptr;
      comment_ptr->tags = vec_tag_ptr;
	    // done with one comment, now push back into the comments vector
	
      	  (*(my_post_ptr->comments)).push_back(comment_ptr);

	}

    }

  // "keys"
  //hopefully i converted keys correctly. if not, replace the commented line
  //my_post_ptr->keys = new vector<Okey *>();
  if (((myv["keys"]).isNull() != true) && ((myv["keys"]["count"]).asInt() > 0))
  { 
    if (my_post_ptr->keys == NULL)
      my_post_ptr->keys = new vector<OKey *>();

    for (i = 0; i < (myv["keys"]["count"]).asInt(); i++)
      {
        Json::Value l_jv = myv["keys"]["data"][i];
        if (l_jv.isNull() != true)
          {
            OKey * okey_ptr = getOKey(l_jv.asString());
            (*(okey_ptr->posts)).push_back(my_post_ptr);
            (*(my_post_ptr->keys)).push_back(okey_ptr);
          }
      }
  }
  

  // "links"
  my_post_ptr->links = new vector<Link *>();
  if ((myv["links"]["count"]).asInt() > 0)
  //  if (((myv["links"]).isNull() != true) &&  //this line is to check if json has reactions field
	//     ((myv["links"]["count"]).asInt() > 0))
  {
      for (i = 0; i < (myv["links"]["count"]).asInt(); i++)
	{
	  Json::Value l_jv = myv["links"]["data"][i];
	  Link * link_ptr = new Link((l_jv["url"]).asString(),
				     (l_jv["class_id"]).asString(), 
				     (l_jv["object_id"]).asString());

	  (*(my_post_ptr->links)).push_back(link_ptr);
	}

    }


  // "message"
  if ((myv["message"]).isNull() != true)
  {
    Message * msg_ptr = new Message(myv["message"].asString());
    my_post_ptr->msg = msg_ptr;
  }

  // "reactions" 
    if (((myv["reactions"]).isNull() != true) &&   //this line is to check if json has reactions field
	((myv["reactions"]["count"]).asInt() > 0))
    {
      if (my_post_ptr->reactions == NULL)
	my_post_ptr->reactions = new vector<Reaction *>();

      for (i = 0; i < (myv["reactions"]["count"]).asInt(); i++)
	{
	  Json::Value l_jv = myv["reactions"]["data"][i];

	  if (((l_jv["avatar_name"]).isNull() != true) && //if statement to protect from seg faults
	      ((l_jv["vsID"]).isNull() != true) &&
	      ((l_jv["created_time"]).isNull() != true) &&
	      ((l_jv["type"]).isNull() != true))
	    {

	      Person *react_p_ptr = new Person((l_jv["avatar_name"]).asString(),
					       (l_jv["vsID"]).asString()); 
	      JvTime *react_cr_time_ptr =
		new JvTime(((l_jv["created_time"]).asString()).c_str());
	      Reaction * react_ptr = new Reaction(l_jv["type"].asString(),
						  react_p_ptr, react_cr_time_ptr);

	      (*(my_post_ptr->reactions)).push_back(react_ptr);

	    }
	}
    }
      
  // "history"

  if (((myv["history"]).isNull() != true) && ((myv["history"]["data"]).isNull() != true) && ((myv["history"]["data"]).isArray() == true))
    {
      if (my_post_ptr->history == NULL)
        my_post_ptr->history = new vector<Transaction *>();

      for (i = 0; i < (myv["history"]["data"]).size(); i++)
      {
        Json::Value l_jv = myv["history"]["data"][i];
        Transaction * transaction_ptr = Json_Transaction_Common(l_jv);
        if (transaction_ptr != NULL)
        {
          //check if the Transaction already exists --
          vector<Transaction *>::iterator my_it_hs;
          int flag_hs = 0;
          for (my_it_hs = (*(my_post_ptr->history)).begin(); my_it_hs < (*(my_post_ptr->history)).end(); my_it_hs++)
          {
            if (((*((*my_it_hs)->agent)) == (*(transaction_ptr->agent))) && ((*((*my_it_hs)->when)) == (*(transaction_ptr->when))))
            {
              flag_hs = 1;
            }
          }
          if (flag_hs == 0)
          {
            (*(my_post_ptr->history)).push_back(transaction_ptr);
          }
          else
          {
            if (transaction_ptr->agent != NULL)
              delete transaction_ptr->agent;

            if (transaction_ptr->where != NULL)
              delete transaction_ptr->where;

            if (transaction_ptr->srcIP != NULL)
              delete transaction_ptr->srcIP;
              
            if (transaction_ptr->when != NULL)
              delete transaction_ptr->when;

            delete transaction_ptr;                          
          }
        }
      }
    }

  // "id"
  // already done

  // "from"
  // this could be a from replacement
  Person *author_ptr;
  if ((myv["from"]).isNull() != true)
    {
      author_ptr = new Person((myv["from"]["avatar_name"]).asString(),
			      (myv["from"]["vsID"]).asString()); 
      my_post_ptr->author = author_ptr;
    }

  // "to"
    if (((myv["to"]).isNull() != true) &&
	((myv["to"]["count"]).asInt() > 0))
    {
      if (my_post_ptr->receivers == NULL)
	my_post_ptr->receivers = new vector<Person *>();

      for (i = 0; i < (myv["to"]["count"]).asInt(); i++)
	{
	  Json::Value l_jv = myv["to"]["data"][i];

	  if (((l_jv["avatar_name"]).isNull() != true) &&
	      ((l_jv["vsID"]).isNull() != true))
	    {
	      Person * to_ptr = new Person((l_jv["avatar_name"]).asString(),
					   (l_jv["vsID"]).asString());

	      (*(my_post_ptr->receivers)).push_back(to_ptr);
	    }
	}
    }
      

  // "created_time
  if ((myv["created_time"]).isNull() != true)
    {
      my_post_ptr->created = new JvTime(((myv["created_time"]).asString()).c_str());
    }

  // "updated_time"
  if ((myv["updated_time"]).isNull() != true)
    {
      my_post_ptr->updated = new JvTime(((myv["updated_time"]).asString()).c_str());
    }

  // "is_published"
  my_post_ptr->is_published = (myv["is_published"]).asBool();

  return 0;
}
Post *
Json_2_Post
(Json::Value myv)
{
  if (myv.isArray() == true)
    {
      // wrong one
      // {...}
      // [...] NULL
      return NULL;
    }

  Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);

  // "id"
  if ((myv["id"]).isNull() != true)
    {
      char idstr[1024];
      bzero(idstr, 1024);
      sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());
      if (idstr[0] != '\0')
	{
	  char c_prof_buf[256];
	  char c_post_buf[256];
	  bzero(c_prof_buf, 256);
	  bzero(c_post_buf, 256);

	  sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

	  std::string profile_id = { c_prof_buf };
	  std::string post_id = { c_post_buf };

	  // put id attributes to the Post object
	  my_post_ptr->profile_id = profile_id;
	  my_post_ptr->post_id = post_id;
	}
      else
	{
	  std::cerr << "No Post ID presented" << std::endl;
	  exit(-1);
	}
    }
  else
    {
      std::cerr << "No Post ID presented" << std::endl;
      exit(-2);
    }

  int rc = Json_Post_Common(myv, my_post_ptr);

  if (rc != 0)
    {
      std::cerr << "Error Json_Post_Common " << rc << std::endl;
      exit(-3);
    }

  return my_post_ptr;

}

int
Json_Post_Merging
(Json::Value myv, Post * my_post_ptr)
{
  char idstr[1024];
  bzero(idstr, 1024);

  if ((myv).isArray() == true) return -3;
  if ((myv["id"]).isNull() == true) return -1;

  // compare ID first
  // "id" (Let us check first if they share the same ID)
  sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());

  if (idstr[0] != '\0')
    {
      char c_prof_buf[256];
      char c_post_buf[256];
      bzero(c_prof_buf, 256);
      bzero(c_post_buf, 256);

      sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);

      std::string profile_id = { c_prof_buf };
      std::string post_id = { c_post_buf };

      if ((my_post_ptr->profile_id != profile_id) ||
	  (my_post_ptr->post_id    != post_id))
	{	  
	  std::cerr << "Post IDs mismatched!!" << std::endl;
	  return -1;
	}
    }
  else
    {
      std::cerr << "No Post ID presented for the second JSON" << std::endl;
      return -2;
    }

  // now, they matched, let us proceed with the merge action
  return Json_Post_Common(myv, my_post_ptr);

}
      
