CC = gcc
CCFLAGS = -Wall -Wextra -Wvla


tweets: tweets_generator.c markov_chain.c linked_list.c
	$(CC) $(CCFLAGS) $^ -o tweets_generator

snake: snakes_and_ladders.c markov_chain.c linked_list.c
	$(CC) $(CCFLAGS) $^ -o snakes_and_ladders
