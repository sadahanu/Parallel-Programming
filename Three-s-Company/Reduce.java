package fof;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
//by Zhou Yu0 04/07/2014, modifed for enumerate on 04/12/2014
public class Reduce extends Reducer<Text, NullWritable, Text, NullWritable>{
    private NullWritable none = NullWritable.get();
    public void reduce (Text key, Iterable<NullWritable> values, Context context) throws IOException, InterruptedException {
        int sum = 0;
        //if the potential threes happen twice, then a triangle is identified
        for (NullWritable nw: values){
        	sum++;
        	if (sum >2) break;
        }
        if(sum>=2){
        	String combin = key.toString();
        	//Dummy method to list the mutual friends for each user
        	StringTokenizer combinToken = new StringTokenizer(combin);
        	int a = Integer.parseInt(combinToken.nextToken());
        	int b = Integer.parseInt(combinToken.nextToken());
        	int c = Integer.parseInt(combinToken.nextToken());
        	Text aa = new Text(a+" "+b+" "+c);
        	Text bb = new Text(b+" "+a+" "+c);
        	Text cc = new Text(c+" "+a+" "+b);    	
        	context.write(aa, none);
        	context.write(bb, none);
        	context.write(cc, none);
        }
    }
    }
