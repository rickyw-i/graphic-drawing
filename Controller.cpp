#include "Controller.h"
using namespace std;

//moveCommand
//************************************************************************************************
moveCommand::moveCommand(ECController *controller,ECDrawiingContext *shape, int xStart,int yStart, int xEnd, int yEnd) : controller(controller), shape(shape), xStart(xStart), yStart(yStart), xEnd(xEnd), yEnd(yEnd) {}
void moveCommand::Execute()
{
    controller->EMoving(shape, xStart, yStart, xEnd, yEnd);
}

void moveCommand::UnExecute()
{
    controller->EMoving(shape, xEnd, yEnd, xStart, yStart);
}

//groupCommand
//************************************************************************************************
void groupCommand::Execute()
{
    if (group == NULL)
    {
        group = doc->groupDoc(Group);
    }
}
void groupCommand::UnExecute()
{
    doc->unGroupDoc(group);
}

//unGroupCommand
//************************************************************************************************

unGroupCommand::unGroupCommand(ECGraphicDoc *doc, ECGroup *group) :doc(doc), group(group){Group = group->returnGroup();}
void unGroupCommand::Execute()
{
    doc->unGroupDoc(group);
}
void unGroupCommand::UnExecute()
{
    group = doc->groupDoc(Group);
}
//deleteCommand
//************************************************************************************************
deleteCommand::deleteCommand(ECGraphicDoc *doc,ECDrawiingContext *shape) : doc(doc), shape(shape) {}
void deleteCommand::Execute()
{
    doc->removeDoc(shape);
}
void deleteCommand::UnExecute()
{
    shape->setColor(ECGV_BLACK);
    doc->insertDoc(shape);
}

//DrawRectangleCommand
//************************************************************************************************
DrawRectangleCommand::DrawRectangleCommand(ECGraphicDoc *doc,int Dx,int Dy, int Ux, int Uy, int thickness, bool filled) : doc(doc),Dx(Dx),Dy(Dy), Ux(Ux), Uy(Uy),thickness(thickness), filled(filled) {}

void DrawRectangleCommand::Execute()
{
    if (shape == NULL)
    {
        shape = doc->rectangleDoc(Dx, Dy, Ux, Uy, thickness, filled);
    }
    doc->insertDoc(shape);
}

void DrawRectangleCommand::UnExecute()
{
    doc->removeDoc(shape);
}

//DrawEllipseCommand
//************************************************************************************************
DrawEllipseCommand::DrawEllipseCommand(ECGraphicDoc *doc,int Dx,int Dy, int Ux, int Uy, int thickness, bool filled) : doc(doc),Dx(Dx),Dy(Dy), Ux(Ux), Uy(Uy),thickness(thickness), filled(filled) {}
void DrawEllipseCommand::Execute()
{
    if (shape == NULL)
    {
        shape = doc->ellipseDoc(Dx, Dy, Ux, Uy, thickness, filled);
    }
    doc->insertDoc(shape);
}

void DrawEllipseCommand::UnExecute()
{
    doc->removeDoc(shape);
}


//ECRectangle
//************************************************************************************************
ECRectangle::ECRectangle(ECGraphicViewImp *_display,int Dx,int Dy, int Ux, int Uy, int thickness,bool filled) :
    display(_display),Dx(Dx),Dy(Dy), Ux(Ux), Uy(Uy),thickness(thickness),filled(filled)
{setColor(ECGV_BLACK);}

void ECRectangle::draw()
{
    thickness = 3;
    if (working)
    {
        if (filled)
        {
            display->DrawFilledRectangle(Dx, Dy, Ux, Uy,ECGV_BLUE);
        }
        else
        {
            display->DrawRectangle(Dx, Dy, Ux, Uy,thickness,ECGV_BLUE);

        }
        working = false;
    }
    else
    {
        if (filled)
        {
            display->DrawFilledRectangle(Dx, Dy, Ux, Uy,getColor());
        }
        else
        {
            display->DrawRectangle(Dx, Dy, Ux, Uy,thickness,getColor());
        }
    }
    
}

