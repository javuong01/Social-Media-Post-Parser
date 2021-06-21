# Makefile for HW7, ecs36b, s2020
# only History part

CC = g++ -std=c++14
CFLAGS = -g -I/usr/include/jsoncpp

LDFLAGS = 	-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -lcurl -ljsonrpccpp-client

CORE_INCS =	Core.h Person.h JvTime.h Transaction.h Post.h Comment.h Reaction.h Action.h Link.h Message.h Tag.h
CORE_OBJS =	Core.o Person.o JvTime.o Transaction.o Post.o Comment.o Reaction.o Tag.o Action.o Link.o

# rules.
all: 	hw7history hw7search hw7update hw7validate

#
#

hw7client.h:		ecs36b_hw7.json
	jsonrpcstub ecs36b_hw7.json --cpp-server=hw7Server --cpp-client=hw7Client

hw7server.h:		ecs36b_hw7.json
	jsonrpcstub ecs36b_hw7.json --cpp-server=hw7Server --cpp-client=hw7Client

hw7history.o:		hw7history.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7history.cpp

Core.o:			Core.cpp Core.h
	$(CC) -c $(CFLAGS) Core.cpp

Transaction.o:		Transaction.cpp Transaction.h
	$(CC) -c $(CFLAGS) Transaction.cpp

Person.o:		Person.cpp Person.h Core.h
	$(CC) -c $(CFLAGS) Person.cpp

JvTime.o:		JvTime.cpp JvTime.h
	$(CC) -c $(CFLAGS) JvTime.cpp

hw7history:		$(CORE_OBJS) hw7history.o
	$(CC) -o hw7history $(CORE_OBJS) hw7history.o $(LDFLAGS)

Link.o:			Link.cpp Link.h
	$(CC) -c $(CFLAGS) Link.cpp

Action.o:		Action.cpp Action.h
	$(CC) -c $(CFLAGS) Action.cpp

JSON_Post.o:		JSON_Post.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) JSON_Post.cpp

Post.o:			Post.cpp Post.h
	$(CC) -c $(CFLAGS) Post.cpp

Comment.o:		Comment.cpp Comment.h
	$(CC) -c $(CFLAGS) Comment.cpp

Reaction.o:		Reaction.cpp Reaction.h
	$(CC) -c $(CFLAGS) Reaction.cpp

Tag.o:			Tag.cpp Tag.h
	$(CC) -c $(CFLAGS) Tag.cpp

hw7search.o:		hw7client.h hw7search.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7search.cpp

hw7update.o:		hw7client.h hw7update.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7update.cpp

# hw7server.o:		hw7server.h hw7server.cpp $(CORE_INCS)
# 	$(CC) -c $(CFLAGS) hw7server.cpp

# hw7server:		$(CORE_OBJS) hw7server.o JSON_Post.o
# 	$(CC) -o hw7server $(CORE_OBJS) hw7server.o JSON_Post.o $(LDFLAGS)

hw7search:		$(CORE_OBJS) hw7search.o JSON_Post.o
	$(CC) -o hw7search $(CORE_OBJS) hw7search.o JSON_Post.o $(LDFLAGS)

hw7update:		$(CORE_OBJS) hw7update.o JSON_Post.o
	$(CC) -o hw7update $(CORE_OBJS) hw7update.o JSON_Post.o $(LDFLAGS)

hw7validate.o:		hw7client.h hw7validate.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGS) hw7validate.cpp

hw7validate:		$(CORE_OBJS) hw7validate.o JSON_Post.o
	$(CC) -o hw7validate $(CORE_OBJS) hw7validate.o JSON_Post.o $(LDFLAGS)



clean:
	rm -f	*.o *~ core hw7history hw7server hw7search hw7update hw7server.h hw7validate

