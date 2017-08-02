/*
 * cgitest.c -- CGI Test example for the GoAhead WebServer
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: cgitest.c,v 1.2 2001/12/06 16:28:24 bporter Exp $
 */

/******************************** Description *********************************/

/*
 *	Tests the CGI environment variables generated by the GoAhead Webserver
 */
 
/********************************* Includes ***********************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parseformdata.h"
#include "match.h"

/******************************************************************************/
/*
 * Compare strings, ignoring case:  normal strcmp return codes.
 *
 *	WARNING: It is not good form to increment or decrement pointers inside a
 *	"call" to tolower et al. These can be MACROS, and have undesired side
 *	effects.
 */

int strcmpci(char *s1, char *s2)
{
	int		rc;

	if (s1 == NULL || s2 == NULL) 
	{
		return 0;
	}

	if (s1 == s2) 
	{
		return 0;
	}

	do 
	{
		rc = tolower(*s1) - tolower(*s2);
		if (*s1 == '\0') 
		{
			break;
		}
		s1++;
		s2++;
	} while (rc == 0);

	return rc;
}

/*************************************************************************/
/*
 *	gogetenv wraps getenv so that (NULL) is returned if the string is
 *	NULL.
 */

char* gogetenv(char *varname)
{
	char *result;

	if ((result = getenv(varname)) == NULL) 
	{
		result = "(NULL)";
	}
	return result;
}

/*************************************************************************/
/*
 *	Convert a two-char hex string into the char it represents
 */

char x2c(char *what) 
{
   register char digit;

   digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
   digit *= 16;
   digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

   return digit;
}

/*************************************************************************/
/*
 *	Reduce any %xx escape sequences to the characters they represent
 */

void unescape_url(char *url) 
{
    register int i,j;

    for(i = 0, j = 0; url[j]; ++i,++j) 
    {
        if((url[i] = url[j]) == '%') 
        {
            url[i] = x2c(&url[j + 1]) ;
            j += 2 ;
        }
    }

    url[i] = '\0' ;
}

char *getCGIstr(void)
{
    char	*request_method, *cgiinput;
    int		content_length;

    request_method = gogetenv("REQUEST_METHOD");
    if (!request_method) 
        request_method = "NOT-CGI";

    if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD")) 
    {
        if (getenv("QUERY_STRING") == NULL)
            return NULL;
        cgiinput= strdup(gogetenv("QUERY_STRING"));
    } 
    else if (!strcmp(request_method, "POST")) 
    {
        printf("content_type:%s<br>\n", gogetenv("CONTENT_TYPE"));
        if (strcmpci(gogetenv("CONTENT_TYPE"), "application/x-www-form-urlencoded")) 
        {
            printf("getcgivars(): Unsupported Content-Type.\n");
            return NULL;
        }
        if (!(content_length = atoi(gogetenv("CONTENT_LENGTH")))) 
        {
            printf("getcgivars(): No Content-Length was sent with the POST request.\n");
            return NULL;
        }
        if (!(cgiinput = (char *) malloc(content_length + 1))) 
        {
            printf("getcgivars(): Could not malloc for cgiinput.\n") ;
            return NULL;
        }
        if (!fread(cgiinput, content_length, 1, stdin)) 
        {
            printf("Couldn't read CGI input from STDIN.\n");
            return NULL;
        }
        cgiinput[content_length] = '\0' ;
    } 
    else 
    {
        printf("getcgivars(): unsupported REQUEST_METHOD\n") ;
        return NULL;
    }

    return cgiinput;
}

/*************************************************************************/
/*                                                                     
 *	getCGIvars()	Routine to read CGI input variables into an     
 *					array of strings.
 *
 *	Written in 1996 by James Marshall, james@jmarshall.com, except
 *	that the x2c() and unescape_url() routines were lifted directly
 *	from NCSA's sample program util.c, packaged with their HTTPD.
 *
 *	For the latest, see http://www.jmarshall.com/easy/cgi/ .
 *
 *	Read the CGI input and place all name/val pairs into list.
 *	Returns list containing name1, value1, name2, value2, ... , NULL
 */