void ECRectangle::setValue(int _Dx,int _Dy,int _Ux,int _Uy)
{
    Dx = _Dx;
    Dy = _Dy;
    Ux = _Ux;
    Uy = _Uy;
    working = true;
}

bool ECRectangle::selected(int x, int y)
{
    if ((x <= Dx && x>= Ux) ||
        (x <= Ux && x>= Dx))
    {
        return((y <= Dy && y>= Uy) ||
               (y <= Uy && y>= Dy));
    }
    return false;
}

void ECRectangle::EMove(int x, int y)
{
    Dx += x;
    Dy += y;
    Ux += x;
    Uy += y;
    this->setValue(Dx, Dy, Ux, Uy);
}

string ECRectangle::saveInfo()
{
    string res;
    if (filled)
        res += "2 4 ";
    else res +="0 4 ";
    res += to_string(Dx) + " " +to_string(Dy)+ " " +to_string(Dx)+ " " +to_string(Uy) + " " +to_string(Ux)+ " " + to_string(Uy)+ " " + to_string(Ux)+ " " + to_string(Dy)+ " ";
    res += to_string (getColor());
    return res;
}


//ECEllipse
//************************************************************************************************
ECEllipse::ECEllipse(ECGraphicViewImp *display,int Dx,int Dy, int Ux, int Uy, int thickness,bool filled): display(display),Dx(Dx),Dy(Dy), Ux(Ux), Uy(Uy),thickness(thickness),filled(filled) {
    setColor(ECGV_BLACK);
    ellipseValue();
}
void ECEllipse::setValue(int _Dx,int _Dy,int _Ux,int _Uy)
{
    Dx = _Dx;
    Dy = _Dy;
    Ux = _Ux;
    Uy = _Uy;
    ellipseValue();
    working = true;
}
bool ECEllipse::selected(int x, int y)
{
    return (((pow((x-xcenter),2) /pow(radiusx,2))
             +(pow((y-ycenter),2) /pow(radiusy,2))) <= 1);
}
void ECEllipse::draw()
{
    thickness = 3;
    if (working)
    {
        if (filled)
        {
            display->DrawFilledEllipse(xcenter, ycenter, radiusx, radiusy,ECGV_BLUE);
        }
        else
        {
            display->DrawEllipse(xcenter, ycenter, radiusx, radiusy, thickness, ECGV_BLUE);
        }
        
        
        working = false;
    }
    else
    {
        if (filled)
        {
            display->DrawFilledEllipse(xcenter, ycenter, radiusx, radiusy, getColor());
        }
        else
        {
            display->DrawEllipse(xcenter, ycenter, radiusx, radiusy, thickness, getColor());
        }
    }
}
void ECEllipse::EMove(int x, int y)
{
    Dx += x;
    Dy += y;
    Ux += x;
    Uy += y;
    this->setValue(Dx, Dy, Ux, Uy);
}

void ECEllipse::ellipseValue()
{
    xcenter = (Dx + Ux)/2;
    ycenter = (Dy + Uy)/2;
    radiusx = abs((Ux - Dx)/2);
    radiusy = abs((Uy - Dy)/2);
}

//ECGraphicDoc
//************************************************************************************************
ECGraphicDoc::ECGraphicDoc(ECGraphicViewImp *_display) : display(_display){}

void ECGraphicDoc::drawDoc()
{
    for (auto x : graphics)
    {
        x->draw();
    }
}

ECRectangle *ECGraphicDoc::rectangleDoc(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled)
{
    ECRectangle *rec = new ECRectangle(display,Dx, Dy,  Ux,  Uy,  thickness, filled);
    return rec;
}
ECEllipse *ECGraphicDoc::ellipseDoc(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled)
{
    ECEllipse *rec = new ECEllipse(display,Dx, Dy,  Ux,  Uy,  thickness, filled);
    return rec;
}

void ECGraphicDoc::removeDoc(ECDrawiingContext *shape)
{
    graphics.erase(remove(graphics.begin(), graphics.end(), shape), graphics.end());
}

