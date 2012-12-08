# 
#  Makefile
#  bfucked
#  
#  Created by Petter Rodhelind on 2012-01-28.
#  Copyright 2012. All rights reserved.
# 

.PHONY: clean
	
bfucked: src/main.c
	gcc -Wall -o ./bfucked src/*.c

ncurses: src/bf-ncurses.c
	gcc -Wall -lncurses -o ./bfucked-nc src/bfucked.c src/bf-ncurses.c

clean:
	rm -f ./bfucked *.o
