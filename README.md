# EOSIO-game-contract

This contract can be used in any game were one needs to store highscore data.
It has two tables scores that only stores 10 of the highest scores achieved, and
total_scores, that stores all of the scores ever achieved.



## Requirements
- Python 3+
- requests

## Installation
1. Clone Repo
2. Set server host and port fields in config.json
3. Find the right POST request and edit poodle.py Stresser class
4. Install Libraries
````commandline
pip install -r requirments.txt
````

## Usage

```commandline
python3 poodle.py --server
           or
python3 poodle.py --bot
``` 
Feel free to contact me: lazarjovicic808@gmail.com