void ECGraphicDoc::selectDoc(int x, int y, set<ECDrawiingContext *> &SShape)
{
    for (int i = graphics.size() - 1; i >= 0; i--)
    {
        if (SShape.find(graphics[i]) == SShape.end())
        {
            if (graphics[i]->selected(x,y))
            {
                if (SShape.find(graphics[i]) == SShape.end())
                {
                    SShape.insert(graphics[i]);
                    graphics[i]->setColor(ECGV_BLUE);
                    break;
                }
            }
        }
        
    }
}
string ECEllipse::saveInfo()
{
    string res;
    if (filled)
    {
        res +="3 ";
    }
    else {res += "1 ";}
    res +=to_string(xcenter)+" "+ to_string(ycenter)+" "+ to_string(radiusx) +" "+to_string(radiusy)+" "+to_string(getColor());
    return res;
}

//ECController
//************************************************************************************************
ECController::ECController(ECGraphicViewImp &_display) : display(&_display)
{
    
    graphicDoc = new  ECGraphicDoc(display);
    observer = new  Observers(display, this,graphicDoc);
}

ECController::~ECController()
{
    if (file != NULL)
    {
        save(file);
    }
    cout
    <<"*******************\nNum of graphics saved: "<<graphicDoc->returnSize()<<"\n*******************\n";
}
bool ECController::Undo()
{
    if (commands.Undo())
        return 1;
    else return 0;
}
bool ECController::Redo()
{
    if (commands.Redo())
        return 1;
    else return 0;
}

ECDrawiingContext *ECController::rectangleControl(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled)
{
    DrawRectangleCommand *draw = new DrawRectangleCommand(graphicDoc,Dx,Dy, Ux, Uy, thickness, filled);
    commands.ExecuteCmd(draw);
    return graphicDoc->returnLast();
}

ECDrawiingContext *ECController::eplliseControl(int Dx,int Dy, int Ux, int Uy, int thickness,bool filled)
{
    DrawEllipseCommand *draw = new DrawEllipseCommand(graphicDoc,Dx,Dy, Ux, Uy, thickness, filled);
    commands.ExecuteCmd(draw);
    return graphicDoc->returnLast();
}
ECDrawiingContext *ECController::groupControl(set<ECDrawiingContext *> Group)
{
    groupCommand *g = new groupCommand(graphicDoc, Group);
    commands.ExecuteCmd(g);
    return graphicDoc->returnLast();
    
}

void ECController::unGroupControl(ECGroup *group)
{
    unGroupCommand *un = new unGroupCommand(graphicDoc, group);
    commands.ExecuteCmd(un);
}

void ECController::deleteControl(ECDrawiingContext *currentGraph)
{
    deleteCommand *del = new deleteCommand(graphicDoc,currentGraph);
    commands.ExecuteCmd(del);
}

void ECController::moveControl(ECDrawiingContext *currentGraph,int xStart,int yStart,int xEnd, int yEnd)
{
    this->EMoving(currentGraph, xEnd, yEnd, xStart, yStart);
    moveCommand *move = new moveCommand(this,currentGraph,xStart,yStart,xEnd, yEnd);
    commands.ExecuteCmd(move);
    
    
}

void ECController::IMoving(ECDrawiingContext *currentGraph,int Dx,int Dy, int cursorx,int cursory)
{
    currentGraph->setValue(Dx, Dy, cursorx, cursory);
}

void ECController::EMoving(ECDrawiingContext *currentGraph,int Dx,int Dy, int cursorx,int cursory)
{
    
    int x = cursorx - Dx;
    int y = cursory - Dy;
    currentGraph->EMove(x, y);
}

