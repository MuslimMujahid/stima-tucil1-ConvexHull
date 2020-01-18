#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace std;

static string WINDOW = "Convex Hull";

class Node
{
    public:
        int x, y;
        Node()
        {
            x = rand() % 100;
            y = rand() % 100;
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
        int n_convex_hull;
        Node *nodes;
        vector<Line> convex_hull;

        ConvexHull(int N = 0)
        {
            // Only process if N > 0
            if (N <= 0)
                return;

            srand(time(NULL));
            n_node = N;
            n_convex_hull = 0;
            nodes = (Node*)malloc(sizeof(Node)*N);
            nodes = new Node[N];
            nodes[5].x = 0;
            nodes[6].x = 0;
            nodes[7].x = 0;
            Process();
        }

        void Process()
        {
            for (int i = 0; i < n_node-1; ++i)
            {
                for (int j = i+1; j < n_node; ++j)
                {
                    if (IsConvexHull(nodes[i], nodes[j]))
                    {
                        convex_hull.push_back({nodes[i], nodes[j]});
                        n_convex_hull++;
                    }
                }
            }
        }

        bool IsConvexHull(Node p1, Node p2)
        {
            double a = p2.y-p1.y,
            b = p1.x-p2.x,
            c = p1.x*p2.y-p1.y*p2.x;

            int i = 0;
            while (nodes[i].x == p1.x && nodes[i].y == p1.y ||
            nodes[i].x == p2.x && nodes[i].y == p2.y)
                ++i;

            double d = a*nodes[i].x+b*nodes[i].y;
            bool e = d > c;
            
            for (int j = i+1; j < n_node; ++j)
            {   
                int ax = a*nodes[j].x,
                by = b*nodes[j].y;

                if (e && ax+by < c || !e && ax+by > c ||
                ax+by == c && (nodes[j].y > max(p1.y, p2.y) || 
                nodes[j].y < min(p1.y, p2.y)))
                    return false;
                    
            }
            return true;
        }
};

class GUI
{
    private:
        ConvexHull CH;
        int expand,
        margin_v,
        margin_h;
    
    public:
        GUI(ConvexHull CH_, int expand_ = 8, int margin_v_ = 0, int margin_h_ = 0)
        {
            CH = CH_;
            expand = expand_;
            margin_v = margin_v_;
            margin_h = margin_h_;

            cv::namedWindow(WINDOW, CV_WINDOW_AUTOSIZE);
            Set();
        }

        void Set()
        {   
            int width = 100*expand+(margin_h*2),
            height = 100*expand+(margin_v*2);
            cv::Mat window(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

            for (int i = 0; i < CH.n_node; ++i) 
            {
                Node node = CH.nodes[i];
                int nodex = node.x*expand+margin_h,
                nodey = height-(node.y*expand+margin_v);
                circle(window, cv::Point(nodex, nodey), 12.0, cv::Scalar(255, 0, 0), 2, 1);

                cv::putText(window, to_string(node.x) + "," + to_string(node.y), cv::Point(nodex+15, nodey), 
                cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1, CV_AA);
            }

            for (int i = 0; i < CH.n_convex_hull; ++i)
            {
                Line l = CH.convex_hull[i];
                int p1x = l.p1.x*expand+margin_h,
                p1y = height-(l.p1.y*expand+margin_v),
                p2x = l.p2.x*expand+margin_h,
                p2y = height-(l.p2.y*expand+margin_v);

                line(window, cv::Point(p1x, p1y), cv::Point(p2x, p2y), cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

                circle(window, cv::Point(p1x, p1y), 12.0,cv::Scalar(255, 0, 0), -1, 1);
                circle(window, cv::Point(p2x, p2y), 12.0, cv::Scalar(255, 0, 0), -1, 1);
            }

            cv::imshow(WINDOW, window);
            cv::waitKey(0);
        }
};

int main()
{
    ConvexHull CH(20);
    GUI gui(CH, 8, 50, 200);
    return 0;
}