#ifndef HEADER
#define HEADER

struct AdjListNode
{
	int dest;
	struct AdjListNode* next;
};
struct AdjList
{
	struct AdjListNode *head;
};
struct Graph
{
	int V;
	struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest);
struct Graph* createGraph(int V);
void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);

#endif