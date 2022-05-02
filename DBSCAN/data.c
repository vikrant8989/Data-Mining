#include<stdio.h> 
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define pi 3.1416

struct objects {
	float x; 
	float y; 
	int cluster_id;
};

int main() { 
	int a,r,Ra=10,Rb=20,Rc=30,Rd=40,Re=50,Rf=60,Rg=70;
	float p = 65.00;
	float q = 65.00;
	FILE *fp; 
	struct objects o; 
	fp = fopen("db_data.bin", "wb"); 
	for(int i=0; i<1000; i++) 
	{
	        r = rand()%Ra;
	        a = rand()%360;
	        double b = (double)((pi/180)*a);
		o.x = p + (float)(r*cos(b));
		o.y = p + (float)(r*sin(b));
		o.cluster_id = -1; 
		fwrite(&o, sizeof(struct objects), 1, fp);
	}
	for(int i=0; i<2000 ; i++){
	        r = (rand()%(Rc-Rb+1))+Rb;
	        a = rand()%360;
	        double b = (double)((pi/180)*a);
		o.x = p + (float)(r*cos(b));
		o.y = q + (float)(r*sin(b));
		o.cluster_id = -1; 
		fwrite(&o, sizeof(struct objects), 1, fp);	
	}
	
	for(int i=0; i<3000 ; i++){
	        r = (rand()%(Re-Rd+1))+Rd;
	        a = rand()%360;
	        double b = (double)((pi/180)*a);
		o.x = p + (float)(r*cos(b));
		o.y = q + (float)(r*sin(b));
		o.cluster_id = -1; 
		fwrite(&o, sizeof(struct objects), 1, fp);	
	}
	
	for(int i=0; i<4000 ; i++){
	        r = (rand()%(Rg-Rf+1))+Rf;
	        a = rand()%360;
	        double b = (double)((pi/180)*a);
		o.x = p + (float)(r*cos(b));
		o.y = q + (float)(r*sin(b));
		o.cluster_id = -1; 
		fwrite(&o, sizeof(struct objects), 1, fp);	
	}
	fclose(fp);
	printf("The file contents are: \n"); 
	fp = fopen("db_data.bin", "rb"); 
	for(int i=0; i<10000; i++) {
		fread(&o, sizeof(struct objects), 1, fp); 
		printf("%f\t%f\n", o.x, o.y);
	}
	fclose(fp); 
	return 0;
}