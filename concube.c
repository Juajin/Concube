#include<stdio.h>
#include<string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>
#define SIZE 10000
//-I/usr/include/libxml2  -lxml2

static int count=0;
//gcc json.c -o json -ljson -std=c99
void toCSV(char* key,char* val,char * outputName);
void toXML(char*key,char*val,char * outputName);
static  xmlDocPtr doc = NULL;	
static xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;
static xmlDtdPtr dtd = NULL;
void CSV(char *input,char *output);
void XML(char *input,char *output);
static void take_elements(xmlNode * a_node,FILE *fp,xmlNode * child);
static void take_contents(xmlNode * a_node,FILE *fp,xmlNode * child);
char *returnstring(char* ,int,int );
//XML to json
static void readXMLfile(char *Filename,char *outputName);
static void get_JSON_XML(xmlNode * a_node);
static char name[1024];

//for both of them
static json_object * jobj = NULL;
static json_object *jarray = NULL;

//CSV to json
static int row=0;
static int column=1;
static void readCSVfileInitilizeMatrixSize(char *Filename);
static void readCSVfileToJSON(char *Filename,char *outputName);
// command almak için
struct command{
char *input;	//input file name
char *output;	//output file name
char *inputformat;	//input file format
char *outputformat;	//output file format
};
//dosya cvs mi xml mi json mı
int checkcode(char *);
//belli parça string i geri döndürüyor
void json_parse(json_object * jobj,int flag,char *outputName) {
 static char strKey[SIZE];
 static char strVal[SIZE];
 
   enum json_type type;
    json_object_object_foreach(jobj, key, val) {
        type = json_object_get_type(val);
        switch (type) {
            case json_type_string:
            count++;
            if(flag==1){
            toXML(key,json_object_get_string(val),outputName);
	    }
	    strcat(strKey,key);
	    strcat(strKey,",");
	    strcat(strVal,json_object_get_string(val));
	    strcat(strVal,",");
            break;
        }
    }
    count=-1;
    if(flag==0){
      toCSV(strKey,strVal,outputName);
    }
    else{
    toXML(key,json_object_get_string(val),outputName);
    }
}
int main(int argc, char *argv[])
{ char cast[3]=".";
  int checki=0;
  int checko=0;
  int checkt=0;
  int checkitype=0;
  int checkotype=0;
  int restart=0;
  struct command *argumant,argum;
  argumant=&argum;
	for(int i =1;i<argc;i++){
		if(strcmp(argv[i],"-t")==0&&i+1<argc){			//it's check file type
		  if(strcmp(argv[i+1],"xml")==0||strcmp(argv[i+1],"csv")==0||strcmp(argv[i+1],"json")==0){
				checkt=1;
		
		  }
		argumant->outputformat=argv[i+1];	// file type
		argumant->output=argumant->input; 		// base file name
		checkt=1;
		checkotype=1;
		restart=1;
		}
		else if(strcmp(argv[i],"-o")==0&&i+1<argc){
		char *temp=NULL;
		temp=strtok(argv[i+1],cast);
		argumant->output =temp;
		temp=strtok(NULL,cast);
			if(strcmp(temp,"csv")==0||strcmp(temp,"json")==0||strcmp(temp,"xml")==0){
				checkotype=1;
				argumant->outputformat=temp;
			}
		checko=1;
		restart=1;	
		}
		if(strcmp(argv[i],"help")==0){
		printf("\n Welcome!!!\n There is two type of command to usually use.\n ->./concube -i sweetHome.xml(csv,json) -o alabama.(xml,csv,json) -true(xml is compress ore not)\n ->./concube -i sweetHome.csv(xml,json) -t xml(csv,json) -true\n\n");
		}
		if(strcmp(argv[i],"-i")==0&&i+1<argc){
		char *temp=NULL;
		temp=strtok(argv[i+1],cast);
		argumant->input=temp;
		temp=strtok(NULL,cast);
			if(strcmp(temp,"csv")==0||strcmp(temp,"json")==0||strcmp(temp,"xml")==0){
				checkitype=1;
				argumant->inputformat=temp;
			}
		checki=1;
		restart=1;
		}

	}
	if(!(checkitype==1&&checkotype==1)){
	printf("\n Wrong command!! \n Check your input or output file format.\n Excample->  -t xml(csv,json)\n\n");
	}
	else if(!(checki==1&&checko==1&&checkt==0||checki==1&&checkt==1))
	{
		if(restart==0){		
		printf("\n Wrong command!!\n If you need help with command please write ./concube help\n\n");	
		}
	}
	else{					//all command functions
			

		if(strcmp(argumant->inputformat,"xml")==0){
			if(!strcmp(argumant->outputformat,"csv")){//xml to csv
			char *inputName;
			char *outputName;
			inputName=strcat(argumant->input,".xml");
			outputName=strcat(argumant->output,".csv");
			XML(inputName,outputName);
			}
			else{					//xml to json
			char *inputName;
			char *outputName;			
			inputName=strcat(argumant->input,".xml");
			outputName=strcat(argumant->output,".json");
			readXMLfile(inputName,outputName);	
			}
		}
		if(strcmp(argumant->inputformat,"csv")==0){
			if(!strcmp(argumant->outputformat,"xml")){//csv to xml
			char *inputName;
			char *outputName;
			inputName=strcat(argumant->input,".csv");
			outputName=strcat(argumant->output,".xml");			
			}
			else{					 //csv to json
			char *inputName;
			char *outputName;
			inputName=strcat(argumant->input,".csv");
			outputName=strcat(argumant->output,".json");	
			readCSVfileInitilizeMatrixSize(inputName);
     			readCSVfileToJSON(inputName,outputName);		
			}
		}		
		if(strcmp(argumant->inputformat,"json")==0){
			char *inputName;
			inputName=strcat(argumant->input,".json");
			char temp[SIZE];
    			char buf[SIZE];
    			FILE *fp=fopen(inputName,"r");
    			
    			if(fp==NULL){
    			printf("Don't drop that baklava...");
    			}
    			else{
   	 			while(!feof(fp)){
    	  			fgets(temp,SIZE,fp);
    	  			strcat(buf,temp);
    	  
    				}
    			}
    			json_object * jobj = json_tokener_parse(buf);			
			if(!strcmp(argumant->outputformat,"xml")){//json to xml
			char *outputName;
			outputName=strcat(argumant->output,".xml");
			json_parse(jobj,1,outputName);
			}
			else{					 //json to csv
			char *outputName;			
			outputName=strcat(argumant->output,".csv");
			json_parse(jobj,0,outputName);			
			}
		}	

 }
}
//Json to xml and csv
 void toXML(char*key,char*val,char * outputName){
 
 if(count==1){
 char buff[256];
 int i, j;
 doc = xmlNewDoc(BAD_CAST "1.0");
 root_node = xmlNewNode(NULL, BAD_CAST "root");
 xmlDocSetRootElement(doc, root_node);
 }
 else if(count>1){
 
 xmlNewChild(root_node, NULL,BAD_CAST key,BAD_CAST val);
 }
 else if(count==-1) {
 xmlSaveFormatFileEnc(outputName, doc, "UTF-8", 1);
 xmlFreeDoc(doc);
 xmlCleanupParser();
 xmlMemoryDump();
 }
 else{
 }

 }
 void toCSV(char* key,char* val,char * outputName){
 FILE *fp=fopen(outputName,"w");
 fputs(key,fp);
 fputs("\n",fp);
 fputs(val,fp);
 fclose(fp);
 }
