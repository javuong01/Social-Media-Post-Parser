#include <iostream>

// JSON RPC part
#include <stdlib.h>
#include "hw7client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "Post.h"

using namespace jsonrpc;
using namespace std;

int
checkBigID
(char *ss_ptr)
{
  int i, j;
  for (i = 0; i < strlen(ss_ptr); i++)
    {
      char buff[64];
      bzero(buff, 64);
      strncpy(buff, &(ss_ptr[i]), 4);
      if (strcmp(buff, "\"id\"") == 0)
        {
          char *sd_ptr = ((&(ss_ptr[i])) + 4);
          char buff2[64];
          bzero(buff2, 64);
          sscanf(sd_ptr, "%[^\"]", buff2);
          sd_ptr += strlen(buff2) + 1;
          std::cout << "Hit One [" << buff2 << "]\n";
          
          for (j  = 0; j < strlen(sd_ptr); j++)
            {
              if ((sd_ptr[j] == '\"') && (j > 0) && (j < 128))
                {
                  return 0;
                }
              if (j >=128)
                {
                  return -1;
                }
              if (((sd_ptr[j] < '0') || (sd_ptr[j] > '9')) && (sd_ptr[j] != '_'))
                {
                  return -1;
                }
            }
        }
    }
  return -1;
}

int main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  FILE *jf1 = fopen(argv[1], "r");
  if (jf1 == NULL) return -1;

  Json::CharReaderBuilder builder;
  Json::CharReader* reader;
  std::string errors;
  bool parsingSuccessful;

  // *** process JSON File ***
  long lSize1;
  size_t lresult1;

  // obtain file size:
  fseek(jf1, 0 , SEEK_END);
  lSize1 = ftell(jf1);
  rewind(jf1);

  // allocate memory to contain the whole file:
  char *jf1_ptr = (char *) malloc(sizeof(char)*lSize1);
  // copy the file into the buffer:
  lresult1 = fread(jf1_ptr, 1, lSize1, jf1);    
  if (lresult1 != lSize1)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  if (checkBigID(jf1_ptr) !=0)
    {
      printf("big\n");
      exit(-1);
    }
  std::cout << "Here 88\n";
  Json::Value myv_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(jf1_ptr, jf1_ptr + lSize1, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  fclose(jf1);

  //std::cout << "Here\n";
  std::cout << "Here 101\n";
  Post * my_post_ptr = Json_2_Post(myv_message); //just gonna use this line for now since it seems to make more sense
  // Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
  // int rc = Json_Post_Common(myv_message, my_post_ptr);   //ask for help here    so it was originally Json_2_Post, but merging or common seems to fit better?

  //std::cout << "Here 2\n"

  //HttpClient httpclient("http://localhost:8384");
  HttpClient httpclient("http://169.237.6.102:55407");
  hw7Client myClient(httpclient, JSONRPC_CLIENT_V2);
  std::cout << "Here 111\n";
  //std::cout << "Here 3\n";

  Json::Value result_json;

  //read the file
  //convert file ==> Json::Value
  //convert Json::Value ==> C++ Post Object
  //Call update (JSON RPC) [ C++ Post Object -> dumpJ (JSON Value) -> toStyledString ]

	//if (my_post_ptr != NULL) //checks to see if there is actually a json to be updated. 
  //{
    try {
    //std::cout << "Here 4\n";
    std::cout << "Here 125\n";
    std::cout << (my_post_ptr->dumpJ())->toStyledString() << std::endl;
    std::cout << "Here 127\n";
    result_json = myClient.update((my_post_ptr->dumpJ())->toStyledString());
    //std::cout << "Here 5\n"
    std::cout << result_json.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  //}
  return 0;
}