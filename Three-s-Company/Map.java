package fof;

import java.io.IOException;
import java.util.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
//by Zhou Yu0 04/07/2014
public class Map extends Mapper <LongWritable, Text,Text, NullWritable>{
    // change intWritable to NullWritable
    private final static NullWritable none = NullWritable.get(); // value
    public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException{
        String line = value.toString(); //read in the line 
        StringTokenizer tokenizer = new StringTokenizer(line);//seperate the line by " "
        int listlen = tokenizer.countTokens();//total number of friends on the list
        if (listlen < 3) return;//if the nubmer of friends <3, it is not possible to contain a possible three
        int []friends = new int[listlen]; 
        Text outKey;
        for (int i = 0;i<listlen;i++){ //copy the friend list into a integer array
            friends[i] =  Integer.parseInt(tokenizer.nextToken());
        }
        int minfriend = friends[0];
        int midfriend;
        // resolve potential threes, and sort the id
       for (int j = 1; j< (listlen-1);j++){
            if (friends[j] < minfriend) {
                minfriend = friends[j];
                midfriend = friends[0];
            }
            else midfriend = friends[j];
            for(int k = j+1;k < listlen; k++){
                if (friends[k]> midfriend){
                    outKey = new Text(minfriend+" "+midfriend+" "+friends[k]);}
                else if(friends[k]<minfriend){
                    outKey = new Text(friends[k]+ " "+minfriend+" "+midfriend);}
                else outKey = new Text(minfriend+" "+friends[k]+" "+midfriend);
                context.write(outKey, none);}
       }
               
    }
}
