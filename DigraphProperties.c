//List.c
//Created by Koorous Vargha

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Digraph.h"


#define MAX_LEN 2000


int main(int argc, char * argv[])
{
	Digraph G;
	int u, v;
	FILE *in, *out;
	char line[MAX_LEN];
	char* token;

	// check command line for correct number of arguments
	if(argc != 3)
	{
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	// open files for reading and writing 
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");

	//Checks if there's an input file
	if(in == NULL)
	{
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	//Checks if there's an output file
	if(out == NULL)
	{
		printf("Unable to open file %s for writing\n", argv[2]);
		exit(1);
	}

	//Grabs first line in input file
	fgets(line, MAX_LEN, in);
	//Grabs the order of the graph
	token = strtok(line, ",");

	int numVerticies = atoi(token);

	if(numVerticies <= 0)
	{
		fprintf(out, "%s\n", line);
		fprintf(out, "ERROR\n");
		exit(1);
	}

	//Creates newGraph
	G = newDigraph(numVerticies);
	
	//Grabs first verticies
	token = strtok(NULL, ", ");
	//Adds vertices to graph	
	while(token != NULL)
	{
		u = atoi(token);
		token = strtok(NULL, " ");
		v = atoi(token);

		addEdge(G, u, v);
	 	
	 	//Grabs next vertices on line
	 	token = strtok(NULL, ", ");
	}

	
	// read each line of input file and run commands
	while(fgets(line, MAX_LEN, in) != NULL)  
	{   
		//To deal with different operating systems
		token = strtok(line, "\r\n");

		//Prints out input line
		fprintf(out, "%s\n", token);

		//Checks if line is empty
		if(token == NULL)
		{
			fprintf(out, "%s\n", "ERROR");
		}
		else
		{
			//Grabs first word in line
			token = strtok(token, " ");

			if(strcmp(token, "PrintDigraph") == 0)
			{
				//If next item isn't NULL, prints ERROR
				if(strtok(NULL, " ") != NULL)
					fprintf(out, "%s\n", "ERROR");
				else
					printDigraph(out, G);
			}
			else if(strcmp(token, "GetOrder") == 0)
			{
				//If next item isn't NULL, prints ERROR
				if(strtok(NULL, " ") != NULL)
					fprintf(out, "%s\n", "ERROR");
				else
					fprintf(out, "%d\n", getOrder(G));
			}
			else if(strcmp(token, "GetSize") == 0)
			{
				//If next item isn't NULL, prints ERROR
				if(strtok(NULL, " ") != NULL)
					fprintf(out, "%s\n", "ERROR");
				else
					fprintf(out, "%d\n", getSize(G));
			}
			else if(strcmp(token, "GetOutDegree") == 0)
			{
				//Grabs next item on list
				token = strtok(NULL, " ");

				if(token == NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					int degree = getOutDegree(G, atoi(token));

					//Checks if degree doesn't exit or if there's an extra value
					if(degree == -1 || strtok(NULL, " ") != NULL)
						fprintf(out, "ERROR\n");
					else
						fprintf(out, "%d\n", degree);
				}
			}
			else if(strcmp(token, "GetCountSCC") == 0)
			{
				//Grabs next item on list
				token = strtok(NULL, " ");

				if(token != NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					int sccCount = getCountSCC(G);

					//Checks if sccCount doesn't exit or if there's an extra value
					if(sccCount == -1 || strtok(NULL, " ") != NULL)
						fprintf(out, "ERROR\n");
					else
						fprintf(out, "%d\n", sccCount);
				}

			}
			else if(strcmp(token, "GetNumSCCVertices") == 0)
			{
				//Grabs next item on list
				token = strtok(NULL, " ");

				if(token == NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					int numSCCVerticies = getNumSCCVertices(G, atoi(token));

					//Checks if numSCCVerticies doesn't exit or if there's an extra value
					if(numSCCVerticies == -1 || strtok(NULL, " ") != NULL)
						fprintf(out, "ERROR\n");
					else
						fprintf(out, "%d\n", numSCCVerticies);
				}

			}
			else if(strcmp(token, "InSameSCC") == 0)
			{
				//Moves past the chars and moves to the ints
				token = strtok(NULL, " "); 

				//Checks if first value exists
				if(token == NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					//Converts first int into u
					u = atoi(token);

					//Grabs next value
					token = strtok(NULL, " ");

					//Checks if second value exists
					if(token == NULL)
					{
						fprintf(out, "%s\n", "ERROR");
					}
					else
					{
						//Converts second int to v
						v = atoi(token);

						//Checks if u or v are legal verticies and if there's an extra value
						if(u == v || u < 1 || v < 1 || u > getOrder(G) || v > getOrder(G) || strtok(NULL, " ") != NULL)
							fprintf(out, "ERROR\n");
						else	
						{
							//If inSameSCC returns 1 then print YES, else print NO
							if(inSameSCC(G, u, v) == 1)
								fprintf(out, "%s\n", "YES");
							else
								fprintf(out, "%s\n", "NO");
						}
					}
				}
			}
			else if(strcmp(token, "DeleteEdge") == 0)
			{
				//Moves past the chars and moves to the ints
				token = strtok(NULL, " "); 

				//Checks if first value exists
				if(token == NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					//Converts first int into u
					u = atoi(token);

					//Grabs next value
					token = strtok(NULL, " ");

					//Checks if second value exists
					if(token == NULL)
					{
						fprintf(out, "%s\n", "ERROR");
					}
					else
					{
						//Converts second int to v
						v = atoi(token);

						//Checks if u or v are legal verticies and if there's an extra value
						if(u == v || u < 1 || v < 1 || u > getOrder(G) || v > getOrder(G) || strtok(NULL, " ") != NULL)
							fprintf(out, "ERROR\n");
						else	
							fprintf(out, "%d\n", deleteEdge(G, u, v));
					}
				}
	 
			}
			else if(strcmp(token, "AddEdge") == 0)
			{
				//Moves past the chars and moves to the ints
				token = strtok(NULL, " "); 

				//Checks if first value exists
				if(token == NULL)
				{
					fprintf(out, "%s\n", "ERROR");
				}
				else
				{
					//Converts first int into u
					u = atoi(token);

					//Grabs next value
					token = strtok(NULL, " ");

					//Checks if second value exists
					if(token == NULL)
					{
						fprintf(out, "%s\n", "ERROR");
					}
					else
					{
						//Converts second int to v
						v = atoi(token);

						//Checks if u or v are legal verticies and if there's an extra value
						if(u == v || u < 1 || v < 1 || u > getOrder(G) || v > getOrder(G) || strtok(NULL, " ") != NULL)
							fprintf(out, "ERROR\n");
						else	
							fprintf(out, "%d\n", addEdge(G, u, v));
					}
				}
			}
			else
				fprintf(out, "%s\n", "ERROR");
		}
	}

	//Frees memory of digraph
	freeDigraph(&G);
	

	/* close files */
	fclose(in);
	fclose(out);

	//Reached end of code without errors, quit
	return(0);
}

