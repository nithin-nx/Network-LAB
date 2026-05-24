#include<stdio.h>

#define INF 9999

int main()
{
    int n,i,j,s,u,min;

    int c[20][20],d[20],v[20];

    printf("Nodes: ");
    scanf("%d",&n);

    printf("Cost Matrix:\n");

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            scanf("%d",&c[i][j]);

    printf("Source: ");
    scanf("%d",&s);

    for(i=0;i<n;i++)
    {
        d[i]=c[s][i];
        v[i]=0;
    }

    d[s]=0;
    v[s]=1;

    for(i=1;i<n;i++)
    {
        min=INF;

        for(j=0;j<n;j++)
            if(!v[j]&&d[j]<min)
            {
                min=d[j];
                u=j;
            }

        v[u]=1;

        for(j=0;j<n;j++)
            if(!v[j]&&d[u]+c[u][j]<d[j])
                d[j]=d[u]+c[u][j];
    }

    printf("\nShortest Path:\n");

    for(i=0;i<n;i++)
        printf("%d -> %d = %d\n",s,i,d[i]);

    return 0;
}
