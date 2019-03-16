#include<stdio.h>
#include<string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>
#define SIZE 250


// command almak için

static char *input;	//input file name
char *output;	//output file name
char *inputformat;	//input file format
char *outputformat;	//output file format


//controls if it is valid request
int is_XML(char *source);
int is_CSV(char *source);
int is_JSON(char *source);


void XML(char *input,char *output);
static void take_elements(xmlNode * a_node,FILE *fp,xmlNode * child);

/*TO JSON PART*/
//XML
static void readXMLfile(char *Filename,char *to);
static void get_JSON_XML(xmlNode * a_node);
static char name[1024];

//for both of them
static json_object * jobj = NULL;
static json_object *jarray = NULL;

//CSV
static int row=0;
static int column=1;
static void readCSVfileInitilizeMatrixSize(char *Filename);
static void readCSVfileToJSON(char *Filename,char *to);
/*TO XML PART*/
static void readCSVfileToXMLWA(char *Filename,char *to);
static void readCSVfileToXMLNA(char *Filename,char *to);

//XML
/*TO CSV PART*/
static void getNumbers(xmlNode * a_node,int number_tempP);
static void takeElements(xmlNode * a_node,FILE *fp);
static void take_contents(xmlNode * a_node,FILE *fp,xmlNode * child,int i);	
int number_XC=0;
int number_place=0;
int numberP=0;

int main(int argc, char *argv[])
{  int comptru=0;
   int icsv=0;//input char array(name)
   int ixml=0;
   int ijson=0;
   int ocsv=0;//outputformat
   int oxml=0;
   int ojson=0;
   int in_t=0;
   
	for(int i =1;i<argc;i++){
        if( i+1 < argc && strcmp(argv[i],"-i")==0)
         {  
           input=argv[i+1];
           ixml=is_XML(argv[i+1]);
	   icsv=is_CSV(argv[i+1]);
	   ijson=is_JSON(argv[i+1]);
	 }
	else if( i+1 < argc && strcmp(argv[i],"-true")==0)
          comptru=1;
        else if( i+1 < argc && strcmp(argv[i],"-o")==0)
          {
	     output=argv[i+1];
	     oxml=is_XML(argv[i+1]);
	     ocsv=is_CSV(argv[i+1]);
	     ojson=is_JSON(argv[i+1]); 	
	  }
        else if( i+1 < argc && strcmp(argv[i],"-t")==0)
            { outputformat=argv[i+1];
             in_t=1;
	     }
        else if(strcmp(argv[i],"help")==0)
		printf("\n Welcome!!!\n There is two type of command to usually use.\n ->./concube -i sweetHome.xml(csv,json) -o alabama.(xml,csv,json) -true(xml is compress ore not)\n ->./concube -i sweetHome.csv(xml,json) -t xml(csv,json) -true\n\n");
		}
       if(ijson==0&&icsv==0&&ixml==0)
        printf("enter a input");
       if(ojson==0&&ocsv==0&&oxml==0&&in_t==0)
        printf("enter a output or a format type");
       if((ojson==1||ocsv==1||oxml==1)&&in_t==1)
        printf("enter a output OR a format type");
       if(in_t == 1 )
      {
          if(strcmp(outputformat,"json")==0)
            {   char temp[80];
                int i;
		for(i=0;i<strlen(input)-3;i++)
                  temp[i]=input[i];
                
		output=strcat(temp,"json");
                ojson=1;
	    }
          else if(strcmp(outputformat,"xml")==0)
            {   char temp[80];
                int i;
		for(i=0;i<strlen(input)-3;i++)
                  temp[i]=input[i];
                
		output=strcat(temp,"xml");
		oxml=1;
	    }
          else if(strcmp(outputformat,"csv")==0)
            {   char temp[80];
                int i;
		for(i=0;i<strlen(input)-3;i++)
                  temp[i]=input[i];
                
		output=strcat(temp,"csv");
		ocsv=1;
	    }
          else
	    printf("wrong format!!!");
      }
       if(ixml==1&&ocsv==1)
          XML(input,output);
       else if(ixml==1&&ojson==1)
          readXMLfile(input,output);
       else if(icsv==1&&ojson==1)
         {
		readCSVfileInitilizeMatrixSize(input);
    		readCSVfileToJSON(input,output);
	  }
       else if(icsv==1&&oxml==1&&comptru==1)
         {
		readCSVfileInitilizeMatrixSize(input);
    		readCSVfileToXMLWA(input,output);
	  }
	else if(icsv==1&&oxml==1&&comptru==0)
         {
		readCSVfileInitilizeMatrixSize(input);
    		readCSVfileToXMLNA(input,output);
	  }
}
static void readCSVfileToXMLNA(char *Filename,char *to){
	
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
    
      xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[256];
   

    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);

            for(i=1;i<column;i++)
	{ 
            node = xmlNewNode(NULL, BAD_CAST "employee");//in csv,we dont know the table name
           for(j=0;j<row;j++)
           {  if(strcmp(matrix[j][i],"-")!=0)
              {
		    xmlNewChild(node, NULL, BAD_CAST matrix[j][0],BAD_CAST matrix[j][i]);
              }
             
           }
         xmlAddChild(root_node, node); 
	}

  /*
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc(to, doc, "UTF-8", 1);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    xmlMemoryDump();
  
}
static void readCSVfileToXMLWA(char *Filename,char *to){
	
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
    
      xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */
    char buff[256];
   

    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    xmlDocSetRootElement(doc, root_node);
    
   
   
    
  
            for(i=1;i<column;i++)
	{ 
           
            node = xmlNewNode(NULL, BAD_CAST "employee");//in csv,we dont know the table name
           for(j=0;j<row;j++)
           {  if(strcmp(matrix[j][i],"-")!=0)
              {
		   xmlNewProp(node, BAD_CAST  matrix[j][0], BAD_CAST matrix[j][i]); 
              }
             
           }
         xmlAddChild(root_node, node); 
	}

  /*
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc(to, doc, "UTF-8", 1);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    xmlMemoryDump();
  
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
static void readCSVfileToJSON(char *Filename, char *to){
	
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
    json_object_object_add(jobj, "default", jarray);//in csv,we dont know the table name
     FILE *fcsvjson=fopen(to,"w");
     fputs(json_object_to_json_string(jobj),fcsvjson);
     fclose(fcsvjson);
}
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
static void readXMLfile(char *Filename,char *to){
	
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
		FILE *fxmljson=fopen(to,"w");
                fputs(json_object_to_json_string(jobj),fxmljson);
                fclose(fxmljson);

		/*free the document*/
		xmlFreeDoc(doc);

	}

	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();

}