char** getCGIvars(void) 
{
	register int	i;
	char			*request_method, *cgiinput, *nvpair, *eqpos;
	int				content_length, paircount;
  	char			**cgivars, **pairlist;

/*
 *	Depending on the request method, read all CGI input into cgiinput
 *	(really should produce HTML error messages, instead of exit()ing)
 */
    request_method = gogetenv("REQUEST_METHOD");

	if (!request_method) 
	{
		request_method = "NOT-CGI";
	}

    if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD")) 
    {
    	 if (getenv("QUERY_STRING") == NULL)
    	 {
    	 	return NULL;
    	 }
        cgiinput= strdup(gogetenv("QUERY_STRING"));
    } 
    else if (!strcmp(request_method, "POST")) 
    {
      	printf("content_type:%s<br>\n", gogetenv("CONTENT_TYPE"));
       if (strcmpci(gogetenv("CONTENT_TYPE"), "application/x-www-form-urlencoded")) 
        {
            printf("getcgivars(): Unsupported Content-Type.\n");
            return NULL;
        }

        if (!(content_length = atoi(gogetenv("CONTENT_LENGTH")))) 
        {
            printf("getcgivars(): No Content-Length was sent with the POST request.\n");
            return NULL;
        }

        if (!(cgiinput = (char *) malloc(content_length + 1))) 
        {
            printf("getcgivars(): Could not malloc for cgiinput.\n") ;
            return NULL;
        }

        if (!fread(cgiinput, content_length, 1, stdin)) 
        {
            printf("Couldn't read CGI input from STDIN.\n");
            return NULL;
        }

        cgiinput[content_length] = '\0' ;
    } 
    else 
    {
        printf("getcgivars(): unsupported REQUEST_METHOD\n") ;
        return NULL;
    }

/*
 *	Change all plusses back to spaces
 */
    for (i = 0; cgiinput[i]; i++) 
    {
		if (cgiinput[i] == '+') 
		{
			cgiinput[i] = ' ';
		}
	}

/*
 *	First, split on "&" to extract the name-value pairs into pairlist
 */
    pairlist = (char **) malloc(256 * sizeof(char **));
    paircount = 0 ;
    nvpair = strtok(cgiinput, "&");

    while (nvpair) 
    {
        pairlist[paircount++] = strdup(nvpair);
        if (!(paircount % 256)) 
        {
            pairlist= (char **) realloc(pairlist, (paircount + 256)
				* sizeof(char **));
		}

        nvpair = strtok(NULL, "&") ;
    }

/*	
 *	Terminate the pair list with NULL
 */
    pairlist[paircount] = 0;

/*
 *	Then, from the list of pairs, extract the names and values
 */
    cgivars = (char **) malloc((paircount * 2 + 1) * sizeof(char **));
    for (i = 0; i < paircount; i++) 
    {
        if ((eqpos = strchr(pairlist[i], '=')) != NULL)
        {
            *eqpos = '\0' ;
            unescape_url(cgivars[i * 2 + 1] = strdup(eqpos + 1));
        } 
        else 
        {
            unescape_url(cgivars[i * 2 + 1] = strdup(""));
        }
        unescape_url(cgivars[i * 2] = strdup(pairlist[i]));
    }

/* 
 *	Terminate the cgi var list with NULL
 */
    cgivars[paircount * 2] = 0;	
    
/*
 *	Free anything that needs to be freed
 */
    free(cgiinput);

    for (i = 0; pairlist[i]; i++) 
    {
		free(pairlist[i]);
	}

    free(pairlist);

/*
 *	Return the list of name-value strings
 */
    return cgivars;
    
}

char* findCGIvar( char *list[], char *name )
{
	if (list == NULL)
	{
		return NULL;
	}
	
	while( *list != NULL )
	{
   	char *p = *list;
    	char *q = name;

    	if (match(p, q))
    		return *(list+1);
    
    	list += 2;
	}
	return NULL;
}

void freeCGIvars( char *list[] )
{
	char	**head;
	
	head = list;
	while (*list)
	{
		free(*list);
		list++;
	}
	
	if (head)
	{
		free(head);
	}
}

char* getfilebuf( char *filename, int fnamelen, int *buflen )
{
	char	*request_method=NULL, *filebuf=NULL, *bnd=NULL, *line=NULL;
	char	*fpath=NULL, *fname=NULL;
	int		content_length, len, i, ret;
	
    	request_method = gogetenv("REQUEST_METHOD");
	if (!request_method) 
	{
		request_method = "NOT-CGI";
	}

    	if (!strcmp(request_method, "POST"))
   	{
    		if (!(content_length = atoi(gogetenv("CONTENT_LENGTH")))) 
	       {
	            	printf("getfilebuf No Content-Length was sent with the POST request.\n");
	            	return NULL;
	       }
	       //printf("content_length: %d<br>\n", content_length);

	       if (!(filebuf = (char *) malloc(content_length + 1))) 
	       {
	           	printf("getfilebuf Could not malloc for filebuf.\n") ;
	            	return NULL;
	       }
#if (0)
		i = 0;
		while ((ret = getline(&line, &len, stdin)) != -1)
		{
			printf("line%d: %s; len=%d; <br>\n", i++, line, ret);
		}
		return NULL;
#endif
        
	       ret = getline(&bnd, (size_t *)&len, stdin);
	       //printf("bnd: %s; len=%d; ret=%d<br>\n", bnd, strlen(bnd), ret);
	       while ((ret = getline(&line, (size_t *)&len, stdin)) != -1)
	       {
	       	if (strstr(line, "filename=") != NULL)
	       		break;
	       }
	       memset(filename, 0, fnamelen);
		if ((fpath=strstr(line, "filename")) != NULL)
		{
			if ((fname=strrchr(fpath, '\\')) != NULL)
			{
				fname++;
	
				i=0;
				while((*fname != '\"') && (*fname != '\0') && (i < fnamelen-1))
				{
					filename[i++] = *fname;
			 		fname++;
				}
				filename[i] = '\0';
			}
		}

		//printf("filename: %s\n", filename);
		
		getline(&line, (size_t *)&len, stdin);
		getline(&line, (size_t *)&len, stdin);
		
		*buflen = 0;
		//i = 0;
		while ((ret = getline(&line, (size_t *)&len, stdin)) != -1)
		{
	        	if (strncmp(line, bnd, strlen(bnd)-2) == 0)
	        	{
	        		printf("end line: %s\n", line);
	                	break;
	        	}
	        	//printf("line%d: %s; len=%d<br>\n", i++, line, ret);
	   		memcpy(filebuf+(*buflen), line, ret);
	    		*buflen += ret;
		}

		*buflen -= 2;
		if (*buflen < 0)
		{
			*buflen = 0;
		}
	       filebuf[*buflen] = '\0' ;
        
	       if (line)
		{
			free(line);
		}
		if (bnd)
		{
			free(bnd);
		}
			
	        return filebuf;
    	} 
    	else 
    	{
        	printf("getfilebuf unsupported REQUEST_METHOD for file upload.\n") ;
        	return NULL;
    	}
}
 
