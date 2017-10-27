//globals for valuse
int array_1[100]; //for object 1 (r_1)
int array_2[100]; //for object 2 (r_2)
int sum_1=0;
int sum_2=0;

int scan_parse(int array[], int sum){
	num_scans=0;
	//scan for values, then...
	if(val<50){ //50 CM's
		array[num_scans]=val;
		sum=sum+val;
		num_scans=num_scans+1;
	} else{
		return(num_scans);
	}
}

int main(){
	num_scans1=scan_parse(array_1, sum1);
	num_scans2=scan_parse(array_2, sum2);
	return 0;
}
