package fof;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
//by Zhou Yu0 04/07/2014
public class FOF {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		 Configuration conf = new Configuration();
	        
	        Job job = new Job(conf, "FOF");
	        job.setJarByClass(FOF.class);
	        job.setOutputKeyClass(Text.class);
	        job.setOutputValueClass(NullWritable.class);
	        
	        job.setMapperClass(Map.class);
	        job.setReducerClass(Reduce.class);
	        
	        job.setInputFormatClass(TextInputFormat.class);
	        job.setOutputFormatClass(TextOutputFormat.class);
	        
	        FileInputFormat.addInputPath(job, new Path(args[0]));
	        FileOutputFormat.setOutputPath(job, new Path(args[1]));
	        
	        job.waitForCompletion(true);


	}

}
