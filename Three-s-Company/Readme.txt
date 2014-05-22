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