void ECController::load(vector<vector<int> >info)
{
    int num =0;
    while (num <= info.size()-1)
    {
        vector<int> shape = info[num];
        if (shape[0] == 0)
        {
            ECRectangle *rec= graphicDoc->rectangleDoc(shape[2],shape[3], shape[6],shape[7],3,false);
            rec->draw();
            rec->setColor(ECGVColor(shape[10]));
            graphicDoc->insertDoc(rec);
        }
        else if(shape[0] == 1)
        {
            
            ECEllipse *ellipse = graphicDoc->ellipseDoc(shape[1]-shape[3], shape[2]-shape[4], shape[1]+shape[3], shape[2]+shape[4], 3, false);
            ellipse->draw();
            ellipse->setColor(ECGVColor(shape[5]));
            graphicDoc->insertDoc(ellipse);
        }
        else if (shape[0] == 2)
        {
            ECRectangle *rec= graphicDoc->rectangleDoc(shape[2],shape[3], shape[6],shape[7],3,true);
            rec->draw();
            rec->setColor(ECGVColor(shape[10]));
            graphicDoc->insertDoc(rec);
        }
        else if(shape[0] == 3)
        {
            
            ECEllipse *ellipse = graphicDoc->ellipseDoc(shape[1]-shape[3], shape[2]-shape[4], shape[1]+shape[3], shape[2]+shape[4], 3, true);
            ellipse->draw();
            ellipse->setColor(ECGVColor(shape[5]));
            graphicDoc->insertDoc(ellipse);
        }
        else if (shape[0] == 4)
        {
            int group = shape[1];
            vector<vector<int> >info2;
            for (int i = 1; i <=group; i++)
            {
                info2.push_back(info[num+i]);
            }
            load(info2);
//            set<ECDrawiingContext *> Group;
            graphicDoc->Gload(group);
            num += group;
        }
        num+=1;
    }
}


//Observers
//************************************************************************************************

Observers::Observers(ECGraphicViewImp *_display, ECController *_controller, ECGraphicDoc *graphicDoc) :display(_display), controller (_controller), doc(graphicDoc)
{
    display->Attach(this);
}

