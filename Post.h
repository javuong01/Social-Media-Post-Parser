
#ifndef _POST_H_
#define _POST_H_

// Post.h
#include "Core.h"
#include "OKey.h"
#include "Tag.h"
#include "Comment.h"
#include "Transaction.h"
#include "Reaction.h"
#include "Message.h"
#include "Person.h"
#include "Link.h"
#include "Action.h"
#include "JvTime.h"

class OKey;

class Post
{
 private:
 protected:
 public:
  std::string profile_id;
  std::string post_id;

  Person * author;
  std::vector<Person *> * receivers;

  Message * msg;
  std::vector<Link     *> * links;
  std::vector<Action   *> * actions;
  std::vector<Reaction *> * reactions;
  std::vector<Comment  *> * comments;
  std::vector<OKey     *> * keys;
  std::vector<Transaction *> * history;  //This is for keeping data about gps location and time and such

  JvTime * created;
  JvTime * updated;
  bool is_published;

  Post(Person* arg_author, Message* arg_msg);
  Json::Value * dumpJ();
};

// declarations from JSON_Post.cpp (function prototypes)
std::vector<Post *> * getPostbyKey(std::string);
OKey * getOKey(std::string);
std::string Json_2_PostID(Json::Value);
Post * Json_2_Post(Json::Value);
int Json_Post_Merging(Json::Value, Post *);

#endif /* _POST_H_ */
