#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

static string WINDOW = "Convex Hull";

class Node
{
    public:
        int x, y;
        bool convex_hull;
        Node()
        {
            /* When object is created automatically assign random value to x and y */
            convex_hull = false;
            x = rand() % 100;
            y = rand() % 100;
        }

        void Set() // Set this node as part of convex hull
        {
            convex_hull = true;
        }
};

class Line
{
    public:
        Node p1, p2;
};

class ConvexHull
{
    private:
        
    public:
        int n_node;
        int n_line;
        Node *nodes;
        vector<Line> line;

        ConvexHull()
        {
            int N;
            cout << "input N: "; cin >> N; 
             
            /* Only process if N > 0 */
            if (N <= 0)
                return;

            /* Setup variables */
            srand(time(NULL));
            n_node = N;
            n_line = 0;
            nodes = (Node*)malloc(sizeof(Node)*N);
            nodes = new Node[N];

            cout << "\nTime: " << Process() << " microseconds" << endl;
            Print();
        }

        int Process() /*  */
        {
            auto start = high_resolution_clock::now(); // Start counting processing time

            /* Process divided to 4 cases */
            /* 1. n_nodes == 1, the only node become the convex hull */
            /* 2. n_nodes == 2, both nodes is convex hull */
            /* 3. n_nodes == 3, select two node that form maximum line length */
            /* 4. n_nodes > 3, choose the set of nodes that wrap all other nodes inside it */
            if (n_node == 1)
            {
                nodes[0].Set();
            }
            else if (n_node == 2)
            {
                line.push_back({nodes[0], nodes[1]});
                nodes[0].Set();
                nodes[1].Set();
                n_line = 1;
            }
            else if (n_node == 3)
            {
                int maxi = 0, maxj = 1;
                for (int i = 1; i < n_node-1; ++i)
                {
                    for (int j = i+1; j < n_node; ++j)
                    {
                        if (sqrt(pow(nodes[maxi].x-nodes[maxj].x,2)+pow(nodes[maxi].y-nodes[maxj].y, 2)) < sqrt(pow(nodes[i].x-nodes[j].x,2)+pow(nodes[i].y-nodes[j].y, 2)))
                        {
                            maxi = i;
                            maxj = i;
                        }
                    }
                }
                line.push_back({nodes[maxi], nodes[maxj]});
                nodes[maxi].Set();
                nodes[maxj].Set();
                n_line = 1;
            }
            else 
            {
                for (int i = 0; i < n_node-1; ++i)
                {
                    for (int j = i+1; j < n_node; ++j)
                    {
                        if (IsConvexHull(nodes[i], nodes[j]))
                        {
                            line.push_back({nodes[i], nodes[j]});
                            nodes[i].Set();
                            nodes[j].Set();
                            n_line++;
                        }
                    }
                }
            }

            auto stop = high_resolution_clock::now(); // stop counting proecssing time
            auto duration = duration_cast<microseconds>(stop - start);
            return duration.count();
        }

        /* check if particular node is in convex hull for trivial cases */
        bool IsConvexHull(Node p1, Node p2)
        {
            double a = p2.y-p1.y,
            b = p1.x-p2.x,
            c = p1.x*p2.y-p1.y*p2.x;

            int i = 0;
            while (nodes[i].x == p1.x && nodes[i].y == p1.y ||
            nodes[i].x == p2.x && nodes[i].y == p2.y)
                ++i;
            double d = a*nodes[i].x+b*nodes[i].y > c;
            
            for (int j = i+1; j < n_node; ++j)
            {   
                int ax = a*nodes[j].x,
                by = b*nodes[j].y;

                if (d && ax+by < c || !d && ax+by > c ||
                ax+by == c && (nodes[j].y > max(p1.y, p2.y) || 
                nodes[j].y < min(p1.y, p2.y)))
                    return false;
                    
            }
            return true;
        }

        /* Print out convex hull nodes coordinate */
        void Print()
        {
            for (int i = 0; i < n_node; ++i)
            {
                Node p = nodes[i];
                if (p.convex_hull)
                {
                    cout << "(" << p.x << " " << p.y << ")" << endl;
                }
            }
        }
};

int main()
{
    /*  Start program */
    ConvexHull CH;

    return 0;
}