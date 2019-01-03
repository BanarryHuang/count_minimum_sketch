#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR 100
void init(int **map, int r, int c, int *a, int *b, int p){

    int i,j;

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            *(*(map+i)+j)=0;

    for(i=0;i<r;i++)
        *(a+i)=uniform_distribution_secure(1,p-1);

    for(i=0;i<r;i++)
        *(b+i)=uniform_distribution_secure(1,p-1);
}

int uniform_distribution_secure(int rangeLow, int rangeHigh) {

    int range = rangeHigh - rangeLow + 1;
    int secureMax = RAND_MAX - RAND_MAX % range;
    int x;

    do x = rand();
    while (x >= secureMax);

    return rangeLow + x % range;
}

int myhash(char *str, int count, int r, int c, int p, int *a, int *b){

    int i,key=0;

    for(i=0;i<strlen(str);i++)
        key += str[i];

    return (a[count]*key+b[count]) % p % c;
}

void insert(int **map, int r, int c, int p, char *str, int *a, int *b){

    int count,small;
    int hash;

    hash=myhash(str, 0, r, c, p, a, b);
    while(hash<0) hash+=c;
    small=map[0][hash];

    for(count=1;count<r;count++){
        if(small==0) break;
        hash=myhash(str, count, r, c, p, a, b);
        while(hash<0) hash+=c;
        if(map[count][hash]<small)
            small=map[count][hash];
    }

    for(count=0;count<r;count++){
        hash=myhash(str, count, r, c, p, a, b);
        while(hash<0) hash+=c;
        if(map[count][hash]==small)
            map[count][hash]++;
    }
}

int query(int **map, int r, int c, int p, char *str, int *a, int *b){

    int count,small;
    int hash;

    hash=myhash(str, 0, r, c, p, a, b);
    while(hash<0) hash+=c;
    small=map[0][hash];

    for(count=1;count<r;count++){
        if(small==0) break;
        hash=myhash(str, count, r, c, p, a, b);
        while(hash<0) hash+=c;
        if(map[count][hash]<small)
            small=map[count][hash];
    }

    return small;
}

int main(){

    int i,j,row=0,col,prime;
    int **map;
    char *str,x;
    int *a,*b;

    str=malloc(MAX_STR*sizeof(*str));
    str[0]=0;

    freopen("input.txt","r",stdin);

    scanf("%d%d%d",&row,&col,&prime);

    map=malloc(row*sizeof(*map));
    for(i=0;i<row;i++)
        map[i]=malloc(col*sizeof(**map));
    a=malloc(row*sizeof(*a));
    b=malloc(row*sizeof(*b));

    if(row!=0)
        init(map, row, col, a, b, prime);

    while(scanf("%c",&x)!=EOF){
        if(isalpha(x)){
            if(isupper(x)) x+=32;
            strncat(str,&x,1);
        }
        else{
            insert(map,row,col,prime,str,a,b);
            str[0]=0;
        }
    }

    freopen("CON","r",stdin);

    while(scanf("%c",&x)){
        if(x=='0')
            break;
        else if(isalpha(x)){
            if(isupper(x)) x+=32;
            strncat(str,&x,1);
        }
        else if(x=='\n'){
            printf("%d\n",query(map,row,col,prime,str,a,b));
            str[0]=0;
        }
    }

    return 0;
}