void Observers::Update(){
    
    //refresh
    //****************
    if( display->GetCurrEvent()  == ECGV_EV_TIMER)
    {
        controller->freshControl();
    }
    //undo
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_Z)
    {
        controller->Undo();
        display->SetRedraw(true);
    }
    //redo
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_Y)
    {
        controller->Redo();
        display->SetRedraw(true);
    }
    
    //press ctrl
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_CTRL && !ctrl)
    {
        ctrl = true;
    }
    
    //release ctrl
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_UP_CTRL && ctrl)
    {
        ctrl = false;
    }

    
    //switch mode
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_SPACE)
    {
        if (insert_mode)
        {
            insert_mode = false;
        }
        else
        {
            insert_mode = true;
            if (!selectedGraphs.empty() )
            {
                for (int i = 0; i< selectedGraphs.size(); i++ )
                {
                    for (auto x: selectedGraphs)
                    {
                        if (x->type() == 4)
                        {
                            dynamic_cast<ECGroup *>(x)->changeColor(ECGV_BLACK);
                        }
                        x->setColor(ECGV_BLACK);
                    }
                }
                selectedGraphs.clear();
            }
        }
        display->SetRedraw(true);
    }
    
    //F key
    //****************
    if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_F)
    {
        if (filled)
        {
            filled = false;
        }
        else filled = true;
        display->SetRedraw(true);
    }
    
    int cursorx, cursory;
    display->GetCursorPosition(cursorx, cursory);
    
    //insert mode
    //****************
    if (insert_mode)
    {
        //G key
        //****************
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_G)
        {
            if (rectangle)
            {
                rectangle = false;
            }
            else rectangle = true;
            display->SetRedraw(true);
        }
        // Button Down
        //****************
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_BUTTON_DOWN && !button_down)
        {
            Dx = cursorx;
            Dy = cursory;
            button_down = true;
            if (rectangle){
                currentGraph=controller->rectangleControl(Dx,Dy,Dx,Dy,3,filled);
            }
            else{
                currentGraph=controller->eplliseControl(Dx,Dy,Dx,Dy,3,filled);
            }
        }
        //button up
        //****************
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_BUTTON_UP && button_down)
        {
            Ux = cursorx;
            Uy = cursory;
            button_down = false;
            display->SetRedraw(true);
        }
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_MOVING && button_down)
        {
            if( Dx >= 0 )
            {
                controller->IMoving(currentGraph, Dx, Dy, cursorx, cursory);
                display->SetRedraw(true);
            }
        }
    }
    
    //edit mode
    //****************
    else
    {
        //G key
        //****************
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_G)
        {
            if (selectedGraphs.size()>1)
            {
                currentGraph = controller->groupControl(selectedGraphs);
                
                for (auto x: selectedGraphs)
                {
                    x->setColor(ECGV_BLACK);
                }
                selectedGraphs.clear();
                controller->freshControl();
            }
            else if(selectedGraphs.size() == 1)
            {
                for (auto x : selectedGraphs)
                {
                    if (x->type() == 4)
                    {
                        dynamic_cast<ECGroup *>(x)->changeColor(ECGV_BLACK);
                        controller->unGroupControl(dynamic_cast<ECGroup *>(x));
                    }
                }
                selectedGraphs.clear();

            }
        }
        if( display->GetCurrEvent() == ECGV_EV_KEY_UP_G)
        {
            controller->freshControl();
            display->SetRedraw(true);
        }
        //movement by keystrokes
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_UP)
        {
            if (!selectedGraphs.empty())
            {
                for (auto x: selectedGraphs)
                {
                    controller->EMoving(x, 100, 100, 100, 90);
                    controller->moveControl(x,100, 100,100, 90);
                }
            }
            display->SetRedraw(true);
        }
        
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_DOWN)
        {
            if (!selectedGraphs.empty())
            {
                for (auto x: selectedGraphs)
                {
                    controller->EMoving(x, 100, 100, 100, 110);
                    controller->moveControl(x,100, 100,100,110);                }
            }
            display->SetRedraw(true);
        }
        
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_LEFT)
        {
            if (!selectedGraphs.empty())
            {
                for (auto x: selectedGraphs)
                {
                    controller->EMoving(x, 100, 100, 90, 100);
                    controller->moveControl(x,100, 100,90, 100);                }
            }
            display->SetRedraw(true);
        }
        
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_RIGHT)
        {
            if (!selectedGraphs.empty())
            {
                for (auto x: selectedGraphs)
                {
                    controller->EMoving(x, 100, 100, 110, 100);
                    controller->moveControl(x,100, 100,110, 100);                 }
            }
            display->SetRedraw(true);
        }
        
        //Delete(D key down)
        //****************
        if( display->GetCurrEvent() == ECGV_EV_KEY_DOWN_D)
        {
            if (!selectedGraphs.empty())
            {
                for ( auto x: selectedGraphs){
                    controller->deleteControl(x);
                }
                selectedGraphs.clear();
            }
            display->SetRedraw(true);
        }
        
        //mouse down
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_BUTTON_DOWN && !button_down)
        {
            Dx = cursorx;
            Dy = cursory;
            button_down = true;
            tempx = cursorx;
            tempy = cursory;
            display->SetRedraw(true);
            //select function
            //check if there is a graph under where mouse down
            //****************
            if (doc->returnSize() >= 1)
            {
                //ctrl choose
                //****************
                if (selectedGraphs.empty()||ctrl)
                {
                    doc->selectDoc(Dx,Dy,selectedGraphs);
                }
                //****************
                //choose without ctrl
                //****************

                else
                {
                    //clear the old selectedgraphs
                    //****************
                    for (auto x : selectedGraphs)
                    {
                        if (x->type() == 4)
                        {
                            dynamic_cast<ECGroup *>(x)->changeColor(ECGV_BLACK);
                        }
                        else{
                            x->setColor(ECGV_BLACK);}
                    }
                    selectedGraphs.clear();
                    //select
                    //****************
                    doc->selectDoc(Dx,Dy,selectedGraphs);
                }
            }
        }
        
        //mouse up
        //****************
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_BUTTON_UP && button_down)
        {
            Ux = cursorx;
            Uy = cursory;
            
            if ( !(Dx == Ux && Dy==Uy))
            {
                for (auto x: selectedGraphs)
                {
                    controller->moveControl(x,Dx, Dy,Ux, Uy);
                }
            }
             
            button_down = false;
            display->SetRedraw(true);
        }
        
        //mouse moving(edit)
        //****************
        if(display->GetCurrEvent() == ECGV_EV_MOUSE_MOVING && button_down)
        {
            if( Dx >= 0 )
            {
                for (auto x: selectedGraphs)
                {
                    
                    controller->EMoving(x, tempx, tempy, cursorx, cursory);
                }
                tempx = cursorx;
                tempy = cursory;
            }
            display->SetRedraw(true);
        }
        
    }
}

