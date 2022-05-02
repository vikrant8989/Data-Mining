#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "queue.c"

struct objects *setofpoints;

float canvas = 150;

void initialise()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, canvas, -20, canvas, -1, 1);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void initialise_objects(struct objects *point, int n)
{
	for (int i = 0; i < n; i++)
	{
		point[i].cluster_id = -1;
	}
}

float euclidean_dist(struct objects o1, struct objects o2)
{
	float dist = sqrt(((o1.x - o2.x) * (o1.x - o2.x)) + ((o1.y - o2.y) * (o1.y - o2.y)));
	return dist;
}

void plot(int clid, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("Point %d\n", i + 1);
		printf("x : %f\n", setofpoints[i].x);
		printf("y : %f\n", setofpoints[i].y);
		printf("cluster id : %d\n\n", setofpoints[i].cluster_id);

		glBegin(GL_POINTS);
		switch (setofpoints[i].cluster_id)
		{
		case 0:
			glColor3f(0, 0, 0);
			break;		
		case 1:
			glColor3f(1, 0, 0);
			break;
		case 2:
			glColor3f(0, 1, 0);
			break;
		case 3:
			glColor3f(0, 0, 1);
			break;
		case 4:
			glColor3f(1, 1, 0);
			break;
		case 5:
			glColor3f(1, 0, 1);
			break;
		case 6:
			glColor3f(1, 1, 1);
			break;
		case 7:
			glColor3f(0, 1, 1);
			break;
		case 8:
			glColor3f(0.5, 0, 0);
			break;
		case 9:
			glColor3f(0, 0.5, 0);
			break;
		case 10:
			glColor3f(0, 0, 0.5);
			break;
		case 11:
			glColor3f(0.5, 0.5, 0);
			break;
		}
		glVertex2f(setofpoints[i].x, setofpoints[i].y);
		glEnd();
		glFlush();
	}
}

int is_core(struct objects obj, int eps, int minpts, int n)
{
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		float d = euclidean_dist(setofpoints[i], obj);
		if (d <= eps && d != 0)
			count++;
	}
	if (count >= minpts)
		return 1;
	return 0;
}

void Plot_DBSCAN()
{
	FILE *fp;
	int n, eps, minpts, i, clid;
	struct objects o;
	struct queue *f1, *r1;
	f1 = NULL;
	r1 = NULL;

	n = 0;
	fp = fopen("db_data.bin", "rb");
	while (fread(&o, sizeof(struct objects), 1, fp))
		n++;
	fclose(fp);
	printf("N is : %d\n", n);

	fp = fopen("db_data.bin", "rb");
	setofpoints = (struct objects *)malloc(sizeof(struct objects) * n);
	i = 0;
	while (fread(&o, sizeof(struct objects), 1, fp))
	{
		setofpoints[i].x = o.x;
		setofpoints[i].y = o.y;
		setofpoints[i].cluster_id = o.cluster_id;
		i++;
	}
	fclose(fp);

	printf("Enter Epsilon: ");
	scanf("%d", &eps);
	printf("\nEnter MinPts: ");
	scanf("%d", &minpts);

	clid = 0;
	for (int i = 0; i < n; i++)
	{
		//unvisited
		if (setofpoints[i].cluster_id == -1)
		{ 
			if (is_core(setofpoints[i], eps, minpts, n))
			{
				clid++;
				setofpoints[i].cluster_id = clid;
				for (int j = 0; j < n; j++)
				{
					float d = euclidean_dist(setofpoints[j], setofpoints[i]);
					if (d <= eps && d != 0 && setofpoints[j].cluster_id < 1)
					{
						setofpoints[j].cluster_id = clid;
						enqueue(&setofpoints[j], &f1, &r1);
					}
				}
				while (!is_empty(f1))
				{
					struct objects *obj = dequeue(&f1);
					if (is_core(*obj, eps, minpts, n))
					{
						for (int k = 0; k < n; k++)
						{
							float d = euclidean_dist(setofpoints[k], *obj);
							if (d <= eps && d != 0 && setofpoints[k].cluster_id < 1)
							{
								setofpoints[k].cluster_id = clid;
								enqueue(&setofpoints[k], &f1, &r1);
							}
						}
					}
					// else border point
				}
			} // else border or noise
			else if (setofpoints[i].cluster_id < 1)
				setofpoints[i].cluster_id = 0; // NOISE
		}
	}

	plot(clid, n);
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutCreateWindow("DBSCAN");
	initialise();
	glutInitWindowSize(canvas, canvas);
	glutInitWindowPosition(0, 0);
	glutDisplayFunc(Plot_DBSCAN);
	glutMainLoop();
	return 0;
}
