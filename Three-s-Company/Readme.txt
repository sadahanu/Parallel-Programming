Description of the Three's Company problem:
http://hssl.cs.jhu.edu/wiki/doku.php?id=randal:teach:cs420:project_3_2015
In brief:
The Problem: Three's Company

This assignment is a social network version of the triangle counting problem, which has become an archetypal problem in graph analysis. The blog http://www.vertica.com/2011/09/21/counting-triangles/ has a condemnation of Map/Reduce for the task. It also has a very inefficient implementation. Your program will output all triangles (which is different that a count of triangles) in a single map/reduce program.

Identify threesomes of users that are mutual friends in a social network, i.e. A friends with B, B friends with C , and C friends with A. The output should enumerate the mutual friends for each user and avoid duplicate entries, i.e. the trio of users A, B, and C will contribute output:

<A,B,C> 
<B,A,C>   
<C,A,B>
This needs to be implemented in the Map/Reduce paradigm, using no auxiliary data structures and no shared data. The only files used are the inputs to the map stage. The solution will demonstrate one potential parallelism tradeoff, it will expand the data sending more data than is intuitively necessary over the network and examining the expanded data in the reducers. The expansion of data prevents random I/O (e.g., the reading of multiple friend lists by the mappers and reducers) and allows for a high degree of parallelism.

The input to the Map/Reduce program is set of friend lists. Each file in the input contains the id of the user followed by a list of her/his friends. All identifiers are integer values separated by spaces and all friends in the list appear on a single line.


4.1 Explanations for executing code on Hadoop VM (image provided by Kunal)
1. Download input data from S3
Configure the S3cmd by command: s3cmd --configure; Then download the data: e.g. s3cmd get --recursive s3//:friends1000/
2. Configure Hadoop environment
Remove the data and name folders in /app/hadoop/tmp/dfs/
Start hadoop: /usr/local/hadoop$sbin/start-all.sh
Reformat the node: bin/hadoop namenode -format
Stop hadoop:sbin/stop-all.sh
Then restart hadoop
3. Upload data to HDFS 
e.g. bin/hadoop dfs -copyFromLocal /tmp/friends.simple /user/hduser/friends.simple
4. Run programs
Put the source files in hadoop folder. Then use command:
e.g. bin/hadoop jar /usr/local/hadoop/tools/lib/hadoop-straming-2.3.0.jar -mapper fof.mapper1.py -reducer fof.reducer.py -input ./friends.simple -output ./output/frineds.simple.out
If using jar program, then the command is:
bin/hadoop jar ./FOF.jar friends.simple output/friends.simple.out
5. Retrive the output
bin/hadoop dfs -getmerge /user/haduser/output/friends.simple.out /tmp/result-output/simple/
6. Turn off hadoop: sbin/stop-all.sh

4.2 Explanations for executing code on Amazonaws
1. Upload the programs to S3 bucket
I used S3 Browser. The programs are uploaded to s3://pra3/

2. Configure and run EMR clusters

In cluster configuration section, set:
Cluster name: mystreaming
Log folder S3://errorbucketpra3/

In Tags section, set:
key: mykey; value:myvalue

Delete any additional applications listed in the "Applications to be installed" section.

In Hardward Configuration section, set:
For using small instance:
Master: m1.small count:1
core: m1.small count: 2
For using large instance:
Master: m1.small count:1
Core: c1.xlarge count:4
Task: m1.small count:0

In Security and Access section, select a EC2 keypair

In steps section, set:
For streaming mode: 
Add step -> choose streaming program, click on Configure and Add. The settings are:
Name: my streaming program
Mapper: s3://pra3/fof.mapper.py
Reducer: s3://pra3/fof.reducer.py
Input S3 location: s3://friends.simple/ or s3://friends1000/
Output S3 location:s3://zyu92014/output/friends_simple/ or friends_1000/
Action on failure ->choose terminate cluster
Then click on Add.
Select Yes for Auto-terminate 
Then click on Create cluster to run the program.

For Jar mode:
Add step->choose custom JAR, then click on Configure and Add. The settings are:
Name: my Jar program
JAR s3 location: s3://pra3/FOF.jar
Arguments: s3://friends.simple/ s3://bucketname/output/friends_simple/
or Arguments: s3://frineds1000/ s3://bucketname/output/friends_1000/
Action on failure ->choose terminate cluster
Then click on Add.
Select Yes for Auto-terminate 
Then click on Create cluster to run the program.
All the other settings remain as default.

3. Check results from EMR clusters
After the job is finished. The log can be viewed by:
Select the cluster from the cluster list. Click on Steps.Then click on view logs. An alternative way is to view records in logURI (here is s3://errorbucketpra3/)
