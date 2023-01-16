#ifndef EC_TRANDING_NEWS_H
#define EC_TRANDING_NEWS_H

#include "ECCommand.h"
#include "ECGraphicViewImp.h"
#include "ECObserver.h"
#include <set>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class ECController;
class ECGraphicDoc;
class ECRectangle;
class ECEllipse;
class ECGroup;

using namespace std;



class moveCommand : public ECCommand
{
public:
    moveCommand(ECController *controller,ECDrawiingContext *shape, int xStart,int yStart, int xEnd, int yEnd);
    void Execute();
    void UnExecute();
private:
    ECController *controller;
    ECDrawiingContext *shape;
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
};

class groupCommand : public ECCommand
{
public:
    groupCommand(ECGraphicDoc *doc, set<ECDrawiingContext *> Group) :doc(doc), Group(Group){}
    void Execute();
    void UnExecute();
private:
    set<ECDrawiingContext *> Group;
    ECGraphicDoc *doc;
    ECGroup *group;
    
};

class unGroupCommand : public ECCommand
{
public:
    unGroupCommand(ECGraphicDoc *doc, ECGroup *group);
    void Execute();
    void UnExecute();
private:
    ECGraphicDoc *doc;
    set<ECDrawiingContext *> Group;
    ECGroup *group;
    
};



class deleteCommand : public ECCommand
{
public:
    deleteCommand(ECGraphicDoc *doc,ECDrawiingContext *shape);
    void Execute();
    void UnExecute();
private:
    ECGraphicDoc *doc;
    ECDrawiingContext *shape;
};

class DrawRectangleCommand: public ECCommand
{
public:
    DrawRectangleCommand(ECGraphicDoc *doc,int Dx,int Dy, int Ux, int Uy, int thickness, bool filled);
    void Execute();
    void UnExecute();
private:
    ECGraphicDoc *doc;
    ECRectangle *shape;
    int Dx;
    int Dy;
    int Ux;
    int Uy;
    int thickness;
    bool filled;

};
class DrawEllipseCommand: public ECCommand
{
public:
    DrawEllipseCommand(ECGraphicDoc *doc,int Dx,int Dy, int Ux, int Uy, int thickness, bool filled);
    void Execute();
    void UnExecute();
private:
    ECGraphicDoc *doc;
    ECEllipse *shape;
    int Dx;
    int Dy;
    int Ux;
    int Uy;
    int thickness;
    bool filled;

};


