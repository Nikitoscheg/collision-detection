#include <iostream>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;


class Circle{
    public:
    int radius;
    double Xvelocity;
    double Yvelocity;
    double Xcoord;
    double Ycoord;

    Circle(int radius, double Xvelocity, double Yvelocity, double Xcoord, double Ycoord){
        this->radius = radius;
        this->Xvelocity = Xvelocity;
        this->Yvelocity = Yvelocity;
        this->Xcoord = Xcoord;
        this->Ycoord = Ycoord;
    }
};

class QuadTreeNode {
public:
    double x;
    double y;
    double w;
    double h;
    vector<Circle> Circles;
    QuadTreeNode* bottomLeft;
    QuadTreeNode* bottomRight;
    QuadTreeNode* topLeft;
    QuadTreeNode* topRight;
    static const int Capacity = 4;

    QuadTreeNode(double _x, double _y, double _w, double _h) : x(_x), y(_y), w(_w), h(_h), bottomLeft(nullptr), bottomRight(nullptr), topLeft(nullptr), topRight(nullptr) {}

    ~QuadTreeNode() {
        delete bottomRight;
        delete bottomLeft;
        delete topRight;
        delete topLeft;
    }

    bool isLeafNode() const {
        return bottomLeft == nullptr && bottomRight == nullptr && topLeft == nullptr && topRight == nullptr;
    }
    bool isLeafNodeOne() const {
        return bottomLeft == nullptr || bottomRight == nullptr || topLeft == nullptr || topRight == nullptr;
    }

    void insert(const Circle& ball){
        if (!isLeafNode()) {
            if (ball.Xcoord < x + w / 2) {
                if (ball.Ycoord < y + h / 2) {
                    if (!bottomLeft) {
                        bottomLeft = new QuadTreeNode(x, y, w / 2, h / 2);
                    }
                    bottomLeft->insert(ball);
                }
                else {
                    if (!topLeft) {
                        topLeft = new QuadTreeNode(x, y + h / 2, w / 2, h / 2);
                    }
                    topLeft->insert(ball);
                }
            }
            else {
                if (ball.Ycoord < y + h / 2) {
                    if (!bottomRight) {
                        bottomRight = new QuadTreeNode(x + w / 2, y, w / 2, h / 2);
                    }
                    bottomRight->insert(ball);
                }
                else {
                    if (!topRight) {
                        topRight = new QuadTreeNode(x + w / 2, y + h / 2, w / 2, h / 2);
                    }
                    topRight->insert(ball);
                }
            }
        }
        else {
            Circles.push_back(ball);
            if (Circles.size() > Capacity) {
                if (!bottomLeft) {
                    bottomLeft = new QuadTreeNode(x, y, w / 2, h / 2);
                }
                if (!bottomRight) {
                    bottomRight = new QuadTreeNode(x + w / 2, y, w / 2, h / 2);
                }
                if (!topLeft) {
                    topLeft = new QuadTreeNode(x, y + h / 2, w / 2, h / 2);
                }
                if (!topRight) {
                    topRight = new QuadTreeNode(x + w / 2, y + h / 2, w / 2, h / 2);
                }
                redistributeBalls();

            }
        }
    }



    void validate(QuadTreeNode rot) {
        vector<int> numbers_delete;
        bool flagToStop = false;
        //Circles.erase(Circles.begin()+i);
        for(int i = 0; i < Circles.size(); i++){
            flagToStop = false;
            if(Circles[i].Xcoord < x+w/2 && Circles[i].Ycoord < y+h/2){
                if(Circles[i].Xcoord > x+w/2 || Circles[i].Ycoord > y+h/2){
                    numbers_delete.push_back(i);
                    rot.insert(Circles[i]);
                    flagToStop = true;
                }
            }
            if(flagToStop == false && Circles[i].Xcoord < x+w/2 && Circles[i].Ycoord > y+h/2){
                if(Circles[i].Xcoord > x+w/2 || Circles[i].Ycoord < y+h/2){
                    numbers_delete.push_back(i);
                    rot.insert(Circles[i]);
                    flagToStop = true;
                }
            }
            if(flagToStop == false && Circles[i].Xcoord > x+w/2 && Circles[i].Ycoord < y+h/2){
                if(Circles[i].Xcoord < x+w/2 || Circles[i].Ycoord > y+h/2){
                    numbers_delete.push_back(i);
                    rot.insert(Circles[i]);
                    flagToStop = true;
                }
            }
            if(flagToStop == false && Circles[i].Xcoord > x+w/2 && Circles[i].Ycoord > y+h/2){
                if(Circles[i].Xcoord < x+w/2 || Circles[i].Ycoord < y+h/2){
                    numbers_delete.push_back(i);
                    rot.insert(Circles[i]);
                    flagToStop = true;
                }
            }
        }
        for (int i=0; i<numbers_delete.size(); i++){
            Circles.erase(Circles.begin()+numbers_delete[i]);
        }

    }