//Json to xml and csv
void CSV(char *input,char *output){
	FILE *fp = fopen(input,"r");
	char buf[SIZE];
	while (!feof(fp)){
	fgets(buf,SIZE,fp);
	printf("%s",buf);
	}
fclose(fp);
}
void XML(char *input,char *output){
	FILE *fp = fopen(output, "w");
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	const char *Filename = input;
	doc = xmlReadFile(Filename, NULL, 0);

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
	else
	{
		
		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);
		char buff[100];
		xmlNode *child2=root_element->children;
		child2=child2->next;

		take_elements(root_element->children,fp,child2);
				fputs("\n",fp);
                take_contents(root_element->children,fp,child2);
		/*free the document*/
		xmlFreeDoc(doc);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();
	
}

static void take_elements(xmlNode * a_node,FILE *fp,xmlNode * child2){
	xmlNode *cur_node = NULL;
	char *buff[100];
			cur_node = a_node;
			while(cur_node&&cur_node->next!=NULL){
			if (cur_node->type == XML_ELEMENT_NODE&&cur_node->children->content) {	
			if(strcmp(child2->name,cur_node->name)!=0){
						
			fputs(cur_node->name,fp);			
			fputs(",",fp);			
			
			}
			}
			cur_node=cur_node->next;
			take_elements(cur_node->children,fp,child2);
		}
		
}
static void take_contents(xmlNode * a_node,FILE *fp,xmlNode * child){
	xmlNode *cur_node = NULL;
	char *buff[100];
	
		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE&&strcmp(cur_node->children->content," ")>0) {	
			fputs(cur_node->children->content,fp);
			fputs(",",fp);			
			}
			take_contents(cur_node->children,fp,child);
		}
	
}
// XML and CSV to json
/* Recursive function that gets the XML structure and converts to JSON structure*/
static void get_JSON_XML(xmlNode * a_node)