int is_XML(char *source){
	int index;
	int length=0;
        for(index=0;index<strlen(source);index++)
        {
		if(source[index]=='.')
			length=strlen(source)-index-1;
	}
        
       char temp[length];
         for(index=strlen(source)-length;index<strlen(source);index++)
        {
		temp[index+length-strlen(source)]=source[index];
		
	}
      if(strcmp(temp,"xml")==0)
       return 1;
    return 0;
}
int is_CSV(char *source){
	int index;
	int length=0;
        for(index=0;index<strlen(source);index++)
        {
		if(source[index]=='.')
			length=strlen(source)-index-1;
	}
        
       char temp[length];
         for(index=strlen(source)-length;index<strlen(source);index++)
        {
		temp[index+length-strlen(source)]=source[index];
		
	}
      if(strcmp(temp,"csv")==0)
       return 1;
    return 0;
}
int is_JSON(char *source){
	int index;
	int length=0;
        for(index=0;index<strlen(source);index++)
        {
		if(source[index]=='.')
			length=strlen(source)-index-1;
	}
        
       char temp[length];
         for(index=strlen(source)-length;index<strlen(source);index++)
        {
		temp[index+length-strlen(source)]=source[index];
		
	}

      if(strcmp(temp,"json")==0)
       return 1;
    return 0;
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
		
		root_element = xmlDocGetRootElement(doc);
                getNumbers(root_element,0);
                FILE *fp=fopen(output,"w");
		takeElements(root_element,fp);
 		take_contents(root_element,fp,root_element->children->next,0);
		fclose(fp);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();
	
}

static void getNumbers(xmlNode * a_node,int number_tempP)
{
	xmlNode *cur_node = NULL;
        int number=0;
      
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
          
		if (cur_node->type == XML_ELEMENT_NODE) {
                    if(strcmp(cur_node->children->content,"\n")==0)
                   	 number_tempP++;
			
		   number++;
               
                if(number>number_XC)
                {number_XC=number;
                 number_place=number_tempP;
                }
	      }
	   getNumbers(cur_node->children,number_tempP);
	}
}
static void takeElements(xmlNode * a_node,FILE *fp)
{
	xmlNode *cur_node = NULL;
       	xmlNode *temp_node = NULL; 
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
           
		if (cur_node->type == XML_ELEMENT_NODE ) {
                 if(strcmp(cur_node->children->content,"\n")==0)
                      numberP++;
                 if(numberP == number_place)
		{
		   temp_node=cur_node->next->next;
		   if(temp_node != NULL && strcmp(cur_node->children->content,"\n")!=0)
                   {
			 fputs(cur_node->name,fp);
			 fputs(",",fp);
		   }
		   else if(strcmp(cur_node->children->content,"\n")!=0)
                   {
			 fputs(cur_node->name,fp);
			 fputs("\n",fp);
		   }
		 }		    	
		}
		takeElements(cur_node->children,fp);
	}

	
}
static void take_contents(xmlNode * a_node,FILE *fp,xmlNode * child,int i){
	xmlNode *cur_node = NULL;
	char *buff[100];
        
        
		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
     if (cur_node->type == XML_ELEMENT_NODE && strcmp(cur_node->children->content,"\n")!=0) {	                
	         if(strcmp(cur_node->children->content," ")>0&& cur_node->next->next != NULL)
                       { fputs(cur_node->children->content,fp);
			fputs(",",fp);}
	           if(strcmp(cur_node->children->content," ")>0&& cur_node->next->next == NULL)
			  { fputs(cur_node->children->content,fp);
			   fputs("\n",fp);}			 

			}
			
			take_contents(cur_node->children,fp,child,i);
		}
	
}
