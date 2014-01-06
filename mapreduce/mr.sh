#!/bin/sh
hadoop jar /usr/local/hadoop-1.2.1/contrib/streaming/hadoop-streaming-1.2.1.jar -input ./matrix -output ./test -mapper mapper.py -reducer reducer.py -file mapper.py -file reducer.py 