    void redistributeBalls() {
        for (const auto& ball : Circles) {
            insert(ball);
        }
        Circles.clear();
    }

    void findCollisions(vector<Circle>& results) {
        if (!isLeafNode() || !isLeafNodeOne()) {
            topLeft->findCollisions(results);
            topRight->findCollisions(results);
            bottomLeft->findCollisions(results);
            bottomRight->findCollisions(results);
        }
        else {
            for (int i = 0; i < Circles.size(); i++) {
                for (int j = i + 1; j < Circles.size(); ++j) {
                    if (isCollision(Circles[i], Circles[j])){
                        results.push_back(Circles[i]);
                        results.push_back(Circles[j]);
                    }
                }
            }
            Velocity();
        }
    }

    void findCollisionsByCircle(vector<Circle>& results, const Circle& circle) {
        if (!isLeafNode() || !isLeafNodeOne()) {
            bool isLeft = false;
            bool isRight = false;
            bool isTop = false;
            bool isBottom = false;
            if (circle.Xcoord - circle.radius < x + w / 2) {
                isLeft = true;
            }
            if (circle.Xcoord + circle.radius > x + w / 2) {
                isRight = true;
            }
            if (circle.Ycoord - circle.radius < y + h / 2) {
                isTop = true;
            }
            if (circle.Ycoord + circle.radius > y + h / 2) {
                isBottom = true;
            }

            if (isTop && isLeft) {
                topLeft->findCollisions(results);
            }
            if (isTop && isRight){
                topRight->findCollisions(results);
            }
            if (isBottom && isLeft){
                bottomLeft->findCollisions(results);
            }
            if (isBottom && isRight){
                bottomRight->findCollisions(results);
            }
        }
        else {
            for (int i = 0; i < Circles.size(); i++) {
                if (isCollision(Circles[i],  circle)){
                    results.push_back(Circles[i]);
                }
            }
        }
    }

    bool isCollision(const Circle& C1, const Circle& C2) {
        double distance = sqrt(pow(C1.Xcoord - C2.Xcoord, 2) + pow(C1.Ycoord - C2.Ycoord, 2));
        return distance <= C1.radius + C2.radius;
    }

    void Velocity(){
        for(int i=0; i<Circles.size(); i++) {
            for (int j = i; j<Circles.size(); j++) {
                if (i!=j){
                    if (isCollision(Circles[i], Circles[j])) {
                        Circles[i].Xvelocity *= -1;
                        Circles[i].Yvelocity *= -1;
                        Circles[j].Yvelocity *= -1;
                        Circles[j].Xvelocity *= -1;
                    }
                }
            }
        }
    }

};


int main(){
    QuadTreeNode root(0,0,100,100);
    vector<Circle>Circles;
    Circle ball1(5, 10, 10, 20, 5);
    Circle ball2(10, 20,10, 20, 20);
    Circle ball3(15, 20,15, 40, 40);
    Circle ball4(20, 5,0, 75, 40);

    Circles.push_back(ball1);
    Circles.push_back(ball2);
    Circles.push_back(ball3);
    Circles.push_back(ball4);

    root.insert(ball1);
    root.insert(ball2);
    root.insert(ball3);
    root.insert(ball4);

    vector<Circle>collisions = Circles;
     root.findCollisionsByCircle(collisions, ball1);
     root.findCollisionsByCircle(collisions, ball2);
     root.findCollisionsByCircle(collisions, ball3);
     root.findCollisionsByCircle(collisions, ball4);

    //root.findCollisions(collisions);

    double t = 0;
    double dt = 0.1;
    while(true){

        root.validate(root);
        for (auto& ball: collisions){
            ball.Xcoord += ball.Xvelocity * dt;
            ball.Ycoord += ball.Yvelocity * dt;
        }
        for(auto& ball : collisions){
            if(ball.Xcoord-ball.radius < 0 || ball.Xcoord + ball.radius > 100){
                ball.Xvelocity *= -1;
            }
            if (ball.Ycoord - ball.radius < 0 || ball.Ycoord + ball.radius > 100){
                ball.Yvelocity *= -1;
            }
        }


//        for(int i=0; i<collisions.size(); i++) {
//            for (int j = i; j < collisions.size(); j++) {
//                if (i!=j){
//                    if (root.isCollision(collisions[i], collisions[j])) {
//                        collisions[i].Xvelocity *= -1;
//                        collisions[i].Yvelocity *= -1;
//                        collisions[j].Yvelocity *= -1;
//                        collisions[j].Xvelocity *= -1;
//                    }
//                }
//            }
//        }

        for (auto& ball: collisions){
            cout << ball.Xcoord<< "," <<ball.Ycoord << "," << ball.radius << "," << t << endl;

        }

        // for(const auto& ball : collisions){
        //     cout <<"Collision at ("<<ball.Xcoord<<","<<ball.Ycoord<<") with radius "<<ball.radius<<endl;
        // }
        t += dt;
    }
}