{   xmlNode *cur_node = NULL;
    json_object *o = json_object_new_object();//for array
    int did=0;//explanation #1
    //#1->because of its recursive,it will add objects as many as how much element employee or note has elements so;   if they DID("did" comes from),they will stop.
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {//controls if type is an element
                 if(strcmp(cur_node->children->content,"\n")==0)
                    strcpy(name,cur_node->name);//gets the name employeee or note
                if(cur_node->children->content!=NULL && strcmp(cur_node->children->content,"\n")!=0)
                {  
               json_object *jstring = json_object_new_string(cur_node->children->content);//takes content
                   json_object_object_add(o, cur_node->name, jstring);//adds with it's name
                   if(did==0)//adds to jarray and stops
                   json_object_array_add(jarray, o);
                   did++;
                }
               }
    
	get_JSON_XML(cur_node->children);
	}
}
static void readCSVfileInitilizeMatrixSize(char *Filename){
	
	FILE *fcsv=fopen(Filename,"r");


   char line[2048] ;
   const char s[2] = ",";
   char *token;
   fgets(line, sizeof(line), fcsv);
       
   /* get the first token */
   token = strtok(line, s);
   
   /* walk through other tokens */
   while( token != NULL ) 
   {
      token = strtok(NULL, s);
      row++;
   }

   while (fgets(line, sizeof(line), fcsv)) {
        
        column++;
    }
   
   fclose(fcsv);
}
static void readCSVfileToJSON(char *Filename,char * outputName){
	
	FILE *fcsv=fopen(Filename,"r");


   char line[2048] ;
   const char s[2] = ",\n";
   char *token;
   int i=0;
   int j=0;
   char matrix[row][column][90];
   
       
  while (fgets(line, sizeof(line), fcsv)) {
        token = strtok(line, s);
   while( token != NULL ) 
   {  strcpy(matrix[i][j],token);
      token = strtok(NULL, s);
      i++;
   }
        j++;
        i=0;
    }
    fclose(fcsv);
    
      jarray = json_object_new_array();
	  for(i=1;i<column;i++)
	{ 
           json_object *o = json_object_new_object();
           for(j=0;j<row;j++)
           {  if(strcmp(matrix[j][i],"-")!=0)
              {
              json_object *jstring = json_object_new_string(matrix[j][i]);//takes content
              json_object_object_add(o, matrix[j][0], jstring);//adds with it's name 
              }
             
           }
          json_object_array_add(jarray, o);
	}
     jobj = json_object_new_object();
    json_object_object_add(jobj, "employee", jarray);//in csv,we dont know the table name
     FILE *fcsvjson=fopen(outputName,"w");
     fputs(json_object_to_json_string(jobj),fcsvjson);
     fclose(fcsvjson);
}
static void readXMLfile(char *Filename,char *outputName){
	
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	doc = xmlReadFile(Filename, NULL, 0);


	if (doc == NULL)
		printf("error: could not parse file %s\n", Filename);
	
	else
	{

		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);
                /*Creating a json object*/
                jobj = json_object_new_object();
                /*Creating a json array*/
	        jarray = json_object_new_array();
                //converts the XML 
                get_JSON_XML(root_element);
                //adds jarray the jobj
                json_object_object_add(jobj, name, jarray);
                //prints to a file
		FILE *fxmljson=fopen(outputName,"w");
                fputs(json_object_to_json_string(jobj),fxmljson);
                fclose(fxmljson);

		/*free the document*/
		xmlFreeDoc(doc);

	}

	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();

}
// XML and CSV to json
