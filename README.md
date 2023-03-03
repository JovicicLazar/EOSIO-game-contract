# EOSIO-game-contract

This contract can be used in any game were one needs to store highscore data.
It has two tables scores that only stores 10 of the highest scores achieved, and
total_scores, that stores all of the scores ever achieved.

##It has 2 actions:
1.insertscores() - action that inserts scores into the tables
2.clearscores() - action that clears total_scores table 


## Requirements
- cleos
- eosio-cpp
- eosio node
