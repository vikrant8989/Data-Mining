#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
typedef struct data
{
    float x, y;
} data;
void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 500, 0, 500, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
}
void kmean()
{
    // assuming cluster == 3
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    int n;
    printf("Enter number of point\n");
    scanf("%d", &n);
    FILE *fp;
    fp = fopen("data.bin", "w");
    if (fp == NULL)
    {
        printf("\nError opened file\n");
    }
    for (int i = 0; i < n; i++)
    {
        data d;
        d.x = rand() % 500;
        d.y = rand() % 500;
        fwrite(&d, sizeof(data), 1, fp);
    }
    fclose(fp);
    fp = fopen("data.bin", "r");
    if (fp == NULL)
    {
        printf("\nError opened file\n");
    }
    data c1, c2, c3;
    fread(&c1, sizeof(data), 1, fp);
    fread(&c2, sizeof(data), 1, fp);
    fread(&c3, sizeof(data), 1, fp);
    fseek(fp, 0, SEEK_SET);
    int t = 0;
    data d;
    // three cluster
    data cluster1[n];
    data cluster2[n];
    data cluster3[n];
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int iterate = 1;
    while (fread(&d, sizeof(data), 1, fp))
    {
        float d1 = sqrt((d.x - c1.x) * (d.x - c1.x) + (d.y - c1.y) * (d.y - c1.y));
        float d2 = sqrt((d.x - c2.x) * (d.x - c2.x) + (d.y - c2.y) * (d.y - c2.y));
        float d3 = sqrt((d.x - c3.x) * (d.x - c3.x) + (d.y - c3.y) * (d.y - c3.y));
        if (d1 < d2)
        {
            if (d1 < d3)
            {
                cluster1[t1].x = d.x;
                cluster1[t1].y = d.y;
                t1++;
            }
            else
            {
                // cluster3
                cluster3[t3].x = d.x;
                cluster3[t3].y = d.y;
                t3++;
            }
        }
        else
        {
            if (d2 < d3)
            {
                cluster2[t2].x = d.x;
                cluster2[t2].y = d.y;
                t2++;
            }
            else
            {
                cluster3[t3].x = d.x;
                cluster3[t3].y = d.y;
                t3++;
            }
        }
        if (t == n - 1)
        {
            // glLineWidth(3)
            float avgx = 0;
            float avgy = 0;
            printf("\ncluster 1 points");
            glColor3f(1, 0, 0);
            for (int i = 0; i < t1; i++)
            {
                avgx += cluster1[i].x;
                avgy += cluster1[i].y;
                printf("(%.2f,%.2f)", cluster1[i].x, cluster1[i].y);
                glVertex2f(cluster1[i].x, cluster1[i].y);
            }
            // new centroid
            c1.x = avgx / t1;
            c1.y = avgy / t1;

            avgx = 0;
            avgy = 0;
            printf("\ncluster 2 points");
            glColor3f(0, 1, 0);
            for (int i = 0; i < t2; i++)
            {
                avgx += cluster2[i].x;
                avgy += cluster2[i].y;
                printf("(%.2f,%.2f)", cluster2[i].x, cluster2[i].y);
                glVertex2f(cluster2[i].x, cluster2[i].y);
            }
            c2.x = avgx / t2;
            c2.y = avgy / t2;
            avgx = 0;
            avgy = 0;
            printf("\ncluster 3 points");
            glColor3f(0, 0, 1);
            for (int i = 0; i < t3; i++)
            {
                avgx += cluster3[i].x;
                avgy += cluster3[i].y;
                printf("(%.2f,%.2f)", cluster3[i].x, cluster3[i].y);
                glVertex2f(cluster3[i].x, cluster3[i].y);
            }
            c3.x = avgx / t3;
            c3.y = avgy / t3;
            printf("\n iterating %d completed", iterate);

            printf("\ncentroid of cluster 1 after %d iteration :(%.2f,%.2f)", iterate, c1.x, c1.y);
            printf("\ncentroid of cluster 2 after %d iteration :(%.2f,%.2f)", iterate, c2.x, c2.y);
            printf("\ncentroid of cluster 3 after %d iteration :(%.2f,%.2f)", iterate, c3.x, c3.y);
            char ch;
            printf("\n Do you want to iterate more! y or n ");
            scanf(" %c", &ch);
            if (ch == 'n')
                break;
            t = -1;
            iterate++;
            t1 = 0;
            t2 = 0;
            t3 = 0;
            fseek(fp, 0, SEEK_SET);
        }
        t++;
    }
    fclose(fp);

    glEnd();
    glFlush();
    // to end glBegin()
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("K Mean");
    init();
    glutDisplayFunc(kmean);
    glutMainLoop();
}
