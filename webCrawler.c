#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
# define MAXURL_PER_PAGE 5
#define MAX_DEPTH '3'
#define MAX_URL 100
struct Node
{
    int depth;
    char* url;
    int isVisited;
    struct Node *next;
    struct Node *prev;
};

typedef struct Node node;

int isUniqueInList(int index,node**HashArray,node *nextaddress,char*str)
{
    int status;
    if(HashArray[index]==0)
        return 1;
    else
    {
        node *ptr=HashArray[index];
        while(ptr!=nextaddress)
        {
            status=strcmp(ptr->url,str);
            if(status==0)
                return 0;
            ptr=ptr->next;
        }
    }
    return 1;
}

int onlyHtmlFile(char *result)
{
    int l=strlen(result);
    int i=l-4;
    if(result[i]=='.' && (( result[i+1]=='p' && result[i+2]=='d' && result[i+3]=='f')||
    (result[i+1]=='d' &&result[i+2]=='o' &&result[i+3]=='c')||(result[i+1]=='z' &&result[i+2]=='i'&&result[i+3]=='p')||
    (result[i+1]=='r' &&result[i+2]=='a'&&result[i+3]=='r')||(result[i+1]=='p' &&result[i+2]=='n'&&result[i+3]=='g')||
    (result[i+1]=='g' &&result[i+2]=='i'&&result[i+3]=='f')||(result[i+1]=='j' &&result[i+2]=='p'&&result[i+3]=='g')))
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int isBaseCorrect(char *word,char *base)
{
    int i;
    for(i=0;base[i]!='\0';i++)
    {
        if(word[i]!=base[i])
        {
            return 0;
        }
    }
    return 1;
}

int generateHashFunction(char *result,int arraysize)
{
    int len=strlen(result);
    return ((len)%arraysize);
}

node* GetNextAddress(int index,int arraysize,node** HashArray)
{
    int i;
    node* nextaddress=NULL;
    for(i=index+1;i<arraysize;i++)
    {
        if(HashArray[i]!=0)
        {
            nextaddress=HashArray[i];
            break;
        }
    }
    return nextaddress;
}

node* getPrevAddress(int index,int arraysize,node** HashArray)
{
    int i;
    node* prevaddress=NULL;
    for(i=index-1;i>=0;i--)
    {
        if(HashArray[i]!=0)
        {
            prevaddress=HashArray[i];
            break;
        }
    }
    return prevaddress;
}

void addtoHashArray(int index,node**HashArray,node**ADDRESS,node*NODE)
{
    if(HashArray[index]==0)
    {
        HashArray[index]=NODE;
    }
    *ADDRESS=HashArray[index];
}

void addToList(char * str,node** start,int depth,int index,node**HashArray,int arraysize)
{
	node* ptr;
    node* NODE=(node*)malloc(sizeof(node));
	NODE->url=str;
	NODE->isVisited=0;
	NODE->depth=depth;
    node*nextaddress;
    node*ADDRESS;
    node* prevaddress;

    addtoHashArray(index,HashArray,&ADDRESS,NODE);

    nextaddress=GetNextAddress(index,arraysize,HashArray);

    if(nextaddress==NULL)
    {
        NODE->next=NULL;
        prevaddress=getPrevAddress(index,arraysize,HashArray);
        ptr=prevaddress;
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=NODE;
        NODE->prev=ptr;
    }

    else if(nextaddress->prev==NULL)
    {
        NODE->next=*start;
        nextaddress->prev=NODE;
        NODE->prev=NULL;
        *start=NODE;
    }

    else
    {
        node*ptr1;
        if(ADDRESS!=NODE)
        {
            ptr1=ADDRESS;
        }
        else
        {
            ptr1=nextaddress->prev;
        }
        while(ptr1!=nextaddress)
        {
            ptr1=ptr1->next;
        }
        ptr1->prev->next=NODE;
        NODE->prev=ptr1->prev;
        NODE->next=ptr1;
        ptr1->prev=NODE;
    }
}

void  getFunction(char*str,long n,int depth,char* base,int *count,node**start,char*dir,int *fileno,node**HashArray,int arraysize)
{
    int index;
    node* nextaddress;
    if(depth>0)
    {
        if(*start==NULL)
        {
            node *NODE=(node*)malloc(sizeof(node));
            NODE->url=base;
            NODE->depth=depth;
            NODE->next=NULL;
            NODE->prev=NULL;
            NODE->isVisited=1;
            node*ADDRESS;
            index=generateHashFunction(base,arraysize);
            addtoHashArray(index,HashArray,&ADDRESS,NODE);
            *start=NODE;
        }

        char check[]="<a ";
        char check1[]="href";
        int x,g,f,status=1;
        char * null="\0\0";
        long i;
        int l,j,k=0;
        char* p[100];
        char** new_array;
        new_array =(char**) malloc(30 * sizeof(char*));
        ///	result[0]='\0';
        for(i=0;i<n;i++)
        {
            char *result=(char*)malloc(sizeof(char)*500);
            status=1;
            x=0;
            while(str[i]!=check[0]&&str[i+1]!=check[1])
            {
                i++;
            }
            while(str[i]==check[x]&&check[x]!='\0')
            {
                i++;
                x++;
            }
            if(check[x]=='\0')
            {
                f=0,g=0;
                while(str[i]!=check1[0]||str[i+1]!=check1[1]||str[i+2]!=check1[2]||str[i+3]!=check1[3])
                {
                    i++;
                }
                while(str[i]==check1[f]&&check1[f]!='\0')
                {
                    i++;
                    f++;
                }
                if(check1[f]=='\0')
                {
                    while(str[i]==32)
                    i++;

                    if(str[i]=='=')
                    {
                        while(str[i]!=34)
                            i++;

                        i++;
                        while(str[i]!=34)
                        {

                            if(str[i]!='<'&&str[i]!='>')
                            {
                                result[g]=str[i];
                                g++;
                                i++;
                            }
                            else
                            {
                            result[0]='\0';
                            status=0;
                            break;
                            }

                        }
                        result[g]='\0';


                        if(status==1)
                        {

                            while(str[i]!='>'&&i<n)
                            i++;

                            if(str[i]=='>')
                            {

                                while(((str[i]!='<'||str[i+1]!='/'||str[i+2]!='a'||str[i+3]!='>'))&&(str[i]!='<'||str[i+1]!='a')&&i<n)
                                    i++;

                                if(str[i]=='<'&&str[i+1]=='/'&&str[i+2]=='a'&&str[i+3]=='>')
                                {
                                    result[g]='\0';
                                    if(isBaseCorrect(result,base))
                                    {
                                        index=generateHashFunction(result,arraysize);
                                        nextaddress=GetNextAddress(index,arraysize,HashArray);
                                        if((*count)>MAXURL_PER_PAGE)
                                        {
                                            if(isUniqueInList(index,HashArray,nextaddress,result))
                                            {
                                                if(onlyHtmlFile(result))
                                                {
                                                    addToList(result,start,depth,index,HashArray,arraysize);
                                                    (*count)++;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            i=n;
                                        }
                                    }
                                }
                                else if((str[i]=='<'&&str[i+1]=='a')||i==n)
                                {
                                    sprintf(result,"%s",null);
                                    free(result);
                                    i--;
                                }
                            }
                            else
                            {
                                sprintf(result,"%s",null);
                                free(result);
                            }
                        }
                    }
                }
            }
        }
        crawlThePage(depth,start,base,dir,fileno,HashArray,arraysize);
        free(new_array);
    }
    else
        return;
}

void getPage(char *url)
{
	char urlbuffer[MAX_URL+300]={0};
	strcat(urlbuffer,"wget -O ");
	strcat(urlbuffer,"temp.txt ");
	strcat(urlbuffer,url);
	system(urlbuffer);
}

void writeToNewFile(char *path1,int depth,char* seedUrl,char* url)
{
	char *newstring=(char*)malloc(sizeof(char)*300);
	char *string;
	FILE *fp;
	FILE *writeptr;
	long fsize;
	int i=0;
	fp=fopen("temp.txt","r");
	sprintf(newstring,"URL=%s DEPTH=%d SEED_URL=%s\n\n ",url,depth,seedUrl);
	writeptr=fopen(path1,"w");
	while(newstring[i]!='\0')
	{
		fputc(newstring[i++],writeptr);
	}
	char ch;
	while((ch=fgetc(fp))!=EOF)
		fputc(ch,writeptr);

	free(newstring);
	fclose(fp);
	fclose(writeptr);

}

void stringcopy(char* path1,char*path)
{
	int i;
	for(i=0;path[i]!='\0';i++)
	{
		path1[i]=path[i];
	}
	path1[i]='\0';
}

void createPathNewFile(char *path,int *fileno,int depth,char *seedUrl,char* url)
{
	char itostring[10];
	char *string;
	long fsize;
	FILE *fp;
	fp=fopen("temp.txt","r");
	sprintf(itostring,"%d",*fileno);
	char file[]="/temp";
	char f[]=".html";
	char path1[40];

	stringcopy(path1,path);
	strcat(path1,file);
	strcat(path1,itostring);
	strcat(path1,f);

	writeToNewFile(path1,depth,seedUrl,url);

}

void readTheFile(int depth,char* seedUrl,node **start,char* dir,int* fileno,char *url,node** hashArray,int arrSize)
{
	FILE *fp;
	int l;
	char *result;
	char** arr;
	int count=0;
	fp=fopen("temp.txt","r");

	if(fp)
	{
		createPathNewFile(dir,fileno,depth,seedUrl,url);

		fseek(fp,0,SEEK_END);
		long fsize=ftell(fp);
		fseek(fp,0,SEEK_SET);
		char *string=malloc(fsize+1);
		fread(string,fsize,1,fp);

		getFunction(string,fsize,depth,seedUrl,&count,start,dir,fileno,hashArray,arrSize);
	}

}

void crawlThePage(int depth,node ** start,char*url,char*dir,int* fileno,node**hashArray,int arraysize)
{
    node *ptr=*start;
	while(ptr!=NULL)
	{
		if(ptr->isVisited==0)
		{
            (*fileno)++;
            ptr->isVisited=1;
            getPage(ptr->url);
            readTheFile(ptr->depth-1,url,start,dir,fileno,ptr->url,hashArray,arraysize);
		}
        ptr=ptr->next;
	}
}

int isValidUrl(char *url)
{
	char* command=(char*)malloc(sizeof(char)*500);
	char *first;
	first="wget --spider ";

	strcpy(command,first);
	strcat(command,url);
	if(!system(command))
	{

		free(command);
		return 1;
	}
	else
	{
		free(command);
		return 0;
	}
}

int isValidDir(char *dir)
{
	struct stat statbuf;
	if ( stat(dir, &statbuf) == -1 ){
		fprintf(stderr, "-----------------\n");
		fprintf(stderr, "Invalid directory\n");
		fprintf(stderr, "-----------------\n");
		return 0;
	}

	//Both check if there's a directory and if it's writable
	if ( !S_ISDIR(statbuf.st_mode) ){
		fprintf(stderr, "-----------------------------------------------------\n");
		fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
		fprintf(stderr, "-----------------------------------------------------\n");
		return 0;
	}

	if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR ){
		fprintf(stderr, "------------------------------------------\n");
		fprintf(stderr, "Invalid directory entry. It isn't writable\n");
		fprintf(stderr, "------------------------------------------\n");
		return 0;
	}
	return 1;
}

int isValidDepth(char *depth)
{
	int length=strlen(depth);
	if(length>2)
	{
		return 0;
	}
	else
	{
		if(depth[0]>='1' &&depth[0]<=MAX_DEPTH)
		{
			return depth[0]-'0';

		}
		else
		{
			return 0;
		}
	}
}

void  main(int argc,char* argv[])
{
	int depth,i;
	node *start=NULL;
	int fileno=1;
	int arrSize=100;
	char* url=(char*)malloc(sizeof(char)*(strlen(argv[2]+1)));
	char *dir=(char*)malloc(sizeof(char)*(strlen(argv[3]+1)));
	//char str[]="wget --spider ";

	node* new_array[100] ;
	for(i=0;i<arrSize;i++)
    {
        new_array[i]=0;
    }

	strcpy(url,argv[2]);
	url[strlen(url)]='\0';
	strcpy(dir,argv[3]);
	dir[strlen(dir)] = '\0';

	if(isValidUrl(url))
	{
		if(isValidDir(dir))
		{
			if(depth=isValidDepth(argv[1]))
			{
				getPage(url);
				readTheFile(depth,url,&start,argv[3],&fileno,url,new_array,arrSize);
				printf("%d",fileno);
			}
			else
			{
				printf("\ninvalid Depth Depth should be greater than 0 and less than %d\n",(MAX_DEPTH-'0') + 1);

			}
		}
		else
		{
			printf("\n Not a Valid Directory");
		}
    }
	else
	{
		printf("\nUrl is not valid");
	}
}