class ECRectangle : public ECDrawiingContext
{
public:
    ECRectangle(ECGraphicViewImp *_display,int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    void draw();
    void setValue(int _Dx,int _Dy,int _Ux,int _Uy);
    bool selected(int x, int y);
    void EMove(int x, int y);
    string saveInfo();

    int type(){
        if (filled)
            return 2;
        else return 0;
    }
private:
    ECGraphicViewImp *display;
    bool working = true;
    int Dx;
    int Dy;
    int Ux;
    int Uy;
    int thickness;
    bool filled;
};


class ECEllipse : public ECDrawiingContext
{
public:
    ECEllipse(ECGraphicViewImp *display,int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    void setValue(int _Dx,int _Dy,int _Ux,int _Uy);
    bool selected(int x, int y);
    void draw();
    void EMove(int x, int y);
    int type(){
        if (filled)
            return 3;
        else return 1;
    }
    string saveInfo();

public:
    void ellipseValue();
    int xcenter;
    int ycenter;
    int radiusx;
    int radiusy;
private:
    ECGraphicViewImp *display;
    bool working = true;
    int Dx;
    int Dy;
    int Ux;
    int Uy;
    int thickness;
    bool filled;
};

class ECGroup: public ECDrawiingContext
{
public:
    ECGroup(set<ECDrawiingContext *> Group) : Group(Group){}
    void setValue(int _Dx,int _Dy,int _Ux,int _Uy)
    {
        for (auto x: Group)
        {
            x->setValue(_Dx, _Dy, _Ux, _Uy);
        }
    }
    void changeColor(ECGVColor color)
    {
        for (auto x: Group)
        {
            if (x->type() == 4)
            {
                dynamic_cast<ECGroup *>(x)->changeColor(color);
            }
            x->setColor(color);
        }
    }
    bool selected(int x, int y)
    {
        for (auto g: Group)
        {
            if (g->selected(x,y))
            {
                changeColor(ECGV_BLUE);
                return true;
            }
        }
        return false;
    }
    void draw()
    {
        
        for (auto x : Group)
        {
            x->draw();
        }
    }
    void EMove(int x, int y)
    {
        for (auto g: Group)
        {
            g->EMove(x,y);
        }
    }
    int type(){return 4;};
    set<ECDrawiingContext *> returnGroup(){return Group;}
    string saveInfo()
    {
        string res;
        res +="4 " + to_string(Group.size());
        for (auto x : Group)
        {
            res += "\n";
            res += x->saveInfo();
        }
        return res;
        
    }
private:
    set<ECDrawiingContext *> Group;
};

class ECGraphicDoc
{
public:
    ECGraphicDoc(ECGraphicViewImp *_display);
    void drawDoc();
    ECRectangle *rectangleDoc(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    ECEllipse *ellipseDoc(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    ECGroup *groupDoc(set<ECDrawiingContext *> Group)
    {
        for (auto x: Group)
        {
            removeDoc(x);
        }
        ECGroup *g = new ECGroup(Group);
        insertDoc(g);
        return g;
    }
    void unGroupDoc(ECGroup *group)
    {
        set<ECDrawiingContext *> Group = group->returnGroup();
        for (auto x : Group)
        {
            insertDoc(x);
        }
        removeDoc(group);
    }
    void insertDoc(ECDrawiingContext *shape){graphics.push_back(shape);}
    void removeDoc(ECDrawiingContext *shape);
    ECDrawiingContext *returnLast(){return graphics.back();}
    void selectDoc(int x, int y, set<ECDrawiingContext *> &SShape);
    int returnSize(){return graphics.size();}
    vector<ECDrawiingContext *> returnG(){return graphics;}
    void Gload(int num)
    {
        set<ECDrawiingContext *> Group;
        for (int i = num; i > 0; i--)
        {
            Group.insert(graphics[graphics.size()-i]);
        }
        ECGroup * g =groupDoc(Group);
        g->draw();

    }
    
private:
    ECGraphicViewImp *display;
    vector<ECDrawiingContext *> graphics;
};



class Observers : public ECObserver
{
public:
    Observers(ECGraphicViewImp *_display, ECController *_controller,
              ECGraphicDoc *graphicDoc);
    void Update();
    
private:
    ECGraphicDoc *doc;
    set<ECDrawiingContext *> selectedGraphs;
    ECGraphicViewImp *display;
    ECController *controller;
    ECDrawiingContext *currentGraph;
private:
    int Dx=-100, Dy=-100, Ux=-100, Uy=-100;
    int tempx = 0, tempy = 0;
    bool button_down = false;
    bool insert_mode = true;
    bool rectangle = true;
    bool ctrl = false;
    bool filled = false;
    
};
    
class ECController
{
public:
    ECController(ECGraphicViewImp &_display);
    ~ECController();
    bool Undo();
    bool Redo();
    
    void freshControl(){graphicDoc->drawDoc();}
    
    
    ECDrawiingContext *rectangleControl(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    ECDrawiingContext *eplliseControl(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled);
    ECDrawiingContext *groupControl(set<ECDrawiingContext *> Group);
    void unGroupControl(ECGroup *group);
    
    void deleteControl(ECDrawiingContext *currentGraph);
    
    void moveControl(ECDrawiingContext *currentGraph,int xStart,int yStart,int xEnd, int yEnd);
        
    void IMoving(ECDrawiingContext *currentGraph,int Dx,int Dy, int cursorx,int cursory);
    
    void EMoving(ECDrawiingContext *currentGraph,int Dx,int Dy, int cursorx,int cursory);
    void init(char **argv)
    {
        file = argv;
        ifstream f(argv[1]);
        string str;
        vector<vector<int> > info;
        
        if (f.is_open())
        {
            getline(f, str);
            while (getline(f, str))
            {
                vector<int> shape;
                istringstream ss(str);
                string word;
                while(ss >> word) {
                    shape.push_back(stoi(word));
                }
                info.push_back(shape);
            }
            if (info.size() >= 1)
            {
                load(info);
            }
            f.close();
        }
        display->SetRedraw(true);
    }
    
    void save(char **argv)
    {
        ofstream f(argv[1], ofstream::out);
        f<<graphicDoc->returnSize();
        vector<ECDrawiingContext *> graphics = graphicDoc->returnG();
        for (auto x : graphics)
        {
            f<<endl;
            f<<x->saveInfo();
        }
        f.close();
    }
    
    
private:
    
    void load(vector<vector<int> >info);
    ECGraphicViewImp *display;
    ECCommandHistory commands;
    Observers *observer;
    ECGraphicDoc *graphicDoc;
    char **file;
    
};



#endif
