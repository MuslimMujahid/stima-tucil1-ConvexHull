#include <cstdlib>
#include <ctime>
#include <cmath>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;


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
        vector<Line> convex_hull;
        Node *Nodes;
        int N_Nodes;
        int N_Convex_hull;

    public:
        ConvexHull(int N)
        {
            N_Convex_hull = 0;
            N_Nodes = N;
            Node tmp[N];
            Nodes = (Node*)malloc(sizeof(Node)*N);
            Nodes = tmp;

            Process();
        }

        void Process()
        {
            auto start = high_resolution_clock::now(); 

            for (int i = 0; i < N_Nodes-1; ++i)
            {
                for (int j = i+1; j < N_Nodes; ++j)
                {
                    if (IsConvexHull(Nodes[i], Nodes[j]))
                    {
                        convex_hull.push_back({Nodes[i], Nodes[j]});
                        N_Convex_hull++;
                    }  
                }
            }

            auto stop = high_resolution_clock::now(); 
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time process : " << duration.count() << " microseconds" << endl;
            Show();
        }

        bool IsConvexHull(Node p1, Node p2)
        {
            double a = p2.y-p1.y,
            b = p1.x-p2.x,
            c = p1.x*p2.y-p1.y*p2.x;

            int i = 0;
            while (Nodes[i].x == p1.x && Nodes[i].y == p1.y ||
            Nodes[i].x == p2.x && Nodes[i].y == p2.y)
            {
                ++i;
            }
            bool d = a*Nodes[i].x+b*Nodes[i].y > c;

            for (int j = i+1; j < N_Nodes; ++j)
            {   
                int ax = a*Nodes[j].x,
                by = b*Nodes[j].y;

                if (d && ax+by < c || !d && ax+by > c)
                    return false;
            }
            return true;
        }
        
        void Show()
        {
            cv::namedWindow("chart", CV_WINDOW_AUTOSIZE);
            
            int expand = 8,
            margin_top_bottom = 50,
            margin_left_right = 300;

            cv::Mat chart(100*expand+margin_top_bottom, 
            100*expand+margin_left_right, 
            CV_8UC3, 
            cv::Scalar(255, 255, 255));

            for (int i = 0; i < N_Nodes; ++i) 
            {
                Node node = Nodes[i];
                int nodex = node.x*expand+margin_left_right/2,
                nodey = node.y*expand+margin_top_bottom/2;
                circle(chart, cv::Point(nodex, nodey), 12.0, cv::Scalar(255, 0, 0), 2, 1);

                cv::putText(chart, to_string(node.x) + "," + to_string(node.y), cv::Point(nodex+15, nodey), 
                cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 0), 1, CV_AA);
            }

            for (int i = 0; i < N_Convex_hull; ++i)
            {
                Line l = convex_hull[i];
                int p1x = l.p1.x*expand+margin_left_right/2,
                p1y = l.p1.y*expand+margin_top_bottom/2,
                p2x = l.p2.x*expand+margin_left_right/2,
                p2y = l.p2.y*expand+margin_top_bottom/2;

                line(chart, 
                cv::Point(p1x, p1y), 
                cv::Point(p2x, p2y), 
                cv::Scalar(0, 0, 255), 
                1, 
                cv::LINE_AA);

                circle(chart, 
                cv::Point(p1x, p1y), 12.0,cv::Scalar(255, 0, 0), -1, 1);
                circle(chart, 
                cv::Point(p2x, p2y), 12.0, cv::Scalar(255, 0, 0), -1, 1);
            }

            cv::imshow("chart", chart);
            cv::waitKey(0);
        }
};

int main()
{
    srand(time(NULL));
    ConvexHull CH(20);
    return 0;
}